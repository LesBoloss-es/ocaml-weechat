type gui_buffer

(** {2 13. Display} *)

external printf: gui_buffer -> string -> unit = "caml_weechat_printf"
let printf buffer = Format.kasprintf (printf buffer)


(** {2 14. Hooks} *)

type hook

external hook_command: string -> string -> string -> string -> string
                       -> (gui_buffer -> string array -> string array -> int)
                       -> hook
                       = "caml_weechat_hook_command_bytecode"
                         "caml_weechat_hook_command_native"
external hook_fd: int -> flag_read:bool -> flag_write:bool -> (int -> int)
                  -> hook = "caml_weechat_hook_fd"
external unhook: hook -> unit = "caml_weechat_unhook"

(** {2 15. Buffers} *)

external buffer_new: string
                     -> (gui_buffer -> string -> int)
                     -> (gui_buffer -> int)
                     -> gui_buffer
                     = "caml_weechat_buffer_new"
external current_buffer: unit -> gui_buffer = "caml_weechat_current_buffer"

(** {2 19. Commands} *)

external command: gui_buffer -> string -> int = "caml_weechat_command"

(** {2 Cleanups} *)

external plugin_end: unit -> unit = "caml_weechat_plugin_end"
