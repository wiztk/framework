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

#ifndef WIZTK_CORE_PROPERTY_HPP_
#define WIZTK_CORE_PROPERTY_HPP_

#ifndef __PROPERTY__
#define __PROPERTY__(NAME) (p_->NAME)
#endif

namespace wiztk {
namespace base {

/**
 * @ingroup base
 * @brief A template class to be used as private property
 * @tparam T The owner class
 *
 * This template class is usually used as the base class for private structure
 * and hence builds the private property in a object with the help of
 * std::unique_ptr.
 *
 * For example:
 *
 * In a header file:
 *
 * @code
 * class MyView {
 *
 *   public:
 *
 *     MyView();
 *
 *     // ...
 *
 *   private:
 *
 *     struct Private;
 *
 *     std::unique_ptr<Private> p_;
 *
 * };
 * @endcode
 *
 * And in the source file:
 *
 * @code
 * #include "wiztk/base/property.hpp"
 * #include "wiztk/base/memory.hpp"
 *
 * struct MyView::Private : public base::Property<MyView> {
 *
 *   // Other property members
 *
 * };
 *
 * MyView::MyView {
 *   p_ = base::make_unique<Private>(this);
 * }
 * @endcode
 *
 * In this way, the public header files can provide clean and understandable
 * code and hide the details.
 *
 * A Property object can know the owner it belongs to with the owner member
 * variable.
 */
template<typename T>
class Property {

 public:

  Property() = delete;
  Property(const Property &) = delete;
  Property &operator=(const Property &) = delete;
  Property(Property &&) = delete;
  Property &operator=(Property &&) = delete;

  inline explicit Property(T *owner)
      : owner_(owner) {}

  virtual ~Property() = default;

  T *owner() const { return owner_; }

 private:

  T *owner_ = nullptr;

};

} // namespace base
} // namespace wiztk

#endif // WIZTK_CORE_PROPERTY_HPP_
