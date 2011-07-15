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

/* PXLdefinitions */

/* Operator Tags */
#define PXL_BeginSession              0x41
#define PXL_EndSession                0x42
#define PXL_BeginPage                 0x43
#define PXL_EndPage                   0x44
#define PXL_SetColorSpace             0x6a
#define PXL_SetCursor                 0x6b
#define PXL_SetPageOrigin             0x75

#define PXL_VendorUnique              0x46

/* Data Type Tags */
#define PXL_ubyte(a)                0xc0, (a)
#define PXL_uint16(a)               0xc1, ((a)&0xff), (((a)>>8)&0xff)
#define PXL_uint32(a)               0xc2, ((a)&0xff), (((a)>>8)&0xff), (((a)>>16)&0xff), (((a)>>24)&0xff)
#define PXL_uint16_xy(a,b)          0xd1, ((a)&0xff), (((a)>>8)&0xff), ((b)&0xff), (((b)>>8)&0xff)
/* FIXME - need to be more sophisticated with signs */
#define PXL_sint16_xy(a,b)          0xd3, ((a)&0xff), (((a)>>8)&0xff), ((b)&0xff), (((b)>>8)&0xff) 

#define PXL_real32_xy               0xd5 

/* Attribute Tags */
#define PXL_attr_ubyte              0xf8

/* Attribute Name */

#define PXL_SetPageDefaultCTM         PXL_attr_ubyte, 0x74
              
#define PXL_BlockHeight              PXL_attr_ubyte, 99
#define PXL_ColorSpace               PXL_attr_ubyte, 3
#define PXL_ErrorReport              PXL_attr_ubyte, 143
#define PXL_Measure                  PXL_attr_ubyte, 134
#define PXL_MediaSize                PXL_attr_ubyte, 37
#define PXL_CustomMediaSize          PXL_attr_ubyte, 47
#define PXL_CustomMediaSizeUnits     PXL_attr_ubyte, 48
#define PXL_Orientation              PXL_attr_ubyte, 40
#define PXL_PageOrigin               PXL_attr_ubyte, 42
#define PXL_Point                    PXL_attr_ubyte, 76
#define PXL_SourceWidth              PXL_attr_ubyte, 108
#define PXL_SourceHeight             PXL_attr_ubyte, 107
#define PXL_StartLine                PXL_attr_ubyte, 109
#define PXL_UnitsPerMeasure          PXL_attr_ubyte, 137
#define PXL_VUExtension              PXL_attr_ubyte, 145
#define PXL_VUDataLength             PXL_attr_ubyte, 146
#define PXL_VUStripCount             PXL_attr_ubyte, 147
#define PXL_VUStripHeight            PXL_attr_ubyte, 148
#define PXL_VUVersion                PXL_attr_ubyte, 149
#define PXL_VUTextObjectType         PXL_attr_ubyte, 150
#define PXL_VUColorMode              PXL_attr_ubyte, 151
#define PXL_VUColorEnumeration       PXL_attr_ubyte, 152

#define PXL_JR3BeginImage            0x00,0x40,0x70,0x68
#define PXL_JR3ReadImage             0x01,0x40,0x70,0x68
#define PXL_JR3EndImage              0x02,0x40,0x70,0x68

#define JR3BeginImage            0x68704000
#define JR3ReadImage             0x68704001
#define JR3EndImage              0x68704002


/* Enumurations */
/* -- Measure */
#define PXL_eInch                    0 
/* -- ErrorReport */                  
#define PXL_eBackChAndErrPage        3

/* -- Orientation */
#define PXL_ePortraitOrientation     0

/* -- ColorSpace */
#define PXL_eSRGB                 6
#define PXL_eGraySub                 7


/* convenience function */
#define uint32_conv(a) ((a)&0xff), (((a)>>8)&0xff), (((a)>>16)&0xff), (((a)>>24)&0xff)
