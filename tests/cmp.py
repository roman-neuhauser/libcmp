import os, sys

args = sys.argv[1:]
if len(args) == 5:
  opt, fn1, fn2, s1, s2 = args
elif len(args) == 3:
  opt, fn1, fn2, s1, s2 = args + [0, 0]

base, fmt = 1, "{:>6} {:3o} {:3o}"
if 'x' in opt: base, fmt = 0, "{:08} {:02x} {:02x}"

with open(fn1, 'rb') as f1, open(fn2, 'rb') as f2:
  fs = ((f1, int(s1)), (f2, int(s2)))
  sz = min((os.fstat(f.fileno()).st_size - s for f, s in fs))
  for f, s in fs: f.seek(s)
  bs = zip(range(base+s, base+s+sz), *(f.read(sz) for f, s in fs))
  for i, c, d in bs:
    if c == d: continue
    print(fmt.format(i, c, d))

