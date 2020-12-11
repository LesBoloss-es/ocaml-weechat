#ifndef OCAML_WEECHAT_BINDINGS_H
#define OCAML_WEECHAT_BINDINGS_H

#include <weechat/weechat-plugin.h>

#define CAML_NAME_SPACE
#include <caml/mlvalues.h> /* value */

extern struct t_weechat_plugin *weechat_plugin;

value caml_weechat_command(value buffer, value cmd);

#define gui_buffer_unbox(block) (*(struct t_gui_buffer**)Data_custom_val(block))

value caml_weechat_current_buffer(value unit);

#endif
