/*
 *  CUnit - A Unit testing framework library for C.
 *  Copyright (C) 2001        Anil Kumar
 *  Copyright (C) 2004, 2005  Anil Kumar, Jerry St.Clair
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "CUnit.h"
#include "test_function.h"

/* WARNING - MAINTENANCE NIGHTMARE AHEAD
 *
 * If you change any of the tests & suites below, you also need
 * to keep track of changes in the result statistics and reflect
 * any changes in the result report counts in print_example_results().
 *
 * Yes, this could have been designed better using a more
 * automated mechanism.  No, it was not done that way.
 */

#define EXIT_FAILURE 1



static CU_TestInfo tests_dev[] =
{
    { "test dev open/close", test_dev_open_close },
    { "test dev ioctl", test_dev_ioctl },
    { "test dev poll", test_dev_poll },

    CU_TEST_INFO_NULL,
};

static CU_TestInfo tests_wait[] =
{
    { "test wait_event", test_dev_wait_event },

    CU_TEST_INFO_NULL,
};

static CU_TestInfo tests_semaphore[] =
{
    { "test semaphore", test_dev_semaphore },

    CU_TEST_INFO_NULL,
};

static CU_TestInfo tests_MMZ[] =
{
    { "test MMZ", test_MMZ },

    CU_TEST_INFO_NULL,
};


static CU_SuiteInfo suites[] =
{
    { "suite_dev",  test_init, test_destroy, NULL, NULL, tests_dev},
    { "suite_wait",  test_init, test_destroy, NULL, NULL, tests_wait},
    { "suite_sema",  test_init, test_destroy, NULL, NULL, tests_semaphore},
    { "suite_MMZ",  NULL, NULL, NULL, NULL, tests_MMZ},

    CU_SUITE_INFO_NULL,
};

void AddTests(void)
{
    assert(NULL != CU_get_registry());
    assert(!CU_is_test_running());

    /* Register suites. */
    if (CU_register_suites(suites) != CUE_SUCCESS)
    {
        fprintf(stderr, "suite registration failed - %s\n",
                CU_get_error_msg());
        exit(EXIT_FAILURE);
    }

}

void print_example_results(void)
{
    fprintf(stdout, "\n\nExpected Test Results:"
            "\n\n  Error Handling  Type      # Run   # Pass   # Fail"
            "\n\n  ignore errors   suites%9u%9u%9u"
            "\n                  tests %9u%9u%9u"
            "\n                  asserts%8u%9u%9u"
            "\n\n  stop on error   suites%9u%9u%9u"
            "\n                  tests %9u%9u%9u"
            "\n                  asserts%8u%9u%9u\n\n",
            14, 14, 3,
            31, 10, 21,
            89, 47, 42,
            4, 4, 1,
            12, 9, 3,
            12, 9, 3);
}
