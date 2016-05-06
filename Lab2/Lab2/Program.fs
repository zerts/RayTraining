open System
open System.IO
open System.Net
open System.Text
open System.Collections.Specialized

// почтовый адрес
let email = "zertsalov96@yandex.ru"

type JSON = Object of (string * JSON) list

let explode (s:string) = [for c in s -> c]

let getWord l = 
    let rec getW (acc : string) (l : char list) =
        match l with 
        | (x::xs) when x = '"' -> (acc, xs)
        | (x::xs) -> getW (acc + (Char.ToString x)) xs
        | _ -> ("", [])
    getW "" l

let repairWords l = 
    let rec repair (acc : string list) (l : char list) = 
        match l with
        | [] -> acc
        | (x::xs) when (x = ':' || x = '{' || x = '}' || x = '[' || x = ']' || x = ',') -> repair ((Char.ToString x)::acc) xs
        | (x::xs) when x = '"' -> repair ((fst (getWord xs))::acc) (snd (getWord xs))
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

let tokenize (s:string) : TOKEN list = 
    let currToken (s:string) : TOKEN = 
        if ()
        match s with
        | "{" -> OPEN_BRACKET
        | "}" -> CLOSE_BRACKET
        | "[" -> OPEN_ARRAY
        | "]" -> CLOSE_ARRAY
        | "," -> COMMA
        | ":" -> TWO_POINTS
        | "true" -> VALUE_BOOL true
        | "false" -> VALUE_BOOL false
        | _ -> VALUE_STR s


    let rec makeTokens (acc: TOKEN list) = function
        [] -> acc
        | (x::xs) -> makeTokens ((currToken x)::acc) xs
    List.rev (makeTokens [] (getListOfWords s))
    
(*let jsonFromString (s : string) : JSON = 
    let jsonFromList (l : string list) = 
        
    jsonFromList (getListOfWords s)*)

let parse str = 
    if str = "{}" then Object [] 
    else failwith "Wrong JSON structure!"

let lab3 = function
  | Object list -> 0

let stringify = function
  | Object list -> "{}"

let generate = 
  let rnd = new Random()
  match rnd.Next(42) with
    | 0 -> Object []
    | _ -> Object [("random", Object [])]

let main () = 
  let values = new NameValueCollection()
  values.Add("email", email)
  values.Add("content", File.ReadAllText(__SOURCE_DIRECTORY__ + @"/" + __SOURCE_FILE__))

  let client = new WebClient()
  let response = client.UploadValues(new Uri("http://91.239.143.158/lab2"), values)
  let responseString = Text.Encoding.Default.GetString(response)

  printf "%A\n" responseString

