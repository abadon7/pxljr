/**
 * Copyright (c) 2005 Hin-Tak Leung. All rights reserved.
 *
 **/
/*
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
/**
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 **/

/**
 * Copyright (c) 2001-2002 artofcode LLC.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ijs.h"
#include "ijs_server.h"

#include "i_job_info.h"

#define BUF_SIZE 4096

typedef struct _I_ParamList ParamList;

struct _I_ParamList {
  ParamList *next;
  char *key;
  char *value;
  int value_size;
};

static int
i_status_cb (void *status_cb_data,
		  IjsServerCtx *ctx,
		  IjsJobId job_id)
{
  return 0;
}

static int
i_list_cb (void *list_cb_data,
		 IjsServerCtx *ctx,
		 IjsJobId job_id,
		 char *val_buf,
		 int val_size)
{
  const char *param_list = "OutputFile,OutputFD,DeviceManufacturer,DeviceModel,PageImageFormat,Dpi,Width,Height,BitsPerSample,ByteSex,ColorSpace,NumChan,PaperSize,PrintableArea,PrintableTopLeft,TopLeft,Quality,Quality:Quality,PaperType,Copies";
  int size = strlen (param_list);

  if (size > val_size)
    {
      return IJS_EBUF;
    }
  memcpy (val_buf, param_list, size);
  return size;
}

static int
i_enum_cb (void *enum_cb_data,
		 IjsServerCtx *ctx,
		 IjsJobId job_id,
		 const char *key,
		 char *val_buf,
		 int val_size)
{
  const char *val = NULL;

  if (!strcmp (key, "ColorSpace"))
    val = "DeviceRGB,DeviceGray,DeviceCMYK";
  else if (!strcmp (key, "DeviceManufacturer"))
    val = "HEWLETT-PACKARD";
  else if (!strcmp (key, "DeviceModel"))
    val = "hp color LaserJet 3500,hp color LaserJet 3550";
  else if (!strcmp (key, "PageImageFormat"))
    val = "Raster";

  if (val == NULL)
      return IJS_EUNKPARAM;
  else
    {
      int size = strlen (val);

      if (size > val_size)
	  return IJS_EBUF;
      memcpy (val_buf, val, size);
      return size;
    }
}

/* A C implementation of /^(\d\.+\-eE)+x(\d\.+\-eE)+$/ */
static int
i_parse_wxh (const char *val, int size,
		   double *pw, double *ph)
{
  char buf[256];
  char *tail;
  int i;

  for (i = 0; i < size; i++)
    if (val[i] == 'x')
      break;

  if (i + 1 >= size)
      return IJS_ESYNTAX;

  if (i >= sizeof(buf))
      return IJS_EBUF;

  memcpy (buf, val, i);
  buf[i] = 0;
  *pw = strtod (buf, &tail);
  if (tail == buf)
      return IJS_ESYNTAX;

  if (size - i > sizeof(buf))
      return IJS_EBUF;

  memcpy (buf, val + i + 1, size - i - 1);
  buf[size - i - 1] = 0;
  *ph = strtod (buf, &tail);
  if (tail == buf)
      return IJS_ESYNTAX;

  return 0;
}

/**
 * i_find_key: Search parameter list for key.
 *
 * @key: key to look up
 *
 * Return value: ParamList entry matching @key, or NULL.
 **/
static ParamList *
i_find_key (ParamList *pl, const char *key)
{
  ParamList *curs;

  for (curs = pl; curs != NULL; curs = curs->next)
    {
      if (!strcmp (curs->key, key))
	return curs;
    }
  return NULL;
}

