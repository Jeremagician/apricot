open Http_client.Convenience
open Array

(* constantes *)
let home_page = "../bin/sites/home.html"
let base_url = "http://localhost:1970/"

let send_requests (file : string) (number : int) =
	for i = 1 to number
	do
		ignore (http_get (base_url^file))
	done
	
let () =
	if length Sys.argv != 2 then
		print_endline "Please give the number of requests to perform"
	else
		send_requests "" (int_of_string (get Sys.argv 1))
