type resultat = Oui | Non | Inconnu

let p92 n = 
  let tab = Array.make 1000 Inconnu in 
  let rec somme_chiffres k = 
    if k = 0 then 0 
    else (k mod 10) * (k mod 10) + somme_chiffres (k / 10) in
  let get k t = if k >= Array.length t then Inconnu else t.(k) in 
  let set k x t = if k < Array.length t then t.(k) <- x in
  let rec donne_89 k =
    match get k tab with 
    | Inconnu -> let next = somme_chiffres k in 
	         let b = donne_89 next in 
	         set k b tab;
	         b
    | x -> x in
  tab.(1) <- Non;
  tab.(89) <- Oui;
  let s = ref 0 in 
  for k = 1 to n - 1 do 
    if donne_89 k = Oui then incr s
  done;
  !s

