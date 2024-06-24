#include "../../lib_application/include/application.h"

int main() {
  Application app;

  if (app.Initialize()) {
    app.Run();
  }

  app.Shutdown();

  return 0;
}
