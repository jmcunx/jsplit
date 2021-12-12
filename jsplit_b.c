/*
 * Copyright (c) 2012 ... 2020 2021
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
#include <errno.h>
#include <j_lib2.h>
#include <j_lib2m.h>

#include "jsplit.h"

#define SB_WRITE_OMODE "w"
#define SB_READ_OMODE  "r"

char *jsplit_b_c="$Id: jsplit_b.c,v 2.5 2021/02/21 20:53:25 jmccue Exp $";

/*
 * splitbin() -- binary File Split
 */
void splitbin(struct s_work *w, char *ifile)

{
  struct s_file_info in;
  struct s_file_info out;
  char ofname[JS_MAX_SIZE];
  jm_counter fname_counter;
  char buffer;
  size_t bytes_read;
  size_t bytes_written;
  int errsave;

  init_finfo(&(in));
  init_finfo(&(out));
  fname_counter = (jm_counter) 0;

  if (! open_in(&(in.fp), ifile, SB_READ_OMODE, w->err.fp) )
    {
      fprintf(w->err.fp, MSG_ERR_E000, PROG_NAME, SWITCH_CHAR, ARG_HELP);
      exit(EXIT_FAILURE);
    }

  split_oout(w, &out, ofname, SB_WRITE_OMODE, &fname_counter);

  /*** process binary split ***/
  while((bytes_read = fread(&buffer, sizeof(char), 1, in.fp)) > 0)
    {
      in.io += (jm_counter) bytes_read;
      if ((out.io >= w->max_items) && (fname_counter < w->limit ))
	{
	  if (w->verbose)
	    split_log(FALSE, w, &out, ofname, ifile, LIT_LBINARY, LIT_LBYTES);
	  split_oout(w, &out, ofname, SB_WRITE_OMODE, &fname_counter);
	  out.io = (jm_counter) 0;
	  if (w->random_split == TRUE)
	    w->max_items = j2_randrange(w->min_allowed, w->max_allowed);
	}
      bytes_written = fwrite(&buffer,sizeof(char),bytes_read,out.fp);
      errsave = errno;
      if (bytes_written == bytes_read)
        out.io += (jm_counter) bytes_written;
      else
        {
	  fprintf(w->err.fp, MSG_ERR_E055, ofname, strerror(errsave));
	  exit(EXIT_FAILURE);
	}
    } /* end: while */

  if (w->verbose)
    {
      split_log(TRUE, w, &out, ofname, ifile, LIT_LBINARY, LIT_LBYTES);
      fprintf(w->err.fp, MSG_RPT_R19, LIT_READ, in.io, ifile);
    }

  /* DONE */
  close_in(&(in.fp), ifile);
  close_out(&out);
  
} /* splitbin() */

/* END: j_splitb.c */
