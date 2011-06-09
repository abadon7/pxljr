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
#define JPEG_INTERNALS
#include "jinclude.h"
#include "jpeglib.h"
#include "i_job_info.h"


EXTERN(void) rgb_jrycc_start (j_compress_ptr cinfo);

EXTERN(void) rgb_jrycc_convert (j_compress_ptr cinfo,
			JSAMPARRAY input_buf, JSAMPIMAGE output_buf,
			JDIMENSION output_row, int num_rows);



/* Private subobject */

typedef struct {
  struct jpeg_color_converter pub; /* public fields */

  /* Private state for RGB->YCC conversion */
  INT32 * rgb_ycc_tab;          /* => table for RGB to YCbCr conversion */
} my_color_converter;

typedef my_color_converter * my_cconvert_ptr;

/* jpeg quantization table */
static unsigned int qtable[64] = { 2,3,4,5,5,5,5,5,
				   3,6,5,8,5,8,5,8,
				   4,5,5,5,5,5,5,5,
				   5,8,5,8,5,8,5,8,
				   5,5,5,5,5,5,5,5,
				   5,8,5,8,5,8,5,8,
				   5,5,5,5,5,5,5,5,
				   5,8,5,8,5,8,5,8 };

void pxl_set_jpeg_custom_params(i_job_info_t *job_info)
{
  int i,j;

  /* the driver uses 1 for auto setting, 6 for max compression */
  int qfactor = job_info->qfactor; 
  
  for(i = 0; i < 3; i++)
    {
      jpeg_add_quant_table(job_info->cinfo, i, qtable, 0, FALSE);
      
      for(j =0; j < 64; j++)
	{
	  job_info->cinfo->quant_tbl_ptrs[i]->quantval[j] = qtable[j] * qfactor ;
	}

      job_info->cinfo->quant_tbl_ptrs[i]->quantval[0] = qtable[0];
 
      job_info->cinfo->quant_tbl_ptrs[i]->quantval[1] = qtable[1] * (1 + (qfactor - 1) * 0.25) + 0.5;
      job_info->cinfo->quant_tbl_ptrs[i]->quantval[8] = qtable[8] * (1 + (qfactor - 1) * 0.25) + 0.5;

      job_info->cinfo->quant_tbl_ptrs[i]->quantval[2]  = qtable[2]  * (1 + (qfactor - 1) * 0.5) + 0.5;
      job_info->cinfo->quant_tbl_ptrs[i]->quantval[9]  = qtable[9]  * (1 + (qfactor - 1) * 0.5) + 0.5;
      job_info->cinfo->quant_tbl_ptrs[i]->quantval[16] = qtable[16] * (1 + (qfactor - 1) * 0.5) + 0.5;

    }

  job_info->cinfo->comp_info[0].h_samp_factor = 1;
  job_info->cinfo->comp_info[0].v_samp_factor = 1;
  return;
}
