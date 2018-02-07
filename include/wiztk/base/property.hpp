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

#ifndef WIZTK_BASE_PROPERTY_HPP_
#define WIZTK_BASE_PROPERTY_HPP_

#include "wiztk/base/macros.hpp"

#ifndef __PROPERTY__
#define __PROPERTY__(NAME) p_->NAME
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
 * #include <memory.hpp>
 *
 * struct MyView::Private : public base::Property<MyView> {
 *
 *   // Other property members
 *
 * };
 *
 * MyView::MyView {
 *   p_ = std::make_unique<Private>(this);
 * }
 * @endcode
 *
 * In this way, the public header files can provide clean and understandable
 * code and hide the details.
 *
 * A Property object can know the proprietor it belongs to with the proprietor member
 * variable.
 */
template<typename T>
class Property {

 public:

  /**
   * @brief Declare this class is non-copyable and non-movable.
   */
  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(Property);

  /**
   * @brief Disable the default constructor.
   */
  Property() = delete;

  /**
   * @brief Constructor with the given proprietor.
   * @param owner
   */
  inline explicit Property(T *owner)
      : proprietor_(owner) {}

  /**
   * @brief Default destructor.
   */
  virtual ~Property() = default;

  /**
   * @brief Get the proprietor.
   * @return
   */
  T *proprietor() const { return proprietor_; }

 private:

  T *proprietor_ = nullptr;

};

} // namespace base
} // namespace wiztk

#endif // WIZTK_BASE_PROPERTY_HPP_
