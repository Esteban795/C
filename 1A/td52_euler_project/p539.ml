let rec range a b =
  if a >= b then [] else a :: range (a + 1) b

let rec f n =
  if n = 1 then 1
  else 2 * g (n / 2)
and g n =
  if n = 1 then 1
  else if n mod 2 = 1 then 2 * f (n / 2)
  else 2 * f (n / 2) - 1

let s_lent n =
  let hlist = List.map f (range 1 (n + 1)) in
  List.fold_left (+) 0 hlist

let m = 987654321

let modm a =
  let x = a mod m in
  if x < 0 then x + m
  else x

let ( ++ ) a b = modm (a + b)
let ( ** ) a b = modm (a * b)
let ( -- ) a b = modm (a - b)

let rec p n =
  if n = 0 then 0
  else if n = 1 then 1
  else if n = 2 then 2
  else if n = 3 then 2
  else
    let y = p (n / 4) in
    if n mod 4 < 2 then 4 ** y -- 2
    else 4 ** y

let rec s n =
  let k = n / 4 in
  let derniers_termes = List.map p (range (4 * k) (n + 1)) in
  let fin_somme = List.fold_left ( ++ ) 0 derniers_termes in
  if k = 0 then fin_somme
  else 5 ++ fin_somme ++ 16 ** s (k - 1) -- 4 ** (k - 1)
