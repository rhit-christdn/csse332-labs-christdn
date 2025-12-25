/************************************************************************************
 *
 * Copyright (c) 2025 Rose-Hulman Institute of Technology. All Rights Reserved.
 *
 * Should you find any bugs in this file, please contact your instructor as
 * soon as possible.
 *
 ***********************************************************************************/
#include <stdlib.h>

#include "cgrade.h"
#include "cgradescropify.h"
#include "hello.h"

/*
 * This is a test case function that calls whatever function you'd like to
 * test.
 */
int
test_hello_world(void)
{
  int x = print_hello_world();

  CG_ASSERT_INT_EQ(3, x);
  return CG_TEST_PASSED;
}

struct cg_test_suite *
hello_test_suite(void)
{
  struct cg_test_suite *ts =
      cg_test_suite_new("Problem 0: Build Tools Test Suite", 0);

  CG_SUITE_CREATE_GRADED_TEST(
      ts, "test_hello_world", test_hello_world, 5,
      "Check that the print_hello_world function returns correctly.");

  return ts;
}

int
main(int argc, char **argv)
{
#ifdef BUILD_GRADESCOPE
  struct cg_test_suite *ts = hello_test_suite();
  int rc                   = cg_test_suite_runall(ts);

  cg_test_suite_summarize(ts);
  cg_test_suite_gradescopify_tests(ts, "hello_tests.run.json");

  cg_test_suite_remove(ts);
  return (rc > 0) ? EXIT_FAILURE : EXIT_SUCCESS;
#else
  int rc                  = 0;
  struct cg_project *proj = cg_project_new("Problem 0: Build Tools");

  cg_project_add_suite(proj, hello_test_suite());
  cg_project_runall(proj);

  cg_project_summarize(proj);

  rc = (proj->num_failures > 0) ? EXIT_FAILURE : EXIT_SUCCESS;
  cg_project_del(proj);
  return rc;
#endif
}
