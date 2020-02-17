/**
 *  @file nsequtils.c
 *  @version 0.2.0-dev0
 *  @date Wed Dec  4 12:42:12 CST 2019
 *  @copyright 2018 John A. Crow <crowja@gmail.com>
 *  @license Unlicense <http://unlicense.org/>
 */

#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <stdio.h>                               /* FIXME */
#include <string.h>                              /* FIXME */
#include <ctype.h>
#include "nsequtils.h"

#ifdef  IS_NULL
#undef  IS_NULL
#endif
#define IS_NULL(p)   ((NULL == (p)) ? (1) : (0))

#ifdef  FREE
#undef  FREE
#endif
#define FREE(p)      ((NULL == (p)) ? (0) : (free((p)), (p) = NULL))

static double
_align_score(unsigned type, char *a, char *b, double match, double indel, double mismatch,
             double ambig, double *wrk)
{
   /* Computes best score only for global (type = 0) and local (type = 1) alignments */
   unsigned    i, j;
   unsigned    m = strlen(a);
   unsigned    mp1 = m + 1;
   unsigned    n = strlen(b);
   unsigned    np1 = n + 1;
   double      score = -DBL_MAX;
   double      maxscore = (m < n ? m : n) * match;      /* maximum possible score */
   double      tol = DBL_EPSILON;
   double     *wrk1 = wrk;                  /* note that wrk needs to provide at least 2 * mp1 doubles */
   double     *wrk2 = wrk + mp1;

   for (i = 0; i < mp1; i++)
      wrk1[i] = wrk2[i] = 0;

   for (j = 1; j < np1; j++) {
      for (i = 1; i < mp1; i++) {
         double      s0, s1, w;

         if (IS_NULL(strchr("ACGTUacgtu", a[i - 1]))
             || IS_NULL(strchr("ACGTUacgtu", b[j - 1])))
            w = -ambig;                          /* comparing one or more ambiguity codes */

         else
            w = (a[i - 1] == b[j - 1] ? match : -mismatch);     /* unambiguous match/mismatch */

         s0 = wrk1[i - 1] + w;                   /* score without indels */
         s1 = (wrk1[i] > wrk2[i - 1] ? wrk1[i] : wrk2[i - 1]) - indel;  /* score with indels */
         wrk2[i] = s0 > s1 ? s0 : s1;

         if (type == 1)
            wrk2[i] = wrk2[i] > 0 ? wrk2[i] : 0;

         score = wrk2[i] > score ? wrk2[i] : score;

         if (fabs(score - maxscore) < tol * fabs(maxscore))
            goto DONE;
      }

      for (i = 0; i < mp1; i++)
         wrk1[i] = wrk2[i];
   }

 DONE:

   return score;
}

void
nsequtils_clean(char *s, int flag)
{
   unsigned    i, j;
   unsigned    len;

   if (IS_NULL(s))
      return;

   len = strlen(s);

   for (i = 0, j = 0; i < len; i++) {
      if (!isalpha(s[i]))
         continue;

      s[j] = s[i];

      if (flag > 0)
         s[j] = toupper(s[j]);

      else if (flag < 0)
         s[j] = tolower(s[j]);

      j += 1;
   }

   s[j] = '\0';
}

void
nsequtils_count_gc(char *s, unsigned *gc, unsigned *at, unsigned *others)
{
   unsigned    i;
   unsigned    len = strlen(s);

   *at = 0;
   *gc = 0;
   *others = 0;

   for (i = 0; i < len; i++) {
      switch (s[i]) {
         case 'A':
         case 'a':
         case 'T':
         case 't':
         case 'U':
         case 'u':
            *at += 1;
            break;

         case 'C':
         case 'c':
         case 'G':
         case 'g':
            *gc += 1;
            break;

         default:
            *others += 1;
      }
   }
}

void
nsequtils_crush(char *s)
{
   unsigned    i, j;
   unsigned    len = strlen(s);

   for (i = 0, j = 0; i < len; i++) {

      if (isspace(s[i]))
         continue;

      s[j] = s[i];
      j++;
   }

   s[j] = '\0';
}

void
nsequtils_extract_fasta_id(char **id, char *h)
{
   unsigned    len = strcspn(h, "\f\n\r\t\v ");
   *id = realloc(*id, sizeof(char) * (1 + len));
   strncpy(*id, h, len);
   (*id)[len] = '\0';
}

double
nsequtils_nwscore(char *a, char *b, double match, double indel, double mismatch,
                  double ambig, double *wrk)
{
   if (strlen(a) < strlen(b))
      return _align_score(0, a, b, match, indel, mismatch, ambig, wrk);

   else
      return _align_score(0, b, a, match, indel, mismatch, ambig, wrk);
}

unsigned
nsequtils_orf_length(char *s, unsigned offset)
{
   unsigned    i = offset;
   unsigned    length = 0;
   unsigned    flag;
   char        tmp[4];

   while (i < strlen(s)) {
      strncpy(tmp, s + i, 4);
      tmp[3] = '\0';
      nsequtils_clean(tmp, 1);
      nsequtils_to_rna(tmp);
      flag = strcmp(tmp, "UAA") && strcmp(tmp, "UGA") && strcmp(tmp, "UAG");

      if (!flag)
         return length;

      length += strlen(tmp);                     /* strlen(tmp) could be less than 3 at end */
      i += 3;
   }

   return length;
}

