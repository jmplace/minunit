/*
  Minunit is a very minimal library for performing unit tests in C.
  Copyright (C) 2013 Michaël Hauspie
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __MINUNIT_H__
#define __MINUNIT_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Output macro */

/* these will be changed later for platforms that do not have printf */
#include <stdio.h>
#define MU_PRINT_STR(str) printf("%s", (str))
#define MU_PRINT_INT(val) printf("%d", (val))
#define MU_PRINT_CHAR(ch) printf("%c", (ch))

/* Test declaration macro */

/* Type for test suite arrays */
    typedef int (*test_func)(void);

#define MU_SETUP(test_suite) int test_suite##_setup(void)
#define MU_TEAR_DOWN(test_suite) int test_suite##_tear_down(void)
#define MU_TEST(test_suite, test_name) int test_suite##_##test_name(void)
#define MU_TEST_SUITE(test_suite) static test_func test_suite##_tests_array[]

#define MU_ADD_TEST(test_suite, test_name) test_suite##_##test_name
#define MU_TEST_SUITE_END ((void*)0)

    static inline int mu_run_test_suite(test_func setup, test_func tear_down, test_func *tests_array, int *out_success, int *out_total)
    {
	int i;								
	int success = 0;
	for (i = 0 ; tests_array[i] != MU_TEST_SUITE_END ; ++i)
	{									
	    if (setup() == -1)					
		continue;							
	    if (tests_array[i]() == -1)			
		continue;							
	    if (tear_down() == -1)				
		continue;							
	    success++;							
	}
	if (out_total)
	    *out_total = i;
	if (out_success)
	    *out_success = success;
	if (i == success)
	    return 0;
	return -1;
    }

#define MU_RUN_TEST_SUITE(test_suite, success, total) mu_run_test_suite(test_suite##_setup, test_suite##_tear_down, test_suite##_tests_array, (success), (total))
#define MU_REPORT(test_suite, success, total)				\
    do {								\
	MU_PRINT_STR("Suite " #test_suite " ");				\
	MU_PRINT_INT(success);						\
	MU_PRINT_CHAR('/');						\
	MU_PRINT_INT(total);						\
	MU_PRINT_STR(" succeeded\n");					\
    } while(0)
#define MU_RUN_TEST_SUITE_WITH_REPORT(test_suite) do {	\
    int success = 0;					\
    int total = 0;					\
    MU_RUN_TEST_SUITE(test_suite, &success, &total);	\
    MU_REPORT(test_suite, success, total);		\
    } while(0)

/* Assertion macro */
#define MU_ASSERT(test) if ( !(test) ) return -1;
#define MU_ASSERT_EQUAL(val_to_test,reference) MU_ASSERT((val_to_test) == (reference))
#define MU_ASSERT_NOT_EQUAL(val_to_test,reference) MU_ASSERT((val_to_test) != (reference))


#ifdef __cplusplus
} /* extern 'C' */
#endif

#endif
