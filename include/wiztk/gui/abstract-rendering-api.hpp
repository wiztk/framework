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

#ifndef WIZTK_GUI_ABSTRACT_RENDERING_API_HPP_
#define WIZTK_GUI_ABSTRACT_RENDERING_API_HPP_

#include "wiztk/base/defines.hpp"
#include "wiztk/base/sigcxx.hpp"

#include <memory>

namespace wiztk {
namespace gui {

class Surface;

/**
 * @ingroup gui
 * @brief The base class for graphics rendering API to render on a 3D surface
 */
class AbstractRenderingAPI {

  friend class Surface;

 public:

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(AbstractRenderingAPI);

  AbstractRenderingAPI();

  virtual ~AbstractRenderingAPI();

  void Setup(Surface *surface);

  void Release(Surface *surface);

  virtual void SetViewportSize(int width, int height) = 0;

  virtual void MakeCurrent() = 0;

  virtual void SwapBuffers() = 0;

  base::SignalRef<> destroyed() { return destroyed_; }

 protected:

  struct Proxy;

  virtual void OnSetup(Surface *surface) = 0;

  virtual void OnRelease(Surface *surface) = 0;

 private:

  struct Private;

  base::Signal<> destroyed_;

};

} // namespace gui
} // namespace wiztk

#endif // WIZTK_GUI_ABSTRACT_RENDERING_API_HPP_
