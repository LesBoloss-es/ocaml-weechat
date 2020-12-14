let define ~plugin_init ~plugin_end =
  Callback.register "caml_weechat_plugin_init" plugin_init;
  Callback.register "caml_weechat_plugin_end" plugin_end
