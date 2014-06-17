export PATH=${TESTDIR%/*}:$PATH

pmp()
{
   python3 $TESTDIR/cmp.py "$@"
}
