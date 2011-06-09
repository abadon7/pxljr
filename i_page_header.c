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

#include <stdio.h>
#include <string.h>
#include "i_job_info.h"
#include "pxl_macros.h"
#include "pxl_mediasize.h"
#include "pxl_jpeg_header.h"

int i_page_header(i_job_info_t *job_info)
{
  char temp_string[256];
  char *ts;
  int e;
  int i, j;

  unsigned char mediasize_code = mediasize2pxlenum(job_info->pixel_h, job_info->pixel_v);

  if (job_info->qfactor)
    pxl_set_jpeg_custom_params(job_info);

  ts = temp_string;

  
  if( mediasize_code != eDefault)
    {
      ts += sprintf(ts, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",
		PXL_ubyte(PXL_ePortraitOrientation), PXL_Orientation,
		PXL_ubyte(mediasize_code), PXL_MediaSize,
		PXL_uint16_xy(job_info->pixel_h, job_info->pixel_v), PXL_SetPageDefaultCTM,
		PXL_BeginPage);
    }
  else
    {
      /* This is extremely endian and archtecture specific code - REWRITE */
      unsigned char buffer[8];
      float x,y;
      x = (job_info->pixel_h + 200.0) /600.0;
      y = (job_info->pixel_v + 200.0) /600.0;
      memcpy(buffer, &x, 4);
      memcpy(buffer + 4 , &y, 4);
      ts += sprintf(ts, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",
		    PXL_ubyte(PXL_ePortraitOrientation), PXL_Orientation,
		    PXL_real32_xy, buffer[0], buffer[1], buffer[2], buffer[3], 
		    buffer[4], buffer[5], buffer[6], buffer[7], PXL_CustomMediaSize,
		    PXL_ubyte(PXL_eInch), PXL_CustomMediaSizeUnits,
		    PXL_uint16_xy(job_info->pixel_h, job_info->pixel_v), PXL_SetPageDefaultCTM,
		    PXL_BeginPage);
    }

  ts += sprintf(ts, "%c%c%c%c%c%c%c%c",
		PXL_sint16_xy(100, 100), PXL_PageOrigin,
		PXL_SetPageOrigin);
  
  ts += sprintf(ts, "%c%c%c%c%c%c%c%c",
		PXL_sint16_xy(0, 0),  PXL_Point,
		PXL_SetCursor);

  if(job_info->components == 1)
    {
      ts += sprintf(ts, "%c%c%c%c%c", 
		    PXL_ubyte(PXL_eGraySub), PXL_ColorSpace,
		    PXL_SetColorSpace);
    }
  else
    {
      ts += sprintf(ts, "%c%c%c%c%c", 
		    PXL_ubyte(PXL_eSRGB), PXL_ColorSpace,
		    PXL_SetColorSpace);
    }

  ts += sprintf(ts, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",
		PXL_uint32(JR3BeginImage), PXL_VUExtension,
		PXL_uint16(job_info->pixel_h), PXL_SourceWidth,
		PXL_uint16(job_info->pixel_v), PXL_SourceHeight,
		PXL_uint16(54), PXL_VUStripCount,
		PXL_uint16(128), PXL_VUStripHeight,
		PXL_ubyte((job_info->components == 1)? 0 : 4), PXL_VUColorMode,
		PXL_ubyte(0), PXL_VUColorEnumeration,
		PXL_uint32(0x00040000), PXL_VUVersion,
		PXL_uint32((job_info->qfactor? 824 : 0)), PXL_VUDataLength,
		PXL_VendorUnique);

  e = i_write(job_info, temp_string, ts - temp_string);
  if(job_info->qfactor)
    {
      e = i_write(job_info, qtable_header, sizeof(qtable_header));
      for (i = 0; i < 3 ; i++)
	for (j = 0; j < 64 ; j++)
	  {
	    unsigned char temp[4] = {uint32_conv(job_info->cinfo->quant_tbl_ptrs[i]->quantval[j])};
	    i_write(job_info, temp, sizeof(temp));
	  }
      e = i_write(job_info, ((job_info->components == 1) ? control_header_mono : control_header_color), sizeof(control_header_color));
    }
  return 0;
} 
