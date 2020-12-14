#include <weechat/weechat-plugin.h> /* weechat_X */

#include "bindings.h"

#define CAML_NAME_SPACE
#include <caml/mlvalues.h> /* value */
#include <caml/memory.h> /* CAMLparam, CAMLxparam, CAMLlocal, CAMLreturn */
#include <caml/custom.h> /* custom_* */
#include <caml/callback.h> /* caml_callback */
#include <caml/alloc.h> /* caml_copy_string */

struct t_hashtable *hook_table = NULL;


static struct t_hashtable_item*
add_hook_to_table(const struct t_hook* hook, value *closure) {
  if (hook_table == NULL)
    weechat_hashtable_new(
      32, WEECHAT_HASHTABLE_POINTER, WEECHAT_HASHTABLE_POINTER, NULL, NULL
    );

  caml_register_global_root(closure);
  return weechat_hashtable_set(hook_table, hook, closure);
}

void remove_hook_from_table(const struct t_hook* hook) {
  if (hook_table == NULL) return;

  value* closure = weechat_hashtable_get(hook_table, hook);
  if (closure) caml_remove_global_root(closure);

  weechat_hashtable_remove(hook_table, hook);
}

static void hook_table_clear_callback(void* data,
                                      struct t_hashtable *ht,
                                      const void *key,
                                      const void *closure) {
  (void)data;
  (void)ht;
  (void)key;
  caml_remove_global_root((value*)closure);
}

void hook_table_clear() {
  weechat_hashtable_map(hook_table, hook_table_clear_callback, NULL);
  weechat_hashtable_free(hook_table);
}

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
  add_hook_to_table(hook_unbox(hook), closure_ptr);

  CAMLreturn(hook);
}

value caml_weechat_hook_command_bytecode(value *argv, int argc) {
  (void)argc;
  return caml_weechat_hook_command_native(argv[0], argv[1], argv[2], argv[3],
                                          argv[4], argv[5]);
}

value caml_weechat_unhook(value bhook) {
  CAMLparam1(bhook);
  struct t_hook *hook = hook_unbox(bhook);
  weechat_unhook(hook);
  remove_hook_from_table(hook);
  CAMLreturn(Val_int(0));
}
