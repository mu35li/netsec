#!/usr/bin/env python

import sys
import string
import hashlib
from itertools import permutations

target = b'2b2935865b8a6749b0fd31697b467bd7'
salt = b'8kofferradio'

symbols = string.ascii_lowercase + '0123456789'

for i in range(1,7):
    print(i)
    sys.stdout.flush()
    for elem in permutations(symbols, i):
        elem_bytes = b''
        for c in elem:
            elem_bytes += bytes(c, 'utf8')
        m = hashlib.md5()
        m.update(salt + elem_bytes)
        if m.hexdigest() == target.decode():
            print("success; found match!")
            print(elem_bytes)
            sys.stdout.flush()
            sys.exit(0)

print("failed; couldn't find a match!")
sys.exit(1)
