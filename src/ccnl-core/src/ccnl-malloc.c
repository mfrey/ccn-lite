/*
 * @f ccnl-malloc.c
 * @b CCN lite (CCNL), core header file (internal data structures)
 *
 * Copyright (C) 2011-17, University of Basel
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
 * File history:
 * 2017-06-16 created
 */
#include "ccnl-malloc.h"
#include "ccnl-logging.h"
#include "ccnl-overflow.h"


#ifdef USE_DEBUG_MALLOC

#ifdef CCNL_ARDUINO
void*
debug_malloc(size_t s, const char *fn, int lno, double tstamp)
#else
void*
debug_malloc(size_t s, const char *fn, int lno, char *tstamp)
#endif
{
    /** check if the operation can be performed without causing an integer overflow */
    if (!INT_ADD_OVERFLOW_P(s, sizeof(struct mhdr))) {
        struct mhdr *h = (struct mhdr *) malloc(s + sizeof(struct mhdr));
        /** memory allocation failed */
        if (!h) {
            return NULL;
        }

        h->next = mem;
        mem = h;
        h->fname = (char *) fn;
        h->lineno = lno;
        h->size = s;

#ifdef CCNL_ARDUINO
        h->tstamp = tstamp;
#else
        /** determine size of the timestamp */
        size_t timestamp_size = strlen(tstamp);
        /** check if +1 can safely be added */
        if (!INT_ADD_OVERFLOW_P(timestamp_size, 1)) {
            char *timestamp = malloc(timestamp_size + 1); 

            if (timestamp) {
                h->tstamp = strcpy(timestamp, tstamp); 
            /** allocating the timestamp failed */
            } else { 
                /** free previously allocated memory */
                free(h);
                return NULL;
            }
        /** potential integer overflow detected, apparently tstamp was 'garbage'  */
        } else {
            /** free previously allocated memory */
            free(h);
            return NULL;
        }
#endif 
        return ((unsigned char *)h) + sizeof(struct mhdr);
    }

    return NULL;
}

#ifdef CCNL_ARDUINO
void*
debug_calloc(size_t n, size_t s, const char *fn, int lno, double tstamp)
#else
void*
debug_calloc(size_t n, size_t s, const char *fn, int lno, char *tstamp)
#endif
{
    void *p = NULL;
    /** can the operation be performed without causing an integer overflow */
    if (!INT_MULT_OVERFLOW_P(n, s)) {
         p = debug_malloc(n * s, fn, lno, tstamp);

         if (p) {
            memset(p, 0, n*s);
         }
    }

    return p;
}

int
debug_unlink(struct mhdr *hdr)
{
    struct mhdr **pp = &mem;

    for (pp = &mem; pp; pp = &((*pp)->next)) {
        if (*pp == hdr) {
            *pp = hdr->next;
            return 0;
        }
    if (!(*pp)->next)
            break;
    }
    return 1;
}

void*
debug_realloc(void *p, size_t s, const char *fn, int lno)
{
    struct mhdr *h = (struct mhdr *) (((unsigned char *)p) - sizeof(struct mhdr));
    /** 
     * check if the add operation in the realloc/malloc call below would cause an 
     * integer overflow 
     */
    if (INT_ADD_OVERFLOW_P(s, sizeof(struct mhdr))) {
        return NULL;
    }

    if (p) {
        if (debug_unlink(h)) {
            CONSOLE("%s @@@ memerror - realloc(%s:%d) at "
                    "%s:%d does not find memory block\n",
                    timestamp(), h->fname, h->lineno, fn, lno);
            return NULL;
        }

        h = (struct mhdr *) realloc(h, s+sizeof(struct mhdr));

        if (!h) {
            return NULL;
        }
    } else {
        h = (struct mhdr *) malloc(s+sizeof(struct mhdr));

        if (!h) {
            return NULL;
        }
    }

    h->fname = (char *) fn;
    h->lineno = lno;
    h->size = s;
    h->next = mem;
    mem = h;
    return ((unsigned char *)h) + sizeof(struct mhdr);
}

#ifdef CCNL_ARDUINO
void*
debug_strdup(const char *s, const char *fn, int lno, double tstamp)
#else
void*
debug_strdup(const char *s, const char *fn, int lno, char *tstamp)
#endif
{
    char *cp = NULL;

    if (s) {
        size_t str_size = strlen(s);
        if (!INT_ADD_OVERFLOW_P(str_size, 1)) {
            cp = (char*) debug_malloc(str_size +1, fn, lno, tstamp);
             
            if (cp) {
                strcpy(cp, s);
            }
        }
    }

    return cp;
}

void
debug_free(void *p, const char *fn, int lno)
{
    struct mhdr *h = (struct mhdr *) (((unsigned char *)p) - sizeof(struct mhdr));

    if (!p) {
//      CONSOLE("%s: @@@ memerror - free() of NULL ptr at %s:%d\n",
//         timestamp(), fn, lno);
        return;
    }
    if (debug_unlink(h)) {
        CONSOLE(
           "%s @@@ memerror - free() at %s:%d does not find memory block %p\n",
                timestamp(), fn, lno, p);
        return;
    }
#ifndef CCNL_ARDUINO
    if (h->tstamp && *h->tstamp)
         free(h->tstamp);
#endif
    //free(h);
    // instead of free: do a
       memset(h+1, 0x8f, h->size);
    // to discover continued use of a freed memory zone
}



#endif // USE_DEBUG_MALLOC
