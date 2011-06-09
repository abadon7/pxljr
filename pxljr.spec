Summary: Ghostscript IJS Plugin for the HP Color Laserjet 3500/3550 printers
Name: pxljr
Version: 1.1
Release: 1
Copyright: Copyright  (c) 2005 Hin-Tak Leung
Group: Applications/Graphics
Source0:  http://osdn.dl.sourceforge.net/sourceforge/hp-pxl-jetready/pxljr-%{version}.tgz
URL: http://sourceforge.net/projects/hp-pxl-jetready/
Vendor: http://sourceforge.net/projects/hp-pxl-jetready/
Packager: Hin-Tak Leung
Requires: ghostscript >= 6.53
#Requires: ghostscript-fonts
Conflicts: ghostscript = 8.00, ghostscript = 8.14
BuildRoot: /var/tmp/pxljr-root

%description
Support for the HP Color Laserjet 3500/3550 printer family under linux and 
other unix-like systems. This effort is not edorsed by nor affiliated with 
HP. 

%prep

%setup

%build

./autogen.sh 
./configure --prefix=/usr
make

%install

rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/usr/bin
install ijs_pxljr pxljr-print.pl $RPM_BUILD_ROOT/usr/bin

file $RPM_BUILD_ROOT/usr/bin/* | grep ELF | cut -d':' -f1 | xargs strip 

%clean

[ "$RPM_BUILD_ROOT" != "/" ] && rm -rf $RPM_BUILD_ROOT

%post

echo "Done."
echo ""
echo -n "Remember to install the PPD file and restart CUPS afterwards." 
echo ""
exit 0

%files
%defattr(-, root, root)
%doc gs-gamma.ps FAQ* *.ppd
/usr/bin/*

%changelog
* Sun Dec 31 2006 Hin-Tak Leung <htl10@users.sourceforge.net> 
- release 1.1

* Sat Sep 24 2005 Hin-Tak Leung <htl10@users.sourceforge.net> 
- first ever release available in RPMS
