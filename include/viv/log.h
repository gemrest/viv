/* Copyright (C) 2021-2021 Fuwn
 * SPDX-License-Identifier: GPL-3.0-only */

#ifndef VIV_LOG_H
#define VIV_LOG_H

#pragma once

#define VIV_TRACE(format, ...) VIV_LOG_log(VIV_LOG_level_TRACE, format, __VA_ARGS__);
#define VIV_DEBUG(format, ...) VIV_LOG_log(VIV_LOG_level_DEBUG, format, __VA_ARGS__);
#define VIV_INFO(format, ...)  VIV_LOG_log(VIV_LOG_level_INFO,  format, __VA_ARGS__);
#define VIV_WARN(format, ...)  VIV_LOG_log(VIV_LOG_level_WARN,  format, __VA_ARGS__);
#define VIV_ERROR(format, ...) VIV_LOG_log(VIV_LOG_level_ERROR, format, __VA_ARGS__);

typedef enum {
  VIV_LOG_level_TRACE = (1 << 0),
  VIV_LOG_level_DEBUG = (1 << 1),
  VIV_LOG_level_INFO  = (1 << 2),
  VIV_LOG_level_WARN  = (1 << 3),
  VIV_LOG_level_ERROR = (1 << 4)
} VIV_LOG_level;

int VIV_LOG_log(VIV_LOG_level, const char *, ...);

#endif /* VIV_LOG_H */
