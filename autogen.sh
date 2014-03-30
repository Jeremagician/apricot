 #!/bin/sh

echo -n 'bin_PROGRAMS = bin/apricot bin/cgi-bin/adder' > Makefile.am

echo ' ' >> Makefile.am
echo -n 'bin_apricot_SOURCES = ' >> Makefile.am

for f in $(ls src/*.c src/include/apricot/*.h)
do
	echo -n "$f " >> Makefile.am
done

echo '
bin_apricot_LDFLAGS = -lpthread
bin_apricot_CFLAGS = -Wall -Werror -Isrc/include

apricotconfdir=$(sysconfdir)/apricot
apricotconf_DATA = bin/apricot.conf

EXTRA_DIST = bin/apricot.conf

bin_cgi_bin_adder_SOURCES = cgi-bin/adder.c cgi-bin/csapp.c cgi-bin/csapp.h
bin_cgi_bin_adder_LDFLAGS = -lpthread

ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = subdir-objects

distcheck :

' >> Makefile.am

aclocal
autoconf
automake
