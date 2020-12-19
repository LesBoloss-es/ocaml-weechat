# OCaml bindings to the Weechat plugin API

This library allows to write Weechat plugins in OCaml.

This is a thin wrapper on top of the [Weechat plugin API](https://weechat.org/files/doc/stable/weechat_plugin_api.en.html).
Each C funtion `weechat_xxx` from the API is (or will be) bound to an OCaml
function `Weechat.xxx` in the `Weechat_api` module.

We also offer a `Weechat_plugin` module to reduce the amount of boilerplate
necessary to write and compile a Weechat plugin.
An example of such plugin is given in the `example/` folder (see the `dune` file
and the `Makefile` in particular).

**This is a work in progress**.
At the moment very few functions are available but we are planning to bind more
API points as we need them.
Feedbacks and/or requests for more functions are very welcome.
