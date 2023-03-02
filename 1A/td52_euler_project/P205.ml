let rec puissance u n = 
  if n = 0 then [[]] 
  else let v = puissance u (n - 1) in
       let pref x = List.map (fun y -> x :: y) v in
       let listes = List.map pref u in 
       List.flatten listes

let rec ( <|> ) a b = if a >= b then [] else a :: (a + 1 <|> b) in


let proba_brute nd1 nf1 nd2 nf2 =
  let somme = List.fold_left ( + ) 0 in 
  let rec nb_sup u v = 
    match u with
    | [] -> 0
    | x :: xs -> let k = List.fold_left 
		   (fun acc y -> if somme x > somme y then acc + 1 else acc)
		   0 v in
		 k + nb_sup xs v in
  let u = puissance (1 <|> nf1 + 1) nd1 in 
  let v = puissance (1 <|> nf2 + 1) nd2 in 
  let card_fav = nb_sup u v in
  float card_fav /. (float nf1 ** float nd1 *. float nf2 ** float nd2)
  
let proba_brute_2 nd1 nf1 nd2 nf2 =
  let somme = List.fold_left ( + ) 0 in 
  let loi_1 = Array.create (nd1 * nf1 + 1) 0 in 
  let loi_2 = Array.create (nd2 * nf2 + 1) 0 in 
  let u = puissance (1 <|> nf1 + 1) nd1 in 
  let v = puissance (1 <|> nf2 + 1) nd2 in 
  let rec remplit t w = 
    List.iter 
      (fun x -> let k = somme x in t.(k) <- t.(k) + 1)
      w in
  remplit loi_1 u;
  remplit loi_2 v;
  let card_1 = float nf1 ** float nd1 in 
  let card_2 = float nf2 ** float nd2 in 
  let p = ref 0. in
  for i = 0 to Array.length loi_1 - 1 do
    for j = 0 to min (i - 1) (Array.length loi_2 - 1) do
      p := !p +. (float (loi_1.(i) * loi_2.(j))) /. (card_1 *. card_2)
    done
  done;
  !p


let loi n_des n_faces = 
  let n_max = n_des * n_faces in 
  let t = ref (Array.create (n_max + 1) 0) in 
  !t.(0) <- 1;
  for de = 1 to n_des do 
    let nv_t = Array.create (n_max + 1) 0 in
    for k = 0 to n_faces * (de - 1) do
      for face = 1 to n_faces do
	if k + face <= n_max then nv_t.(k + face) <- nv_t.(k + face) + !t.(k)
      done;
    done;
    t := nv_t
  done;
  let s = Array.fold_left ( + ) 0 !t in 
  Array.map (fun x -> float x /. float s) !t;;
    
let proba nd1 nf1 nd2 nf2 = 
  let sommes_cum t = 
    let n = Array.length t in 
    let u = Array.create n t.(0) in 
    for k = 1 to n - 1 do 
      u.(k) <- t.(k) +. u.(k - 1)
    done;
    u in 
  let loi_1 = loi nd1 nf1 in 
  let repart = sommes_cum (loi nd2 nf2) in
  let get_repart k = if k < Array.length repart then repart.(k) else 1. in
  let p = ref 0. in 
  for k = 1 to Array.length loi_1 - 1 do 
    p := !p +. loi_1.(k) *. get_repart (k - 1)
  done;
  !p
  
      
