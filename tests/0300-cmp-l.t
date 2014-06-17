  $ . $TESTDIR/tools.sh

  $ printf "%s\n" foobar > f1
  $ printf "%s\n" foo bar > f2

  $ cmp -l f1 f2
  cmp: EOF on f1
       4 142  12
       5 141 142
       6 162 141
       7  12 162
  [1]

  $ cmp -l f1 f2 3 4

  $ printf "%s\n" bar baz > f1
  $ printf "%s\n" foo bar baz > f2

  $ cmp -l f1 f2
  cmp: EOF on f1
       1 142 146
       2 141 157
       3 162 157
       7 172 162
  [1]

  $ cmp -l f1 f2 0 4

