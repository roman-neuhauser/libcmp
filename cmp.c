/*
 * Copyright (c) 1987, 1990, 1993, 1994
 *  The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef lint
static const char copyright[] =
"@(#) Copyright (c) 1987, 1990, 1993, 1994\n\
  The Regents of the University of California.  All rights reserved.\n";
#endif

#if 0
#ifndef lint
static char sccsid[] = "@(#)cmp.c 8.3 (Berkeley) 4/2/94";
#endif
#endif

#include <sys/cdefs.h>
#if !defined(LIBCMP)
__FBSDID("$FreeBSD: release/10.0.0/usr.bin/cmp/cmp.c 216370 2010-12-11 08:32:16Z joel $");
#endif

#include <sys/types.h>
#include <sys/stat.h>

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "extern.h"

static int usage(void);
static int compare(int, char**, int);

int
main(int argc, char *argv[])
{
  int opts, ch;
  while ((ch = getopt(argc, argv, "hlsxz")) != -1)
    switch (ch) {
    case 'h':   /* Don't follow symlinks */
      opts |= CMP_NOFOLLOW;
      break;
    case 'l':   /* print all differences */
      opts |= CMP_ALLDIFFS;
      break;
    case 's':   /* silent run */
      opts |= CMP_SILENT;
      opts |= CMP_SIZEFIRST;
      break;
    case 'x':   /* hex output */
      opts |= CMP_ALLDIFFS;
      opts |= CMP_ALLHEXES;
      break;
    case 'z':   /* compare size first */
      opts |= CMP_SIZEFIRST;
      break;
    case '?':
    default:
      return usage();
    }
  argv += optind;
  argc -= optind;

  if ((opts & CMP_ALLDIFFS) && (opts & CMP_SILENT))
    errx(ERR_EXIT, "specifying -s with -l or -x is not permitted");

  if (argc < 2 || argc > 4)
    return usage();

  return compare(argc, argv, opts);
}

#define CMP_S_ISLNK(fi) S_ISLNK(fi->st->st_mode)
#define CMP_S_ISREG(fi) S_ISREG(fi->st->st_mode)

static int
cmp_stat(struct finfo *fi, int follow_symlinks)
{
  return fstatat(
    AT_FDCWD
  , fi->path
  , fi->st
  , follow_symlinks ? 0 : AT_SYMLINK_NOFOLLOW
  );
}

static struct finfo*
cmp_open(char const *path, char const *skip, int follow_symlinks)
{
  struct finfo *fi = malloc(sizeof(struct finfo));
  int is_stdin = !strcmp(path, "-");

  if (!fi)
    return NULL;

  fi->path = is_stdin ? "stdin" : path;
  fi->skip = strtol(skip, NULL, 0);
  fi->error = 0;
  fi->fd = -1;

  fi->st = malloc(sizeof(struct stat));
  if (!fi->st)
    goto cmp_open_failure;

  if (is_stdin) {
    fi->fd = 0;
    if (fstat(fi->fd, fi->st))
      goto cmp_open_failure;
  } else {
    if (cmp_stat(fi, follow_symlinks))
      goto cmp_open_failure;
    if (!CMP_S_ISLNK(fi) && -1 == (fi->fd = open(fi->path, O_RDONLY)))
      goto cmp_open_failure;
  }

  return fi;

cmp_open_failure:

  fi->error = errno;
  return fi;
}

static int
cmp_close(struct finfo *fi)
{
  int fd = fi->fd, rv = 0;
  if (fi->st)
    free(fi->st);
  free(fi);
  if (fd > -1)
    rv = close(fd);
  return rv;
}

static int
compare(int argc, char *argv[], int opts)
{
  struct finfo *fi, *f0, *f1, *fs[2] = { NULL, NULL };
  int i;

  for (i = 0; i < 2; ++i) {
    fi = fs[i] = cmp_open(argv[i], argc > (i+2) ? argv[i+2] : "0", !(opts & CMP_NOFOLLOW));

    if (!fi)
      err(ERR_EXIT, "%s", argv[i]);
    if (fi->error) {
      if (opts & CMP_SILENT)
        return ERR_EXIT;
      else
        err(ERR_EXIT, "%s", fi->path);
    }
  }

  f0 = fs[0]; f1 = fs[1];

  if (f0->fd == 0 && f1->fd == 0)
    errx(ERR_EXIT, "standard input may only be specified once");

  for (i = 0; i < 2; ++i) {
    if (CMP_S_ISLNK(fs[i^0]) && !CMP_S_ISLNK(fs[i^1])) {
      if (opts & CMP_SILENT)
        return ERR_EXIT;
      else
        errx(ERR_EXIT, "%s: Not a symbolic link", fs[i^1]->path);
    }
  }

  if (CMP_S_ISLNK(f0) && CMP_S_ISLNK(f1))
    return c_link(f0, f1, opts);
  if (!f0->fd || !f1->fd || !CMP_S_ISREG(f0) || !CMP_S_ISREG(f1))
    return c_special(f0, f1, opts);
  else
    return c_regular(f0, f1, opts);
}

static int
usage(void)
{

  (void)fprintf(stderr,
      "usage: cmp [-l | -s | -x] [-hz] file1 file2 [skip1 [skip2]]\n");
  return ERR_EXIT;
}