static int
set_param (ParamList **ppl, const char *key, const char *value, int value_size)
{
  int key_len = strlen (key);
  ParamList *pl;
#ifdef DEBUG
  fprintf(stderr,"                    set_param: %s=",key);
  fwrite (value, 1, value_size, stderr);
  fputs ("\n", stderr);
#endif
  pl = i_find_key (*ppl, key);

  if (pl == NULL)
    {
      pl = (ParamList *)malloc (sizeof (ParamList));
      pl->next = *ppl;
      pl->key = malloc (key_len + 1);
      memcpy (pl->key, key, key_len + 1);
      *ppl = pl;
    }
  else
    {
      free (pl->value);
    }

  pl->value = malloc (value_size);
  memcpy (pl->value, value, value_size);
  pl->value_size = value_size;

  return 0;
}

/**
 * @printable: An array in which to store the printable area.
 *
 * On return, @printable = PrintableArea[0:1] + TopLeft[0:1]
 **/
static int
i_compute_printable (ParamList *pl, double printable[4])
{
  ParamList *curs;
  double width, height;
  int code;
  double margin = 0.1667; /* The unprintable margin */

  curs = i_find_key (pl, "PaperSize");
  if (curs == NULL)
    return -1;
  code = i_parse_wxh (curs->value, curs->value_size, &width, &height);

  if (code == 0)
    {
      /* We impose symmetric margins */
      printable[0] = width - 2 * margin;
      printable[1] = height - 2 * margin;
      printable[2] = margin;
      printable[3] = margin;
    }

  return code;
}

static int
i_get_cb (void *get_cb_data,
		 IjsServerCtx *ctx,
		 IjsJobId job_id,
		 const char *key,
		 char *val_buf,
		 int val_size)
{
  ParamList *pl = *(ParamList **)get_cb_data;
  ParamList *curs;
  const char *val = NULL;
  char buf[256];
  int code;

  curs = i_find_key (pl, key);
  if (curs != NULL)
    {
      if (curs->value_size > val_size)
	  return IJS_EBUF;

      memcpy (val_buf, curs->value, curs->value_size);
      return curs->value_size;
    }

  if (!strcmp (key, "PrintableArea") || !strcmp (key, "PrintableTopLeft"))
    {
      double printable[4];
      int off = !strcmp (key, "PrintableArea") ? 0 : 2;

      code = i_compute_printable (pl, printable);
      if (code == 0)
	{
	  sprintf (buf, "%gx%g", printable[off + 0], printable[off + 1]);
	  val = buf;
	}
    }

  if (!strcmp (key, "DeviceManufacturer"))
    val = "HEWLETT-PACKARD";
  else if (!strcmp (key, "DeviceModel"))
    val = "hp color LaserJet 3550";
  else if (!strcmp (key, "PageImageFormat"))
    val = "Raster";
  else if (!strcmp (key, "Dpi"))
    val = "600x600";

  if (val == NULL)
      return IJS_EUNKPARAM;
  else
    {
      int size = strlen (val);

      if (size > val_size)
	  return IJS_EBUF;

      memcpy (val_buf, val, size);
      return size;
    }
}

static int
i_set_cb (void *set_cb_data, IjsServerCtx *ctx, IjsJobId job_id,
		const char *key, const char *value, int value_size)
{
  ParamList **ppl = (ParamList **)set_cb_data;
  int code;
  char buf[256];

  /* Convert value to a zero-terminated string */
  if( value_size >= sizeof(buf)){
    return IJS_EBUF;
  }
  memcpy (buf, value, value_size);
  buf[value_size] = 0;

  if (!strcmp (key, "PaperSize"))
    {
      /* Setting PaperSize affects PrintableArea, which is used by ghostscript */
      double width, height;

      code = i_parse_wxh (value, value_size, &width, &height);
      if (code < 0)
	return code;
    }

  if (!strcmp (key, "Dpi"))
    {
      char *s = NULL;
      if(!strcmp (buf, "600"))
        {
          s = "600x600";
	}
      else
        {
          return IJS_ERANGE;
	}
        code = set_param (ppl, "Dpi", s, strlen (s));
        if (code < 0)
	  return code;	/* because we can't go on setting Dpi */
    }

  if (!strcmp (key, "PrintableArea"))
    {
      /* This can't be set */
      fprintf (stderr, "Setting %s is not allowed\n", key);
      return IJS_ERANGE;
    }

  if (!strcmp (key, "PrintableTopLeft"))
    {
      /* This can't be set */
      fprintf (stderr, "Setting %s is not allowed\n", key);
      return IJS_ERANGE;
    }

  /* Update parameter list */
  
  code = set_param (ppl, key, value, value_size);
  if (code < 0)
    return code;
  return 0;
}

