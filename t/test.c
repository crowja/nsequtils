#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <math.h>
#include <string.h>
#include "nsequtils.h"
#include "tinytest.h"

#ifdef _COLOR_CODE
#undef _COLOR_CODE
#endif
#define _COLOR_CODE      0x1B
#ifdef _COLOR_RED
#undef _COLOR_RED
#endif
#define _COLOR_RED       "[1;31m"
#ifdef _COLOR_GREEN
#undef _COLOR_GREEN
#endif
#define _COLOR_GREEN     "[1;32m"
#ifdef _COLOR_YELLOW
#undef _COLOR_YELLOW
#endif
#define _COLOR_YELLOW    "[1;33m"
#ifdef _COLOR_RESET
#undef _COLOR_RESET
#endif
#define _COLOR_RESET     "[0m"

static void
_printf_test_name(char *name, char *info)
{
   printf("%c%s%s%c%s", _COLOR_CODE, _COLOR_YELLOW, name, _COLOR_CODE, _COLOR_RESET);

   if (NULL != info)
      printf(" [%s]\n", info);
   else
      printf("\n");
}

static int
_two_doubles_equal(double x, double y)
{
   double      t = fabs(x) + fabs(y);
   return fabs(x - y) < 4 * DBL_EPSILON * t ? 1 : 0;
}

static void
test_clean(void)
{
   char        a[] = "       a C   g TaaaG   ";
   char        b0[] = "aCgTaaaG";
   char        bu[] = "ACGTAAAG";
   char        bl[] = "acgtaaag";

   _printf_test_name("test_clean", "nsequtils_clean");

   nsequtils_clean(a, 0);
   ASSERT_EQUALS(0, strcmp(a, b0));
   nsequtils_clean(a, 1);
   ASSERT_EQUALS(0, strcmp(a, bu));
   nsequtils_clean(a, -1);
   ASSERT_EQUALS(0, strcmp(a, bl));
}

/* 7 yy */
static void
test_stub(void)
{
   ASSERT("test 1 in test_stub", _two_doubles_equal(0.0, 0.0));
   _printf_test_name("test_stub", NULL);
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
