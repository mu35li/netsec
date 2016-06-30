#!/usr/bin/env python
import sys

if __name__ == "__main__":
    filename = sys.argv[1]
    result = ''
    counter = -1
    with open(filename, "rb") as f:
        f.seek(-10, 2) # seek last 10 bytes
        key = [int(b) for b in f.read(10)]
        f.seek(0, 0) # seek beginning of file again
        while True:
            counter += 1
            counter = counter % 10
            b = f.read(1)
            if b == b'':
                break
            num = int.from_bytes(b, 'little')
            result += chr(num ^ key[counter])
    print(result)
