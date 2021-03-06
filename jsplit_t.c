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

#ifndef _MSDOS
#include <sys/param.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <j_lib2.h>
#include <j_lib2m.h>

#include "jsplit.h"

/*
 * splittxt() -- Text File Split
 */
void splittxt(struct s_work *w, char *ifile)

{
  struct s_file_info in;
  struct s_file_info out;
  char ofname[JS_MAX_SIZE];
  jm_counter fname_counter;
  char *buf = (char *) NULL;
  size_t bsiz = (size_t) 0;

  init_finfo(&(in));
  init_finfo(&(out));
  fname_counter = (jm_counter) 0;

  if (! open_in(&(in.fp), ifile, "r", w->err.fp) )
    {
      fprintf(w->err.fp, MSG_ERR_E000, PROG_NAME, SWITCH_CHAR, ARG_HELP);
      exit(EXIT_FAILURE);
    }

  split_oout(w, &out, ofname, "w", &fname_counter);

  /*** process text split ***/
  while (j2_getline(&buf, &bsiz, in.fp) > (ssize_t) -1)
    {
      in.io++;
      if ((out.io >= w->max_items) && (fname_counter < w->limit ))
	{
	  if (w->verbose)
	    split_log(FALSE, w, &out, ofname, ifile, LIT_LTEXT, LIT_LLINES);
	  split_oout(w, &out, ofname, "w", &fname_counter);
	  out.io = (jm_counter) 0;
	  if (w->random_split == TRUE)
	    w->max_items = j2_randrange(w->min_allowed, w->max_allowed);
	}
      fprintf(out.fp, "%s", buf);
      out.io++;
      memset(buf, JLIB2_CHAR_NULL, bsiz);
    } /* end: while */

  if (strlen(buf) > 0) /* if no \n on last line */
    {
      in.io++;
      out.io++;
      fprintf(out.fp, "%s", buf);
    }

  if (w->verbose)
    {
      split_log(TRUE, w, &out, ofname, ifile, LIT_LTEXT, LIT_LLINES);
      fprintf(w->err.fp, MSG_RPT_R19, LIT_READ, in.io, ifile);
    }

  /* DONE */
  if (buf != (char *) NULL)
    free(buf);

  close_in(&(in.fp), ifile);
  close_out(&out);
  
} /* splittxt() */
