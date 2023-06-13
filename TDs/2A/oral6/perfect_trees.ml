type  arbre = 
  | N of arbre * arbre
  | F 


let rec hauteur t = 
  match t with 
  |F -> 0
  |N (t1,t2) -> 1 + max (hauteur t1) (hauteur t2)


let est_parfait t = 
  let h = hauteur t in 
  let rec aux t1 p = 
    match t1 with
    | F -> p = h
    | N (t1,t2) -> aux t1 (p + 1) && aux t2 (p + 1)
  in aux t 0


let rec est_presque_parfait t = 
  match t with 
  | F -> (true,true,0)
  | N (t1,t2) -> 
      let (p1,pp1,h1) = est_presque_parfait t1 in 
      let (p2,pp2,h2) = est_presque_parfait t2 in 
      if p1 && p2 && h1 = h2 then (true, true, h1 + 1) 
      else if p1 && pp2 && h2 = (h1 - 1) then (false, true, h2 + 1) 
      else if pp1 && p2 && h1 = h2 - 1 then (false, true,h1 + 1)
      else (false,false,1)



  