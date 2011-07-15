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
#include <stdlib.h>
#define JPEG_INTERNALS
#include "jpeglib.h"
#include "i_job_info.h"
#include "mode12.h"


int i_compress_row(i_job_info_t *job_info, 
			 char *ptr_row_current, 
			 char *ptr_row_prev, int wbytes)
{
  if (job_info->compressedbuffer == NULL)
    {
      job_info->compressedbuffer = (unsigned char *)calloc(1, job_info->pixel_h * 128 *4); /* worst case */
      job_info->buffersize =(int *)calloc(1, sizeof(int)); 
      if (job_info->qfactor)
	{
      jpeg_buffer_dest(job_info->cinfo, job_info->compressedbuffer, job_info->buffersize);
      job_info->cinfo->write_JFIF_header = FALSE;
      job_info->cinfo->write_Adobe_marker = FALSE;
      jpeg_start_compress(job_info->cinfo, FALSE);
      job_info->cinfo->master->call_pass_startup = FALSE;
      jpeg_suppress_tables(job_info->cinfo, TRUE);
	}
      else
	{
      *(job_info->buffersize) = 2; /* dirty trick for mode 12 */
	}

    }


  if(job_info->qfactor > 0)
    {
      JSAMPROW row_pointer[1];
      row_pointer[0]  = ptr_row_current;
      jpeg_write_scanlines(job_info->cinfo, row_pointer, 1);
    }
  else
    {
      unsigned char *buffer = job_info->compressedbuffer + *(job_info->buffersize);

      if (job_info->components == 1)
	{
	  *(job_info->buffersize) += mode12gray_compress(job_info->pixel_h, ptr_row_current,
							 ptr_row_prev, buffer);
	}
      else
	{
	  *(job_info->buffersize) += mode12color_compress(job_info->pixel_h, ptr_row_current,
							 ptr_row_prev, buffer);	  
	}
    }
  return 0;

}
