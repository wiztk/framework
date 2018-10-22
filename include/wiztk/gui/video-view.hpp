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

#ifndef WIZTK_VIDEO_VIEW_HPP
#define WIZTK_VIDEO_VIEW_HPP

#include "abstract-view.hpp"

namespace wiztk {
namespace gui {

class Surface;

/**
 * @ingroup gui
 * @brief A view with own surface to display video stream
 *
 * @note This class is currently used for test only
 */
class VideoView final : public AbstractView {

 public:

  VideoView();

  VideoView(int width, int height);

  virtual ~VideoView();

 protected:

  virtual void OnRequestUpdateFrom(AbstractView *view);

  virtual Surface *GetSurface(const AbstractView *view) const;

  virtual void OnSizeChanged(int width, int height);

  virtual void OnMouseEnter(MouseEvent *event);

  virtual void OnMouseLeave(MouseEvent *event);

  virtual void OnMouseMove(MouseEvent *event);

  virtual void OnMouseDown(MouseEvent *event);

  virtual void OnMouseUp(MouseEvent *event) override;

  virtual void OnKeyDown(KeyEvent *event) override;

  virtual void OnKeyUp(KeyEvent *event) override;

  virtual void OnDraw(const Context *context);

 private:

  Surface *surface_;

};

} // namespace gui
} // namespace wiztk

#endif //WIZTK_VIDEO_VIEW_HPP
