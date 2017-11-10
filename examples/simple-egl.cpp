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


#include <wiztk/gui/application.hpp>
#include <wiztk/gui/gl-window.hpp>

#include <GLES2/gl2.h>
#include <sys/time.h>
#include <assert.h>

using namespace wiztk;
using namespace wiztk::gui;

static const char *vert_shader_text =
    "uniform mat4 rotation;\n"
        "attribute vec4 pos;\n"
        "attribute vec4 color;\n"
        "varying vec4 v_color;\n"
        "void main() {\n"
        "  gl_Position = rotation * pos;\n"
        "  v_color = color;\n"
        "}\n";

static const char *frag_shader_text =
    "precision mediump float;\n"
        "varying vec4 v_color;\n"
        "void main() {\n"
        "  gl_FragColor = v_color;\n"
        "}\n";

static GLuint
create_shader(const char *source, GLenum shader_type) {
  GLuint shader;
  GLint status;

  shader = glCreateShader(shader_type);
  assert(shader != 0);

  glShaderSource(shader, 1, (const char **) &source, NULL);
  glCompileShader(shader);

  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (!status) {
    char log[1000];
    GLsizei len;
    glGetShaderInfoLog(shader, 1000, &len, log);
    fprintf(stderr, "Error: compiling %s: %*s\n",
            shader_type == GL_VERTEX_SHADER ? "vertex" : "fragment",
            len, log);
    exit(1);
  }

  return shader;
}

class SimpleEGLWindow : public GLWindow {
 public:

  SimpleEGLWindow()
      : GLWindow(250, 250, "Simple EGL") {}

  virtual  ~SimpleEGLWindow() {
    glDeleteProgram(program);
  }

 protected:

  virtual void OnInitialize() override;

  virtual void OnResize(int width, int height) override;

  virtual void OnRender() override;

 private:
  GLuint program;

  GLuint rotation_uniform;
  GLuint pos;
  GLuint col;

};

void SimpleEGLWindow::OnInitialize() {
  MakeCurrent();

  GLuint frag, vert;
  GLint status;

  frag = create_shader(frag_shader_text, GL_FRAGMENT_SHADER);
  vert = create_shader(vert_shader_text, GL_VERTEX_SHADER);

  program = glCreateProgram();
  glAttachShader(program, frag);
  glAttachShader(program, vert);
  glLinkProgram(program);

  glGetProgramiv(program, GL_LINK_STATUS, &status);
  if (!status) {
    char log[1000];
    GLsizei len;
    glGetProgramInfoLog(program, 1000, &len, log);
    fprintf(stderr, "Error: linking:\n%*s\n", len, log);
    exit(1);
  }

  glUseProgram(program);

  pos = 0;
  col = 1;

  glBindAttribLocation(program, pos, "pos");
  glBindAttribLocation(program, col, "color");
  glLinkProgram(program);

  rotation_uniform =
      glGetUniformLocation(program, "rotation");

  SwapBuffers();
}

void SimpleEGLWindow::OnResize(int width, int height) {

}

void SimpleEGLWindow::OnRender() {
  MakeCurrent();

  static const GLfloat verts[3][2] = {
      {-0.5, -0.5},
      {0.5, -0.5},
      {0, 0.5}
  };
  static const GLfloat colors[3][3] = {
      {1, 0, 0},
      {0, 1, 0},
      {0, 0, 1}
  };
  GLfloat angle;
  GLfloat rotation[4][4] = {
      {1, 0, 0, 0},
      {0, 1, 0, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 1}
  };
  static const uint32_t speed_div = 5;
  struct timeval tv;

  gettimeofday(&tv, NULL);
  uint32_t time = tv.tv_sec * 1000 + tv.tv_usec / 1000;

  angle = (time / speed_div) % 360 * M_PI / 180.0;
  rotation[0][0] = cos(angle);
  rotation[0][2] = sin(angle);
  rotation[2][0] = -sin(angle);
  rotation[2][2] = cos(angle);

  glViewport(0, 0, GetWidth(), GetHeight());

  glUniformMatrix4fv(rotation_uniform, 1, GL_FALSE,
                     (GLfloat *) rotation);

  glClearColor(0.0, 0.0, 0.0, .75);
  glClear(GL_COLOR_BUFFER_BIT);

  glVertexAttribPointer(pos, 2, GL_FLOAT, GL_FALSE, 0, verts);
  glVertexAttribPointer(col, 3, GL_FLOAT, GL_FALSE, 0, colors);
  glEnableVertexAttribArray(pos);
  glEnableVertexAttribArray(col);

  glDrawArrays(GL_TRIANGLES, 0, 3);

  glDisableVertexAttribArray(pos);
  glDisableVertexAttribArray(col);

  SwapBuffers();
}

int main(int argc, char *argv[]) {
  Application app(argc, argv);

  auto *win = new SimpleEGLWindow();
  win->SetAppId("Simple-EGL");
  win->Show();
  // All window objects will be deleted when application exits

  return app.Run();
}
