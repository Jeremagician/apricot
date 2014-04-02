 #!/bin/sh

echo -n 'bin_PROGRAMS = bin/apricot bin/sites/cgi-bin/adder bin/sites/cgi-bin/traceroute bin/sites/cgi-bin/admin bin/sites/cgi-bin/cookie' > Makefile.am

echo ' ' >> Makefile.am
echo -n 'bin_apricot_SOURCES = ' >> Makefile.am

for f in $(ls src/*.c src/include/apricot/*.h)
do
	echo -n "$f " >> Makefile.am
done

echo '
bin_apricot_LDFLAGS = -lpthread `find /usr/lib -name libmagic.so* -type f` -lrt
bin_apricot_CFLAGS = -Wall -Werror -O3 -Isrc/include

apricotconfdir=$(sysconfdir)/apricot
apricotconf_DATA = bin/apricot.conf

EXTRA_DIST = bin/apricot.conf

bin_sites_cgi_bin_adder_SOURCES = cgi-bin/adder.c cgi-bin/csapp.c cgi-bin/csapp.h
bin_sites_cgi_bin_adder_LDFLAGS = -lpthread

bin_sites_cgi_bin_traceroute_SOURCES = cgi-bin/traceroute.c cgi-bin/csapp.c cgi-bin/csapp.h
bin_sites_cgi_bin_traceroute_LDFLAGS = -lpthread

bin_sites_cgi_bin_admin_SOURCES = cgi-bin/admin.c cgi-bin/csapp.c cgi-bin/csapp.h
bin_sites_cgi_bin_admin_LDFLAGS = -lpthread -lrt

bin_sites_cgi_bin_cookie_SOURCES = cgi-bin/cookie.c
bin_sites_cgi_bin_cookie_LDFLAGS = -lrt

ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = subdir-objects

distcheck :

' >> Makefile.am

aclocal
autoconf
automake
