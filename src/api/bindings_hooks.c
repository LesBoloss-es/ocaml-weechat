#include <weechat/weechat-plugin.h> /* weechat_X */

#include "bindings.h"

#define CAML_NAME_SPACE
#include <caml/mlvalues.h> /* value */
#include <caml/memory.h> /* CAMLparam, CAMLxparam, CAMLlocal, CAMLreturn */
#include <caml/custom.h> /* custom_* */
#include <caml/callback.h> /* caml_callback */
#include <caml/alloc.h> /* caml_copy_string */


static struct custom_operations hook_ops = {
 .identifier = "fr.boloss.weechat.hook",
 .finalize = custom_finalize_default,
 .compare = custom_compare_default,
 .hash = custom_hash_default,
 .serialize = custom_serialize_default,
 .deserialize = custom_deserialize_default
};

int __generic_command_callback(const void *pointer,
                               void *data,
                               struct t_gui_buffer *buffer,
                               int argc,
                               char **argv,
                               char **argv_eol) {
  CAMLparam0();
  CAMLlocal4(bbuffer, res, caml_argv, caml_argv_eol);

  (void)data;

  bbuffer = caml_alloc_custom(&gui_buffer_ops, sizeof(struct t_gui_buffer*), 0, 1);
  gui_buffer_unbox(bbuffer) = buffer;

  /* XXX. Highly inefficient. */
  caml_argv = caml_alloc_tuple(argc);
  caml_argv_eol = caml_alloc_tuple(argc);
  for (int i = 0; i < argc; i++) {
    Store_field(caml_argv, i, caml_copy_string(argv[i]));
    Store_field(caml_argv_eol, i, caml_copy_string(argv_eol[i]));
  }

  res = caml_callback3(*((value*)pointer), bbuffer, caml_argv, caml_argv_eol);

  CAMLreturn(Int_val(res));
}

value caml_weechat_hook_command_native(value command,
                                       value description,
                                       value args,
                                       value args_description,
                                       value completion,
                                       value callback) {
  CAMLparam5(command, description, args, args_description, completion);
  CAMLxparam1(callback);

  CAMLlocal1(hook);
  hook = caml_alloc_custom(&hook_ops, sizeof(struct t_hook*), 0, 1);

  value *closure_ptr = malloc(sizeof(value));
  *closure_ptr = callback;
  hook_unbox(hook) = weechat_hook_command(String_val(command),
                                          String_val(description),
                                          String_val(args),
                                          String_val(args_description),
                                          String_val(description),
                                          __generic_command_callback,
                                          closure_ptr,
                                          NULL);
  __caml_closure_table_set(hook_unbox(hook), closure_ptr);

  CAMLreturn(hook);
}

value caml_weechat_hook_command_bytecode(value *argv, int argc) {
  (void)argc;
  return caml_weechat_hook_command_native(argv[0], argv[1], argv[2], argv[3],
                                          argv[4], argv[5]);
}


int __generic_fd_callback(const void *closure, void *data, int fd) {
  CAMLparam0();
  CAMLlocal1(bres);
  (void)data;
  bres = caml_callback(*((value*)closure), Val_int(fd));
  CAMLreturn(Int_val(bres));
}

value caml_weechat_hook_fd(value fd, value flag_read, value flag_write,
                           value callback) {
  CAMLparam4(fd, flag_read, flag_write, callback);
  CAMLlocal1(hook);
  hook = caml_alloc_custom(&hook_ops, sizeof(struct t_hook*), 0, 1);

  value *closure_ptr = malloc(sizeof(value));
  *closure_ptr = callback;
  hook_unbox(hook) = weechat_hook_fd(Int_val(fd),
                                     Int_val(flag_read), Int_val(flag_write), 0,
                                     __generic_fd_callback,
                                     closure_ptr, NULL);
  __caml_closure_table_set(hook_unbox(hook), closure_ptr);

  CAMLreturn(hook);
}


value caml_weechat_unhook(value bhook) {
  CAMLparam1(bhook);
  struct t_hook *hook = hook_unbox(bhook);
  weechat_unhook(hook);
  __caml_closure_table_remove(hook);
  CAMLreturn(Val_int(0));
}
