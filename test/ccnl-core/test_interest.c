/**
 * @f test_interest.c
 * @b Unit tests for interests in ccnl-core
 *
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
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
 
#define NEEDS_PACKET_CRAFTING

#include "ccnl-content.h"
#include "ccnl-interest.h"
#include "ccnl-pkt-builder.h"

void test1()
{
  int result = 0;
  assert_int_equal(result, 0);
}


void test_ccnl_interest_isSame_equal()
{
    ccnl_interest_opts_u options;
    struct ccnl_prefix_s *prefix = ccnl_URItoPrefix("/test/data", 0, NULL);
    struct ccnl_interest_s * interest = ccnl_mkInterestObject(prefix, &options);
            
    int result = ccnl_interest_isSame(interest, interest->pkt);
    assert_int_equal(result, 1);
}

void test_ccnl_interest_isSame_not_equal()
{
    ccnl_interest_opts_u options;
    struct ccnl_prefix_s *prefix = ccnl_URItoPrefix("/test/data", 0, NULL);
    struct ccnl_interest_s * interest = ccnl_mkInterestObject(prefix, &options);

    struct ccnl_prefix_s *another_prefix = ccnl_URItoPrefix("/another/test/data", 0, NULL);
    struct ccnl_interest_s *another_interest = ccnl_mkInterestObject(another_prefix, &options);
            
    int result = ccnl_interest_isSame(interest, another_interest->pkt);
    assert_int_equal(result, 0);

    result = ccnl_interest_isSame(another_interest, interest->pkt);
    assert_int_equal(result, 0);
}
 
int main(void)
{
    const UnitTest tests[] = {
        unit_test(test1),
        unit_test(test_ccnl_interest_isSame_equal),
        unit_test(test_ccnl_interest_isSame_not_equal),
    };
   
    return run_tests(tests);
}
