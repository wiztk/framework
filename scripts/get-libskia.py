#!/usr/bin/env python3

import platform
import re
import chardet
import hashlib
import urllib.request

from utils.url import download

PACKAGE = "libskia"  # the package name
VERSION = "m67"
ARCH = platform.processor()
MD5 = "e3ba61f3a227db06444055e3405d3878"


def checksum():
    filename = PACKAGE + "-" + VERSION + "-" + ARCH + ".tar.gz"

    try:
        with open(filename, 'rb') as file_to_check:
            data = file_to_check.read()
            md5_returned = hashlib.md5(data).hexdigest()
            if MD5 == md5_returned:
                return True
    except FileNotFoundError:
        pass

    return False


def main():
    if checksum():
        # file exists
        return

    url_latest = "https://github.com/wiztk/%s/releases/latest" % PACKAGE
    response = urllib.request.urlopen(url_latest)
    html = response.read()
    encode_type = chardet.detect(html)
    html = html.decode(encode_type['encoding'])

    path = "%s/%s-%s-%s.tar.gz" % (VERSION, PACKAGE, VERSION, ARCH)
    href = re.findall("href=\"/wiztk/%s/releases/download/%s\"" % (PACKAGE, path), html)
    if len(href) > 0:
        url_tarball = "https://github.com/wiztk/%s/releases/download/%s" % (PACKAGE, path)
        filename = PACKAGE + "-" + VERSION + "-" + ARCH + ".tar.gz"
        download(url=url_tarball, filename=filename)


if __name__ == '__main__':
    main()
