/* Copyright (C) 2021-2021 Fuwn
 * SPDX-License-Identifier: GPL-3.0-only */

#ifndef VIV_GEMINI_H
#define VIV_GEMINI_H
#pragma once

static const int GEMINI_port = 1965;

typedef enum {
  GEMINI_status_code_INPUT                       = (1 <<  0), /* 10 */
  GEMINI_status_code_SENSITIVE_INPUT             = (1 <<  1), /* 11 */
  GEMINI_status_code_SUCCESS                     = (1 <<  2), /* 20 */
  GEMINI_status_code_TEMPORARY_REDIRECT          = (1 <<  3), /* 30 */
  GEMINI_status_code_PERMANENT_REDIRECT          = (1 <<  4), /* 31 */
  GEMINI_status_code_TEMPORARY_FAILURE           = (1 <<  5), /* 40 */
  GEMINI_status_code_SERVER_UNAVAILABLE          = (1 <<  6), /* 41 */
  GEMINI_status_code_CGI_ERROR                   = (1 <<  7), /* 42 */
  GEMINI_status_code_PROXY_ERROR                 = (1 <<  8), /* 43 */
  GEMINI_status_code_SLOW_DOWN                   = (1 <<  9), /* 44 */
  GEMINI_status_code_PERMANENT_FAILURE           = (1 << 10), /* 50 */
  GEMINI_status_code_NOT_FOUND                   = (1 << 11), /* 51 */
  GEMINI_status_code_GONE                        = (1 << 12), /* 52 */
  GEMINI_status_code_PROXY_REFUSED               = (1 << 13), /* 53 */
  GEMINI_status_code_BAD_REQUEST                 = (1 << 14), /* 59 */
  GEMINI_status_code_CLIENT_CERTIFICATE_REQUIRED = (1 << 15), /* 60 */
  GEMINI_status_code_CERTIFICATE_NOT_AUTHORISED  = (1 << 16), /* 61 */
  GEMINI_status_code_CERTIFICATE_NOT_VALID       = (1 << 17), /* 62 */
} GEMINI_status_code;

typedef struct {
  GEMINI_status_code status_code_enum;
  int status_code;
} GEMINI_context_header_parsed;

typedef struct {
  char *header;
  GEMINI_context_header_parsed parsed;
  char *meta;
} GEMINI_context_header;

typedef struct {
  GEMINI_context_header header;
  char *content;
} GEMINI_context;

void GEMINI_parse_header(const char *, GEMINI_context_header *);

#endif /* VIV_GEMINI_H */
