all : test charge

clean :
	rm -f test
	rm -f charge
	rm -f *.cm*
	rm -f *.o
	rm -f *~

charge : charge.ml
	ocamlfind ocamlopt -o $@ -linkpkg -package netclient $^

test : test.ml
	ocamlfind ocamlopt -o $@ -linkpkg -package netclient $^
