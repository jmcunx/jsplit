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
#include <j_lib2.h>
#include <j_lib2m.h>

#include "jsplit.h"

#define MAXARG 80

char *jsplit_a_c="$Id: jsplit_a.c,v 2.6 2021/02/21 20:53:25 jmccue Exp $";

/*
 * check_args() -- mane sure all required args supplied
 */
int check_args(int optind, int argc, struct s_work *w)
{
  int i;

  if (w->max_items < 1)
    {
      fprintf(w->err.fp, MSG_ERR_E004LU, w->max_items, SWITCH_CHAR, ARG_MAX_SIZE);
      fprintf(w->err.fp, MSG_ERR_E000, PROG_NAME, SWITCH_CHAR, ARG_HELP);
      return(EXIT_FAILURE);
    }
  if (w->random_split == TRUE)
    {
      if (w->min_allowed < 1)
	{
	  fprintf(w->err.fp, MSG_ERR_E085, SWITCH_CHAR, ARG_MIN_SIZE);
	  fprintf(w->err.fp, MSG_ERR_E000, PROG_NAME, SWITCH_CHAR, ARG_HELP);
	  return(EXIT_FAILURE);
	}
      if (w->max_allowed <= w->min_allowed)
	{
	  fprintf(w->err.fp, MSG_ERR_E084, SWITCH_CHAR, ARG_MIN_SIZE, w->min_allowed, SWITCH_CHAR, ARG_MAX_SIZE, w->max_allowed);
	  fprintf(w->err.fp, MSG_ERR_E000, PROG_NAME, SWITCH_CHAR, ARG_HELP);
	  return(EXIT_FAILURE);
	}
    }

  for (i = optind; i < argc; i++)
    (w->num_files)++;
  if (w->num_files == 0)
    (w->num_files)++;  /* stdin when no files */
  if (w->num_files != 1)
    {
      fprintf(w->err.fp, MSG_ERR_E028);
      fprintf(w->err.fp, MSG_ERR_E000, PROG_NAME, SWITCH_CHAR, ARG_HELP);
      return(EXIT_FAILURE);
    }

  /*** seems fine ***/
  return(EXIT_SUCCESS);

} /* check_args() */

/*
 * process_arg() -- process arguments
 */
void process_arg(int argc, char **argv, struct s_work *w)

{
  char ckarg[MAXARG];
  int opt;
  int ok;
  char *ferr = (char *) NULL; 
  int display_help = (int) FALSE;
  int display_rev  = (int) FALSE;

  snprintf(ckarg, MAXARG, "%c%c%c%c%c%c%c:%c:%c:%c:", 
		 ARG_FORCE, ARG_HELP, ARG_TEXT, ARG_VERBOSE, ARG_VERSION,
                 ARG_RANDOM_SPLIT,
		 ARG_ERR, ARG_MAX_SIZE, ARG_LIMIT_FILES, ARG_MIN_SIZE);

  while ((opt = getopt(argc, argv, ckarg)) != -1)
    {
      switch (opt)
	{
	  case ARG_RANDOM_SPLIT:
	    w->random_split = (int) TRUE;
	    break;
	  case ARG_FORCE:
	    w->force = (int) TRUE;
	    break;
	  case ARG_TEXT:
	    w->split_binary = (int) FALSE;
	    break;
	  case ARG_HELP:
	    display_help = (int) TRUE;
	    break;
	  case ARG_VERBOSE:
	    w->verbose = (int) TRUE;
	    break;
	  case ARG_VERSION:
	    display_rev = (int) TRUE;
	    break;
	  case ARG_ERR:
	    ferr = optarg;
	    break;
	  case ARG_MIN_SIZE:
	    if (j2_is_numr(optarg))
	      w->min_allowed = (jm_counter) atol(optarg);
	    else
	      {
		fprintf(w->err.fp, MSG_ERR_E008, optarg, SWITCH_CHAR, ARG_MIN_SIZE);
		fprintf(w->err.fp, MSG_ERR_E000, PROG_NAME, SWITCH_CHAR, ARG_HELP);
		exit(EXIT_FAILURE);
	      }
	    break;
	  case ARG_LIMIT_FILES:
	    if (j2_is_numr(optarg))
	      w->limit = (jm_counter) atol(optarg);
	    else
	      {
		fprintf(w->err.fp, MSG_ERR_E008, optarg, SWITCH_CHAR, ARG_LIMIT_FILES);
		fprintf(w->err.fp, MSG_ERR_E000, PROG_NAME, SWITCH_CHAR, ARG_HELP);
		exit(EXIT_FAILURE);
	      }
	    break;
	  case ARG_MAX_SIZE:
	    if (j2_is_numr(optarg))
	      {
		w->max_items   = (jm_counter) atol(optarg);
		w->max_allowed = w->max_items;
	      }
	    else
	      {
		fprintf(w->err.fp, MSG_ERR_E008, optarg, SWITCH_CHAR, ARG_MAX_SIZE);
		fprintf(w->err.fp, MSG_ERR_E000, PROG_NAME, SWITCH_CHAR, ARG_HELP);
		exit(EXIT_FAILURE);
	      }
	    break;
	  default:
	    fprintf(w->err.fp, MSG_ERR_E000, PROG_NAME, SWITCH_CHAR, ARG_HELP);
	    exit(EXIT_FAILURE);
	    break;
	}
    }

  /*** if necessary - save stdout/err files and open ***/
  ok = open_out(stderr, &(w->err), ferr, "w", w->force);
  if (ok != EXIT_SUCCESS)
    {
      fprintf(w->err.fp, MSG_ERR_E000, PROG_NAME, SWITCH_CHAR, ARG_HELP);
      exit(EXIT_FAILURE);
    }

  /*** show help/rev and exit ? ***/
  ok = EXIT_SUCCESS;
  if (display_help)
    ok = show_brief_help(w->err.fp, PROG_NAME);
  if (display_rev)
    ok = show_rev(w->err.fp, PROG_NAME);

  /*** Check arguments ***/
  if (ok == EXIT_SUCCESS)
    ok = check_args(optind, argc, w);

  /*** check status ***/
  if (ok != EXIT_SUCCESS)
    {
      close_out(&(w->err));
      exit(ok);
    }

} /* END process_arg() */

/* END: jsplit_a.c */
