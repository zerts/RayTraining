open System
open System.IO
open System.Net
open System.Text
open System.Collections.Specialized

// почтовый адрес
let email = "zertsalov96@yandex.ru"

let explode (s:string) = [for c in s -> c]

let getWord l = 
    let rec getW (acc : string) (l : char list) =
        match l with 
        | (x::xs) when x = '"' -> ("\"" + acc + "\"", xs)
        | (x::xs) -> getW (acc + (Char.ToString x)) xs
        | _ -> ("", [])
    getW "" l

let getNumber f l = 
    let rec getN (acc : string) (l : char list) =
        match l with 
        | (x::xs) when not(Char.IsDigit x) -> (acc, l)
        | (x::xs) -> getN (acc + (Char.ToString x)) xs
        | _ -> ("", [])
    getN (Char.ToString f) l

let repairWords l = 
    let rec repair (acc : string list) (l : char list) = 
        match l with
        | [] -> acc
        | (x::xs) when (x = ':' || x = '{' || x = '}' || x = '[' || x = ']' || x = ',') -> repair ((Char.ToString x)::acc) xs
        | (x::xs) when x = '"' -> repair ((fst (getWord xs))::acc) (snd (getWord xs))
        | 't'::'r'::'u'::'e'::xs -> repair ("true" :: acc) xs
        | '0'::'f'::'a'::'l'::'s'::'e'::xs -> repair ("false" :: acc) xs
        | 'n'::'u'::'l'::'l'::xs -> repair ("NULL" :: acc) xs
        | (x::xs) when (Char.IsDigit x) -> repair ((fst (getNumber x xs))::acc) (snd (getNumber x xs))
        | (x::xs) -> repair acc xs
    repair [] l

let getListOfWords (s : string) = 
    List.rev (repairWords (explode s))
    
//getListOfWords "{ \"firstName\": \"Иван\", \"lastName\": \"Иванов\", \"address\": { \"streetAddress\": \"Московское ш., 101, кв.101\", \"city\": \"Ленинград\", \"postalCode\": 101101 }, \"phoneNumbers\": [ \"812 123-1234\", \"916 123-4567\" ] }"

type TOKEN = 
    | VALUE_STR of string
    | VALUE_INT of int
    | VALUE_BOOL of bool
    | COMMA | OPEN_BRACKET
    | CLOSE_BRACKET | OPEN_ARRAY
    | CLOSE_ARRAY | TWO_POINTS
    | NULL

let listToString l : string =
    let rec lts (acc : string) ll=
        match ll with
        | '"'::[] -> acc
        | x::xs -> lts (acc + (Char.ToString x)) xs
    lts "" l

let tokenize (s:string) : TOKEN list = 
    let currToken (s:string) : TOKEN = 
        match explode s with
        | '"'::ss -> VALUE_STR (listToString ss)
        | '{'::[] -> OPEN_BRACKET
        | '}'::[] -> CLOSE_BRACKET
        | '['::[] -> OPEN_ARRAY
        | ']'::[] -> CLOSE_ARRAY
        | ','::[] -> COMMA
        | ':'::[] -> TWO_POINTS
        | 't'::'r'::'u'::'e'::[] -> VALUE_BOOL true
        | '0'::'f'::'a'::'l'::'s'::'e'::[] -> VALUE_BOOL false
        | 'n'::'u'::'l'::'l'::[] -> NULL
        | _ -> VALUE_INT (Convert.ToInt32 s)


    let rec makeTokens (acc: TOKEN list) = function
        [] -> acc
        | (x::xs) -> makeTokens ((currToken x)::acc) xs
    List.rev (makeTokens [] (getListOfWords s))

//tokenize "{ \"firstName\": \"Иван\", \"lastName\": \"Иванов\", \"address\": { \"streetAddress\": \"Московское ш., 101, кв.101\", \"city\": \"Ленинград\", \"postalCode\": 101101 }, \"phoneNumbers\": [ \"812 123-1234\", \"916 123-4567\" ] }"

type JSON =
  | OBJECT of (string * JSON) list
  | ARRAY of JSON list
  | NUMBER of int
  | STRING of string
  | BOOLEAN of bool
  | NULL


