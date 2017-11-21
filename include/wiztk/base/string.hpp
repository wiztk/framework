/*
 * Copyright 2017 The WizTK Authors.
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

#ifndef WIZTK_BASE_STRING_HPP_
#define WIZTK_BASE_STRING_HPP_

#include "wiztk/base/macros.hpp"

#include <cstring>
#include <string>
#include <algorithm>
#include <memory>

namespace wiztk {
namespace base {

// Forward declaration
template<typename STRING_TYPE>
class StringPieceT;

typedef std::string String;
typedef std::u16string String16;
typedef std::u32string String32;

typedef StringPieceT<std::string> StringPiece;
typedef StringPieceT<std::u16string> StringPiece16;
typedef StringPieceT<std::u32string> StringPiece32;

/**
 * @ingroup base
 * @brief A string-like object that points to a sized piece of memory.
 * @tparam STRING_TYPE Must be a std::basic_string type
 *
 * @ref stringpiece.h in ICU or string_piece.h in Chromium
 */
template<typename STRING_TYPE>
class StringPieceT {

 public:

  typedef size_t size_type;
  typedef typename STRING_TYPE::value_type value_type;
  typedef const value_type *pointer;
  typedef const value_type &reference;
  typedef const value_type &const_reference;
  typedef ptrdiff_t difference_type;
  typedef const value_type *const_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

  static const size_type npos;

  /**
   * @brief Default constructor, creates an empty StringPieceT
   */
  StringPieceT() : ptr_(nullptr), length_(0) {}

  /**
   * @brief Constructs from a NUL-terminated string type pointer.
   * @param str a NUL-terminated string type pointer
   */
  StringPieceT(const value_type *str)
      : ptr_(str),
        length_((nullptr == str) ?
                0 : static_cast<size_type >(STRING_TYPE::traits_type::length(str))) {}

  /**
   * @brief Constructs from a STRING_TYPE
   * @param str a STRING_TYPE
   */
  StringPieceT(const STRING_TYPE &str)
      : ptr_(str.data()), length_(str.size()) {}

  /**
   * @brief Constructs from a string type pointer and a specified length.
   * @param offset
   * @param len
   */
  StringPieceT(const value_type *offset, size_type len)
      : ptr_(offset), length_(len) {}

  /**
   * @brief Substring of another StringPieceT.
   * @param other
   * @param pos
   */
  StringPieceT(const StringPieceT &other, size_type pos) {
    if (pos < 0) pos = 0;
    else if (pos > other.length_) pos = other.length_;

    ptr_ = other.ptr_ + pos;
    length_ = other.length_ - pos;
  }

  /**
   * @brief Substring of another StringPieceT.
   * @param other
   * @param pos
   * @param len
   */
  StringPieceT(const StringPieceT &other, size_type pos, size_type len) {
    if (pos < 0) pos = 0;
    else if (pos > other.length_) pos = other.length_;

    if (len < 0) len = 0;
    else if (len > other.length_ - pos) len = other.length_ - pos;

    ptr_ = other.ptr_ + pos;
    length_ = len;
  }

  /**
   * @brief Substring of another StringPieceT.
   * @param begin
   * @param end
   */
  StringPieceT(const typename STRING_TYPE::const_iterator &begin,
               const typename STRING_TYPE::const_iterator &end) {
    length_ = static_cast<size_type >(std::distance(begin, end));
    ptr_ = length_ > 0 ? &*begin : nullptr;
  }

  /**
   * @brief Default destructor
   */
  ~StringPieceT() = default;

  /**
   * @brief Returns the string pointer.
   * @return The string pointer or nullptr if empty
   */
  const value_type *data() const { return ptr_; }

  /**
   * @brief Returns the string length.
   * @return the string length
   *
   * @note Same as length().
   */
  size_type size() const { return length_; }

  /**
   * @brief Returns the string length.
   * @return the string length
   *
   * @note Same as size().
   */
  size_type length() const { return length_; }

  /**
   * @brief Returns whether the string is empty.
   * @return true if string is empty
   */
  bool empty() const { return 0 == length_; }

  /**
   * @brief Sets to an empty string.
   */
  void clear() {
    ptr_ = nullptr;
    length_ = 0;
  }

  /**
   * @brief Reset the string piece to refer to new data.
   * @param data
   * @param len
   */
  void set(const value_type *data, size_type len) {
    ptr_ = data;
    length_ = len;
  }

  /**
   * @brief Reset the string piece to refer to new data.
   * @param str
   */
  void set(const value_type *str) {
    ptr_ = str;
    length_ = (nullptr != str) ? STRING_TYPE::traits_type::length(str) : 0;
  }

  value_type operator[](size_type i) const { return ptr_[i]; }

  value_type front() const { return ptr_[0]; }

  value_type back() const { return ptr_[length_ - 1]; }

  /**
   * @brief Remove the first n string units.
   * @param n
   */
  void remove_prefix(size_type n) {
    ptr_ += n;
    length_ -= n;
  }

