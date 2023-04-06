/*
 * Copyright (c) 1994 1995 1996 ... 2023 2024
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

/*
 * jsplit_j.c -- routines from lib j_lib2
 */

#ifndef _MSDOS
#include <sys/param.h>
#endif

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "jsplit.h"

#ifdef _AIX
#define HAS_GETLINE 1
#endif
#ifdef linux
#define HAS_GETLINE 1
#endif

#ifdef __NetBSD_Version__
#define HAS_GETLINE 1
#define BSD_RAND 1
#endif
#ifdef OpenBSD
#define HAS_GETLINE 1
#define BSD_RAND 1
#endif
#ifdef __FreeBSD__
#define HAS_GETLINE 1
#define BSD_RAND 1
#endif
#ifdef __DragonFly__
#define HAS_GETLINE 1
#define BSD_RAND 1
#endif

#ifndef FILE_URANDOM
#define FILE_URANDOM "/dev/urandom"
#endif
#ifndef FILE_RANDOM
#define FILE_RANDOM  "/dev/random"
#endif

#define SIZE_GETLINE_BUF 256

/*
 * j2_f_exist() -- determines if a file exists
 */
int j2_f_exist(char *file_name)

{
  if (file_name == (char *) NULL)
    return((int) FALSE);

#ifdef _MSDOS
  if (access(file_name, 00) == -1)
    return (FALSE);
  else
    return (TRUE);
#else
  struct stat file_info;
  if (stat(file_name, &file_info) == 0)
    return (TRUE);
  else
    return (FALSE);
#endif

} /* j2_f_exist() */

/*
 * j2_clr_str() -- Clears a string with a char & ensure it ends with NULL
 */
long int j2_clr_str(char *s, char c, int size)

{
  if (s == (char *) NULL)
    return(0L);

  memset(s, (int) c, (size_t) size);
  s[size - 1 ] = JLIB2_CHAR_NULL;

  return((long int) strlen(s));

} /* j2_clr_str() */

/*
 * j2_getline() -- A front end to getline(3) or a hack for
 *                 systems without getline(3)
 */
SSIZE_T j2_getline(char **buf, size_t *n, FILE *fp)
{
#ifdef HAS_GETLINE
  return(getline(buf, n, fp));
#else

  if ((*buf) == (char *) NULL)
    {
      (*n) = SIZE_GETLINE_BUF + 1;
      (*buf) = (char *) malloc(((*n) * sizeof(char)));
      if ((*buf) == (char *) NULL)
	return(-1);
      j2_clr_str((*buf), (*n), JLIB2_CHAR_NULL);
    }

  if (fgets((*buf), (*n), fp) == (char *) NULL)
    return(-1);
  return((SSIZE_T) strlen((*buf)));

#endif

} /* j2_getline() */

/*
 * j2_is_numr() -- determines if all characters are numeric
 */
int j2_is_numr(char *s)

{
  if (s == (char *) NULL)
    return((int) FALSE); /* NULL pointer */

  for ( ; (*s) != JLIB2_CHAR_NULL; s++)
    {
      if ( ! isdigit((int)(*s)) )
	return(FALSE);
    }

  return(TRUE);

} /* j2_is_numr() */

/*
 * j2_getseed() -- get a seed value
 */
unsigned int j2_getseed(void)
{
  unsigned int s = (unsigned int) 0;
  int fd, r;

  fd = open(FILE_URANDOM, O_RDONLY);

  if (fd >= 0)
    {
      r = read(fd, &s, sizeof(s));
      close(fd);
      if (r != sizeof(s))
	s = 0U;
    }
  if (s == 0U)
    s = (unsigned int) time(0);

  return(s);

} /* j2_getseed() */

/*
 * j2_randrange() -- returns a random number between a range
 */
unsigned int j2_randrange(unsigned int low, unsigned int max)
{
  unsigned int c = (unsigned int) 0;

#ifdef BSD_RAND
  c = (arc4random() % (max - low + 1)) + low; 
#else
  unsigned int s = (unsigned int) 0;
  s = j2_getseed();
  srand(s);
  c = (rand() % (max - low + 1)) + low; 
#endif

  return(c);

} /* j2_randrange() */
