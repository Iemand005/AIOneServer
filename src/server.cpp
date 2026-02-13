#include "crow.h"

int main() {

  crow::SimpleApp app;

  crow::mustache::set_global_base("../src/front");

  CROW_ROUTE(app, "/")([](){
    // return "Heeey" ;
    auto page = crow::mustache::load_text("C:\\Users\\Lasse\\Documents\\Projects\\AIOneWeb\\src\\front\\index.html");
    return page;
  });


  app.port(18080).multithreaded().run();

  return 69;
}