  void remove_suffix(size_type n) {
    length_ -= n;
  }

  int compare(const StringPieceT &other) const {
    int r = word_memcmp(ptr_, other.ptr_, (length_ < other.length_ ? length_ : other.length_));
    if (r == 0) {
      if (length_ < other.length_) r = -1;
      else if (length_ > other.length_) r = +1;
    }
    return r;
  }

  // This is the style of conversion preferred by std::string_view in C++17.
  explicit operator STRING_TYPE() const { return as_string(); }

  STRING_TYPE as_string() const {
    // std::string doesn't like to take a NULL pointer even with a 0 size.
    return empty() ? STRING_TYPE() : STRING_TYPE(data(), size());
  }

  const_iterator begin() const { return ptr_; }

  const_iterator end() const { return ptr_ + length_; }

  const_reverse_iterator rbegin() const {
    return const_reverse_iterator(ptr_ + length_);
  }

  const_reverse_iterator rend() const {
    return const_reverse_iterator(ptr_);
  }

  size_type max_size() const { return length_; }

  size_type capacity() const { return length_; }

  void copy_to(STRING_TYPE *dst) const {
    copy_to(*this, dst);
  }

  /**
   * @brief Copy this string to given string buffer.
   * @param buf
   * @param n
   * @param pos
   * @return
   */
  size_type copy_to(value_type *buf, size_type n, size_type pos = 0) const {
    return copy_to(*this, buf, n, pos);
  }

  /**
   * @brief Append this string to another one.
   * @param dst
   */
  void append_to(STRING_TYPE *dst) const {
    append_to(*this, dst);
  }

  /**
   * @brief Returns whether this string starts with "x"
   * @param x
   * @return
   */
  bool starts_with(const StringPieceT &x) const {
    return ((this->length_ >= x.length_) &&
        (word_memcmp(this->ptr_, x.ptr_, x.length_) == 0));
  }

  /**
   * @brief Returns whether this string ends with "x"
   * @param x
   * @return
   */
  bool ends_with(const StringPieceT &x) const {
    return ((this->length_ >= x.length_) &&
        (word_memcmp(this->ptr_ + (this->length_ - x.length_),
                     x.ptr_, x.length_) == 0));
  }

  /**
   * @brief Search for a character or substring at a given offset.
   */
  size_type find(const StringPieceT &s,
                 size_type pos = 0) const {
    return find(*this, s, pos);
  }

  /**
   * @brief Search for a character or substring at a given offset.
   * @param c
   * @param pos
   * @return
   */
  size_type find(value_type c, size_type pos = 0) const {
    return find(*this, c, pos);
  }

  /**
   * @brief Reverse find
   * @param s
   * @param pos
   * @return
   */
  size_type rfind(const StringPieceT &s,
                  size_type pos = StringPieceT::npos) const {
    return reverse_find(*this, s, pos);
  }

  /**
   * @brief Reverse find
   * @param c
   * @param pos
   * @return
   */
  size_type rfind(value_type c, size_type pos = StringPieceT::npos) const {
    return reverse_find(*this, c, pos);
  }

  /**
   * @brief Find the first occurence of one of a set of characters.
   * @param s
   * @param pos
   * @return
   */
  size_type find_first_of(const StringPieceT &s,
                          size_type pos = 0) const {
    return find_first_of(*this, s, pos);
  }

  size_type find_first_of(value_type c, size_type pos = 0) const {
    return find_first_of(*this, c, pos);
  }

  /**
   * @brief Find the first occurence not of a set of characters.
   * @param s
   * @param pos
   * @return
   */
  size_type find_first_not_of(const StringPieceT &s,
                              size_type pos = 0) const {
    return find_first_not_of(*this, s, pos);
  }

  /**
   * @brief Find the first occurence not of a set of characters.
   * @param c
   * @param pos
   * @return
   */
  size_type find_first_not_of(value_type c, size_type pos = 0) const {
    return find_first_not_of(*this, c, pos);
  }

  /**
   * @brief Find the last occurence of one of a set of characters.
   * @param s
   * @param pos
   * @return
   */
  size_type find_last_of(const StringPieceT &s,
                         size_type pos = StringPieceT::npos) const {
    return find_last_of(*this, s, pos);
  }

  /**
   * @brief Find the last occurence of one of a set of characters.
   * @param c
   * @param pos
   * @return
   */
  size_type find_last_of(value_type c,
                         size_type pos = StringPieceT::npos) const {
    return find_last_of(*this, c, pos);
  }

  /**
   * @brief Find the last occurence not of a set of characters.
   * @param s
   * @param pos
   * @return
   */
  size_type find_last_not_of(const StringPieceT &s,
                             size_type pos = StringPieceT::npos) const {
    return find_last_not_of(*this, s, pos);
  }

  /**
   * @brief Find the last occurence not of a set of characters.
   * @param c
   * @param pos
   * @return
   */
  size_type find_last_not_of(value_type c,
                             size_type pos = StringPieceT::npos) const {
    return find_last_not_of(*this, c, pos);
  }

