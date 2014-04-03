bin_PROGRAMS = bin/apricot bin/sites/cgi-bin/adder bin/sites/cgi-bin/traceroute bin/sites/cgi-bin/admin bin/sites/cgi-bin/cookie 
bin_apricot_SOURCES = src/conf.c src/cookie.c src/csapp.c src/dispatcher.c src/dynamic.c src/http_error.c src/http_header.c src/include/apricot/conf.h src/include/apricot/cookie.h src/include/apricot/csapp.h src/include/apricot/dispatcher.h src/include/apricot/dynamic.h src/include/apricot/http_codes.h src/include/apricot/http_error.h src/include/apricot/http_header.h src/include/apricot/log.h src/include/apricot/magic.h src/include/apricot/master.h src/include/apricot/mime.h src/include/apricot/mqueue.h src/include/apricot/pool.h src/include/apricot/rewrite.h src/include/apricot/static.h src/include/apricot/utils.h src/include/apricot/worker.h src/log.c src/main.c src/master.c src/mime.c src/mqueue.c src/pool.c src/rewrite.c src/static.c src/utils.c src/worker.c 
bin_apricot_LDFLAGS = -lpthread `find /usr/lib -name libmagic.so* -type f` -lrt
bin_apricot_CFLAGS = -Wall -Werror -O3 -Isrc/include

apricotconfdir=$(sysconfdir)/apricot
apricotconf_DATA = bin/apricot.conf

EXTRA_DIST = bin/apricot.conf

bin_sites_cgi_bin_adder_SOURCES = cgi-bin/adder.c cgi-bin/cgi.h cgi-bin/cgi.c

bin_sites_cgi_bin_traceroute_SOURCES = cgi-bin/traceroute.c cgi-bin/cgi.h cgi-bin/cgi.c

bin_sites_cgi_bin_admin_SOURCES = cgi-bin/admin.c cgi-bin/cgi.h cgi-bin/cgi.c
bin_sites_cgi_bin_admin_LDFLAGS = -lrt

bin_sites_cgi_bin_cookie_SOURCES = cgi-bin/cookie.c
bin_sites_cgi_bin_cookie_LDFLAGS = -lrt

ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = subdir-objects

distcheck :


