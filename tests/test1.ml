open Http_client.Convenience

(* constantes *)
let home_page = "../bin/sites/home.html"
let base_url = "http://localhost:1970"

(* lit un fichier et renvoit son contenu dans une chaine *)
let read_file (name : string) =
	let f = open_in name in
	let file_size = in_channel_length f in
	let buf = String.create file_size in	(* on crée un buffer de la taille du fichier *)
		ignore (input f buf 0 file_size);
		close_in f;
		buf

(* teste si la page est égale au contenu de son fichier *)
let test_eq_page (uri : string) (f : string) =
	let received_from_server = http_get uri in
	let from_file = read_file f in
		received_from_server = from_file

let () =
	if test_eq_page (base_url^"/") home_page then
		begin
			print_endline "equal";
			exit 0
		end
	else
		begin
			print_endline "not equal";

			print_endline "--- file ---";
			print_endline (read_file home_page);
			print_endline "-------------";

			print_endline "--- web page ---";
			print_endline (http_get (base_url^"/"));
			print_endline "-------------";
			exit 1
		end
