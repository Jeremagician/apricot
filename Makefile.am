bin_PROGRAMS = bin/apricot 
bin_apricot_SOURCES = src/csapp.c src/dispatcher.c src/dynamic.c src/include/apricot/csapp.h src/include/apricot/dispatcher.h src/include/apricot/dynamic.h src/include/apricot/http_codes.h src/include/apricot/log.h src/include/apricot/master.h src/include/apricot/pool.h src/include/apricot/rewrite.h src/include/apricot/static.h src/include/apricot/worker.h src/log.c src/main.c src/master.c src/pool.c src/rewrite.c src/static.c src/worker.c 
bin_apricot_LDFLAGS = -lpthread
bin_apricot_CFLAGS = -Wall -Werror -Isrc/include

apricotconfdir=$(sysconfdir)/apricot
apricotconf_DATA = bin/apricot.conf

EXTRA_DIST = bin/apricot.conf

ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = subdir-objects

distcheck :


