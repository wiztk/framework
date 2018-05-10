#!/usr/bin/env python3

import sys
import urllib.request
import hashlib


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


def show_progress(current, total):
    done = int(50 * current / total)
    sys.stdout.write("\r-- [%s%s]" % ('=' * done, ' ' * (50 - done)))
    sys.stdout.flush()


def download(url, filename):
    response = urllib.request.urlopen(url)
    length = response.getheader('content-length')
    if length:
        length = int(length)
        block_size = max(12800, length // 100)
    else:
        block_size = 1000000  # default size

    size = 0
    with open(filename, 'wb') as f:
        data = response.read(block_size)
        if not data:
            return

        f.write(data)
        size += len(data)
        if length:
            show_progress(size, length)

        while True:
            data = response.read(block_size)
            if not data:
                break
            f.write(data)
            size += len(data)
            if length:
                show_progress(size, length)

        sys.stdout.write("\n")
        sys.stdout.flush()


def main():
    # TODO: use argparse module
    url = sys.argv[1]
    md5 = sys.argv[2]
    filename = url.split('/')[-1]

    if checksum(filename=filename, hashcode=md5):
        # file exists
        return

    download(url=url, filename=filename)


if __name__ == '__main__':
    main()
