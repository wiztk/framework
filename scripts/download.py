#!/usr/bin/env python3

import sys
import hashlib

from utils.url import download

# TODO: use argparse module
URL = sys.argv[1]
MD5 = sys.argv[2]


def checksum(filename, hashcode):
    try:
        with open(filename, 'rb') as file_to_check:
            data = file_to_check.read()
            md5_returned = hashlib.md5(data).hexdigest()
            if hashcode == md5_returned:
                return True
    except FileNotFoundError:
        pass

    return False


def main():
    filename = URL.split('/')[-1]

    if checksum(filename=filename, hashcode=MD5):
        # file exists
        return

    download(url=URL, filename=filename)


if __name__ == '__main__':
    main()
