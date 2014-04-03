all : test

clean :
	rm test
	rm *.cm*
	rm *.o
	rm *~

test : test.ml
	ocamlfind ocamlopt -o $@ -linkpkg -package netclient $^
