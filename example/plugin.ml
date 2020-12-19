open Weechat_api


module List = struct
  include List

  let rec iter_opt f = function
    | [] -> Some ()
    | x :: xs ->
      match f x with
      | None -> None
      | Some () -> iter_opt f xs
end


module Parsing = struct
  let as_assoc = function
    | `Assoc assoc -> Some assoc
    | _ -> None

  let as_string = function
    | `String s -> Some s
    | _ -> None

  let as_list = function
    | `List l -> Some l
    | _ -> None
end


(* For nicer error handling *)

let (>>=) = Option.bind
let (let*) x f = Option.bind x f
let (let+) x f = Option.map f x
let (and+) x y =
  match x, y with
  | Some x, Some y -> Some (x, y)
  | _ -> None


let log = open_out "/tmp/weechat_signal.log" |> Format.formatter_of_out_channel
let log args =
  Format.fprintf log args


module Signald = struct
  type t = {socket: Unix.file_descr; ic: in_channel}

  exception SignalErr of string
  let error fmt = Format.kasprintf (fun s -> raise (SignalErr s)) fmt

  let username = ref ""

  module Group = struct
    type version = V1 | V2
    type t = {
      version: version;
      title: string;
      id: string;
    }

    let parse_v1 payload =
      let* assoc = Parsing.as_assoc payload in
      let+ title = List.assoc_opt "name" assoc >>= Parsing.as_string
      and+ id = List.assoc_opt "groupId" assoc >>= Parsing.as_string in
      {title; version = V1; id}

    let parse_v2 payload =
      let* assoc = Parsing.as_assoc payload in
      let+ title = List.assoc_opt "title" assoc >>= Parsing.as_string
      and+ id = List.assoc_opt "id" assoc >>= Parsing.as_string in
      {title; version = V2; id}
  end

  let make socket =
    let ic = Unix.in_channel_of_descr socket in
    {socket; ic}

  let send t =
    Format.kasprintf
      (fun s -> log "Sending: %s@." s;
        let _ = Unix.write_substring t.socket s 0 (String.length s) in
        let _ = Unix.write_substring t.socket "\n" 0 1 in
        ())

  let subscribe t (number: string) =
    username := number;
    send t {|{"type": "subscribe", "username": "%s"}|} number

  let list_groups t (number: string) =
    send t {|{"type": "list_groups", "username": "%s"}|} number

  let close t = close_in t.ic
end


let groups = Hashtbl.create 19


let init_signald path =
  let socket = Unix.(socket PF_UNIX SOCK_STREAM 0) in
  let rec try_connect retry =
    try Unix.connect socket (Unix.ADDR_UNIX path)
    with Unix.Unix_error _ ->
      log "Could not connect to socket %s, retrying in %d seconds@." path retry;
      Unix.sleep retry;
      try_connect (retry + 1)
  in
  try_connect 1;
  Signald.make socket

let update_group_list payload =
  let okv1 =
    match List.assoc_opt "groups" payload with
    | None -> Some ()
    | Some gv1 ->
      let* gv1 = Parsing.as_list gv1 in
      List.iter_opt
        (fun g ->
          let+ g = Signald.Group.parse_v1 g in
          if not (Hashtbl.mem groups g.id) then
            Hashtbl.add groups g.id g)
        gv1
  and okv2 =
    match List.assoc_opt "groupsv2" payload with
    | None -> Some ()
    | Some gv2 ->
      let* gv2 = Parsing.as_list gv2 in
      List.iter_opt
        (fun g ->
          let+ g = Signald.Group.parse_v2 g in
          if not (Hashtbl.mem groups g.id) then
            Hashtbl.add groups g.id g)
        gv2
  in
  match okv1, okv2 with
  | Some (), Some () -> Some ()
  | _ -> None

let show_group_list buffer =
  Hashtbl.iter
    (fun _ (g: Signald.Group.t) ->
      Weechat.printf buffer "[%s] %s\n" g.id g.title)
    groups

let process_line main_buffer line =
  let payload = Yojson.Basic.from_string line in
  let* assoc = Parsing.as_assoc payload in
  let* typ = List.assoc_opt "type" assoc >>= Parsing.as_string in
  match typ with
  | "group_list" ->
    let* data = List.assoc_opt "data" assoc in
    let* data = Parsing.as_assoc data in
    let* () = update_group_list data in
    Some (show_group_list main_buffer)
  | _ -> None
let process_line main_buffer line =
  match process_line main_buffer line with
  | Some () -> 0
  | None ->
    Weechat.printf main_buffer "unhandled message from signald: %s" line;
    0

let plugin_init () =
  let signald_buffer = Weechat.buffer_new
    "signald"
    (fun buffer _ ->
      Weechat.printf buffer "[This is a readonly buffer]";
      0)
    (fun _ -> 0)
  in

  let signald = init_signald "/run/signald/signald.sock" in

  let _ = Weechat.hook_command
    "signal"
    "Interact with weechat-ocaml-signal"
    "list groups | subscribe NUMBER"
    "TODO"
    ""
    (fun _ argv _ ->
      if Array.length argv != 3 then -1
      else
        match argv.(1), argv.(2) with
        | "list", "groups" ->
          Signald.list_groups signald !Signald.username;
          0
        | "subscribe", username ->
          Signald.subscribe signald username;
          0
        | _ -> -1)
  in

  let _ = Weechat.hook_fd
    (Obj.magic signald.socket)
    ~flag_read:true
    ~flag_write:false
    (fun _ ->
      try process_line signald_buffer (input_line signald.ic)
      with End_of_file -> 0)
  in

  ()

let () =
  Weechat_plugin.define
    ~plugin_init:plugin_init
    ~plugin_end:Weechat.plugin_end
