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

#include "display_private.hpp"

#include "wiztk/base/property.hpp"

#include "wiztk/gui/application.hpp"
#include "wiztk/gui/main-loop.hpp"
#include "wiztk/gui/theme.hpp"

#include <getopt.h>

#include <csignal>
#include <iostream>

using std::cerr;
using std::endl;

namespace wiztk {
namespace gui {

static inline const char *get_basename(const char *filepath) {
  return strrchr(filepath, '/') ? strrchr(filepath, '/') + 1 : filepath;
}

/**
 * @brief The private structure used in Application
 */
struct Application::Private {

  WIZTK_DECLARE_NONCOPYABLE_AND_NONMOVALE(Private);

  Private() = default;

  ~Private() = default;

  Display *display = nullptr;

  int argc = 0;

  char **argv = nullptr;

  MainLoop *main_loop = nullptr;

  void ParseArguments();

  void PrintHelp();

  static void HandleSignalInt(int);

};

void Application::Private::ParseArguments() {
  static struct option long_options[] = {
      {"help", no_argument, nullptr, 0},      // Show help information.
      {"listen", required_argument, nullptr, 0},      // Followed with number of TCP/IP port to listen.
      {"verbose", no_argument, nullptr, 0},      //Enable verbose mode.
      {nullptr, 0, nullptr, 0}
  };
  static const char *short_options = "hl:v012";

  int c;
  int digit_optind = 0;

  while (true) {
    int this_option_optind = optind ? optind : 1;
    int option_index = 0;

    c = getopt_long(argc, argv, short_options, long_options, &option_index);
    if (c == -1)
      break;

    switch (c) {
      case 0: {
        const char *name = long_options[option_index].name;
        if (strcmp(name, "help") == 0) {
          PrintHelp();
        } else if (strcmp(name, "listen") == 0) {
          if (optarg) {
            printf("listen with port num: %s\n", optarg);
          }
        } else if (strcmp(name, "verbose") == 0) {
          printf("verbose mode\n");
        }

        break;
      }

      case '0':
      case '1':
      case '2': {
        if (digit_optind != 0 && digit_optind != this_option_optind)
          printf("digits occur in two different argv elements.\n");
        digit_optind = this_option_optind;
        printf("option %c\n", c);
        break;
      }

      case 'h': {
        PrintHelp();
        break;
      }

      case 'l': {
        printf("listen with port num: %s\n", optarg);
        break;
      }

      case 'v': {
        printf("verbose mode\n");
        break;
      }

      case '?':break;

      default: {
        printf("?? getopt returned character code 0%o ??\n", c);
        break;
      }
    }
  }

  if (optind < argc) {
    printf("non-option ARGV-elements: ");
    while (optind < argc) {
      printf("%s ", argv[optind++]);
    }
    printf("\n");
  }
}

void Application::Private::PrintHelp() {
  fprintf(stdout, "Usage: %s [options] [args...]\n"
      "\n"
      "Options:\n"
      "\t-h,--help\n\t\tShow this message.\n"
      "\t-l,--listen: <num>\n\t\tListen to the given port.\n"
      "\t-v,--verbose\n\t\tVerbose mode."
      "\n\n", get_basename(argv[0]));
}

void Application::Private::HandleSignalInt(int) {
  _DEBUG("%s\n", "Get SIGINT");
  kInstance->Exit();
}

// ----------

Application *Application::GetInstance() {
  return kInstance;
}

Application *Application::kInstance = nullptr;

Application::Application(int argc, char *argv[]) {
  if (kInstance != nullptr)
    throw std::runtime_error("Error! There should be only one application instance!");

  kInstance = this;

  p_ = std::make_unique<Private>();
  p_->argc = argc;
  p_->argv = argv;

  p_->ParseArguments();

  // Set log handler to a lambda function
  wl_log_set_handler_client([](const char *format, va_list args) {
    vfprintf(stderr, format, args);
  });

  __PROPERTY__(display) = new Display;

  try {
    __PROPERTY__(display)->Connect(nullptr);
  } catch (const std::runtime_error &e) {
    cerr << e.what() << endl;
    exit(EXIT_FAILURE);
  }

  // Load theme
  Theme::Initialize();

  __PROPERTY__(main_loop) = MainLoop::Initialize(__PROPERTY__(display));
}

Application::~Application() {
  __PROPERTY__(display)->Disconnect();
  delete __PROPERTY__(display);
  __PROPERTY__(display) = nullptr;

  Theme::Release();

  kInstance = nullptr;
}

int Application::Run() {
  struct sigaction sigint;
  sigint.sa_handler = Private::HandleSignalInt;
  sigemptyset(&sigint.sa_mask);
  sigint.sa_flags = SA_RESETHAND;
  sigaction(SIGINT, &sigint, NULL);

  __PROPERTY__(main_loop)->Run();

  return 0;
}

void Application::Exit() {
  __PROPERTY__(main_loop)->Quit();
  // TODO: check if need to clean other resources
}

int Application::GetArgc() const {
  return p_->argc;
}

char **Application::GetArgv() const {
  return p_->argv;
}

Display *Application::GetDisplay() const {
  return __PROPERTY__(display);
}

} // namespace gui
} // namespace wiztk
