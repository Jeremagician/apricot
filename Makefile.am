bin_PROGRAMS = bin/apricot bin/sites/cgi-bin/adder 
bin_apricot_SOURCES = src/conf.c src/csapp.c src/dispatcher.c src/dynamic.c src/http_error.c src/http_header.c src/include/apricot/conf.h src/include/apricot/csapp.h src/include/apricot/dispatcher.h src/include/apricot/dynamic.h src/include/apricot/http_codes.h src/include/apricot/http_error.h src/include/apricot/http_header.h src/include/apricot/log.h src/include/apricot/master.h src/include/apricot/mime.h src/include/apricot/pool.h src/include/apricot/rewrite.h src/include/apricot/static.h src/include/apricot/utils.h src/include/apricot/worker.h src/log.c src/main.c src/master.c src/mime.c src/pool.c src/rewrite.c src/static.c src/utils.c src/worker.c 
bin_apricot_LDFLAGS = -lpthread -lmagic
bin_apricot_CFLAGS = -Wall -Werror -Isrc/include

apricotconfdir=$(sysconfdir)/apricot
apricotconf_DATA = bin/apricot.conf

EXTRA_DIST = bin/apricot.conf

bin_sites_cgi_bin_adder_SOURCES = cgi-bin/adder.c cgi-bin/csapp.c cgi-bin/csapp.h
bin_sites_cgi_bin_adder_LDFLAGS = -lpthread

ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = subdir-objects

distcheck :


