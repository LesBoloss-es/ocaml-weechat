#include "bindings.h"

#define CAML_NAME_SPACE
#include <caml/mlvalues.h> /* value */
#include <caml/memory.h> /* CAMLparam, CAMLxparam, CAMLlocal, CAMLreturn */
#include <caml/alloc.h>


value caml_weechat_prefix(value prefix) {
  CAMLparam1(prefix);
  CAMLlocal1(res);
  const char* p = weechat_prefix(String_val(prefix));
  if (p) {
    res = caml_alloc(1, 0);
    Store_field(res, 0, caml_copy_string(p));
  } else {
    res = Val_int(0);
  }
  CAMLreturn(res);
}


value caml_weechat_printf(value buffer, value message) {
  CAMLparam2(buffer, message);

  /* XXX. This is a stupid implementation of printf: format specifiers are
   * ignored. */
  weechat_printf(gui_buffer_unbox(buffer), "%s", String_val(message));

  CAMLreturn(Val_unit);
}
