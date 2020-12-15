#ifndef OCAML_WEECHAT_BINDINGS_H
#define OCAML_WEECHAT_BINDINGS_H

#include <weechat/weechat-plugin.h>

#define CAML_NAME_SPACE
#include <caml/mlvalues.h> /* value */

/* The global plugin pointer must be declared in the client code */
extern struct t_weechat_plugin *weechat_plugin;


/* 14. Hooks */

#define hook_unbox(block) (*(struct t_hook**) Data_custom_val(block))

value caml_weechat_hook_command(value command,
                                value description,
                                value args,
                                value args_description,
                                value completion,
                                value callback);
value caml_weechat_hook_fd(value fd, value flag_read, value flag_write, value callback);
value caml_weechat_unhook(value hook);


/* 13. Display */

value caml_weechat_printf(value buffer, value message);


/* 15. Buffers */

extern struct custom_operations gui_buffer_ops;
#define gui_buffer_unbox(block) (*(struct t_gui_buffer**)Data_custom_val(block))

value caml_weechat_buffer_new(value name, value input_cb, value close_cb);
value caml_weechat_current_buffer(value unit);


/* 19. Commands */

value caml_weechat_command(value buffer, value cmd);


/* Extra stuff */

extern struct t_hashtable *__caml_closure_table;

struct t_hashtable_item *__caml_closure_table_set(const void *key, value *closure);
void __caml_closure_table_remove(const void *key);
void __caml_closure_table_free();

#endif
