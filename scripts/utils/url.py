#!/usr/bin/env python3

"""A simple script to process URLs.
"""

import requests
import sys


def download(url, filename):
    with open(filename, "wb") as f:
        print("Downloading %s" % filename)
        response = requests.get(url, stream=True)
        total_length = response.headers.get('content-length')

        if total_length is None:
            f.write(response.content)
        else:
            dl = 0
            total_length = int(total_length)
            for chunk in response.iter_content(chunk_size=12800):
                dl += len(chunk)
                f.write(chunk)
                done = int(50 * dl / total_length)
                sys.stdout.write("\r[%s%s]" % ('=' * done, ' ' * (50 - done)))
                sys.stdout.flush()

        sys.stdout.write("\n")
        sys.stdout.flush()
