  $ . $TESTDIR/tools.sh

  $ printf "%s\n" foobar > f1
  $ printf "%s\n" foo bar > f2
  $ ln -s f1 l1
  $ ln -s f2 l2
  $ ln -s ffoobar l3
  $ ln -s Foobar l4

  $ pmp -xh f1 f2
  00000003 62 0a
  00000004 61 62
  00000005 72 61
  00000006 0a 72

  $ cmp -xh f1 f2
  cmp: EOF on f1
  00000003 62 0a
  00000004 61 62
  00000005 72 61
  00000006 0a 72
  [1]

  $ pmp -xh l1 l2
  00000003 62 0a
  00000004 61 62
  00000005 72 61
  00000006 0a 72

  $ cmp -xh l1 l2
  00000001 31 32
  [1]

  $ cmp -xh l3 l4
  cmp: EOF on l4
  00000000 66 46
  00000001 66 6f
  00000003 6f 62
  00000004 62 61
  00000005 61 72
  [1]

  $ cmp -xh l3 l4 2 1

  $ cmp -xh f1 l2
  cmp: f1: Not a symbolic link
  [2]

  $ cmp -xh l1 f2
  cmp: f2: Not a symbolic link
  [2]

  $ cmp -xh - l2 </dev/null
  cmp: stdin: Not a symbolic link
  [2]

  $ cmp -xh l1 - </dev/null
  cmp: stdin: Not a symbolic link
  [2]

