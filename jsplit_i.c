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
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <j_lib2.h>
#include <j_lib2m.h>

#include "jsplit.h"

/*
 * init_w() -- initialize work area
 */
void init_w(struct s_work *w, char *a)

{
  init_finfo(&(w->err));
  w->err.fp    = stderr;

  w->min_allowed      = (jm_counter) 1;
  w->max_allowed      = JS_MAX_BYTES;
  w->max_items        = JS_MAX_BYTES;
  w->limit            = JS_MAX_SPLIT; /* max number of output files */
  w->num_files        = 0;
  w->verbose          = (int) FALSE;
  w->force            = (int) FALSE;
  w->split_binary     = (int) TRUE;
  w->random_split     = (int) FALSE;

}  /* init_w() */

/*
 * init() -- initialize
 */
void init(int argc, char **argv, struct s_work *w)

{

  init_w(w, argv[0]);

  process_arg(argc, argv, w);

  if (w->random_split == TRUE)
    w->max_items = j2_randrange(w->min_allowed, w->max_allowed);

}  /* init() */
