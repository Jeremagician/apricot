all : test1

clean :
	rm test1
	rm *.cm*
	rm *.o

test1 : test1.ml
	ocamlfind ocamlopt -o $@ -linkpkg -package netclient $^
