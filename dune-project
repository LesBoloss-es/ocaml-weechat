(lang dune 2.0)

(generate_opam_files true)
(name "weechat")
(version "0.1")
(license "GPL-3.0-or-later")
(authors "Kerl <kerl@wkerl.me>")
(maintainers "Kerl <kerl@wkerl.me>")
(source (github LesBoloss-es/ocaml-weechat))
(homepage "https://github.com/LesBoloss-es/ocaml-weechat")

(package
 (name weechat)
 (synopsis "OCaml bindings to the Weechat plugin API")
 (depends ("dune" :build)))
