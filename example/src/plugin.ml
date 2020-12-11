open Bindings

let weechat_plugin_init () =
  let buffer = weechat_current_buffer () in
  weechat_command buffer "Hello from OCaml"

let () =
  Callback.register "weechat_plugin_init" weechat_plugin_init
