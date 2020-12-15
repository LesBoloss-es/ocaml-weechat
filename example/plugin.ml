open Weechat_api

let say buffer _ argv_eol =
  let message = argv_eol.(1) in
  Weechat.command
    buffer
    (Format.sprintf "From OCaml: \"%s\"" message)

let shrug buffer _ _ =
  Weechat.command buffer "¯\\_(ツ)_/¯"

let listen my_buffer socket =
  let ic = Unix.in_channel_of_descr socket in
  fun _ ->
    try
      let line = input_line ic in
      Weechat.printf my_buffer "Read on socket: %s" line;
      0
    with End_of_file -> 0

let listen_hook = ref None

let start_listening my_buffer _ _ argv_eol =
  let path = argv_eol.(1) in
  let socket = Unix.socket Unix.PF_UNIX Unix.SOCK_STREAM 0 in
  Unix.connect socket (Unix.ADDR_UNIX path);
  let hook =Weechat.hook_fd
    (Obj.magic socket)
    ~flag_read:true
    ~flag_write:false
    (listen my_buffer socket)
  in
  listen_hook := Some (hook);
  0

let plugin_init () =
  let my_buffer = Weechat.buffer_new
    "ocaml-test"
    (fun buffer text ->
      Weechat.printf buffer "You just typed: %s" text;
      0)
    (fun _ -> 0)
  in

  let say_hook = Weechat.hook_command
    "ocamlsay" "Say something from OCaml"
    "msg" "msg: the message to say"
    ""
    say
  in

  let _ = Weechat.hook_command
    "ocamlshutup"
    "Disable the /ocamlsay command definitively"
    "" "" ""
    (fun _ _ _ -> Weechat.unhook say_hook; 0)
  in

  let _ = Weechat.hook_command
    "shrug"
    "print a shrug emoji in the current buffer"
    "" "" "" shrug
  in

  let _ = Weechat.hook_command
    "ocamllisten"
    "start listening on a unix socket"
    "sock_addr" "sock_addr: address of the unix socket" "%(filename)"
    (start_listening my_buffer)
  in

  let _ = Weechat.hook_command
    "ocamlstoplistening"
    "stop listening on the unix socket"
    "" "" ""
    (fun _ _ _ ->
      match !listen_hook with
        | Some hook -> Weechat.unhook hook; 0
        | None -> 0)
  in

  ()

let () =
  Weechat_plugin.define
    ~plugin_init:plugin_init
    ~plugin_end:Weechat.plugin_end
