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

/*
  Letter                  216 mm              279 mm         
                          (8.5 inches)        (11.0 inches)
  Legal                   216 mm              356 mm         
                          (8.5 inches)        (14.0 inches)
  Executive               184 mm (7.25        267 mm         
                          inches)             (10.5 inches)
  Executive (JIS)         216 mm (8.5 inches) 330 mm (12.99  
                                              inches)
  A5                      148 mm (5.83        210 mm         
                          inches)             (8.27 inches)
  A4                      210.0 mm (8.27      297 mm         
                          inches)             (11.69 inches)
  Envelope Commercial #10 105 mm (4.12        241 mm         
                          inches)             (9.5 inches)
  Envelope DL (ISO)       110 mm              220 mm         
                          (4.3 inches)        (8.66 inches)
  Envelope C5 (ISO)       162 mm              229 mm         
                          (6.38 inches)       (9.01 inches)
  Envelope B5 (ISO)       176 mm              250 mm         
                          (6.93 inches)       (9.84 inches)
  Envelope Monarch        98 mm               190 mm         
                          (3.87 inches)       (7.5 inches)
  B5 (ISO)                176 mm              250 mm         
                          (6.93 inches)       (9.84 inches
  B5 (JIS)                182 mm              257 mm         
		          (7.16 inches)       (10.12 inches)
  Double Postcard         148 mm              199 mm         
                          (5.83 inches)       (7.87 inches)		       
  16K                     197 mm              273 mm         
		          (7.75 inches)       (10.75 inches)
  8.5x13                  216 mm (8.5 inches) 330 mm (13.0
					      inches)
  11x17                   279 mm              431 mm         
		          (11.0 inches)       (17.0 inches)
  12x18                   304 mm              457 mm
		          (12.0 inches)       (18.0 inches)
  B4 (JIS)                257 mm              363 mm         
		          (10.12 inches)      (14.33 inches)
  A3                      296 mm              419 mm         
		          (11.69 inches)      (16.53 inches)
  8K		          273 mm         393 mm         
		          (10.75 inches) (15.50 inches)
*/

#include "pxl_mediasize.h"

/* 600/72 */
#define pt2pixel 8.3333333

/* horizontal rounded up to 32
   vertical rounded up to 128
   gives one mm either way, and 100 pixels unprintable margins */

#define CHECKSIZE(a,b,c,d, e) \
if ((a > c * pt2pixel - 224) && (a < c * pt2pixel - 144) \
    && (b > d * pt2pixel - 224) && ( b < d * pt2pixel - 48)) \
return e


/* return PCL XL paper codes */

unsigned char mediasize2pxlenum(int pixel_h, int pixel_v)
{
  CHECKSIZE(pixel_h, pixel_v, 612, 792,  eLetterPaper)    ;
  CHECKSIZE(pixel_h, pixel_v, 612, 1008, eLegalPaper)     ;
  CHECKSIZE(pixel_h, pixel_v, 595, 842,  eA4Paper)        ;
  CHECKSIZE(pixel_h, pixel_v, 522, 756,  eExecPaper)      ;
  CHECKSIZE(pixel_h, pixel_v, 792, 1224, eLedgerPaper)    ; /* Ledge is 11x17 landscape */
  CHECKSIZE(pixel_h, pixel_v, 842, 1191, eA3Paper)        ;
  CHECKSIZE(pixel_h, pixel_v, 297, 684,  eCOM10Envelope)  ;
  CHECKSIZE(pixel_h, pixel_v, 279, 540,  eMonarchEnvelope);
  CHECKSIZE(pixel_h, pixel_v, 459, 649,  eC5Envelope)     ;
  CHECKSIZE(pixel_h, pixel_v, 312, 624,  eDLEnvelope)     ;
  CHECKSIZE(pixel_h, pixel_v, 729, 1032, eJB4Paper)       ;
  CHECKSIZE(pixel_h, pixel_v, 516, 729,  eJB5Paper)       ;
  CHECKSIZE(pixel_h, pixel_v, 499, 709,  eB5Paper)        ;
  CHECKSIZE(pixel_h, pixel_v, 499, 709,  eB5Envelope)     ;
  CHECKSIZE(pixel_h, pixel_v, 282, 420,  eJPostcard)      ;
  CHECKSIZE(pixel_h, pixel_v, 420, 564,  eJDoublePostcard);
  CHECKSIZE(pixel_h, pixel_v, 420, 595,  eA5Paper)        ;
  CHECKSIZE(pixel_h, pixel_v, 297, 420,  eA6Paper)        ;
  CHECKSIZE(pixel_h, pixel_v, 363, 516,  eJB6Paper)       ;
  CHECKSIZE(pixel_h, pixel_v, 774, 1114, eJIS8K)          ;
  CHECKSIZE(pixel_h, pixel_v, 558, 774,  eJIS16K)         ;
  CHECKSIZE(pixel_h, pixel_v, 612, 935,  eJISExec)        ;

  return eDefault;
}
