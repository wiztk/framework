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

#ifndef WIZTK_GUI_ABSTRACT_EGL_BACKEND_HPP_
#define WIZTK_GUI_ABSTRACT_EGL_BACKEND_HPP_

#include "wiztk/gui/abstract-rendering-backend.hpp"

#include <memory>

namespace wiztk {
namespace gui {

/**
 * @ingroup gui
 * @brief Abstract EGL graphic backend.
 */
class WIZTK_EXPORT AbstractEGLBackend : public AbstractRenderingBackend {

 public:

  AbstractEGLBackend();

  ~AbstractEGLBackend() override;

  bool IsValid() const;

  int GetVersionMajor() const;

  int GetVersionMinor() const;

 protected:

  struct Private;

 private:

  std::unique_ptr<Private> p_;

};

} // namespace gui
} // namespace wiztk

#endif // WIZTK_GUI_ABSTRACT_EGL_BACKEND_HPP_
