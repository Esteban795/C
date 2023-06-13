open Scanf
open PQ


(* La fonction lit_graphe renvoie un tableau de liste de paires
  représentant le graphe en liste d'ajacence c'est à dire que si x est
  voisin de y à distance d alors la fonction renverra un tableau t tel
  que la liste t.(x) contiendra la paire (y,d) et t.(y) contiendra la
  paire (x,d) *)   
let lit_graphe () = 
  let fp_graphe = Scanning.open_in "graphe.txt" in
  let read_int () = bscanf fp_graphe "%d" (fun x:int -> x) in
  let read_arete () = bscanf fp_graphe " %d %f" (fun x y -> (x,y)) in
  let read_newline () = bscanf fp_graphe "%c" (fun x -> ()) in
  let nb_noeuds = read_int () in
  let _ = read_newline () in
  let graphe = Array.make nb_noeuds [] in
  for i = 0 to nb_noeuds-1 do
    let nb_voisins = read_int () in
    for j = 0 to nb_voisins-1 do
      graphe.(i) <- read_arete () :: graphe.(i)
    done ;
    read_newline () 
  done ;
  Scanning.close_in fp_graphe ;
  graphe

let lit_position () =
  let fp_pos = Scanning.open_in "positions.txt" in
  let read_int () = bscanf fp_pos "%d%c" (fun x _ -> x) in
  let read_paire x = bscanf fp_pos " %f %f%c" (fun x y _-> (x,y)) in
  let nb_noeuds = read_int () in
  let positions = Array.init nb_noeuds read_paire in
  Scanning.close_in fp_pos ;
  positions


  
  
let graphe = lit_graphe ()    
(* 
   let positions = lit_position () 
   on n'a pas besoin de lire le fichier position pour l'instant.
*) 
