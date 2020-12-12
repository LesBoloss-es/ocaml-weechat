open Weechat_api

let say buffer _ argv_eol =
  let message = argv_eol.(1) in
  Weechat.command
    buffer
    (Format.sprintf "From OCaml: \"%s\"\n" message)

let weechat_plugin_init () =
  Weechat.hook_command
    "ocamlsay"
    "Say something from OCaml"
    "msg"
    "msg: the message to say"
    ""
    say

let () =
  Callback.register "weechat_plugin_init" weechat_plugin_init
