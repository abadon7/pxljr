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

#define I_VERSION "1.0"

#include <stdio.h>
#include <sys/time.h>
#include "jpeglib.h"

/* job_info is created as a simplier version of
   ParamList which can be used by the header/etc routes
   without passing along string parameters */

typedef struct _i_job_info i_job_info_t; 

struct _i_job_info {
  FILE *outfile;
  int model;
  int components;
  int dpi_h;
  int dpi_v;
  int pixel_h;
  int pixel_v;
  int papertype;
  int copies;
  struct jpeg_compress_struct *cinfo;
  struct jpeg_error_mgr *jerr;
  unsigned char *compressedbuffer;
  int *buffersize;
  int qfactor;
};

int i_job_header(i_job_info_t *job_info);

int i_page_header(i_job_info_t *job_info);

int i_page_footer(i_job_info_t *job_info);

int i_job_footer(i_job_info_t *job_info);

int i_write(i_job_info_t *job_info, char *buffer, int length); 

int i_compress_row(i_job_info_t *job_info, char *ptr_current, char *ptr_row_prev, int wbytes);

int i_print_stripe(i_job_info_t *job_info, int i_stripe);

int i_status(int argc, char **argv);

int i_init(i_job_info_t **job_info);

int i_exit(i_job_info_t *job_info);

#define MODEL_CLJ3500 0
#define MODEL_CLJ3550 1

#define JOB_STARTED_NO  0
#define JOB_STARTED_YES 1

void pxl_set_jpeg_custom_params(i_job_info_t *job_info);

void jpeg_buffer_dest(j_compress_ptr cinfo, JOCTET* outbuff, int *current_size);

