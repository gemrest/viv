/* Copyright (C) 2021-2021 Fuwn
 * SPDX-License-Identifier: GPL-3.0-only */

#include "viv/viv.h"

#include <stdbool.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <string.h>
#include <unistd.h>

#include "viv/dynamic_array.h"
#include "viv/cli.h"
#include "viv/gemini.h"
#include "viv/log.h"
#include "viv/ssl.h"
#include "viv/ui.h"

int main(int argc, char *argv[]) {
  CLI_handle cli_handle;
  GEMINI_context gemini_ctx;
  char request_buffer[1024]; /* char *request; */
  SSL_CTX *ssl_context;
  VIV_SSL_connection_context connection_context;
  SSL *ssl;
  bool debug;
  bool trace;
  char response_buffer[16384];

  cli_handle = CLI_cli(argc, (const char **)argv);
  gemini_ctx.content = "";
  gemini_ctx.header.header = "";
  if ((gemini_ctx.header.header = calloc(0, strlen(gemini_ctx.header.header))) == NULL) {
    VIV_ERROR("%s\n", "calloc(...) failed")
  }
  if (cli_handle.port != 1965) { /* There's probably faster ways, but this works... */
    sprintf(request_buffer, "gemini://%s:%d/\r\n", cli_handle.argv[0], cli_handle.port);
  } else {
    sprintf(request_buffer, "gemini://%s\r\n", cli_handle.argv[0]);
  }
  debug = (cli_handle.options & CLI_option_DEBUG);
  trace = (cli_handle.options & CLI_option_TRACE);

  SSL_library_init();

  ssl_context = VIV_SSL_init_ctx();
  connection_context = VIV_SSL_open_connection((const char **)cli_handle.argv[0], cli_handle.port);
  if ((ssl = SSL_new(ssl_context)) == NULL) {
    VIV_ERROR("SSL_new(...) failed: %s\n", ERR_error_string(ERR_get_error(), NULL))
  }

  if (SSL_set_tlsext_host_name(ssl, cli_handle.argv[0]) == 0) {
    VIV_ERROR("SSL_set_tlsext_host_name(...) failed: %s\n", ERR_error_string(ERR_get_error(), NULL))
  }
  if (SSL_set_fd(ssl, connection_context.socket) == 0) {
    VIV_ERROR("SSL_set_fd(...) failed: %s\n", ERR_error_string(ERR_get_error(), NULL))
  }

  if (SSL_connect(ssl) == -1) {
    VIV_ERROR("SSL_connect(...) failed: %s\n", ERR_error_string(ERR_get_error(), NULL))
  } else {
    int read_iteration;
    int bytes_read;

    read_iteration = 0;

    if (cli_handle.options & CLI_option_SHOW_CERTS) { VIV_SSL_show_certs(ssl); }

    SSL_write(ssl, request_buffer, (int)strlen(request_buffer));

    for (;;) {
      bytes_read = SSL_read(ssl, response_buffer, sizeof(response_buffer));
      response_buffer[bytes_read] = 0;

      if (bytes_read == 0) { break; }

      if (trace) {
        VIV_TRACE(
          "read_iteration == %d: "
          "sizeof(SSL_read(...)): %lu, strlen(SSL_read(...)): %lu\n",
          read_iteration, sizeof(response_buffer), strlen(response_buffer)
        )
      }

      if (read_iteration == 0) {
        if ((gemini_ctx.header.header = realloc(
          gemini_ctx.header.header,
          strlen(response_buffer) + 1
        )) == NULL) {
          VIV_ERROR("%s", "realloc(...) failed")
        }
        strcpy(gemini_ctx.header.header, response_buffer);
        if (debug) { GEMINI_parse_header(gemini_ctx.header.header, &gemini_ctx.header); }
      } else {
        char *original_content;

        if ((original_content = calloc(1, strlen(gemini_ctx.content) + 1)) == NULL) {
          VIV_ERROR("%s", "malloc(...) failed")
        }
        if (strlen(gemini_ctx.content) > 0) { strcpy(original_content, gemini_ctx.content); }

        /* TODO(FIX): calloc() causes memory leak */
        if ((gemini_ctx.content = calloc(
          1,
          strlen(gemini_ctx.content) + strlen(response_buffer) + 1
        )) == NULL) {
          VIV_ERROR("%s", "malloc(...) failed")
        }
        /* TODO(FIX): realloc() causes Segmentation Fault */
        /* gemini_ctx.content = realloc(
          gemini_ctx.content,
          strlen(gemini_ctx.content) + strlen(response_buffer) + 1
        ); */

        strcat(gemini_ctx.content, original_content);
        strcat(gemini_ctx.content, response_buffer);

        free(original_content);
      } read_iteration++;
    }
    SSL_shutdown(ssl);
    SSL_free(ssl);

    /* I have zero clue why the content has corruption within the first
     * thirteen characters... let's just cut it off... - Fuwn, ~2021. 08. 28.
     *
     * I'm back with more! Turns out, this was somehow related to the fact that
     * I was strcpy(..)-ing into uninitialized memory. As soon as I replaced the
     * malloc(...)s with calloc(...)s, it was fixed! - Fuwn, 2021. 08. 31. */
    /* gemini_ctx.content = &gemini_ctx.content[12]; */
  }

  VIV_INFO(
    "header  (strlen: %d):   %s",
    strlen(gemini_ctx.header.header),
    gemini_ctx.header.header
  )
  VIV_INFO(
    "content (strlen: %d): %s",
    strlen(gemini_ctx.content),
    gemini_ctx.content
  )

  VIV_DYNAMIC_ARRAY_dynamic_array *array = VIV_DYNAMIC_ARRAY_create(0);

  VIV_split(gemini_ctx.content, '\n', VIV_add_to_dynamic_array, array);

  char *lines[VIV_DYNAMIC_ARRAY_get_count(array)];

  for (int i = 0; i < (int)VIV_DYNAMIC_ARRAY_get_count(array); ++i) {
    lines[i] = VIV_DYNAMIC_ARRAY_get(array, i);

    if (strlen(lines[i]) > 1) {
      lines[i][strlen(lines[i]) - 1] = 0;
      if (lines[i][strlen(lines[i]) - 1] == '\r') {
        lines[i][strlen(lines[i]) - 1] = ' ';
      }
    } else if (strlen(lines[i]) == 1) {
      lines[i] = " ";
    }
  }

  UI_initialise(lines, (int)(sizeof(lines) / sizeof(*lines)));

  VIV_DYNAMIC_ARRAY_for_each(array, free);
  VIV_DYNAMIC_ARRAY_delete(array);

  close(connection_context.socket);
  SSL_CTX_free(ssl_context);
  ERR_free_strings();
  EVP_cleanup();

  free(gemini_ctx.header.header);

  return EXIT_SUCCESS;
}

