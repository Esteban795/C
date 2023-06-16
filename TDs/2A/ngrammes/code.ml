Printexc.record_backtrace true;;
let max_var = 128


type model = (string,int array) Hashtbl.t

let histogram (target : string) (k : int) (text : string) = 
  let histo = Array.make max_var 0 in 
  let len_text = String.length text in
  for i = 0 to len_text - k - 1 do
    if String.sub text i k = target then histo.(int_of_char text.[i + k]) <- 1 + histo.(int_of_char text.[i + k])
  done;
  histo

let create_kgram model k text =
  let len_text = String.length text in
  for i = 0 to len_text - k do 
    let target = String.sub text i k in
    if not (Hashtbl.mem model target) then 
      begin
        let histo = histogram target k text in 
        Hashtbl.add model target histo
      end
  done;;

let build_n_gram (text : string) (n : int) : (model) = 
  let ngram = Hashtbl.create n in
  for k = 0 to n do
    create_kgram ngram k text
  done;
  ngram

  
let rec pick_random (model : model) (target : string) =
  let count_arr = Hashtbl.find model target in 
  let total_count = (Array.fold_left (+) 0 count_arr) in
  if total_count = 0 then pick_random model "" (*No successor : we use a random letter from the source text.*)
  else 
    begin
      let random = Random.int total_count in
      let nb_occ = ref 0 in 
      let i = ref (-1) in
      while !nb_occ <= random do
          incr i;
          nb_occ := !nb_occ + count_arr.(!i)
      done;
      Char.chr !i
    end

let predict (model : model) ngram = 
  let k = ref (String.length ngram) in 
  let kg = ref ngram in 
  while not (Hashtbl.mem model !kg) do
    decr k;
    kg := String.sub !kg 1 !k;
  done;
  pick_random model !kg

let auto_generate model n seed generated_len =
  let gen = ref seed in
  let len = ref (String.length seed) in
  while !len < generated_len do
      let m = min !len n in
      let ngram = String.sub !gen (!len - m) m in
      gen := !gen ^ (String.make 1 (predict model ngram));
      incr len
  done;
  !gen

let print_htbl_entry s arr = 
  Printf.printf "String %s : \n" s;
  for i = 0 to max_var - 1 do 
    Printf.printf "%d " arr.(i)
  done;
  Printf.printf "\n\n";;

let print_htable (htbl : model) = 
  Hashtbl.iter (fun k v -> print_htbl_entry k v) htbl

let read_seed filename = 
  let f = open_in filename in 
  let seed = really_input_string f (in_channel_length f) in 
  close_in f;
  seed

let _ = 
  Random.self_init ();
  let path = Sys.argv.(1) in 
  let seed = read_seed path in 
  let n = int_of_string Sys.argv.(2) in 
  let start = Sys.argv.(3) in
  let len = int_of_string Sys.argv.(4) in  
  let model = build_n_gram seed n in
  Printf.printf "Auto generated text : \n%s" (auto_generate model n start len)