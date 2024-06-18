#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>

struct ApplicationSettings {
  // We can load this from a config file

  int windowWidth;
  int windowHeight;
  std::string windowTitle;
  bool fullscreen;

  // We can add more application settings here

  ApplicationSettings()
      : windowWidth(1200), windowHeight(800), windowTitle("Window title"),
        fullscreen(false) {}
};

#endif // !SETTINGS_H
