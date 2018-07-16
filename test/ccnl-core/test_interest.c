#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
 
#include "ccnl-interest.h"
 
void test1(void **state)
{
  int result = 0;
  assert_int_equal(result, 0);
}
 
int main(void)
{
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(test1),
  };
 
  return cmocka_run_group_tests_name("core: interest tests", tests, NULL, NULL);
}
