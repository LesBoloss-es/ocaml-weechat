#include "bindings.h"

#include <weechat/weechat-plugin.h> /* weechat_X */

#define CAML_NAME_SPACE
#include <caml/mlvalues.h> /* value */
#include <caml/memory.h> /* CAMLparamX, CAMLreturn */


value caml_weechat_command(value buffer, value cmd) {
  CAMLparam2(buffer, cmd);

  weechat_command(NULL, String_val(cmd));

  CAMLreturn(Val_int(0));
}
