#!/usr/bin/env python3

"""
A simple script to download file
"""

import urllib.request


def download(url, filename):
    with urllib.request.urlopen(url) as res, open(filename, 'wb') as out_file:
        data = res.read()
        out_file.write(data)
