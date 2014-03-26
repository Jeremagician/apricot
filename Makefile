# This Makefile is temporary project will use autotools soon
.PHONY=clean all

all:
	cd src && make
	cd cgi-bin && make
	@mv -f src/apricot .

clean:
	cd src && make clean
	cd cgi-bin && make clean
	@rm -f apricot
