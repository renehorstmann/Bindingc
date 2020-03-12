/**
 * Copyright (c) 2020 renehorstmann
 *
 * Modified version of:
 * Copyright (c) 2017 rxi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See `log.c` for details.
 */

#ifndef LOGC_LOG_H
#define LOGC_LOG_H

#ifndef LOGC_LEADING_TEXT
#define LOGC_LEADING_TEXT ""
#endif

#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>

// use the following definition to stop using colors
// #define LOG_DO_NOT_USE_COLOR

typedef void (*bc_log_lock_function)(void *user_data, bool lock);

enum bc_log_level {
    LOG_TRACE, LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR, LOG_WTF, LOG_NUM_LEVELS
};

#define bc_log_trace(...) log_base_(LOG_TRACE, LOGC_LEADING_TEXT, __FILE__, __LINE__, __VA_ARGS__)
#define bc_log_debug(...) log_base_(LOG_DEBUG, LOGC_LEADING_TEXT, __FILE__, __LINE__, __VA_ARGS__)
#define bc_log_info(...)  log_base_(LOG_INFO,  LOGC_LEADING_TEXT, __FILE__, __LINE__, __VA_ARGS__)
#define bc_log_warn(...)  log_base_(LOG_WARN,  LOGC_LEADING_TEXT, __FILE__, __LINE__, __VA_ARGS__)
#define bc_log_error(...) log_base_(LOG_ERROR, LOGC_LEADING_TEXT, __FILE__, __LINE__, __VA_ARGS__)
#define bc_log_wtf(...)   log_base_(LOG_WTF,   LOGC_LEADING_TEXT, __FILE__, __LINE__, __VA_ARGS__)

void bc_log_set_log_file(FILE *file);
void bc_log_set_min_level(enum bc_log_level level);
void bc_log_set_quiet(bool set);
void bc_log_set_locking_function(bc_log_lock_function fun);
void bc_log_set_locking_function_user_data(void *user_data);

void log_base_(enum bc_log_level level, const char *leading_text, const char *file, int line, const char *format, ...);

#endif //LOGC_LOG_H