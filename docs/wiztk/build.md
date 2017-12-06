Build the source code
=====================

## Checkout source code

```shell
$ git clone --recursive https://github.com/wiztk/framework.git
```

## Install dependencies

Use the shell script to install prerequisites if you're using Ubuntu or Fedora:

```shell
$ cd <path to skland>
$ ./scripts/install-dependencies.sh
```

## Build the source code

Create a temporary directory and generate Makefiles:

```shell
$ cd <path to skland>
$ mkdir build
$ cd build
$ cmake ..
```

Follow the instruction to checkout or download prebuilts:

```shell
$ git clone https://github.com/wiztk/prebuilts.git --depth 1
```

Run `make` to build the project.

```shel
$ make
```

This will build and generate examples in `bin/` and library in `lib/`.

Available Cmake options:

- `-DCMAKE_BUILD_TYPE=<value>`: value = 'Release' or 'Debug'
- `-DBUILD_UNIT_TEST=<value>`: value = 'On', 'True', 'Off' or 'False'

## Build the document

```shell
$ make doc
```

This will generate html document through doxygen in `html/`, open the
 `html/index.html` with your favorite web browser.
