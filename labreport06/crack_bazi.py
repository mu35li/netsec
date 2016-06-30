#!/usr/bin/env python
def crack(filename, bsize):
    """Decrypt a bazi-encrypted file given the file's
    name and the blocksize (usually 10)
    """

    result = ''
    counter = -1
    with open(filename, "rb") as f:
        f.seek(-bsize, 2) # seek last n bytes
        key = [int(b) for b in f.read(bsize)]
        f.seek(0, 0) # seek beginning of file again
        while True:
            counter += 1
            counter = counter % bsize
            b = f.read(1)
            if b == b'':
                break
            num = int.from_bytes(b, 'little')
            result += chr(num ^ key[counter])
    return result


if __name__ == "__main__":
    import sys
    print(crack(sys.argv[1], 10))
