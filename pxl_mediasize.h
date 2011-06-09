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

/* PCL XL paper codes */
typedef enum {
eDefault        = 96, 
eLetterPaper    = 0, 
eLegalPaper     = 1, 
eA4Paper        = 2,
eExecPaper      = 3, 
eLedgerPaper    = 4, 
eA3Paper        = 5, 
eCOM10Envelope  = 6, 
eMonarchEnvelope= 7, 
eC5Envelope     = 8, 
eDLEnvelope     = 9, 
eJB4Paper       = 10, 
eJB5Paper       = 11, 
eB5Paper        = 13,
eB5Envelope     = 12, 
eJPostcard      = 14, 
eJDoublePostcard= 15,
eA5Paper        = 16, 
eA6Paper        = 17, 
eJB6Paper        = 18, 
eJIS8K          = 19,
eJIS16K         = 20, 
eJISExec        = 21
} MediaSize;

unsigned char mediasize2pxlenum(int pixel_h, int pixel_v);
