#include "bindings.h"

#include <weechat/weechat-plugin.h> /* weechat_* */

#define CAML_NAME_SPACE
#include <caml/mlvalues.h> /* value */
#include <caml/memory.h> /* CAMLparamX, CAMLreturn */
#include <caml/custom.h> /* custom_* */


struct custom_operations gui_buffer_ops = {
 .identifier = "fr.boloss.weechat.gui_buffer",
 .finalize = custom_finalize_default,
 .compare = custom_compare_default,
 .hash = custom_hash_default,
 .serialize = custom_serialize_default,
 .deserialize = custom_deserialize_default
};

value caml_weechat_current_buffer(value unit) {
  CAMLparam1(unit);
  CAMLlocal1(buffer);
  buffer = caml_alloc_custom(&gui_buffer_ops, sizeof(struct t_gui_buffer*), 0, 1);
  gui_buffer_unbox(buffer) = weechat_current_buffer();
  CAMLreturn(buffer);
}
