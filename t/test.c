#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <math.h>
#include <string.h>
#include "nsequtils.h"
#include "tinytest.h"
#include "tinyhelp.h"

static void
test_clean(void)
{
   char        a[] = "       a C   g TaaaG   ";
   char        b0[] = "aCgTaaaG";
   char        bu[] = "ACGTAAAG";
   char        bl[] = "acgtaaag";

   fprintf_test_info(stdout, "test_clean", "nsequtils_clean");
   nsequtils_clean(a, 0);
   ASSERT_EQUALS(0, strcmp(a, b0));
   nsequtils_clean(a, 1);
   ASSERT_EQUALS(0, strcmp(a, bu));
   nsequtils_clean(a, -1);
   ASSERT_EQUALS(0, strcmp(a, bl));
}

/* 6 yy */
static void
test_stub(void)
{
   fprintf_test_info(stdout, "test_stub", NULL);
   ASSERT_EQUALS(0, 0);
}

int
main(void)
{
   printf("%s\n", nsequtils_version());
   test_stub();                                  /* only to quiet compiler warnings */
   RUN(test_clean);
   return TEST_REPORT();
}
