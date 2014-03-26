# This Makefile is temporary project will use autotools soon
.PHONY=clean all

all:
	cd src && make
	@mv -f src/apricot .

clean:
	cd src && make clean
	@rm -f apricot
