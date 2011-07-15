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

/* control register 0x8000, byte to come */
static unsigned char qtable_header[] = 
  {
    0x00,0x80,0x00,0x03,0x00,0x00
  };


/* control register 0x8001, byte to come, data */
static unsigned char control_header_color[] = 
  {
    0x01,0x80,0x2C,0x00,0x00,0x00,  /* control register 0x8001, byte to come */
    0x01,0xE0,0x14,0x66,            /* Color 0x6614_E001 */
    0x01,0x00,0x00,0x00,            /* Color, 13 bit precision for the CSC matrix: MORE */
    0x00,0x20,0x00,0x00,   0x00,0x00,0x00,0x00,   0x00,0x00,0x00,0x00,
    0x00,0x20,0x00,0x00,   0x00,0xE0,0x00,0x00,   0x00,0x00,0x00,0x00,
    0x00,0x20,0x00,0x00,   0x00,0x00,0x00,0x00,   0x00,0xE0,0x00,0x00    
  };

static unsigned char control_header_mono[] = 
  {
    0x01,0x80,0x2C,0x00,0x00,0x00,  /* control register 0x8001, byte to come */
    0x05,0xE0,0x00,0x00,            /* Mono 0x0000_E005 */
    0x00,0x00,0x00,0x00,            /* Mono, 13 bit precision for the CSC matrix: MORE */
    0x00,0x20,0x00,0x00,   0x00,0x00,0x00,0x00,   0x00,0x00,0x00,0x00,
    0x00,0x20,0x00,0x00,   0x00,0xE0,0x00,0x00,   0x00,0x00,0x00,0x00,
    0x00,0x20,0x00,0x00,   0x00,0x00,0x00,0x00,   0x00,0xE0,0x00,0x00    
  };
