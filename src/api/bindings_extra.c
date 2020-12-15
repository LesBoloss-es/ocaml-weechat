#include "bindings.h"

#include <weechat/weechat-plugin.h> /* weechat_* */

#define CAML_NAME_SPACE
#include <caml/mlvalues.h> /* value */
#include <caml/memory.h> /* CAMLparamX, CAMLreturn */
#include <caml/custom.h> /* custom_* */


/* *** [ Closure table management ] *** */

/* We maintain a table of memory locations pointing at ocaml closures and for
 * which we have no better way to handle the caml_{register,remove}_global_root
 * logic than storing them here.
 * The pointer used as key should be the pointer of the hook/buffer/whatever
 * weechat object the closure is related to.
 * The xxx_set and xxx_remove functions handle the registration and removal
 * logic but it is the responsibility of the user to call these functions at the
 * appropriate moment.
 * Note that all pointers are automatically removed in the plugin_end function.
 */


struct t_hashtable *__caml_closure_table = NULL;


struct t_hashtable_item *__caml_closure_table_set(const void *key, value *closure) {
  if (__caml_closure_table == NULL)
    __caml_closure_table = weechat_hashtable_new(
      19, WEECHAT_HASHTABLE_POINTER, WEECHAT_HASHTABLE_POINTER, NULL, NULL
    );

  caml_register_global_root(closure);
  return weechat_hashtable_set(__caml_closure_table, key, closure);
}


void __caml_closure_table_remove(const void *key) {
  if (__caml_closure_table == NULL) return;

  value *closure = weechat_hashtable_get(__caml_closure_table, key);
  if (closure) caml_remove_global_root(closure);

  weechat_hashtable_remove(__caml_closure_table, key);
}


static void __caml_closure_table_free_cb(void *data, struct t_hashtable *t,
                                         const void *key, const void *closure) {
  (void)data;
  (void)t;
  caml_remove_global_root((value*)closure);
  /* This is ridiculous but it seems I have to get the closure again to have a
   * mutable pointer on it... */
  free(weechat_hashtable_get(__caml_closure_table, key));
}

void __caml_closure_table_free() {
  weechat_hashtable_map(
    __caml_closure_table, __caml_closure_table_free_cb, NULL
  );
  weechat_hashtable_free(__caml_closure_table);
}


/* *** [ plugin_end ] *** */

/* This function must be called in weechat_plugin_end */
value caml_weechat_plugin_end(value unit) {
  CAMLparam1(unit);
  __caml_closure_table_free();
  CAMLreturn(Val_int(0));
}
