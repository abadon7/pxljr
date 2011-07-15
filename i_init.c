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

#include <stdlib.h>
#include "i_job_info.h"

int i_init(i_job_info_t **i_job_info)
{
  i_job_info_t *job;
  job = (i_job_info_t *)calloc(1, sizeof(i_job_info_t));
  job->outfile = NULL;
  job->qfactor = 6; /* 6 for compatibility with hpijs ; 1 for auto-setting */
  
  /* libjpeg.doc : step 1 of 7 */
    job->cinfo = (struct jpeg_compress_struct *)calloc(1, sizeof(struct jpeg_compress_struct));
   job->jerr  = (struct jpeg_error_mgr *)calloc(1, sizeof(struct jpeg_error_mgr));
    job->cinfo->err = jpeg_std_error(job->jerr);
   jpeg_create_compress(job->cinfo);
   job->compressedbuffer = NULL;
   job->buffersize = NULL;

   *i_job_info = job;
  return 0;
}

