/* Copyright (C) 2021-2021 Fuwn
 * SPDX-License-Identifier: GPL-3.0-only */

#ifndef VIV_VIV_H
#define VIV_VIV_H
#pragma once

typedef void(*split_fn)(const char *, int, void *);

/* static const char *VIV_version = "1.0.0"; */

int VIV_exit(int, const char *, ...);
/* http://www.martinbroadhurst.com/split-a-string-in-c.html */
void VIV_split(const char *, char, split_fn, void *);
void VIV_add_to_dynamic_array(const char *, int, void *);

char *strsep(char **__restrict, const char *__restrict);

#endif /* VIV_VIV_H */
