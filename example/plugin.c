#include <stdlib.h>
#include <stdio.h>

#include <weechat/weechat-plugin.h>

#include <caml/mlvalues.h>
#include <caml/callback.h>
#include <caml/alloc.h>

WEECHAT_PLUGIN_NAME("length")
WEECHAT_PLUGIN_DESCRIPTION("Test plugin for WeeChat")
WEECHAT_PLUGIN_AUTHOR("Martin Pépin <kerl@wkerl.me>")
WEECHAT_PLUGIN_VERSION("0.1")
WEECHAT_PLUGIN_LICENSE("GPL3")

struct t_weechat_plugin *weechat_plugin = NULL;

int ocaml_nb_chars(const char*);
int ocaml_nb_chars(const char* s) {
  const value res = caml_callback(
    *caml_named_value("ocaml_nb_chars"),
    caml_copy_string(s)
  );
  return Int_val(res);
}

/* callback for command "/length" */

int command_length_cb(const void *pointer, void *data,
                      struct t_gui_buffer *buffer,
                      int argc, char **argv, char **argv_eol) {
    /* make C compiler happy */
    (void) pointer;
    (void) data;
    (void) buffer;
    (void) argv;

    if (argc > 1) {
      const int nb_chars = ocaml_nb_chars(argv_eol[1]);
      char msg[128];
      for (int i = 0; i < 128; i++) msg[i] = '\0';
      snprintf(msg, 128, "arg has length %d\n", nb_chars);
      weechat_command(NULL, msg);
    }

    return WEECHAT_RC_OK;
}

int weechat_plugin_init(struct t_weechat_plugin *plugin,
                        int argc, char *argv[]) {

  /* Initialize the OCaml runtime.
   * XXX. I didn't manage to get caml_startup to accept argv */
  (void)argc;
  (void)argv;
  char* nul_ptr = NULL;
  caml_startup(&nul_ptr);

  weechat_plugin = plugin;

  weechat_hook_command("length",
                       "Computes the length of its argument",
                       "message",
                       "message: argument",
                       NULL,
                       &command_length_cb, NULL, NULL);

  return WEECHAT_RC_OK;
}

int weechat_plugin_end (struct t_weechat_plugin *plugin) {
  (void) plugin;
  return WEECHAT_RC_OK;
}
