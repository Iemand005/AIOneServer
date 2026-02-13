#include "crow.h"

int main() {

  crow::SimpleApp app;

  CROW_ROUTE(app, "/")([](){
    // return "Heeey" ;
    auto page = crow::mustache::load_text("./front/index.html");
    return page;
  });


  app.port(18080).multithreaded().run();

  return 69;
}