/*
 * Copyright (c) 2012 ... 2023 2024
 *     John McCue <jmccue@jmcunx.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
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

#ifndef JSPLIT_H

#define JSPLIT_H "INCLUDED"

#define PROG_NAME  "jsplit"

typedef unsigned long jm_counter;  /* allow 16 bit systems */

#define JS_MAX_BYTES 1000
#define JS_MAX_SPLIT ((jm_counter) 9999999)
#define JS_MAX_SIZE  11           /* format v######.x   # < JS_MAX_SPLIT */
#define JS_FNAME     "v%07lu.x"   /* see jm_counter above if changed      */

#ifndef JLIB2_CHAR_NULL
#define NO_JLIB 1
#define JLIB2_CHAR_NULL  ((char) '\0')
#endif

#ifndef NULL
#define NULL '\0'
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#ifndef SSIZE_T
#define SSIZE_T ssize_t
#endif

/*** structures ***/
struct s_file_info
  {
    FILE *fp;
    char *fname;
    jm_counter io;
  } ;

struct s_work
  {
    struct s_file_info err;     /* default stderr              */
    jm_counter min_allowed;     /* Minimum recs allowed        */
    jm_counter max_allowed;     /* split into # of bytes/lines */
    jm_counter max_items;       /* split into # of bytes/lines */
    jm_counter limit;           /* limit # of files created    */
    int num_files;              /* # of files to process       */
    int verbose;                /* TRUE or FALSE               */
    int force;                  /* TRUE or FALSE               */
    int split_binary;           /* TRUE or FALSE               */
    int random_split;           /* TRUE or FALSE               */
  } ;

#ifdef NO_JLIB
#define ARG_ERR           'e'  /* Output Error File                  */
#define ARG_FORCE         'f'  /* force create files                 */
#define ARG_HELP          'h'  /* Show Help                          */
#define ARG_LIMIT_FILES   'l'  /* Limit the number of files created  */
#define ARG_MAX_SIZE      'M'  /* Maximum Size                       */
#define ARG_MIN_SIZE      'L'  /* Low value for File Split           */
#define ARG_RANDOM_SPLIT  'r'  /* Random size on split               */
#define ARG_TEXT          't'  /* Process text mode                  */
#define ARG_VERBOSE       'v'  /* Verbose                            */
#define ARG_VERSION       'V'  /* Show Version Information           */
#define FILE_NAME_STDIN   "-"
#define LIT_INFO_01       "\tBuild: %s %s\n"
#define LIT_LBINARY       "binary"
#define LIT_LBYTES        "bytes"
#define LIT_LLINES        "lines"
#define LIT_LTEXT         "text"
#define LIT_READ          "read"
#define LIT_REV           "Revision"
#define LIT_TOTAL         "Total"
#define LIT_WRITTEN       "written"
#define MSG_ERR_E000      "Try '%s %c%c' for more information\n"
#define MSG_ERR_E002      "ERROR E002: Cannot open '%s' for write, processing aborted\n"
#define MSG_ERR_E004LU    "ERROR E004: value %lu invalid value for %c%c\n"
#define MSG_ERR_E008      "ERROR E008: '%s' is an invalid value for %c%c, must be numeric\n"
#define MSG_ERR_E025      "ERROR E025: File %s cannot be created, already exists\n"
#define MSG_ERR_E028      "ERROR E028: too many files specified\n"
#define MSG_ERR_E055      "ERROR E055: Write Error on File '%s' : %s\n"
#define MSG_ERR_E056      "ERROR E056: Open Error on File '%s' : %s\n"
#define MSG_ERR_E084      "ERROR E084: Value for %c%c (%ld) less than the Value for %c%c (%ld)\n"
#define MSG_ERR_E085      "ERROR E085: Value for %c%c greater than 0\n"
#define MSG_RPT_R12       "Splitting %s file %s\n"
#define MSG_RPT_R13       "          into multiple files with a maximum\n"
#define MSG_RPT_R14       "          of %lu %s per file,\n"
#define MSG_RPT_R15       "          limiting files created to %lu\n\n"
#define MSG_RPT_R16       "%s  in/out counts:\n"
#define MSG_RPT_R17       "    I/O type       I/O %s  File Name\n"
#define MSG_RPT_R18       "    --------- --------------- ------------------\n"
#define MSG_RPT_R19       "    %9s %15lu %s\n"
#define MSG_RPT_R20       "    ========= =============== ==================\n"
#define MSG_RPT_R21       "          Output Size Randomized using range of %lu to %lu\n"
#define SWITCH_CHAR       '-'
#define USG_MSG_ARG_ERR           "\t%c%c file\t\t: Write errors to file 'file', default stderr\n"
#define USG_MSG_ARG_FORCE         "\t%c%c\t\t: force create of files when found\n"
#define USG_MSG_ARG_HELP          "\t%c%c\t\t: Show brief help and exit\n"
#define USG_MSG_ARG_LIMIT_FILES   "\t%c%c num\t\t: Limit the number of files created\n"
#define USG_MSG_ARG_MAX_2         "\t%c%c max\t\t: Split file into 'max' bytes/lines per output file\n"
#define USG_MSG_ARG_MIN_2         "\t%c%c min\t\t: Split file into 'min' bytes/lines per output file\n"
#define USG_MSG_ARG_RANDOM_SPLIT  "\t%c%c\t\t: Split files using a Random Max Value\n"
#define USG_MSG_ARG_TEXT_1        "\t%c%c\t\t: Split in text mode\n"
#define USG_MSG_ARG_VERBOSE_5     "\t%c%c\t\t: show run stats on stderr\n"
#define USG_MSG_ARG_VERSION       "\t%c%c\t\t: Show revision information and exit\n"
#define USG_MSG_OPTIONS           "Options\n"
#define USG_MSG_USAGE_5           "usage:\t%s [OPTIONS] [FILE]\n"
#endif /* NO_JLIB */

/*** prototypes ***/
void init(int, char **, struct s_work *);
void init_finfo(struct s_file_info *);
int  show_brief_help(FILE *, char *);
int  show_rev(FILE *, char *);
void process_arg(int, char **, struct s_work *);
int  open_out(FILE *, struct s_file_info *, char *, char *, int);
void close_out(struct s_file_info *);
int  open_in(FILE **, char *, char *, FILE *);
void close_in(FILE **, char *);
void split_oout(struct s_work *, struct s_file_info *, char *, char *, jm_counter *);
char *get_progname(char *, char *);
void splitbin(struct s_work *, char *);
void splittxt(struct s_work *, char *);
void split_log(int, struct s_work *, struct s_file_info *, char *, char *, char *, char *);

#ifdef NO_JLIB
SSIZE_T j2_getline(char **buf, size_t *n, FILE *fp);
int j2_f_exist(char *file_name);
int j2_is_numr(char *s);
long int j2_clr_str(char *s, char c, int size);
unsigned int j2_getseed(void);
unsigned int j2_randrange(unsigned int low, unsigned int max);
#endif /* NO_JLIB */

#endif /*  JSPLIT_H  */
