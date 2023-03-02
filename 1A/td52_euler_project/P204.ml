(* renvoie la liste des nombres premiers <= n (méthode du crible) *)
let premiers n = 
  let tab = Array.init (n + 1) (fun x -> x) in 
  let res = ref [] in
  for k = 2 to n do 
    if tab.(k) <> 0 then begin
      res := k :: !res;
      let x = ref (2 * k) in 
      while !x <= n do 
	tab.(!x) <- 0;
	x := !x + k
      done
    end
  done;
  !res

let rec hamming prem borne = 
  match prem with
  | [] -> 1
  | x :: xs -> let h = hamming xs borne in 
	       if x <= borne then h + hamming prem (borne / x) 
	       else h
