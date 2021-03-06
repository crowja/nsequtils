/**
 *  @file nsequtils.h
 *  @version 0.2.0-dev0
 *  @date Wed Dec  4 12:42:12 CST 2019
 *  @copyright 2018 John A. Crow <crowja@gmail.com>
 *  @license Unlicense <http://unlicense.org/>
 */

#ifndef NSEQUTILS_H
#define NSEQUTILS_H

/**
 *  @brief FIXME Example function.
 *  @details FIXME longer description here ...
 *  @param cp [in] Pointer to a string.
 */
void        nsequtils_clean(char *s, int flag);

/**
 *  @brief FIXME Example function.
 *  @details FIXME longer description here ...
 *  @param cp [in] Pointer to a string.
 */
void        nsequtils_count_gc(char *s, unsigned *gc, unsigned *at, unsigned *others);

/**
 *  @brief FIXME Example function.
 *  @details FIXME longer description here ...
 *  @param cp [in] Pointer to a string.
 */
void        nsequtils_crush(char *s);

/**
 *  @brief FIXME Example function.
 *  @details FIXME longer description here ...
 *  @param cp [in] Pointer to a string.
 */
void        nsequtils_extract_fasta_id(char **id, char *h);

/**
 *  @brief FIXME Example function.
 *  @details FIXME longer description here ...
 *  @param cp [in] Pointer to a string.
 */
double      nsequtils_nwscore(char *a, char *b, double match, double indel,
                              double mismatch, double ambig, double *wrk);

/**
 *  @brief FIXME Example function.
 *  @details FIXME longer description here ...
 *  @param cp [in] Pointer to a string.
 */
unsigned    nsequtils_orf_length(char *s, unsigned offset);

/**
 *  @brief FIXME Example function.
 *  @details FIXME longer description here ...
 *  @param cp [in] Pointer to a string.
 */
void        nsequtils_revcom(char *s);

/**
 *  @brief FIXME Example function.
 *  @details FIXME longer description here ...
 *  @param cp [in] Pointer to a string.
 */
double      nsequtils_swscore(char *a, char *b, double match,
                              double indel, double mismatch, double ambig, double *wrk);
/**
 *  @brief FIXME Example function.
 *  @details FIXME longer description here ...
 *  @param cp [in] Pointer to a string.
 */
void        nsequtils_to_dna(char *s);

/**
 *  @brief FIXME Example function.
 *  @details FIXME longer description here ...
 *  @param cp [in] Pointer to a string.
 */
void        nsequtils_to_rna(char *s);

/**
 *  @brief FIXME Example function.
 *  @details FIXME longer description here ...
 *  @param cp [in] Pointer to a string.
 */
int         nsequtils_left_align(unsigned type, char *a, char *b,
                                 double match, double indel,
                                 double mismatch, double ambig,
                                 double *wrk, unsigned *pos, double *score);

/**
 *  @brief Return the version of this package.
 *  @details FIXME longer description here ...
 */
const char *nsequtils_version(void);

#endif
