/*-
 * Copyright (c) 1991, 1993, 1994
 *	The Regents of the University of California.  All rights reserved.
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
 *
 *	@(#)extern.h	8.3 (Berkeley) 4/2/94
 *
 * $FreeBSD: release/10.0.0/usr.bin/cmp/extern.h 227156 2011-11-06 08:14:16Z ed $
 *
 */

#define OK_EXIT		0
#define DIFF_EXIT	1
#define ERR_EXIT	2	/* error exit code */

#define CMP_NOFOLLOW    (1 << 0)
#define CMP_SILENT      (1 << 1)
#define CMP_SIZEFIRST   (1 << 2)
#define CMP_ALLDIFFS    (1 << 3)
#define CMP_ALLHEXES    (1 << 4)

struct finfo
{
  char const *path;
  struct stat *st;
  int fd;
  int error;
  off_t skip;
};

int	c_link(struct finfo *, struct finfo *, int);
int	c_regular(struct finfo *, struct finfo *, int);
int	c_special(struct finfo *, struct finfo *, int);
int	diffmsg(struct finfo *, struct finfo *, off_t, off_t, int);
int	eofmsg(struct finfo *, int);
