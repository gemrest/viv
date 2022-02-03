/* Copyright (C) 2021-2021 Fuwn
 * SPDX-License-Identifier: GPL-3.0-only */

#include "viv/cli.h"

#include <stdlib.h>

#include "viv/flag.h"
#include "viv/gemini.h"
#include "viv/log.h"
#include "viv/viv.h"

CLI_handle CLI_cli(int argc, const char *argv[]) {
  CLI_handle cli_handle;

  cli_handle.options = 0x0; /* 0b0000 */

  bool *help = flag_bool(
    "help",
    false,
    "you are here"
  );
  bool *debug = flag_bool(
    "debug",
    false,
    "enable debug information"
  );
  bool *certs = flag_bool(
    "certs",
    false,
    "show certificates"
  );
  bool *trace = flag_bool(
    "trace",
    false,
    "enable trace information"
  );
  size_t *port = flag_size(
    "port",
    GEMINI_port,
    "specify a custom port to use instead of 1965"
  );

  if (!flag_parse(argc, (char **)argv)) {
    CLI_usage(stderr, argv[0]);
    flag_print_error(stderr);
    VIV_exit(EXIT_FAILURE, NULL);
  }

  if (*help) {
    CLI_usage(stdout, argv[0]);
    VIV_exit(EXIT_SUCCESS, NULL);
  }

  cli_handle.argc = flag_rest_argc();
  cli_handle.argv = flag_rest_argv();

  if (cli_handle.argc <= 0) {
    VIV_WARN("%s", "no url provided\n\n")
    CLI_usage(stderr, argv[0]);
    VIV_exit(EXIT_FAILURE, NULL);
  }

  if (*certs) { cli_handle.options = (cli_handle.options | CLI_option_SHOW_CERTS); }
  if (*debug) { cli_handle.options = (cli_handle.options | CLI_option_DEBUG); }
  if (*trace) { cli_handle.options = (cli_handle.options | CLI_option_TRACE); }
  if (*port) {
    cli_handle.options = (cli_handle.options | CLI_option_PORT);
    cli_handle.port = (int)*port;
  }

  return cli_handle;
}

void CLI_usage(FILE *stream, const char *viv_path) {
  fprintf(stream, "usage: %s [option] <hostname> [port]\n", viv_path);
  fprintf(stream, "options:\n");
  flag_print_options(stream);
}
