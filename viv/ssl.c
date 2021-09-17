/* Copyright (C) 2021-2021 Fuwn
 * SPDX-License-Identifier: GPL-3.0-only */

#include "viv/ssl.h"

#include <arpa/inet.h>
#include <malloc.h>
#include <netdb.h>
#include <openssl/err.h>
#include <string.h>
#include <unistd.h>

#include "viv/log.h"

VIV_SSL_connection_context VIV_SSL_open_connection(const char **hostname, int port) {
  VIV_SSL_connection_context connection_context;
  struct hostent *host;
  struct sockaddr_in addr;
  const char *hostname_copy;

  if (strstr((const char *)hostname, "/")) {
    /* https://stackoverflow.com/questions/9210528/split-string-with-delimiters-in-c */
    hostname_copy = strdup((const char *)hostname);
    connection_context.domain = strsep((char **)&hostname_copy, "/");
    strcpy((char *)hostname, connection_context.domain);
    printf("%s\n", strsep((char **)&hostname_copy, "/"));
  }

  if ((host = gethostbyname((const char *)hostname)) == NULL) {
    VIV_ERROR("gethostbyname(...) failed: %s\n", ERR_error_string(ERR_get_error(), NULL))
  }

  connection_context.socket = socket(PF_INET, SOCK_STREAM, 0);
  bzero(&addr, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = *(long *)(host->h_addr);

  if (connect(connection_context.socket, (struct sockaddr *)&addr, sizeof(addr)) != 0) {
    close(connection_context.socket);
    VIV_ERROR("connect(...) failed: %s\n", ERR_error_string(ERR_get_error(), NULL))
  }

  return connection_context;
}

SSL_CTX *VIV_SSL_init_ctx(void) {
  const SSL_METHOD *ssl_method;
  SSL_CTX *ssl_context;

  OpenSSL_add_all_algorithms();
  ERR_load_BIO_strings();
  ERR_load_crypto_strings();
  SSL_load_error_strings();

  ssl_method = SSLv23_client_method(); /* TLSv1_2_client_method() */

  if ((ssl_context = SSL_CTX_new(ssl_method)) == NULL) {
    VIV_ERROR("SSL_CTX_new(...) failed: %s\n", ERR_error_string(ERR_get_error(), NULL))
  }

  return ssl_context;
}

void VIV_SSL_show_certs(SSL *ssl) {
  X509 *certificate;
  char *line;

  certificate = SSL_get_peer_certificate(ssl);

  VIV_DEBUG("encryption: %s\n", SSL_get_cipher(ssl))

  if (certificate != NULL) {
    VIV_DEBUG("%s", "server certificate:\n")

    line = X509_NAME_oneline(X509_get_subject_name(certificate), 0, 0);
    VIV_DEBUG("  subject: %s\n", line)
    free(line);

    line = X509_NAME_oneline(X509_get_issuer_name(certificate), 0, 0);
    VIV_DEBUG("  issuer:  %s\n", line)
    free(line);

    X509_free(certificate);
  } else {
    VIV_DEBUG("%s", "no server certificate was found\n")
  }
}
