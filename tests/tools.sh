export PATH=${TESTDIR%/*}:$PATH

pmp()
{
   python $TESTDIR/cmp.py "$@"
}
