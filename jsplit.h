/*
 * Copyright (c) 2012 ... 2021 2022
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

#endif /*  JSPLIT_H  */