let rec parse (json : TOKEN list) : JSON =
  let rec parse' json =
    let rec parseObject list = function
      | TOKEN.CLOSE_BRACKET :: t -> (OBJECT (List.rev list)), t
      | TOKEN.COMMA :: t -> parseObject list t
      | TOKEN.VALUE_STR s :: TOKEN.TWO_POINTS :: t ->
        let a, t = parse' t
        parseObject ((s, a) :: list) t
    let rec parseArray list = function
      | TOKEN.CLOSE_ARRAY :: t -> (JSON.ARRAY (List.rev list)), t
      | TOKEN.COMMA :: t -> parseArray list t
      | ob -> 
        let a, t = parse' ob
        parseArray (a :: list) t  
    match json with
      | TOKEN.OPEN_BRACKET :: t -> parseObject [] t
      | TOKEN.OPEN_ARRAY :: t -> parseArray [] t
      | TOKEN.NULL :: t -> JSON.NULL, t
      | TOKEN.VALUE_STR s :: t -> JSON.STRING s, t
      | TOKEN.VALUE_INT s :: t -> JSON.NUMBER s, t
      | TOKEN.VALUE_BOOL s :: t -> JSON.BOOLEAN s, t
      | _ -> failwith "Incorrect identification"
  match parse' json with
    | res, [] -> res
    | _ -> failwith "Wrong JSON structure"



let s = """
{
  "a": 1
  "a": 1
  "a": 1
  "a": 1
  "b": {
    "c": [1,2,3]
  }
}
"""

let ss = "{ \"firstName\": \"Иван\", \"lastName\": \"Иванов\", \"lastName\": \"Иванов\", \"address\": { \"streetAddress\": \"Московское ш., 101, кв.101\", \"city\": \"Ленинград\", \"postalCode\": 101101 }, \"phoneNumbers\": [ \"812 123-1234\", \"916 123-4567\" ] }"

let myTree = ss |> tokenize |> parse

let getDepth (json : JSON) : int = 
    let rec gd (currLevel : int) (currJson : JSON) = 
        match currJson with
        | JSON.OBJECT (l) -> (List.max (List.map (fun c -> gd (currLevel + 1) (snd c)) l))
        | _ -> currLevel
    gd 0 json

getDepth myTree

let countTheLevel (level : int) (json : JSON) : int = 
    let rec ctl (currLevel : int) (currJson : JSON) = 
        match currJson with
        | JSON.OBJECT (l) ->
            if (currLevel = level) then (List.length l)
            else (List.sum (List.map (fun c -> ctl (currLevel + 1) (snd c)) l))
        | _ -> 0
    ctl 0 json

let countLevels (json : JSON) =
    List.zip [0..(getDepth json)] [for level in [0..(getDepth json)] -> (countTheLevel level json)]

countLevels myTree

let getMaxLevel (json : JSON) : int = 
    snd (List.max (List.zip [for level in [0..(getDepth json)] -> (countTheLevel level json)] [0..(getDepth json)]))

getMaxLevel myTree //функция, возвращающая решение варианта лабы
    
let generate = 
  let rnd = new Random()
  match rnd.Next(42) with
    | 0 -> JSON.OBJECT []
    | _ -> JSON.OBJECT [("random", JSON.OBJECT [])]

let jsonToString (json : JSON) : string =
    let needComma l = 
        match l with
        | [] -> ""
        | _ -> ", "
    let rec arrayToString (acc : string) arr = 
         match arr with
         | [] -> "[" + acc + "]"
         | (JSON.NULL::xs) -> arrayToString (acc + "null" + (needComma xs)) xs
         | (JSON.BOOLEAN (value)::xs) -> arrayToString (acc + (if value then "true" else "false") + (needComma xs)) xs
         | (JSON.NUMBER (value)::xs) -> arrayToString (acc + value.ToString() + (needComma xs)) xs
         | (JSON.STRING (value)::xs) -> arrayToString (acc + "\"" + value + "\""  + (needComma xs)) xs
         | (JSON.ARRAY (value)::xs) -> arrayToString (acc + arrayToString "" value + (needComma xs)) xs
    let rec jts (currJson : JSON) : string = 
        match currJson with
        | JSON.NULL -> ": null"
        | JSON.BOOLEAN (value) -> if value then "true" else "false"
        | JSON.NUMBER (value) -> value.ToString()
        | JSON.STRING (value) -> "\"" + value + "\"" 
        | JSON.ARRAY (value) -> arrayToString "" value
        | JSON.OBJECT (value) -> "{\n" 
                                  + (fst (List.head value)) + "\": " + jts (snd (List.head value)) 
                                  + String.Concat (List.map (fun x -> ",\n\"" + (fst x) + "\": " + jts (snd x)) (List.tail value)) 
                                  + "\n}"
    jts json

jsonToString myTree

let main () = 
  let values = new NameValueCollection()
  values.Add("email", email)
  values.Add("content", File.ReadAllText(__SOURCE_DIRECTORY__ + @"/" + __SOURCE_FILE__))

  let client = new WebClient()
  let response = client.UploadValues(new Uri("http://91.239.143.158/lab2"), values)
  let responseString = Text.Encoding.Default.GetString(response)

  printf "%A\n" responseString


main()