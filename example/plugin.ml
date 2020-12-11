open Weechat_api

let weechat_plugin_init () =
  let buffer = Weechat.current_buffer () in
  Weechat.command buffer "Hello from OCaml"

let () =
  Callback.register "weechat_plugin_init" weechat_plugin_init
