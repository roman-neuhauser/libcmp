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
  struct finfo *fs[2];
  off_t skips[2];
  char *bufs[2];
  char buf1[PATH_MAX], *p1;
  char buf2[PATH_MAX], *p2;
  ssize_t len;
  off_t byte;
  int dfound;
  u_char ch;
  int i;

  fs[0] = f0;
  fs[1] = f1;
  bufs[0] = buf1;
  bufs[1] = buf2;
  skips[0] = f0->skip;
  skips[1] = f1->skip;

  for (i = 0; i < 2; ++i) {
    if ((len = readlink(fs[i]->path, bufs[i], PATH_MAX - 1)) < 0) {
      if (!(opts & CMP_SILENT))
        err(ERR_EXIT, "%s", fs[i]->path);
      else
        return ERR_EXIT;
    }

    if (skips[i] > len)
      skips[i] = len;
    bufs[i][len] = '\0';
  }

  dfound = 0;
  byte = 1;
  p1 = bufs[0] + skips[0];
  p2 = bufs[1] + skips[1];
  for (; *p1 && *p2; p1++, p2++) {
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
        return diffmsg(f0, f1, byte, 1, opts);
    }
    byte++;
  }

  if (*p1 || *p2)
    return eofmsg (*p1 ? f1 : f0, opts);
  return dfound ? DIFF_EXIT : OK_EXIT;
}
