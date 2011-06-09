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
#include "mode12.h"


#define min(a, b) (a>b? b:a)

#define PIXELWIDTH 3

int mode12color_compress(int pixelcount, unsigned char *current_start,
			unsigned char *previous_start, unsigned char *compressed)
{
  unsigned char *ptr_current = current_start;
  unsigned char *ptr_previous = previous_start;
  unsigned char *comp_ptr = compressed;
  unsigned char cached_color[PIXELWIDTH] = {0xff, 0xff, 0xff}; /* MORE: what if this is the first literal? */
  unsigned char *current_end = current_start + pixelcount * PIXELWIDTH;
  
  while (ptr_current < current_end) { /* Detect a run of unchanged bytes. */
    unsigned char *run = ptr_current;
    unsigned char *diff_start;
    int offset;
    
    while (ptr_current < current_end && !memcmp(ptr_current, ptr_previous, PIXELWIDTH)) {
      ptr_current+=PIXELWIDTH, ptr_previous+=PIXELWIDTH;
    }
    
    /* Detect a run of changed bytes. */
    /* We know that *ptr_current != *ptr_previous. */
    diff_start = ptr_current;
    while (ptr_current < current_end && memcmp(ptr_current, ptr_previous, PIXELWIDTH)) {
      ptr_current+=PIXELWIDTH, ptr_previous+=PIXELWIDTH;
    }
    /* Now [run..diff_start) are unchanged, and */
    /* [diff_start..ptr_current) are changed. */
    offset = (diff_start - run)/PIXELWIDTH;
    if (diff_start == current_end)
      {
	/* coding the last offset as RLE of count 2 without the byte following */
	if (offset >= 3)
	  {
	    *comp_ptr++ = eRLE | eeNewPixel | (3 << 3);
	    offset -=3;
	    while (offset >= 255)
	      {
		*comp_ptr++ = 255;
		offset -= 255;
	      }
	    *comp_ptr++ = offset;	    
	  }
	else
	  {
	    *comp_ptr++ = eRLE | eeNewPixel | (offset << 3);
	  }
      }
    else
      {
	/* go back and see if it is rle or literal */
	unsigned char rle[PIXELWIDTH];
	unsigned char pixel_src;
	unsigned char cmd;
	int count;

	memcpy(rle, diff_start, PIXELWIDTH);
	
	ptr_current = diff_start;

	/* MORE: This is the windows driver order - the compression
	   efficiency is all the time, so it is really up to
	   choice */
	if (0)
	  {
	  }
	/*
	else if ((ptr_current > current_start) && !memcmp(rle, (ptr_current -1),PIXELWIDTH))
	  {
	    pixel_src = eeWPixel;
	  }
	else if ((ptr_current - 1 < current_end) &&  
		 (!memcmp(rle, previous_start + (ptr_current - current_start) + PIXELWIDTH, PIXELWIDTH)))
	  {
	    pixel_src = eeNEPixel;
	  }
	   else if (!memcmp(rle, cached_color, PIXELWIDTH))
	  {
	    pixel_src = eeCachedColor;
	  }
	*/
	else
	  {
	    pixel_src = eeNewPixel;
	    memcpy(cached_color, rle, PIXELWIDTH);
	  }
	
	while (!memcmp(rle, ptr_current, PIXELWIDTH))
	  {
	    ptr_current+=PIXELWIDTH;
	  }
	
	if (ptr_current - diff_start > PIXELWIDTH)
	  {
	    /* rle */
	    ptr_previous = (previous_start + (ptr_current - current_start)); 
	    count = (ptr_current - diff_start)/PIXELWIDTH - 2;
	    cmd = eRLE;
	  }
	else
	  {
	    /* MORE: literal can't go beyond 8 */
	    count = min((ptr_current - diff_start),8 * PIXELWIDTH);
	    ptr_previous = previous_start + (count + diff_start - current_start); 
	    ptr_current = diff_start + count ;
	    count /= PIXELWIDTH;
	    count -=1;
	    cmd = eLiteral;
	  }
	
	*comp_ptr++ = cmd | pixel_src | (min(offset, 3) <<3) | min(count,7);

	if (offset >= 3)
	  {
	    offset -=3;
	    while (offset >= 255)
	      {
		*comp_ptr++ = 255;
		offset -= 255;
	      }
	    *comp_ptr++ = offset;
	  }
	
	if(pixel_src == eeNewPixel)
	  {
	    *comp_ptr++ = *diff_start;
	    *comp_ptr++ = *(diff_start+1);
	    *comp_ptr++ = *(diff_start+2);
	  }
	
	if((cmd == eLiteral) && (count > 0))
	  {
	    unsigned char *temp_ptr = diff_start + PIXELWIDTH;
	    /* we are not reusing count in this case */
	      while (count > 0)
		{
		  *comp_ptr++ = *temp_ptr;
		  *comp_ptr++ = *(temp_ptr+1);
		  *comp_ptr++ = *(temp_ptr+2);
		  count--;
		}
	  }
	if (count >= 7)
	  {
	    count -=7;
	    while (count >= 255)
	      {
		*comp_ptr++ = 255;
		count -= 255;
	      }
	    *comp_ptr++ = count;
	  }
      } /* not the last one */
  } /* while not end */
  return comp_ptr - compressed;
}
