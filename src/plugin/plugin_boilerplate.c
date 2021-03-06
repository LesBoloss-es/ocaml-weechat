#include <stdlib.h>
#include <stdio.h>

#include <weechat/weechat-plugin.h>

#include <caml/mlvalues.h>
#include <caml/callback.h>
#include <caml/alloc.h>

struct t_weechat_plugin *weechat_plugin = NULL;

int weechat_plugin_init(struct t_weechat_plugin *plugin,
                        int argc, char *argv[]) {

  /* Initialize the OCaml runtime.
   * XXX. I didn't manage to get caml_startup to accept argv */
  (void)argc;
  (void)argv;
  char* nul_ptr = NULL;
  caml_startup(&nul_ptr);

  /* Initialize the plugin global pointer. */
  weechat_plugin = plugin;

  /* Give the control to OCaml. */
  caml_callback(*caml_named_value("caml_weechat_plugin_init"), Val_unit);

  return WEECHAT_RC_OK;
}

int weechat_plugin_end (struct t_weechat_plugin *plugin) {
  (void) plugin;

  /* Let us perform some cleanups from OCaml */
  caml_callback(*caml_named_value("caml_weechat_plugin_end"), Val_unit);

  return WEECHAT_RC_OK;
}
