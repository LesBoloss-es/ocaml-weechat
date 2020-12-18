open Weechat_api


let log = open_out "/tmp/weechat_signal.log" |> Format.formatter_of_out_channel
let log args =
  Format.fprintf log args


module Signald = struct
  type t = {socket: Unix.file_descr; ic: in_channel}

  let make socket =
    let ic = Unix.in_channel_of_descr socket in
    {socket; ic}

  let send t =
    Format.kasprintf
      (fun s -> log "Sending: %s@." s;
        let _ = Unix.write_substring t.socket s 0 (String.length s) in
        ())

  let subscribe t (number: string) =
    send t {|{"type": "subscribe", "username": %s}|} number

  let close t = close_in t.ic
end

let init_signald path =
  let socket = Unix.(socket PF_UNIX SOCK_STREAM 0) in
  let rec try_connect retry =
    try Unix.connect socket (Unix.ADDR_UNIX path)
    with Unix.Unix_error _ ->
      log "Could not connect to socket %s, retrying in %d seconds@." path retry;
      Unix.sleep retry;
      try_connect (retry + 1)
  in
  try_connect 1;
  Signald.make socket

let process_line main_buffer line =
  (* Match on line["type"] here *)
  Weechat.printf main_buffer "Unhandled input: %s" line;
  0

let plugin_init () =
  let signald_buffer = Weechat.buffer_new
    "signald"
    (fun buffer _ ->
      Weechat.printf buffer "[This is a readonly buffer]";
      0)
    (fun _ -> 0)
  in

  let signald = init_signald "/run/signald/signald.sock" in

  let _ = Weechat.hook_command
    "signalsub"
    "Start receiving messages for a given number"
    "number"
    "number: phone number for which to receive messages"
    ""
    (fun _ argv _ -> Signald.subscribe signald argv.(1); 0)
  in

  let _ = Weechat.hook_fd
    (Obj.magic signald.socket)
    ~flag_read:true
    ~flag_write:false
    (fun _ ->
      try process_line signald_buffer (input_line signald.ic)
      with End_of_file -> 0)
  in

  ()

let () =
  Weechat_plugin.define
    ~plugin_init:plugin_init
    ~plugin_end:Weechat.plugin_end
