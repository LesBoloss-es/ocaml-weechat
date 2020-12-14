#include "bindings.h"

#include <weechat/weechat-plugin.h> /* weechat_* */

#define CAML_NAME_SPACE
#include <caml/mlvalues.h> /* value */
#include <caml/memory.h> /* CAMLparamX, CAMLreturn */
#include <caml/custom.h> /* custom_* */

/* Free the memory occupied by hook_table when the plugin is unloaded. */

value caml_weechat_plugin_end(value unit) {
  CAMLparam1(unit);
  hook_table_clear();
  CAMLreturn(Val_int(0));
}
