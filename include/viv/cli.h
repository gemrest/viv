/* Copyright (C) 2021-2021 Fuwn
 * SPDX-License-Identifier: GPL-3.0-only */

#ifndef VIV_CLI_H
#define VIV_CLI_H
#pragma once

#include <stdio.h>

/* static const char *CLI_help = "usage: %s [option] <hostname> [port]\n"
                                  "options:\n"
                                  "  -c, --cert     print the received certificates\n"
                                  "  -h, --help     you are here\n"
                                  "  -v, --version  print viv's version information\n"; */

typedef enum {
  CLI_option_SHOW_CERTS  = (1 << 0),
  CLI_option_DEBUG       = (1 << 1),
  CLI_option_PORT        = (1 << 2),
  CLI_option_TRACE       = (1 << 3)
} CLI_option;

typedef struct {
  unsigned char options : 4;
  int argc;
  char **argv;
  int port;
} CLI_handle;

CLI_handle CLI_cli(int argc, const char *argv[]);
void CLI_usage(FILE *, const char *);

#endif /* VIV_CLI_H */
