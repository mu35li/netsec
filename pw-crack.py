#!/usr/bin/env python

import sys
import string
import hashlib
from itertools import permutations

target = b'2b2935865b8a6749b0fd31697b467bd7'
#target = b'1c2905a199b3f9e2bf40cea968a061a2'
salt = b'8kofferradio'

symbols = string.ascii_lowercase + '0123456789'

for elem in permutations(symbols, 5):
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
