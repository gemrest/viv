/* Copyright (C) 2021-2021 Fuwn
 * SPDX-License-Identifier: GPL-3.0-only */

#include "viv/log.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "viv/viv.h"

int VIV_LOG_log(VIV_LOG_level level, const char *format, ...) {
  FILE *log_file;
  char log_format[1024];
  va_list arguments;
  int print_result;

  log_file = stdout;

  if      (level & VIV_LOG_level_TRACE)   { strcpy(log_format, "[trace] "); }
  else if (level & VIV_LOG_level_DEBUG) { strcpy(log_format, "[debug] "); }
  else if (level & VIV_LOG_level_INFO)  { strcpy(log_format, "[info]  "); }
  else if (level & VIV_LOG_level_WARN)  {
    strcpy(log_format, "[warn]  ");
    log_file = stderr;
  } else if (level & VIV_LOG_level_ERROR) {
    strcpy(log_format, "[error] ");
    log_file = stderr;
  }

  va_start(arguments, format);

  print_result = vfprintf(log_file, strcat(log_format, format), arguments);

  va_end(arguments);

  if (level & VIV_LOG_level_ERROR) { VIV_exit(EXIT_FAILURE, NULL); }

  return print_result;
}
