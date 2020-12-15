type gui_buffer

(** {2 14. Hooks} *)

type hook

external hook_command: string -> string -> string -> string -> string
                       -> (gui_buffer -> string array -> string array -> int)
                       -> hook
                       = "caml_weechat_hook_command_bytecode"
                         "caml_weechat_hook_command_native"
external unhook: hook -> unit = "caml_weechat_unhook"

(** {2 15. Buffers} *)

external current_buffer: unit -> gui_buffer = "caml_weechat_current_buffer"

(** {2 19. Commands} *)

external command: gui_buffer -> string -> int = "caml_weechat_command"

(** {2 Cleanups} *)

external plugin_end: unit -> unit = "caml_weechat_plugin_end"