#ifndef OCAML_WEECHAT_BINDINGS_H
#define OCAML_WEECHAT_BINDINGS_H

#include <weechat/weechat-plugin.h>

#define CAML_NAME_SPACE
#include <caml/mlvalues.h> /* value */

/* The global plugin pointer must be declared in the client code */
extern struct t_weechat_plugin *weechat_plugin;

/*
 * 3.14 Hooks
 */

value caml_weechat_command(value buffer, value cmd);

/*
 * 3.15 Buffers
 */

#define gui_buffer_unbox(block) (*(struct t_gui_buffer**)Data_custom_val(block))

value caml_weechat_current_buffer(value unit);

#endif
