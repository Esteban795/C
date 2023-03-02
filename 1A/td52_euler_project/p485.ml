let sieve n =
  let t = Array.make (n + 1) 0 in
  for p = 1 to n do
    let mul = ref p in
    while !mul <= n do
      t.(!mul) <- t.(!mul) + 1;
      mul := !mul + p
    done;
  done;
  t

let max_array t i length =
  let m = ref t.(i) in
  for k = 1 to length - 1 do
    m := max !m t.(i + k)
  done;
  !m

let p485 u k =
  let t = sieve u in
  let m = ref (max_array t 1 k) in
  let s = ref (!m) in
  for n = 1 to u - k  do
    if !m = t.(n) then m := max_array t (n + 1) k
    else m := max !m t.(n + k);
    s := !s + !m
  done;
  !s

let () =
  let u = int_of_string Sys.argv.(1) in
  let k = int_of_string Sys.argv.(2) in
  let result = p485 u k in
  Printf.printf "S(%i, %i) = %i\n" u k result