/**
 * Finds a parameter in the param list, and allocates a null terminated
 * string with the value.
 **/
static char *
find_param (ParamList *pl, const char *key)
{
  ParamList *curs;
  char *result;

  curs = i_find_key (pl, key);
  if (curs == NULL)
    return NULL;

  result = malloc (curs->value_size + 1);
  memcpy (result, curs->value, curs->value_size);
  result[curs->value_size] = 0;
  return result;
}

static void
free_param_list (ParamList *pl)
{
  ParamList *next;

  for (; pl != NULL; pl = next)
    {
      next = pl->next;
      free (pl->key);
      free (pl->value);
      free (pl);
    }
}

static int
pl_to_jobinfo (ParamList *pl, IjsPageHeader ph, i_job_info_t *job_info)
{
  char *fn;
  char *s;

  /* Check options */

  if (job_info->outfile == NULL)
    {
      fn = find_param (pl, "OutputFile");
      /* todo: check error! */
	  
      if (fn == NULL)
        {
          fn = find_param (pl, "OutputFD");
          if (fn != NULL)
  	    {
	    job_info->outfile = fdopen (atoi (fn), "w");
	    }
        }
      else
        {
          job_info->outfile = fopen (fn, "w");
        }
      if (job_info->outfile == NULL)
        {
          fclose (stdin);
          fclose (stdout);
          return 1;
        }
      if (fn != NULL)
        free (fn);
    }

  /* DeviceModel */
  s = find_param(pl, "DeviceModel");
  if (s == NULL)
    {
      fprintf(stderr, "Printer DeviceModel not set, aborting!\n");
      return 1;
    }

  if (strcmp(s, "hp color LaserJet 3550") == 0)
    {
      job_info->model = MODEL_CLJ3550 ;
    }
  else if (strcmp(s, "hp color LaserJet 3500") == 0)
    {
      job_info->model = MODEL_CLJ3500 ;
    }
  else 
    {
      fprintf(stderr, "Unknown Printer %s, aborting!\n", s);
      return 1;
    }

  /* PaperSize */ /* FIXME */
  s = find_param(pl, "PaperSize");
  if (s == NULL)
    {
      fprintf(stderr, "PaperSize not set, using default (A4 210x297mm)\n");
    }
  else
    {
      double width, height;
      int code;
      code = i_parse_wxh (s, strlen(s), &width, &height);
      if (code == 0)
	{
	  fprintf(stderr, "PaperSize = %s\n", s);
	}
      else
	{
          fprintf(stderr, "Unparsable PaperSize %s, aborting!\n", s);
          return 1;
	}
    }


  /* Dpi */
  s = find_param(pl, "Dpi");
  if (s == NULL)
    {
      fprintf(stderr, "Dpi not set, using default (600)\n");
      job_info->dpi_h = 600;
      job_info->dpi_v = 600;
    }
  else if (strcmp(s, "600") == 0)
    {
      job_info->dpi_h = 600;
      job_info->dpi_v = 600;
    }
  else 
    {
      fprintf(stderr, "Unknown Dpi value %s, aborting!\n", s);
      return 1;
    }

  /* PaperType */
  s = find_param(pl, "PaperType");
  if (s == NULL)
    {
      fprintf(stderr, "PaperType not set, using default Normal (0)\n");
      job_info->papertype = 0;
    }
  else 
    {
      fprintf(stderr, "Unknown PaperType value %s, aborting!\n", s);
      return 1;
    }

  /* Copies */
  s = find_param(pl, "Copies");
  if (s == NULL)
    {
      fprintf(stderr, "Copies not set, using default (1)\n");
      job_info->copies = 1;
    }
  else 
    {
      job_info->copies = atoi(s);
      if(!job_info->copies)
	{
	  fprintf(stderr, "Unknown Copies value %s, aborting!\n", s);
	  return 1;
	}
      else
	{
	  fprintf(stderr, "Printing %d copies\n", job_info->copies);
	}
    }

  /* Copies */
  s = find_param(pl, "Quality:Quality");
  if (s == NULL)
    {
      fprintf(stderr, "Quality not set, set HPIJS compatible\n");
      job_info->qfactor = 6;
    }
  else 
    {
      int quality = atoi(s);
      switch(quality)
	{
	case 2:
	  job_info->qfactor = 0;
	  fprintf(stderr, "Quality: Best\n");
	  break;
	case 1:
	  job_info->qfactor = 1;
	  fprintf(stderr, "Quality: Medium\n");
	  break;
	case 0:
	  job_info->qfactor = 6;
	  fprintf(stderr, "Quality: Low\n");
	  break;
	default:
	  job_info->qfactor = 6;
	  fprintf(stderr, "Quality: unrecognised, set to Low\n");
	  break;
	}
    }

  /* Number of channels */
  if ((ph.n_chan != 1) && (ph.n_chan != 3))
    {
      fprintf(stderr, "Number of channels is %i (unsupported), aborting!\n",
              ph.n_chan);
      return 1;
    }
  else
    {
      job_info->components = ph.n_chan;
    }

  /* Bits per sample */
  if ((ph.bps != 1) && (ph.bps != 8))
    {
      fprintf(stderr, "Bit per sample is %i (unsupported), aborting!\n",
              ph.bps);
      return 1;
    }

  /* BitMap Size */
  job_info->pixel_h = (ph.width + 0x1f) & ~0x1f;
  job_info->pixel_v = (ph.height + 0x7f) & ~0x7f;
  fprintf(stderr, "  Bitmap size %ix%i (%gx%g in)\n",
          job_info->pixel_h,
	  job_info->pixel_v,
	  (double)job_info->pixel_h / job_info->dpi_h,
	  (double)job_info->pixel_v / job_info->dpi_v);

  /* Dpi of the bitmap */
  if (ph.xres != (double)job_info->dpi_h
      || ph.yres != (double)job_info->dpi_v)
    {
       fprintf(stderr, "Dpi of bitmap (%gx%g) is different from selected dpi (%gx%g)",
               ph.xres,
	       (double)job_info->dpi_h,
               ph.yres,
	       (double)job_info->dpi_v);
       return 1;
    }

  /* If we get here, all it's OK */
  return 0;
}

