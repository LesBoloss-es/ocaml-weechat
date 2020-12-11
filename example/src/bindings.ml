type gui_buffer

(** {3.14 Hooks} *)

external weechat_command: gui_buffer -> string -> unit = "caml_weechat_command"

(** {3.15 Buffers} *)

external weechat_current_buffer: unit -> gui_buffer = "caml_weechat_current_buffer"
