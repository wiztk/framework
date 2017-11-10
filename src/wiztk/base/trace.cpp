/*
 * Copyright 2016 Freeman Zhang <zhanggyb@gmail.com>
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

#include <wiztk/base/trace.hpp>

#include <stdarg.h>

namespace wiztk {

std::string Trace::kFileName = "trace.log";

int Trace::kDepth = 0;

// arbitrarily support nesting 34 deep for no particular reason
const char *Trace::kNestString = "| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | ";

time_t Trace::kLastTime = 0;

Trace::Trace(const char *func_name, const char *format, ...) {
  char str[kBufferSize] = {0};
  sprintf(str, "%s: %s", func_name, format);
  va_list args;
  va_start(args, format);
  SaveLine(kDepth, kDepth * 2, str, args);
  va_end(args);
  ++kDepth;
}

void Trace::Log(const char *func_name, const char *format, ...) {
  char str[kBufferSize] = {0};
  sprintf(str, "%s: %s", func_name, format);
  va_list args;
  va_start(args, format);
  SaveLine(kDepth, kDepth * 2, str, args);
  va_end(args);
}

void Trace::SetFileName(const std::string &filename) {
  kFileName = filename;
}

int Trace::SaveLine(int depth, int align, const char *format, va_list args) {
  // TODO: make sure method is thread save

  FILE *fp = fopen(kFileName.c_str(), "a+");
  if (fp == NULL) return 0;

  time_t t;
  time(&t);

  // TODO: use std::locale
  char time_stamp[32] = {0};
  strftime(time_stamp, sizeof(time_stamp),
           "%Y%m%d.%H%M%S", localtime(&t));

  // only log the timestamp when the time changes
  int len = fprintf(fp, "%s %*.*s> (%d) ",
                    (kLastTime != t) ? time_stamp : "               ",
                    align,
                    align,
                    kNestString,
                    depth);
  kLastTime = t;
  len += vfprintf(fp, format, args);
  len += fwrite("\n", 1, 1, fp);
  fflush(fp);
  fclose(fp);

  return len;
}

}