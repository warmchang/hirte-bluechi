/*
 * Copyright Contributors to the Eclipse BlueChi project
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */
#pragma once

#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Constants */
#define SYMBOL_WILDCARD "*"
#define SYMBOL_GLOB_ALL '*'
#define SYMBOL_GLOB_ONE '?'

#define streq(a, b) (strcmp((a), (b)) == 0)
#define strneq(a, b, n) (strncmp((a), (b), (n)) == 0)

#define streqi(a, b) (strcasecmp(a, b) == 0)
#define strneqi(a, b, n) (strncasecmp(a, b, n) == 0)

#define bool_to_str(b) b ? "true" : "false"

static inline bool ascii_isdigit(char a) {
        return a >= '0' && a <= '9';
}

static inline bool ascii_isalpha(char a) {
        return (a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z');
}

static inline char *strcat_dup(const char *a, const char *b) {
        size_t a_len = strlen(a);
        size_t b_len = strlen(b);
        size_t len = a_len + b_len + 1;
        char *res = malloc(len);
        if (res) {
                memcpy(res, a, a_len);
                memcpy(res + a_len, b, b_len);
                res[a_len + b_len] = 0;
        }

        return res;
}

static inline bool copy_str(char **p, const char *s) {
        char *dup = strdup(s);
        if (dup == NULL) {
                return false;
        }
        free(*p);
        *p = dup;
        return true;
}

static inline bool is_wildcard(const char *in) {
        return streq(in, SYMBOL_WILDCARD);
}

static inline bool is_glob(const char *s) {
        return s != NULL && (strchr(s, SYMBOL_GLOB_ALL) != NULL || strchr(s, SYMBOL_GLOB_ONE) != NULL);
}

static inline bool str_has_prefix(const char *s, const char *prefix) {
        return strncmp(s, prefix, strlen(prefix)) == 0;
}

// NOLINTBEGIN(misc-no-recursion)
static inline bool match_glob(const char *str, const char *glob) {
        if (str == NULL || glob == NULL) {
                return false;
        }

        if (*glob == SYMBOL_GLOB_ALL) {
                return match_glob(str, glob + 1) || (*str && match_glob(str + 1, glob));
        }

        if (*str) {
                return ((SYMBOL_GLOB_ONE == *glob) || (*str == *glob)) && match_glob(str + 1, glob + 1);
        }

        return !*glob;
}
// NOLINTEND(misc-no-recursion)

static inline bool isempty(const char *a) {
        return !a || a[0] == '\0';
}

static inline bool ends_with(const char *str, const char *suffix) {
        if (str == NULL || suffix == NULL) {
                return false;
        }

        size_t str_length = strlen(str);
        size_t suffix_length = strlen(suffix);
        if (str_length < suffix_length) {
                return false;
        }
        return strncmp(str + str_length - suffix_length, suffix, suffix_length) == 0;
}

typedef struct StringBuilder StringBuilder;
struct StringBuilder {
        char *str;
        size_t len; /* Not including terminating zero */
        size_t capacity;
};

#define STRING_BUILDER_INIT { NULL, 0, 0 }
#define STRING_BUILDER_DEFAULT_CAPACITY 1024

bool string_builder_init(StringBuilder *builder, size_t initial_capacity);
void string_builder_destroy(StringBuilder *builder);
bool string_builder_append(StringBuilder *builder, const char *str);
bool string_builder_printf(StringBuilder *builder, const char *format, ...);
