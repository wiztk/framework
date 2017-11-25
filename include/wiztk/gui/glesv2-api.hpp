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

#ifndef WIZTK_GUI_GLESV2_API_HPP_
#define WIZTK_GUI_GLESV2_API_HPP_

#include "abstract-rendering-api.hpp"

namespace wiztk {
namespace gui {

/**
 * @ingroup gui
 * @brief OpenGL ES V2
 */
class GLESV2API : public AbstractRenderingAPI {

 public:

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(GLESV2API);

  GLESV2API();

  virtual ~GLESV2API();

  virtual void SetViewportSize(int width, int height) final;

  virtual void MakeCurrent() final;

  virtual void SwapBuffers() final;

 protected:

  virtual void OnSetup(ViewSurface *surface) final;

  virtual void OnRelease(ViewSurface *surface) final;

 private:

  struct Private;

  void Destroy();

  std::unique_ptr<Private> p_;

};

} // namespace gui
} // namespace wiztk

#endif // WIZTK_GUI_GLESV2_API_HPP_
