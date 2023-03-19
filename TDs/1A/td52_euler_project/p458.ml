let rec range a b =
  if a >= b then [] else a :: range (a + 1) b

let size m =
  (Array.length m, Array.length m.(0))

let ( ** ) a b =
  let n, p = size a in
  let p, q = size b in
  let c = Array.make_matrix n q 0 in
  for i = 0 to n - 1 do
    for j = 0 to q - 1 do
      let s = ref 0 in
      for k = 0 to p - 1 do
        s := (!s + a.(i).(k) * b.(k).(j)) mod 1_000_000_000
      done;
      c.(i).(j) <- !s
    done;
  done;
  c

let id n =
  Array.init n (fun i -> Array.init n (fun j -> if i = j then 1 else 0))
    
let rec exp a p =
  if p = 0 then id (Array.length a)
  else if p mod 2 = 0 then exp (a ** a) (p / 2)
  else a ** exp a (p - 1)


let mat =
  let m = Array.make_matrix 6 6 0 in
  for j = 0 to 5 do m.(0).(j) <- 1 done;
  for k = 1 to 5 do
    m.(k).(k - 1) <- 7 - k;
    for i = k to 5 do
      m.(k).(i) <- 1
    done;
  done;
  m

let x_init =
  let m = Array.make_matrix 6 1 0 in
  m.(0).(0) <- 7;
  m

let p458 n =
  let x_final = exp mat (n - 1) ** x_init in
  let ones = Array.make_matrix 1 6 1 in
  (ones ** x_final).(0).(0)
