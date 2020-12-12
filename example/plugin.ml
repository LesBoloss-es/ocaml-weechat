open Weechat_api

let say buffer _ argv_eol =
  let message = argv_eol.(1) in
  Weechat.command
    buffer
    (Format.sprintf "From OCaml: \"%s\"" message)

let shrug buffer _ _ =
  Weechat.command buffer "¯\\_(ツ)_/¯"

let weechat_plugin_init () =
  let _ = Weechat.hook_command
    "ocamlsay"
    "Say something from OCaml"
    "msg"
    "msg: the message to say"
    ""
    say
  in
  let _ = Weechat.hook_command
    "shrug"
    "print a shrug emoji in the current buffer"
    ""
    ""
    ""
    shrug
  in
  ()

let () =
  Callback.register "weechat_plugin_init" weechat_plugin_init