void
nsequtils_revcom(char *s)
{
   char        c;
   unsigned    i, j;
   unsigned    len = strlen(s);

   for (i = 0; i < len; i++) {
      switch (s[i]) {
         case 'a':
            s[i] = 't';
            break;
         case 'c':
            s[i] = 'g';
            break;
         case 'g':
            s[i] = 'c';
            break;
         case 't':
            s[i] = 'a';
            break;
         case 'u':
            s[i] = 'a';
            break;
         case 'm':
            s[i] = 'k';
            break;
         case 'r':
            s[i] = 'y';
            break;
         case 'w':
            s[i] = 'w';
            break;
         case 's':
            s[i] = 's';
            break;
         case 'y':
            s[i] = 'r';
            break;
         case 'k':
            s[i] = 'm';
            break;
         case 'v':
            s[i] = 'b';
            break;
         case 'h':
            s[i] = 'd';
            break;
         case 'd':
            s[i] = 'h';
            break;
         case 'b':
            s[i] = 'v';
            break;
         case 'n':
            s[i] = 'n';
            break;
         case 'A':
            s[i] = 'T';
            break;
         case 'C':
            s[i] = 'G';
            break;
         case 'G':
            s[i] = 'C';
            break;
         case 'T':
            s[i] = 'A';
            break;
         case 'U':
            s[i] = 'A';
            break;
         case 'M':
            s[i] = 'K';
            break;
         case 'R':
            s[i] = 'Y';
            break;
         case 'W':
            s[i] = 'W';
            break;
         case 'S':
            s[i] = 'S';
            break;
         case 'Y':
            s[i] = 'R';
            break;
         case 'K':
            s[i] = 'M';
            break;
         case 'V':
            s[i] = 'B';
            break;
         case 'H':
            s[i] = 'D';
            break;
         case 'D':
            s[i] = 'H';
            break;
         case 'B':
            s[i] = 'V';
            break;
         case 'N':
            s[i] = 'N';
            break;
         default:
            s[i] = 'N';
      }
   }

   /* Reverse */
   for (i = 0, j = len - 1; i < j; i++, j--) {
      c = s[i];
      s[i] = s[j];
      s[j] = c;
   }
}

double
nsequtils_swscore(char *a, char *b, double match, double indel, double mismatch,
                  double ambig, double *wrk)
{
   if (strlen(a) < strlen(b))
      return _align_score(1, a, b, match, indel, mismatch, ambig, wrk);

   else
      return _align_score(1, b, a, match, indel, mismatch, ambig, wrk);
}


void
nsequtils_to_dna(char *s)
{
   unsigned    i;
   unsigned    len = strlen(s);

   for (i = 0; i < len; i++) {
      switch (s[i]) {

         case 'u':
            s[i] = 't';
            break;

         case 'U':
            s[i] = 'T';
            break;

         default:
            break;
      }
   }
}

void
nsequtils_to_rna(char *s)
{
   unsigned    i;
   unsigned    len = strlen(s);

   for (i = 0; i < len; i++) {
      switch (s[i]) {

         case 't':
            s[i] = 'u';
            break;

         case 'T':
            s[i] = 'U';
            break;

         default:
            break;
      }
   }
}

int
nsequtils_left_align(unsigned type, char *a, char *b, double match, double indel,
                     double mismatch, double ambig, double *wrk, unsigned *pos,
                     double *score)
{
   /* Computes best score and its position only for global (type = 0) and local (type = 1) alignments */
   unsigned    i, j;
   unsigned    m = strlen(a);
   unsigned    mp1 = m + 1;
   unsigned    n = strlen(b);
   unsigned    np1 = n + 1;
   unsigned    best_pos = 0;
   double      best_score = -DBL_MAX;
   double     *wrk1 = wrk;
   double     *wrk2 = wrk + mp1;

   for (i = 0; i < mp1; i++)
      wrk1[i] = 0, wrk2[i] = 0;

   for (j = 1; j < np1; j++) {
      for (i = 1; i <= mp1; i++) {
         double      s0, s1, w;

         if (IS_NULL(strchr("acgtuACGTU", a[i - 1]))
             || IS_NULL(strchr("acgtuACGTU", b[j - 1])))
            w = -ambig;

         else
            w = (a[i - 1] == b[j - 1] ? match : -mismatch);

         s0 = wrk1[i - 1] + w;                   /* score without indels */
         s1 = (wrk1[i] > wrk2[i - 1] ? wrk1[i] : wrk2[i - 1]) - indel;  /* score with indels */
         wrk2[i] = s0 > s1 ? s0 : s1;

         if (type == 1)
            wrk2[i] = (wrk2[i] > 0 ? wrk2[i] : 0);

         if (wrk2[i] > best_score) {
            best_score = wrk2[i];
            best_pos = i;
         }
      }

      for (i = 0; i < mp1; i++)
         wrk1[i] = wrk2[i];
   }

   *score = best_score;
   *pos = (best_pos > 0 ? best_pos - 1 : 0);

   return 0;
}

const char *
nsequtils_version(void)
{
   return "0.2.0-dev0";
}

#undef IS_NULL
#undef FREE
