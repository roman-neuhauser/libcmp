  $ . $TESTDIR/tools.sh

  $ printf "%s\n" fubar > f1
  $ printf "%s\n" Fubar > f2
  $ printf "%s\n" snafubar > f3
  $ ln -s f1 l1
  $ ln -s f2 l2
  $ ln -s l2 l22
  $ ln -s x1 lx

  $ cmp -z f1 f2
  f1 f2 differ: char 1, line 1
  [1]

  $ cmp -z f1 f3
  f1 f3 differ: size
  [1]

  $ hexdump -Cs1 f1
  00000001  75 62 61 72 0a                                    |ubar.|
  00000006
  $ hexdump -Cs1 f2
  00000001  75 62 61 72 0a                                    |ubar.|
  00000006
  $ hexdump -Cs4 f3
  00000004  75 62 61 72 0a                                    |ubar.|
  00000009

  $ cmp -z f1 f2 1 1

  $ cmp -z f1 f3 1 4

  $ cmp -z f1 l1

  $ cmp -z f1 l2
  f1 l2 differ: char 1, line 1
  [1]

  $ cmp -z f2 l22

  $ cmp -z f2 lx
  cmp: lx: No such file or directory
  [2]

