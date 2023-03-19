let rec range a b =
  if a >= b then [] else a :: range (a + 1) b

let p523 n =
  let t = Array.make (n + 1) None in 
  let rec g n =
    if n = 0 then 0
    else
      match t.(n) with
      | Some x -> x
      | None -> let res = n + List.fold_left (+) 0 (List.map g (range 0 n)) in
                t.(n) <- Some res;
                res in
  let esp_g n =
    let foi = float_of_int in
    foi (g n) /. foi (n + 1) in
  List.fold_left (+.) 0. (List.map esp_g (range 0 n))
