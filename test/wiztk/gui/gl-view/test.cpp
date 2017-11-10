//
// Created by zhanggyb on 16-9-19.
//

#include "test.hpp"

#include <wiztk/gui/application.hpp>
#include <wiztk/gui/window.hpp>
#include "wiztk/gui/gl-view.hpp"
#include "wiztk/gui/glesv2-api.hpp"

#include <GLES2/gl2.h>
#include <sys/time.h>

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

class Triangle : public GLView {

 public:

  Triangle() {
    SetRenderingAPI(new GLESV2API);
  }

  virtual ~Triangle() {
    glDeleteProgram(program_);
  }

 protected:

  virtual void OnInitialize() override;

  virtual void OnResize(int width, int height) override;

  virtual void OnRender() override;

 private:

  GLuint program_ = 0;

  GLuint rotation_uniform_ = 0;
  GLuint pos_ = 0;
  GLuint col_ = 0;

};

void Triangle::OnInitialize() {
  MakeCurrent();

  GLuint frag, vert;
  GLint status;

  frag = create_shader(frag_shader_text, GL_FRAGMENT_SHADER);
  vert = create_shader(vert_shader_text, GL_VERTEX_SHADER);

  program_ = glCreateProgram();
  glAttachShader(program_, frag);
  glAttachShader(program_, vert);
  glLinkProgram(program_);

  glGetProgramiv(program_, GL_LINK_STATUS, &status);
  if (!status) {
    char log[1000];
    GLsizei len;
    glGetProgramInfoLog(program_, 1000, &len, log);
    fprintf(stderr, "Error: linking:\n%*s\n", len, log);
    exit(1);
  }

  glUseProgram(program_);

  pos_ = 0;
  col_ = 1;

  glBindAttribLocation(program_, pos_, "pos");
  glBindAttribLocation(program_, col_, "color");
  glLinkProgram(program_);

  rotation_uniform_ = (GLuint)
      glGetUniformLocation(program_, "rotation");

  SwapBuffers();
}

void Triangle::OnResize(int width, int height) {

}

void Triangle::OnRender() {
  MakeCurrent();

  static const GLfloat verts[3][2] = {
      {-0.5f, -0.5f},
      {0.5f, -0.5f},
      {0.f, 0.5f}
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

  gettimeofday(&tv, nullptr);
  uint32_t time = (uint32_t) (tv.tv_sec * 1000 + tv.tv_usec / 1000);

  angle = (GLfloat) ((time / speed_div) % 360 * M_PI / 180.0);
  rotation[0][0] = cos(angle);
  rotation[0][2] = sin(angle);
  rotation[2][0] = -sin(angle);
  rotation[2][2] = cos(angle);

  glViewport(0, 0, GetWidth(), GetHeight());

  glUniformMatrix4fv(rotation_uniform_, 1, GL_FALSE,
                     (GLfloat *) rotation);

  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  glVertexAttribPointer(pos_, 2, GL_FLOAT, GL_FALSE, 0, verts);
  glVertexAttribPointer(col_, 3, GL_FLOAT, GL_FALSE, 0, colors);
  glEnableVertexAttribArray(pos_);
  glEnableVertexAttribArray(col_);

  glDrawArrays(GL_TRIANGLES, 0, 3);

  glDisableVertexAttribArray(pos_);
  glDisableVertexAttribArray(col_);

  SwapBuffers();
}

Test::Test()
    : testing::Test() {
}

Test::~Test() {

}

/*
 *
 */
TEST_F(Test, slider_1) {
  int argc = 1;
  char argv1[] = "gui-gl-view";  // to avoid compile warning
  char *argv[] = {argv1};

  Application app(argc, argv);

  auto *win = new Window(480, 360, "Test GLView");
  win->SetAppId("EGLWidget");

  auto *view = new Triangle;
  win->SetContentView(view);

  win->Show();

  int result = app.Run();

  ASSERT_TRUE(result == 0);
}
