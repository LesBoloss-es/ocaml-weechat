#ifndef OCAML_WEECHAT_BINDINGS_H
#define OCAML_WEECHAT_BINDINGS_H

#include <weechat/weechat-plugin.h>

#define CAML_NAME_SPACE
#include <caml/mlvalues.h> /* value */

/* The global plugin pointer must be declared in the client code */
extern struct t_weechat_plugin *weechat_plugin;

/* 14. Hooks */

typedef int (*__weechat_command_callback_t)
            (const void*, void*, struct t_gui_buffer*, int, char**, char**);

#define hook_unbox(block) (*(struct t_hook**) Data_custom_val(block))

value caml_weechat_hook_command(value command,
                                value description,
                                value args,
                                value args_description,
                                value completion,
                                value callback);

/* 15. Buffers */

extern struct custom_operations gui_buffer_ops;
#define gui_buffer_unbox(block) (*(struct t_gui_buffer**)Data_custom_val(block))

value caml_weechat_current_buffer(value unit);

/* 19. Commands */

value caml_weechat_command(value buffer, value cmd);

#endif
