/* Copyright (C) 2021-2021 Fuwn
 * SPDX-License-Identifier: GPL-3.0-only */

#include <viv/gemini.h>

#include <stdlib.h>
#include <string.h>

#include <viv/log.h>
#include <viv/viv.h>

void GEMINI_parse_header(
  const char *header,
  GEMINI_context_header *context_header
) {
  context_header->parsed.status_code = atoi(header);
  context_header->meta = malloc(strlen(&header[0]));
  strcpy(context_header->meta, &header[3]);

  /* Get the first digit of status_code_as_an_int */
  /* Not used anymore, but I left it in, in case I need it later. */
  /* while (status_code_as_an_int >= 10) {
    status_code_as_an_int /= 10;
  } */

  switch (context_header->parsed.status_code) {
    case 10: {
      context_header->parsed.status_code_enum = GEMINI_status_code_INPUT;
    } break;
    case 11: {
      context_header->parsed.status_code_enum = GEMINI_status_code_SENSITIVE_INPUT;
    } break;
    case 20: {
      context_header->parsed.status_code_enum = GEMINI_status_code_SUCCESS;
    } break;
    case 30: {
      context_header->parsed.status_code_enum = GEMINI_status_code_TEMPORARY_REDIRECT;
    } break;
    case 31: {
      context_header->parsed.status_code_enum = GEMINI_status_code_PERMANENT_REDIRECT;
    } break;
    case 40: {
      context_header->parsed.status_code_enum = GEMINI_status_code_TEMPORARY_FAILURE;
    } break;
    case 41: {
      context_header->parsed.status_code_enum = GEMINI_status_code_SERVER_UNAVAILABLE;
    } break;
    case 42: {
      context_header->parsed.status_code_enum = GEMINI_status_code_CGI_ERROR;
    } break;
    case 43: {
      context_header->parsed.status_code_enum = GEMINI_status_code_PROXY_ERROR;
    } break;
    case 44: {
      context_header->parsed.status_code_enum = GEMINI_status_code_SLOW_DOWN;
    } break;
    case 50: {
      context_header->parsed.status_code_enum = GEMINI_status_code_PERMANENT_FAILURE;
    } break;
    case 51: {
      context_header->parsed.status_code_enum = GEMINI_status_code_NOT_FOUND;
    } break;
    case 52: {
      context_header->parsed.status_code_enum = GEMINI_status_code_GONE;
    } break;
    case 53: {
      context_header->parsed.status_code_enum = GEMINI_status_code_PROXY_REFUSED;
    } break;
    case 59: {
      context_header->parsed.status_code_enum = GEMINI_status_code_BAD_REQUEST;
    } break;
    case 60: {
      context_header->parsed.status_code_enum = GEMINI_status_code_CLIENT_CERTIFICATE_REQUIRED;
    } break;
    case 61: {
      context_header->parsed.status_code_enum = GEMINI_status_code_CERTIFICATE_NOT_AUTHORISED;
    } break;
    case 62: {
      context_header->parsed.status_code_enum = GEMINI_status_code_CERTIFICATE_NOT_VALID;
    } break;
    default: {
      VIV_ERROR("invalid status code: %d\n", context_header->parsed.status_code)
      VIV_exit(EXIT_FAILURE, NULL);
    }
  }
}
