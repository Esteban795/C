type maillon = {
    mutable donnee : int;
    mutable prec : maillon option;
    mutable suiv : maillon option;
  }

type liste_chainee = {
    mutable debut : maillon option;
    mutable fin : maillon option
  }


type lru = {
    liste : liste_chainee;
    hachage : (int, string * maillon option) Hashtbl.t;
    mmu : int -> string;
    mutable longueur : int;
    capacite : int;
  }

let nouveau_noeud x = 
  Some 
  {donnee = x;
    prec = None;
    suiv = None
  }

  
let ajout_debut (lst : liste_chainee) x = 
  let noeud = nouveau_noeud x in
  match lst.debut with 
  | None -> (*la liste chaînée est complètement vide*)
    lst.debut <- noeud;
    lst.fin <- noeud        
  | Some m -> 
    noeud.suiv <- lst.debut;
    lst.debut.prec <- noeud;
    lst.debut <- noeud;
  
let suppression_maillon (lst : liste_chainee) (m : maillon) = 
  


let chiffre = function
  | 0 -> "zero"
  | 1 -> "un"
  | 2 -> "deux"
  | 3 -> "trois"
  | 4 -> "quatre"
  | 5 -> "cinq"
  | 6 -> "six"
  | 7 -> "sept"
  | 8 -> "huit"
  | 9 -> "neuf"
  | _ -> failwith "pas un chiffre"
