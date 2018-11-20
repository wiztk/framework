/*
 * Copyright 2017 - 2018 The WizTK Authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "private.hpp"

#include <sys/mman.h>
#include <stdlib.h>
#include <malloc.h>
#include <unistd.h>

#define HAVE_POSIX_FALLOCATE
#define HAVE_MKOSTEMP

#ifdef HAVE_POSIX_FALLOCATE
#include <fcntl.h>
#include <new>
#endif

namespace wiztk {
namespace gui {

int SharedMemoryPool::Private::CreateAnonymousFile(off_t size) {
  static const char temp[] = "/wiztk-XXXXXX";
  const char *path;
  char *name;
  int fd;
  int ret;

  path = getenv("XDG_RUNTIME_DIR");
  if (!path) {
    errno = ENOENT;
    return -1;
  }

  name = (char *) malloc(strlen(path) + sizeof(temp));
  if (!name)
    return -1;

  strcpy(name, path);
  strcat(name, temp);

  fd = CreateTempFile(name);

  free(name);

  if (fd < 0)
    return -1;

#ifdef HAVE_POSIX_FALLOCATE
  ret = posix_fallocate(fd, 0, size);
  if (ret != 0) {
    close(fd);
    errno = ret;
    return -1;
  }
#else
  ret = ftruncate(fd, size);
  if (ret < 0) {
    close(fd);
    return -1;
  }
#endif

  return fd;
}

int SharedMemoryPool::Private::CreateTempFile(char *tmpname) {
  int fd;

#ifdef HAVE_MKOSTEMP
  fd = mkostemp(tmpname, O_CLOEXEC);
  if (fd >= 0)
    unlink(tmpname);
#else
  fd = mkstemp(tmpname);
  if (fd >= 0) {
    fd = set_cloexec_or_close(fd);
    unlink(tmpname);
  }
#endif

  return fd;
}

}
}