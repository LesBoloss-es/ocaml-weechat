(** {2 15 Buffers} *)

type gui_buffer

external current_buffer: unit -> gui_buffer = "caml_weechat_current_buffer"

(** {2 19 Commands} *)

external command: gui_buffer -> string -> unit = "caml_weechat_command"
