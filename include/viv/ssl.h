/* Copyright (C) 2021-2021 Fuwn
 * SPDX-License-Identifier: GPL-3.0-only */

#ifndef VIV_SSL_H
#define VIV_SSL_H

#pragma once

/* https://stackoverflow.com/a/11405862/14452787 */
#define h_addr h_addr_list[0]

#include <openssl/ssl.h>

/* Definitions within this header will be prefixed by VIV_SSL opposed to just
 * SSL_ as the SSL_ namespace is already occupied by OpenSSL. */

typedef struct {
  int socket;
  const char *domain;
  const char *path;
} VIV_SSL_connection_context;

VIV_SSL_connection_context VIV_SSL_open_connection(const char **, int);
SSL_CTX *VIV_SSL_init_ctx(void);
void VIV_SSL_show_certs(SSL *);

#endif /* VIV_SSL_H */
