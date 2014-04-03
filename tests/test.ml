open Http_client.Convenience
open Unix

(* constantes *)
let home_page = "../bin/sites/home.html"
let base_url = "http://localhost:1970/"

(* lit un fichier et renvoit son contenu dans une chaine *)
let read_file (name : string) =
	let f = open_in name in
	let file_size = in_channel_length f in
	let buf = String.create file_size in	(* on crée un buffer de la taille du fichier *)
		ignore (input f buf 0 file_size);
		close_in f;
		buf

(* type mime d'une ressource *)
let get_mime_type (uri : string) =
	let msg = http_get_message uri in
	let response_header = msg#response_header in
	let content_type = response_header#content_type () in
		match content_type with
			 (main_mime_type, _) -> main_mime_type

(* code de retour http d'une requete *)
let get_http_code (uri : string) =
	(http_get_message uri)#response_status_code

(* teste si la page est égale au contenu de son fichier *)
let test_eq_page (uri : string) (f : string) =
	let received_from_server = http_get uri in
	let from_file = read_file f in
		received_from_server = from_file

(* TEST1 : corps de la page html égal au contenu du fichier *)
let test1 () =
	if test_eq_page base_url home_page then
		begin
			print_endline "homepage OK";
			true
		end
	else
		begin
			print_endline "FAIL";

			print_endline "--- file ---";
			print_endline (read_file home_page);
			print_endline "-------------";

			print_endline "--- web page ---";
			print_endline (http_get (base_url^"/"));
			print_endline "-------------";
			false
		end

(* TEST2 : verifie le code de retour http *)
let test2 (url : string) (code : int) =
	let result = get_http_code (base_url^url) in
		if result = code then
			begin
				print_endline (url^" OK "^(string_of_int result));
				true
			end
		else
			begin
				print_endline ("FAIL " ^ (string_of_int result));
				false
			end

(* TEST3 : verifie l'arret du serveur avec l'interface d'admin *)
let test3 () =
	try
		ignore (http_get (base_url^"cgi-bin/admin?action=serverstop"));
		sleep 2;
		ignore (http_get base_url);
		print_endline "serverstop FAIL";
		false
	with
		_ -> print_endline "serverstop OK"; true
		
(* TEST4 : gestion des types mime *)
let test4 (file : string) (mime_type : string) =
	let uri = (base_url ^ file) in
	let received_mime_type = get_mime_type uri in
		if received_mime_type = mime_type then
			begin
				print_endline ("mime type of " ^ uri ^ " OK");
				true
			end
		else
			begin
				print_endline ("mime type of " ^uri ^ "FAIL");
				false
			end

let () =
	let result = test1 () && 
				test2 "" 200 &&
				test2 "pyramide.jpg" 200 &&
				test2 "notfound.html" 404 &&
				test2 "cgi-bin/does-not-exist" 404 && (* cgi inexistant *)
				test2 "cgi-bin/does-not-exist?var=value" 404 && (* cgi inexistant avec parametres *)
				test2 "cgi-bin/adder" 200 &&
				test2 "cgi-bin/admin" 200 &&
			 (*	test3 () && test l'arret via l'interface admin *)
				test4 "pyramide.jpg" "image/jpeg" &&
				test4 "home.html" "text/html" &&
				test4 "cgi-bin/admin" "text/html" &&
				test4 "cgi-bin/traceroute" "text/plain" &&
				test4 "factorial.lisp" "text/plain" 
	in	
		exit (if result then 0 else 1)