int VIV_exit(int exit_code, const char *format, ...) {
  if (format == NULL) { exit(exit_code); }

  va_list args;
  int print_result;

  va_start(args, format);

  print_result = vfprintf(exit_code ? stderr : stdout, format, args);

  va_end(args);

  return print_result;
}

void VIV_split(const char *string, char separator, split_fn function, void *data) {
  unsigned int start, stop;

  start = 0;

  for (stop = 0; string[stop]; ++stop) {
    if (string[stop] == separator) {
      function(string + start, stop - start, data);
      start = stop + 1;
    }
  }

  function(string + start, stop - start, data);
}

/* https://stackoverflow.com/a/58244503/14452787 */
char *strsep(char **__restrict stringp, const char *__restrict delim) {
  char *rv = *stringp;

  if (rv) {
    *stringp += strcspn(*stringp, delim);

    if (**stringp) {
      *(*stringp)++ = '\0';
    } else {
      *stringp = 0;
    }
  }

  return rv;
}

void VIV_add_to_dynamic_array(const char *string, int length, void *data) {
  VIV_DYNAMIC_ARRAY_dynamic_array *array = data;
  char *token = calloc(length + 1, 1);

  memcpy(token, string, length);

  VIV_DYNAMIC_ARRAY_add_tail(array, token);
}
