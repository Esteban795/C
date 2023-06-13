
(********************************************************************)
(* Concours Centrale-Supélec                                        *)
(* Sujet 0 - MPI                                                    *)
(* https://www.concours-centrale-supelec.fr                         *)
(* CC BY-NC-SA 3.0                                                  *)
(********************************************************************)

let texte_preface = "préfacedanslaquelleilestétablique,malgréleursnomsenosetenis,leshérosdel'histoirequenousallonsavoirl'honneurderaconteranoslecteursn'ontriendemythologique."

type lexique = (string, int) Hashtbl.t

(* Lexique correspondant à l'exemple proposé dans le sujet *)
let dico_ex : lexique =
  let h = Hashtbl.create 4 in
  let add (m, c) = Hashtbl.add h m c in
  List.iter add [("a", 1); ("ab", 1); ("aba", 2); ("bb", 1)];
  h

(* Vérifie qu'un fichier de lexique est conforme au format demandé *)
let verifie_format nom_fichier =
  let f = open_in nom_fichier in
  begin
    try
      while true do
        match String.split_on_char ' ' (input_line f) with
        | [mot; occ] ->
           assert (String.length mot <= 100);
           assert (int_of_string occ >= 1);
        | _ -> failwith "Erreur de format"
      done
    with End_of_file -> close_in f
  end


let read_lex (filename : string) = 
  let f_in = open_in filename in 
  let htable = Hashtbl.create 10000 in 
  begin
    try
      while true do
        match String.split_on_char ' ' (input_line f_in) with
        | [mot; occ] ->
           assert (String.length mot <= 100);
           assert (int_of_string occ >= 1);
           Hashtbl.add htable mot (int_of_string occ)
        | _ -> failwith "Erreur de format"
      done
    with End_of_file -> close_in f_in
  end

let mots_diff (lex : lexique) = 
  Hashtbl.length lex

let longest_length (lex : lexique) = 
  let max_length = ref 1 in 
  Hashtbl.iter (fun mot occ -> if String.length mot > !max_length then max_length := String.length mot) lex
  
let nb_unique_words (lex : lexique) = 
  let count = ref 0 in 
  Hashtbl.iter (fun mot occ -> if occ = 1 then incr count;) lex;
  !count


let est_mot (lex : lexique) (s : string) = 
  Hashtbl.mem lex s 

let score (lex : lexique) (s : string) = 
  if est_mot lex s then log (float_of_int (Hashtbl.find lex s))
  else neg_infinity


let segmentations (lex : lexique) (s : string) = 
  let l = String.length s in 
  let (res : string list list ref) = ref [] in 
  let rec aux start (seg : string list) = 
    for len = 1 to l - start - 1 do 
      let word = String.sub s start len in 
      match Hashtbl.mem lex word with 
      | true when start + len = l -> 
          let temp = word :: seg in 
          res := temp :: !res
      | true -> aux (start + len) (word :: seg)
      | false -> 
    done;