#!/usr/bin/perl -w
#
# Copyright (c) 2006 Hin-Tak Leung

#
#    This program is free software; you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation; either version 2 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program; if not, write to the Free Software
#    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
#

use strict;
use Getopt::Std;

$ENV{'PATH'} = ".:" . $ENV{'PATH'};

our($opt_g, $opt_q);

getopts('gq:');

my @extraopts = ();

my ($colorcode, $gammafile, $quality);

if ($opt_g) {
    $colorcode = 0;
    unless ($ARGV[0] =~ /hpijs/)
    {
	push @extraopts, "-sProcessColorModel=DeviceGray"; 
    }
}
else
{
    $colorcode = 2;
}

if ($opt_q)
{
    $quality = "$opt_q";
}
else
{
    $quality = "2";
}


if (-f "gs-gamma.ps") 
{
    $gammafile = "gs-gamma.ps";
}
elsif (-f "/usr/doc/pxljr-1.1/gs-gamma.ps")
{
    $gammafile = "/usr/doc/pxljr-1.1/gs-gamma.ps";
}


my @command = ("gs",
      "-dFIXEDMEDIA",
      "-q",
      "-dBATCH",
      "-dQUIET",
      "-dNOPAUSE",
      "-sDEVICE=ijs",
      "-sIjsServer=$ARGV[0]",
      "-dIjsUseOutputFD",
      "-sOutputFile=$ARGV[2]",
      @extraopts,
      "-dDEVICEWIDTHPOINTS=595",
      "-dDEVICEHEIGHTPOINTS=842",
      "-r600",
      "-sIjsParams=Quality:Quality=${quality},Quality:ColorMode=${colorcode},Quality:MediaType=0,Quality:PenSet=2",
      "-sDeviceManufacturer=HEWLETT-PACKARD",
      "-sDeviceModel=hp color LaserJet 3550",
      $gammafile, 
      $ARGV[1]); 

exec(@command);

