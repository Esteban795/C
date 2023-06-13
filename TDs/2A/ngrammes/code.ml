type ngrammes = {
  htbl : (string, float array) Hashtbl.t
}

let max_var = 128

let histogramme (target : string) (k : int) (texte : string) = 
  let histo = Array.make max_var 0 in 
  let texte_len = String.length texte in
  for i = 0 to texte_len - k do
    if String.sub texte i k == target then histo.(int_of_char texte.[i]) <- 1 + histo.(int_of_char texte.[i])
  done;
  histo


let create_kgram htbl k texte =
  let len_texte = String.length texte in
  for i = 0 to len_texte - k do 
    let target = String.sub texte i k in 
    let histo = histogramme target k texte in 
    Hashtbl.add htbl target histo
  done;;

let construire_n_grammes texte n : ((string,int array) Hashtbl.t) = 
  let range = ref 0 in 
  let ng = Hashtbl.create n in
  for k = 0 to n - !range do
    create_kgram ng k texte
  done;
  ng


let choisir_aleatoire (htbl : (string,int array) Hashtbl.t) (target : string) =
  let occs = Hashtbl.find htbl target in 
  let total = ref 0 in
  for i = 0 to max_var - 1 do
    total := !total + occs.(i)
  done;
  let n = ref (Random.int !total) in 
  let index = ref 0 in 
  for i = 0 to max_var - 1 do 
    if !n >= occs.(i) then 
      begin 
      incr index;
      n := !n - occs.(i)
      end
  done;
  !index