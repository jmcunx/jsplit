/*
 * Copyright (c) 2012 ... 2016 2017 2018 
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

#include <sys/param.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <errno.h>
#include <j_lib2.h>
#include <j_lib2m.h>

#include "jsplit.h"

char *jsplit_u_c="$Id: jsplit_u.c,v 2.6 2021/02/21 20:53:25 jmccue Exp $";

/*
 * split_log() -- Show data
 */
void split_log(int eof, struct s_work *w, struct s_file_info *out, 
               char *ofile, char *ifile, char *t1, char *t2)
{
  static int show_heading = (int) TRUE;
  static jm_counter totl = (jm_counter) 0;

  if (show_heading)
    {
      show_heading = (int) FALSE;
      fprintf(w->err.fp, MSG_RPT_R12, t1, ifile);
      fprintf(w->err.fp, MSG_RPT_R13);
      fprintf(w->err.fp, MSG_RPT_R14, w->max_allowed, t2);
      if (w->random_split == TRUE)
	fprintf(w->err.fp, MSG_RPT_R21, w->min_allowed, w->max_allowed);
      fprintf(w->err.fp, MSG_RPT_R15, w->limit);
      fprintf(w->err.fp, MSG_RPT_R16, ifile);
      fprintf(w->err.fp, MSG_RPT_R17, t2);
      fprintf(w->err.fp, MSG_RPT_R18);
    }

  fprintf(w->err.fp, MSG_RPT_R19, LIT_WRITTEN, out->io, ofile);
  totl += out->io;

  if (eof)
    {
      fprintf(w->err.fp, MSG_RPT_R18);
      fprintf(w->err.fp, MSG_RPT_R19, LIT_TOTAL, totl, LIT_WRITTEN);
      fprintf(w->err.fp, MSG_RPT_R20);
    }

} /* split_log() */

/*
 * split_oout() -- close and open a new split file
 */
void split_oout(struct s_work *w, struct s_file_info *out, char *ofname, 
                char *omode, jm_counter *c)
{

  if ((*c) >= JS_MAX_SPLIT)
    return;

  if ((*c) != (jm_counter) 0)
    close_out(out);

  (*c)++;
  memset(ofname, JLIB2_CHAR_NULL, JS_MAX_SIZE);

  snprintf(ofname, JS_MAX_SIZE, JS_FNAME,(*c));

  /*** always fail if output exists, this prevents ***/
  /*** mixing different split data                 ***/
  if ( open_out(w->err.fp, out, ofname, omode, (int) FALSE) == EXIT_FAILURE )
    {
      fprintf(w->err.fp, MSG_ERR_E000, PROG_NAME, SWITCH_CHAR, ARG_HELP);
      exit(EXIT_FAILURE);
    }

} /* split_oout() */

/*
 * open_in() -- open in file
 */
int open_in(FILE **in, char *fname, char *omode, FILE *fp_err)

{
  int errsave;

  if (fname == (char *) NULL)
    {
      (*in) = stdin;
      return((int) TRUE);
    }
  if (strcmp(fname, FILE_NAME_STDIN) == 0)
    {
      (*in) = stdin;
      return((int) TRUE);
    }

  (*in) = fopen(fname, omode);
  errsave = errno;

  if ((*in) == (FILE *) NULL)
    {
      fprintf(fp_err, MSG_ERR_E056, fname, strerror(errsave));
      return((int) FALSE);
    }

  return((int) TRUE);

} /* open_in() */

/*
 * close_in() -- Close an input file
 */
void close_in(FILE **in, char *fname)

{
  if (fname == (char *) NULL)
    return;
  if (strcmp(fname, FILE_NAME_STDIN) == 0)
    return;
  
  if ((*in) != (FILE *) NULL)
    {
      fclose((*in));
      (*in) = stdin;
    }

} /* close_in() */

/*
 * open_out() -- save the file anem and check status
 */
int open_out(FILE *wfp, struct s_file_info *f, char *fname, 
             char *omode, int force)

{
  int errsave;

  if (fname == (char *) NULL)
    return(EXIT_SUCCESS);

  if (force == (int) FALSE)
    {
      if ( j2_f_exist(fname) )
	{
	  fprintf(wfp, MSG_ERR_E025, fname);
	  return(EXIT_FAILURE);
	}
    }

  f->fp = fopen(fname, omode);
  errsave = errno;
  if (f->fp == (FILE *) NULL)
    {
      f->fp = stderr;  /* needs to be something */
      fprintf(wfp, MSG_ERR_E002, fname);
      fprintf(wfp, "\t%s\n", strerror(errsave));
      return(EXIT_FAILURE);
    }

  /*** success, save file name ***/
  f->fname = strdup(fname);
  return(EXIT_SUCCESS);

} /* open_out() */

/*
 * close_out() -- close output
 */
void close_out(struct s_file_info *f)
{

  if (f->fname != (char *) NULL)
    {
      fclose(f->fp);
      free(f->fname);
      f->fname = (char *) NULL;
    }

} /* close_out() */

/*
 * init_finfo() -- initialize out file structure
 */
void init_finfo(struct s_file_info *f)

{

  f->fp    = (FILE *) NULL;
  f->fname = (char *) NULL;
  f->io    = (jm_counter) 0;

} /* init_finfo() */

/* END: jsplit_u.c */
