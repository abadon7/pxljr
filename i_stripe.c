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
#include <stdlib.h>
#include "i_job_info.h"
#include "pxl_macros.h"

static int i_stripe_header(i_job_info_t *job_info, int startline, int blockheight, int datalength)
{
  char temp_string[256];
  char *ts;
  int e;

  ts = temp_string;

  ts += sprintf(ts, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",
		PXL_uint32(JR3ReadImage), PXL_VUExtension,
		PXL_uint16(startline), PXL_StartLine,
		PXL_uint16(blockheight), PXL_BlockHeight,
		PXL_ubyte(0), PXL_VUTextObjectType,
		PXL_uint32(0x00040000), PXL_VUVersion,
		PXL_uint32(datalength), PXL_VUDataLength,
		PXL_VendorUnique);

  e = i_write(job_info, temp_string, ts - temp_string);
  return e;
} 

int i_print_stripe(i_job_info_t *job_info, int i_stripe)
{
  char temp_string[256];
  char *ts;
  int e;

  ts = temp_string;

  if(job_info->qfactor)
    jpeg_finish_compress(job_info->cinfo);

  i_stripe_header(job_info, i_stripe *128, 128,*(job_info->buffersize) + 4);

  ts += sprintf(ts, "%c%c", 
		(job_info->qfactor? 0x21:0x11), 
		0x90);
  ts += sprintf(ts, "%c%c%c%c", uint32_conv(*(job_info->buffersize) - 2)); 

  e = i_write(job_info, temp_string, ts - temp_string);
  /* skip over the SOI marker */
  e = i_write(job_info, job_info->compressedbuffer + 2 , *(job_info->buffersize) - 2);

  if (job_info->compressedbuffer != NULL)
    {
      free(job_info->compressedbuffer);
      free(job_info->buffersize);
      job_info->compressedbuffer = NULL;
      job_info->buffersize = NULL;
    }

  return 0;
}
