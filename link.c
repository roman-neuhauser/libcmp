/*-
 * Copyright (c) 2005 Brian Somers <brian@FreeBSD.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/cdefs.h>
#if !defined(LIBCMP)
__FBSDID("$FreeBSD: release/10.0.0/usr.bin/cmp/link.c 149388 2005-08-23 13:13:13Z brian $");
#endif

#include <sys/types.h>
#include <err.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "extern.h"

int
c_link(struct finfo *f0, struct finfo *f1, int opts)
{
  char buf1[PATH_MAX], *p1;
  char buf2[PATH_MAX], *p2;
  int dfound, len1, len2;
  off_t byte;
  u_char ch;

  const char *file1 = f0->path;
  const char *file2 = f1->path;
  off_t skip1 = f0->skip;
  off_t skip2 = f1->skip;

  if ((len1 = readlink(file1, buf1, sizeof(buf1) - 1)) < 0) {
    if (!(opts & CMP_SILENT))
      err(ERR_EXIT, "%s", file1);
    else
      return ERR_EXIT;
  }

  if ((len2 = readlink(file2, buf2, sizeof(buf2) - 1)) < 0) {
    if (!(opts & CMP_SILENT))
      err(ERR_EXIT, "%s", file2);
    else
      return ERR_EXIT;
  }

  if (skip1 > len1)
    skip1 = len1;
  buf1[len1] = '\0';

  if (skip2 > len2)
    skip2 = len2;
  buf2[len2] = '\0';

  dfound = 0;
  byte = 1;
  for (p1 = buf1 + skip1, p2 = buf2 + skip2; *p1 && *p2; p1++, p2++) {
    if ((ch = *p1) != *p2) {
      if (opts & CMP_ALLHEXES) {
        dfound = 1;
        (void)printf("%08llx %02x %02x\n",
            (long long)byte - 1, ch, *p2);
      } else if (opts & CMP_ALLDIFFS) {
        dfound = 1;
        (void)printf("%6lld %3o %3o\n",
            (long long)byte, ch, *p2);
      } else
        return diffmsg(file1, file2, byte, 1, opts);
    }
    byte++;
  }

  if (*p1 || *p2)
    return eofmsg (*p1 ? f1 : f0, opts);
  return dfound ? DIFF_EXIT : OK_EXIT;
}
