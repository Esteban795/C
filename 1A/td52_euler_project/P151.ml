let somme = List.fold_left ( + ) 0 

let get t key = 
  if Hashtbl.mem t key then Hashtbl.find t key else 0.


let rec enfants (conf, p, s_conf) = 
  match conf with
  | [] -> []
  | 0 :: xs -> 
     List.map 
       (fun (c, q) -> (0 :: c, q)) 
       (enfants (xs, p, s_conf))
  | n :: xs -> 
     let pas_pris = 
       List.map (fun (c, q) -> (n :: c, q)) (enfants (xs, p, s_conf)) in 
     let nv_conf = 
       ((n - 1) :: List.map (( + ) 1) xs, float n *. p /. s_conf) in 
     nv_conf :: pas_pris
  
let update t conf p = 
  let s_conf = float (somme conf) in 
  let nv_confs = enfants (conf, p, s_conf) in 
  List.iter
    (fun (conf, p) -> 
      let q = get t conf in 
      Hashtbl.replace t conf (p +. q))
    nv_confs

(* génère la config initiale [1 A0, 0 A1, 0 A2, ..., 0 A(n - 1)] *)
let init n = 
  let rec kzeros k =
    if k = 0 then [] else 0 :: kzeros (k - 1) in 
  1 :: kzeros (n - 1)

let proba_unique t = 
  Hashtbl.fold 
    (fun conf p acc -> if somme conf = 1 then p +. acc else acc) 
    t
    0.

(*  Pour A0 -> A4, prendre k = 5 *)
let p151 k = 
  let rec principale n t = 
    if n = 0 then 
      0. 
    else begin
      let nv_t = Hashtbl.create 1000 in 
      Hashtbl.iter (update nv_t) t;
      let p = proba_unique t in 
      let esp = principale (n - 1) nv_t in 
      p +. esp
    end in
  let t = Hashtbl.create 1000 in 
  Hashtbl.add t (init k) 1.;
  principale (1 lsl (k - 1)) t -. 2.

