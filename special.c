/*-
 * Copyright (c) 1991, 1993, 1994
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

#if 0
#ifndef lint
static char sccsid[] = "@(#)special.c 8.3 (Berkeley) 4/2/94";
#endif
#endif

#include <sys/cdefs.h>
#if !defined(LIBCMP)
__FBSDID("$FreeBSD: release/10.0.0/usr.bin/cmp/special.c 223376 2011-06-21 20:44:06Z delphij $");
#endif

#include <sys/types.h>

#include <err.h>
#include <stdlib.h>
#include <stdio.h>

#include "extern.h"

int
c_special(struct finfo *f0, struct finfo *f1, int opts)
{
  int ch1, ch2;
  off_t byte, line;
  FILE *fp1, *fp2;
  int dfound;

  off_t skip1 = f0->skip;
  off_t skip2 = f1->skip;

  if ((fp1 = fdopen(f0->fd, "r")) == NULL)
    err(ERR_EXIT, "%s", f0->path);
  if ((fp2 = fdopen(f1->fd, "r")) == NULL)
    err(ERR_EXIT, "%s", f1->path);

  dfound = 0;
  while (skip1--)
    if (getc(fp1) == EOF)
      goto eof;
  while (skip2--)
    if (getc(fp2) == EOF)
      goto eof;

  for (byte = line = 1;; ++byte) {
    ch1 = getc(fp1);
    ch2 = getc(fp2);
    if (ch1 == EOF || ch2 == EOF)
      break;
    if (ch1 != ch2) {
      if (opts & CMP_ALLHEXES) {
        dfound = 1;
        (void)printf("%08llx %02x %02x\n",
            (long long)byte - 1, ch1, ch2);
      } else if (opts & CMP_ALLDIFFS) {
        dfound = 1;
        (void)printf("%6lld %3o %3o\n",
            (long long)byte, ch1, ch2);
      } else {
        return diffmsg(f0, f1, byte, line, opts);
      }
    }
    if (ch1 == '\n')
      ++line;
  }

eof:
  if (ferror(fp1))
    err(ERR_EXIT, "%s", f0->path);
  if (ferror(fp2))
    err(ERR_EXIT, "%s", f1->path);
  if (feof(fp1)) {
    if (!feof(fp2))
      return eofmsg(f0, opts);
  } else
    if (feof(fp2))
      return eofmsg(f1, opts);
  if (dfound)
    return DIFF_EXIT;
  return OK_EXIT;
}