  /**
   * @brief Returns a substring of this StringPieceT.
   * @param pos
   * @param n
   * @return
   */
  StringPieceT substr(size_type pos,
                      size_type n = StringPieceT::npos) const {
    return substr(*this, pos, n);
  }

 private:

  static void copy_to(const StringPieceT &str, STRING_TYPE *dst) {
    if (str.empty()) dst->clear();
    else dst->assign(str.data(), str.size());
  }

  static size_type copy_to(const StringPieceT &str,
                           value_type *buf,
                           size_type n, size_type pos) {
    size_type ret = std::min(str.size() - pos, n);
    memcpy(buf, str.data() + pos, ret * sizeof(value_type));
    return ret;
  }

  static void append_to(const StringPieceT &str, STRING_TYPE *dst) {
    if (!str.empty())
      dst->append(str.data(), str.size());
  }

  static size_type find(const StringPieceT &str, const StringPieceT &s, size_type pos) {
    if (pos > str.size()) return StringPieceT::npos;

    const_iterator result = std::search(str.begin() + pos, str.end(), s.begin(), s.end());
    const auto xpos = static_cast<size_type>(result - str.begin());
    return xpos + s.size() <= str.size() ? xpos : npos;
  }

  static size_type reverse_find(const StringPieceT &str, const StringPieceT &s, size_t pos) {
    if (str.size() < s.size())
      return StringPieceT::npos;

    if (s.empty())
      return std::min(str.size(), pos);

    const_iterator last = str.begin() + std::min(str.size() - s.size(), pos) + s.size();
    const_iterator result = std::find_end(str.begin(), last, s.begin(), s.end());
    return result != last ?
           static_cast<size_type>(result - str.begin()) : npos;
  }

  static size_type find_first_of(const StringPieceT &str,
                                 const StringPieceT &s,
                                 size_type pos) {
    const_iterator found = std::find_first_of(str.begin() + pos, str.end(), s.begin(), s.end());

    if (found == str.end())
      return StringPieceT::npos;

    return found - str.begin();
  }

  static size_type find_first_not_of(const StringPieceT &str,
                                     const StringPieceT &s,
                                     size_type pos) {
    if (0 == str.size()) return StringPieceT::npos;

    bool found = false;
    for (size_type i = pos; i < str.size(); ++i) {
      found = false;
      for (size_type j = 0; j < s.size(); ++j) {
        if (str[i] == s[j]) {
          found = true;
          break;
        }
      }
      if (!found) return i;
    }

    return StringPieceT::npos;
  }

  static size_type find_last_of(const StringPieceT &str, const StringPieceT &s, size_type pos) {
    if (0 == str.size()) return StringPieceT::npos;

    for (size_type i = std::min(pos, str.size() - 1);; --i) {
      for (size_type j = 0; j < s.size(); ++j) {
        if (str.data()[i] == s[j])
          return i;
      }
      if (0 == i) break;
    }
    return StringPieceT::npos;
  }

  static size_type find_last_not_of(const StringPieceT &str, const StringPieceT &s, size_type pos) {
    if (0 == str.size()) return StringPieceT::npos;

    bool found = false;
    for (size_type i = std::min(pos, str.size() - 1);; --i) {
      found = false;
      for (size_type j = 0; j < s.size(); j++) {
        if (str.data()[i] == s[j]) {
          found = true;
          break;
        }
      }
      if (!found) return i;
      if (0 == i) break;
    }

    return StringPieceT::npos;
  }

  static StringPieceT substr(const StringPieceT &str,
                             size_type pos,
                             size_type n) {
    if (pos > str.size()) pos = str.size();
    if (n > str.size() - pos) n = str.size() - pos;
    return StringPieceT(str.data() + pos, n);
  }

  static int word_memcmp(const value_type *p,
                         const value_type *p2,
                         size_type N) {
    return STRING_TYPE::traits_type::compare(p, p2, N);
  }

  const value_type *ptr_;

  size_type length_;

};

template<typename STRING_TYPE>
const typename StringPieceT<STRING_TYPE>::size_type
    StringPieceT<STRING_TYPE>::npos = typename StringPieceT<STRING_TYPE>::size_type(-1);

template<typename STRING_TYPE>
WIZTK_EXPORT bool operator==(const StringPieceT<STRING_TYPE> &x, const StringPieceT<STRING_TYPE> &y) {
  return 0 == x.compare(y);
}

template<typename STRING_TYPE>
WIZTK_EXPORT bool operator!=(const StringPieceT<STRING_TYPE> &x, const StringPieceT<STRING_TYPE> &y) {
  return !(x == y);
}

WIZTK_EXPORT std::ostream &operator<<(std::ostream &out,
                                      const String16 &utf16);

WIZTK_EXPORT std::ostream &operator<<(std::ostream &out,
                                      const String32 &utf32);

} // namespace base
} // namespace wiztk

#endif // WIZTK_BASE_STRING_HPP_
