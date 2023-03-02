let rec gcd a b =
  if b = 0 then a
  else gcd b (a mod b)

let reduce (a, b) =
  let d = gcd a b in
  (a / d, b / d)

let ( ** ) (a, b) (c, d) =
  reduce (a * c, b * d)

let rec zip u v =
  match u, v with
  | [], [] -> []
  | x :: xs, y :: ys -> (x, y) :: zip xs ys
  | _ -> failwith "different lengths"

let rec range a b =
  if a >= b then [] else a :: range (a + 1) b

let eval (a, b) =
  (float_of_int a) /. (float_of_int b)

let product = List.fold_left ( ** ) (1, 1)

let p_not_seen =
  let nums = range 41 61 in
  let dens = range 51 71 in
  product (zip nums dens)

let expectation =
  let p = 1. -. eval p_not_seen in
  7. *. p
