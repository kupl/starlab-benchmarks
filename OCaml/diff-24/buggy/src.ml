type aexp =
  | Const of int
  | Var of string
  | Power of (string * int)
  | Times of aexp list
  | Sum of aexp list

exception InvalidArgument

let rec diff ((aexp : aexp), (str : string)) : aexp =
  match aexp with
  | Const n -> Const 0
  | Var s -> if s = str then Const 1 else Const 0
  | Power (s, n) ->
      if s = str then Times [ Const n; Power (s, n - 1) ] else Const 0
  | Sum [] -> raise InvalidArgument
  | Times [] -> raise InvalidArgument
  | Sum el -> Sum (List.map (fun (x : aexp) -> diff (x, str)) el)
  | Times el ->
      Sum
        (List.map
           (fun (x : aexp) ->
             Times (diff (x, str) :: List.filter (fun (y : aexp) -> x != y) el))
           el)
