open Weechat_api

let say buffer _ argv_eol =
  let message = argv_eol.(1) in
  Weechat.command
    buffer
    (Format.sprintf "From OCaml: \"%s\"" message)

let shrug buffer _ _ =
  Weechat.command buffer "¯\\_(ツ)_/¯"

let plugin_init () =
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
    (fun _ _ _ -> Weechat.unhook say_hook; 1)
  in
  let _ = Weechat.hook_command
    "shrug"
    "print a shrug emoji in the current buffer"
    "" "" "" shrug
  in
  ()

let () =
  Weechat_plugin.define
    ~plugin_init:plugin_init
    ~plugin_end:Weechat.plugin_end
