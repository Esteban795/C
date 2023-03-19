let squares = Array.init 10_000 (fun i -> i * i)

let is_square x =
  let rec aux i j =
    let m = (i + j) / 2 in
    i < j &&
      (squares.(m) = x || if squares.(m) < x then aux (m + 1) j else aux i m) in
  aux 0 (Array.length squares)


let fixed_c c =
  let n = ref 0 in
  let c2 = c * c in
  for a_plus_b = 2 to 2 * c do
    if is_square (c2 + a_plus_b * a_plus_b) then
      let nb = a_plus_b / 2 + 1 - max 1 (a_plus_b - c) in
      n := !n + nb
  done;
  !n

let first_greater bound =
  let n = ref 0 in
  let c = ref 0 in
  while !n <= bound do
    incr c;
    n := !n + fixed_c !c;
  done;
  (!n, !c)

let () =
  let bound = int_of_string Sys.argv.(1) in
  let n, c = first_greater bound in
  Printf.printf "First exceeding %i for M = %i (number of cuboids is then %i).\n" bound c n
