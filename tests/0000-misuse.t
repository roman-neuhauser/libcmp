  $ . $TESTDIR/tools.sh

  $ touch f1 f2

  $ cmp
  usage: cmp [-l | -s | -x] [-hz] file1 file2 [skip1 [skip2]]
  [2]

  $ cmp -
  usage: cmp [-l | -s | -x] [-hz] file1 file2 [skip1 [skip2]]
  [2]

  $ cmp f1
  usage: cmp [-l | -s | -x] [-hz] file1 file2 [skip1 [skip2]]
  [2]

  $ cmp x1 f2
  cmp: x1: No such file or directory
  [2]

  $ cmp f1 x2
  cmp: x2: No such file or directory
  [2]

  $ cmp x1 f2
  cmp: x1: No such file or directory
  [2]

  $ cmp f1 f2 0 0 0
  usage: cmp [-l | -s | -x] [-hz] file1 file2 [skip1 [skip2]]
  [2]

  $ cmp - x2
  cmp: x2: No such file or directory
  [2]

  $ cmp x1 -
  cmp: x1: No such file or directory
  [2]

  $ cmp - -
  cmp: standard input may only be specified once
  [2]

  $ cmp -hls
  cmp: specifying -s with -l or -x is not permitted
  [2]

  $ cmp -hsx
  cmp: specifying -s with -l or -x is not permitted
  [2]

  $ cmp -hz
  usage: cmp [-l | -s | -x] [-hz] file1 file2 [skip1 [skip2]]
  [2]

  $ cmp -ls
  cmp: specifying -s with -l or -x is not permitted
  [2]

  $ cmp -lz
  usage: cmp [-l | -s | -x] [-hz] file1 file2 [skip1 [skip2]]
  [2]

  $ cmp -sx
  cmp: specifying -s with -l or -x is not permitted
  [2]

  $ cmp -sz
  usage: cmp [-l | -s | -x] [-hz] file1 file2 [skip1 [skip2]]
  [2]

