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
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>

#ifdef IS_BSD
#include <err.h>
#endif

#include <j_lib2.h>
#include <j_lib2m.h>
#include "jsplit.h"

char *jsplit_c="$Id: jsplit.c,v 2.6 2021/02/21 20:53:25 jmccue Exp $";

/*
 * process_all() -- Process all files
 */
void process_all(struct s_work *w, char *fname)

{

  if (w->split_binary)
    splitbin(w, fname);
  else
    splittxt(w, fname);

}  /* process_all() */

/*
 * main()
 */
int main(int argc, char **argv)

{
  struct s_work w;
  int i;

#ifdef OpenBSD
  if(pledge("stdio rpath wpath cpath",NULL) == -1)
    err(1,"pledge\n");
#endif

  init(argc, argv, &w);

  /*** process all files ***/
  for (i = optind; i < argc; i++)
    process_all(&w, argv[i]);
  if (i == optind)
    process_all(&w, FILE_NAME_STDIN);

  close_out(&(w.err));
  exit(EXIT_SUCCESS);

}  /* main() */

/* END: jsplit.c */