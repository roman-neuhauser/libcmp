  $ . $TESTDIR/tools.sh

  $ printf "%s\n" foobar > f1
  $ printf "%s\n" foo bar > f2

  $ cmp -s f1 f2
  [1]

  $ hexdump -Cs3 f1
  00000003  62 61 72 0a                                       |bar.|
  00000007
  $ hexdump -Cs4 f2
  00000004  62 61 72 0a                                       |bar.|
  00000008
  $ cmp -s f1 f2 3 4

  $ printf "%s\n" bar baz > f1
  $ printf "%s\n" foo bar baz > f2

  $ cmp -s f1 f2
  [1]

  $ hexdump -Cs0 f1
  00000000  62 61 72 0a 62 61 7a 0a                           |bar.baz.|
  00000008
  $ hexdump -Cs4 f2
  00000004  62 61 72 0a 62 61 7a 0a                           |bar.baz.|
  0000000c
  $ cmp -s f1 f2 0 4

