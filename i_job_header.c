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

#include "jpeg_jcs.h"

int i_job_header(i_job_info_t *job_info)
{
  char temp_string[256];
  char *ts;
  int e;

  /* libjpeg.doc: step 3 of 7 */
  job_info->cinfo->image_width = job_info->pixel_h;
  job_info->cinfo->image_height = 128;
  job_info->cinfo->input_components = job_info->components;
  if(job_info->components == 1)
    {
      job_info->cinfo->in_color_space = JCS_GRAYSCALE;
    }
  else
    {
      job_info->cinfo->in_color_space = JCS_RGB; /* colorspace of input image */
    }
  jpeg_set_defaults(job_info->cinfo);
  job_info->cinfo->jpeg_color_space = JCS_JETREADY;

  ts = temp_string;

  /* PJL */
  ts += sprintf(ts, "\x01b%%-12345X@PJL SET STRINGCODESET=UTF8\x00a");

  if (job_info->components == 1) 
    {
      ts += sprintf(ts, "@PJL SET PLANESINUSE=1\x00a");
    }

  ts += sprintf(ts, "@PJL SET COPIES=1\x00a");
  if (job_info->duplex) {
    ts += sprintf(ts, "@PJL SET DUPLEX=ON\x00d\x00a");
    if (job_info->tumble) {
      ts += sprintf(ts, "@PJL SET BINDING=SHORTEDGE\x00d\x00a");
    } else {
      ts += sprintf(ts, "@PJL SET BINDING=LONGEDGE\x00d\x00a");
    }
  } else {
    ts += sprintf(ts, "@PJL SET DUPLEX=OFF\x00d\x00a");
  }
  ts += sprintf(ts, "@PJL SET RESOLUTION=600\x00a");
  ts += sprintf(ts, "@PJL SET TIMEOUT=90\x00a");
  ts += sprintf(ts, "@PJL ENTER LANGUAGE=PCLXL\x00a");
  ts += sprintf(ts, ") HP-PCL XL;3;0;Comment, PCL-XL JetReady generator\x00a");

  /* PCL XL */
  ts += sprintf(ts, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",
		PXL_ubyte(PXL_eInch),                PXL_Measure,
		PXL_ubyte(PXL_eBackChAndErrPage),    PXL_ErrorReport,
		PXL_uint16_xy(600, 600),             PXL_UnitsPerMeasure,
		PXL_BeginSession);

  e = i_write(job_info, temp_string, ts - temp_string);
  return 0;
} 