int
main (int argc, char **argv)
{
  IjsServerCtx *ctx;
  IjsPageHeader ph;
  int status;
  ParamList *pl = NULL;
  i_job_info_t *job_info = NULL;
  int i_job_started = JOB_STARTED_NO;
  int current_page_number = 1;
  
  if (argc > 1) /* the IJS plugin is being run stand-alone */ 
    {
      /* we'll try to get status if the plugin is being run stand-alone */
      i_status(argc, argv);
      exit(0);
    } 

  ctx = ijs_server_init ();
  if (ctx == NULL)
    return (1);
  ijs_server_install_status_cb (ctx, i_status_cb, &pl);
  ijs_server_install_list_cb (ctx, i_list_cb, &pl);
  ijs_server_install_enum_cb (ctx, i_enum_cb, &pl);
  ijs_server_install_set_cb (ctx, i_set_cb, &pl);
  ijs_server_install_get_cb (ctx, i_get_cb, &pl);
  
  i_init(&job_info);

  do 
    {
      int total_bytes, bytes_left;
      ParamList *curs;
      
      char *ptr_row_prev;
      char *ptr_row_current;
      
      int bytes_per_row;
      int bytes_per_row_padded;
      int total_stripes;
      int i_stripe;

      fprintf (stderr, "getting page header\n");
      
      status = ijs_server_get_page_header (ctx, &ph);
      
      if (status) 
	{
	  if (status < 0)
	    {
	      fprintf(stderr, "ijs_server_get_page_header failed: %d\n",
		      status);
	    }
	  break;
	}
      
      fprintf (stderr, "Received header for page %d: width %d x height %d\n",
	       current_page_number, ph.width, ph.height);
      current_page_number++;
      
      /* Before starting, dump IJS parameters */

      for (curs = pl; curs != NULL; curs = curs->next)
	{
	  fprintf (stderr, "%% IJS parameter: %s = ", curs->key);
	  fwrite (curs->value, 1, curs->value_size, stderr);
	  fputs ("\n", stderr);
	}

      /* Convert parameters to internal format */

      status = pl_to_jobinfo(pl, ph, job_info);
      if (status != 0)
        {
          fprintf (stderr, "parameters conversion failed, aborting\n");
	  exit (1);
        }

      /* Job Header */      
      
      if (i_job_started == JOB_STARTED_NO) /* only do one job header per job */
	{
          status = i_job_header(job_info);      
          if (status != 0)
            {
              fprintf (stderr, "output error\n");
	      exit (1);
            }
	  i_job_started = JOB_STARTED_YES ;	
        }
      
      /* Page header */
      
      status = i_page_header(job_info);
      if (status != 0)
        {
          fprintf (stderr, "output error\n");
          exit (1);
        }
      
      bytes_per_row = (ph.n_chan * ph.bps * ph.width + 8 - 1) / 8 ;
      total_bytes = bytes_per_row * ph.height;

      bytes_per_row_padded = ((ph.width + 0x1f) & ~0x1f) * ph.n_chan;
      

      /* last one may have to be padded */
      total_stripes = (ph.height + 128 - 1) / 128;
      
      ptr_row_prev = (char *)malloc(bytes_per_row_padded);
      ptr_row_current = (char *)malloc(bytes_per_row_padded);

      bytes_left = total_bytes;
      
      for (i_stripe = 0 ; i_stripe < total_stripes ; i_stripe++)
	{
	  int i, i_row;
	  
	  /* clear both rows at stripe start */
	  for (i = 0 ; i < bytes_per_row_padded ; i++)
	    {
	      *(ptr_row_current + i) = (char) 0xff;
	      *(ptr_row_prev + i) = (char) 0xff;
	    }
	  
	  for (i_row = 0 ; i_row < 128 ; i_row++)
	    {
	      /* Page body */
	      
	      char *ptr_temp;
	      
	      if (bytes_left > 0)		/* no padding situation */
	        {
	          status = ijs_server_get_data (ctx, ptr_row_current, bytes_per_row);
	          bytes_left -= bytes_per_row;

	          if (status)
		    {
		      fprintf (stderr, "page aborted!\n");
		      break;
		    }
                }
	      else
	        {
		  /* padding situation */
		  memcpy (ptr_row_current, ptr_row_prev, bytes_per_row_padded); /* repeat last line */
		}
	      
	      i_compress_row(job_info, 
			     ptr_row_current, 
			     ptr_row_prev, bytes_per_row_padded);	      
	      ptr_temp = ptr_row_current;
	      ptr_row_current = ptr_row_prev;
	      ptr_row_prev = ptr_temp;
	    }
	  i_print_stripe(job_info, i_stripe);
	}
      
      /* Page footer */

      status = i_page_footer (job_info);
      if (status != 0)
        {
          fprintf (stderr, "output error\n");
          exit (1);
        }
      
    }
  while (status == 0);
  
  /* Job footer */

  if (i_job_started == JOB_STARTED_YES)
    {
      status = i_job_footer (job_info);
    }

  if (status > 0) status = 0; /* normal exit */
  
  i_exit(job_info);
  ijs_server_done (ctx);
  
  free_param_list (pl);
  return status;
}

