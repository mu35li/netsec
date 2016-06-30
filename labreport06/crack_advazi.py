#!/usr/bin/env python
def crack(filename, bsize):
    """Decrypt an advazi-encrypted file given the file's
    name and the blocksize (usually 10)
    """

    result = ''
    with open(filename, "rb") as f:
        f.seek(-bsize, 2) # seek last n bytes

        # initialize key as last n bytes
        key = [int(b) for b in f.read(bsize)]

        f.seek(0, 0) # seek beginning of file again
        whole_file = f.read() # read in the entire file

        # compute padding
        padding = 0
        counter = bsize - 1
        for i in range(len(whole_file) - 1, 0, -1):
            if whole_file[i] != key[counter]:
                break
            else:
                padding += 1
            counter -= 1
            counter %= 10

        # decrypt key with padding value
        for i in range(len(key)):
            key[i] ^= padding

        counter = -1
        f.seek(0, 0) # seek beginning of file again

        # decrypt plaintext
        while True:
            counter += 1
            counter %= 10
            b = f.read(1)
            if b == b'':
                break
            num = int.from_bytes(b, 'little')
            result += chr(num ^ key[counter])
    return result[:len(result) - padding]

if __name__ == "__main__":
    import sys
    print(crack(sys.argv[1], 10))
