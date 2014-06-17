  $ . $TESTDIR/tools.sh

  $ printf "%s\n" foobar > f1
  $ printf "%s\n" foo bar > f2

  $ cmp -x f1 f2
  cmp: EOF on f1
  00000003 62 0a
  00000004 61 62
  00000005 72 61
  00000006 0a 72
  [1]

  $ cmp -x f1 f2 3 4

  $ printf "%s\n" bar baz > f1
  $ printf "%s\n" foo bar baz > f2

  $ cmp -x f1 f2
  cmp: EOF on f1
  00000000 62 66
  00000001 61 6f
  00000002 72 6f
  00000006 7a 72
  [1]

  $ cmp -x f1 f2 0 4

