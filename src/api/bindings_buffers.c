#include "bindings.h"

#include <weechat/weechat-plugin.h> /* weechat_* */

#define CAML_NAME_SPACE
#include <caml/mlvalues.h> /* value */
#include <caml/memory.h>   /* CAMLparamX, CAMLreturn */
#include <caml/custom.h>   /* custom_* */
#include <caml/alloc.h>    /* caml_copy_string */
#include <caml/callback.h> /* caml_callback* */


struct custom_operations gui_buffer_ops = {
 .identifier = "fr.boloss.weechat.gui_buffer",
 .finalize = custom_finalize_default,
 .compare = custom_compare_default,
 .hash = custom_hash_default,
 .serialize = custom_serialize_default,
 .deserialize = custom_deserialize_default
};


int __generic_buffer_input_cb(const void *pointer, void *data,
                              struct t_gui_buffer *buffer,
                              const char* input_data) {
  CAMLparam0();
  CAMLlocal3(bbuffer, binput_data, bres);

  (void)data;

  bbuffer = caml_alloc_custom(&gui_buffer_ops, sizeof(struct t_gui_buffer*), 0, 1);
  gui_buffer_unbox(bbuffer) = buffer;
  binput_data = caml_copy_string(input_data);

  bres = caml_callback2(*((value*)pointer), bbuffer, binput_data);

  CAMLreturn(Int_val(bres));
}

int __generic_buffer_close_cb(const void *pointer, void *callbacks,
                              struct t_gui_buffer *buffer) {
  CAMLparam0();
  CAMLlocal2(bbuffer, bres);

  (void)pointer;

  bbuffer = caml_alloc_custom(&gui_buffer_ops, sizeof(struct t_gui_buffer*), 0, 1);
  gui_buffer_unbox(bbuffer) = buffer;

  bres = caml_callback(*((value*)callbacks), bbuffer);

  /* The callbacks array will not be used anymore after this point and will be
   * freed by weechat. We must tell the GC to forget it. */
  caml_remove_global_root((value*)callbacks);
  caml_remove_global_root(((value*)callbacks) + 1);

  CAMLreturn(Int_val(bres));
}

value caml_weechat_buffer_new(value name, value input_cb, value close_cb) {
  CAMLparam3(name, input_cb, close_cb);
  CAMLlocal1(buffer);
  buffer = caml_alloc_custom(&gui_buffer_ops, sizeof(struct t_gui_buffer*), 0, 1);

  value *buffer_callbacks = calloc(2, sizeof(value));
  buffer_callbacks[0] = input_cb;
  buffer_callbacks[1] = close_cb;

  caml_register_global_root(buffer_callbacks);
  caml_register_global_root(buffer_callbacks + 1);
  gui_buffer_unbox(buffer) = weechat_buffer_new(String_val(name),
                                                __generic_buffer_input_cb,
                                                buffer_callbacks,
                                                NULL,
                                                __generic_buffer_close_cb,
                                                NULL,
                                                buffer_callbacks);
  CAMLreturn(buffer);
}

value caml_weechat_current_buffer(value unit) {
  CAMLparam1(unit);
  CAMLlocal1(buffer);
  buffer = caml_alloc_custom(&gui_buffer_ops, sizeof(struct t_gui_buffer*), 0, 1);
  gui_buffer_unbox(buffer) = weechat_current_buffer();
  CAMLreturn(buffer);
}
