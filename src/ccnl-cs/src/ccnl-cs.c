/*
 * @file ccnl-cs.c
 * @brief CS - Content Store
 *
 * Copyright (C) 2018 HAW Hamburg
 * Copyright (C) 2018 MSA Safety
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */
#include "ccnl-cs.h"

#include <stdio.h>
#include <string.h>

/** Maximum number of entries in the content store */
static size_t max_entries = 32;

void
ccnl_cs_init(ccnl_cs_ops_t *ops,
             ccnl_cs_op_add_t add_fun,
             ccnl_cs_op_lookup_t lookup_fun,
             ccnl_cs_op_remove_t remove_fun,
             ccnl_cs_op_clear_t clear_fun,
             ccnl_cs_op_print_t print_fun,
             ccnl_cs_op_age_t age_fun,
             ccnl_cs_op_exists_t exists_fun) {

    if (ops) {
        ops->add = add_fun; 
        ops->lookup = lookup_fun; 
        ops->remove = remove_fun;
        ops->clear = clear_fun;
        ops->print = print_fun;
        ops->age = age_fun;
        ops->exists= exists_fun;
    }

    return;
}

void ccnl_cs_set_cs_capacity(size_t size) {
    max_entries = size;
}

size_t ccnl_cs_get_cs_capacity(void) {
    return max_entries;
}

ccnl_cs_status_t
ccnl_cs_add(ccnl_cs_ops_t *ops,
            const ccnl_cs_name_t *name,
            const ccnl_cs_content_t *content) {
    int result = CS_CONTENT_IS_INVALID;

    if (ops) {
        if (name) {
            if (content) {
                return ops->add(name, content); 
            }
        } else {
            result = CS_NAME_IS_INVALID;
        }
    } else {
        result = CS_OPTIONS_ARE_NULL;
    }

    return result;
}

ccnl_cs_status_t
ccnl_cs_lookup(ccnl_cs_ops_t *ops,
               const ccnl_cs_name_t *name, ccnl_cs_content_t *content) {
    int result = CS_CONTENT_IS_INVALID;

    if (ops) {
        if (name) {
            if (content) {
                return ops->lookup(name, content);
            }
        } else {
            result = CS_NAME_IS_INVALID;
        }
    } else {
        result = CS_OPTIONS_ARE_NULL;
    }

    return result;
}

ccnl_cs_status_t
ccnl_cs_remove(ccnl_cs_ops_t *ops,
               const ccnl_cs_name_t *name) {
    int result = CS_NAME_IS_INVALID;

    if (ops) {
        if (name) {
            return ops->remove(name);
        }
    } else {
        result = CS_OPTIONS_ARE_NULL;
    }

    return result;
}

ccnl_cs_status_t
ccnl_cs_clear(ccnl_cs_ops_t *ops) {
    int result = CS_OPTIONS_ARE_NULL;
    
    if (ops) {
        return ops->clear();
    }

    return result;
}

int
ccnl_cs_print(ccnl_cs_ops_t *ops) {
    int result = CS_OPTIONS_ARE_NULL;
    
    if (ops) {
        return ops->print();
    }

    return result;
}

ccnl_cs_status_t
ccnl_cs_age(ccnl_cs_ops_t *ops) {
    int result = CS_OPTIONS_ARE_NULL;

    if (ops) {
        return ops->age();
    }

    return result;
}

ccnl_cs_status_t
ccnl_cs_exists(ccnl_cs_ops_t *ops,
        const ccnl_cs_name_t *name,
        ccnl_cs_match_t mode) {
    int result = CS_OPTIONS_ARE_NULL;

    if (ops) {
        return ops->exists(name, mode);
    }

    return result;
}
