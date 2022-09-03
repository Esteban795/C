let afficher_caracteres stream str len =   
  for i = 0 to len - 1 do 
    Printf.fprintf stream "%c" str.[i];
  done;
  Printf.fprintf stream "\n"

let read_file () = 
  let f = open_in "test" in
  let rec loop () =  
    try
      let s = input_line f in 
      print_newline (print_endline s);
      loop ()
    with 
      |End_of_file -> () in
    loop ();
    close_in f

let copy nom1 nom2 = 
    let f1 = open_in nom1 in
    let f2 = open_out nom2 in
    let rec loop () = 
      try
        let s = input_line f1 in
        afficher_caracteres f2 s (String.length s);
        loop ()
      with
        |End_of_file -> () in
      loop ();
      close_in f1;
      close_out f2


let () =
  let nom1 = Sys.argv.(1) in
  let nom2 = Sys.argv.(2) in
  copy nom1 nom2
  
  


