#include <drogon/drogon.h>
#include "controllers/Hydrology_V1_ModelController.h"
#include "controllers/RandomController.h"

int main() {
    //Set HTTP listener address and port
    drogon::app().addListener("127.0.0.1", 8080);
    //Load config file
    //drogon::app().loadConfigFile("../config.json");
    //drogon::app().loadConfigFile("../config.yaml");
    //Run HTTP framework,the method will block in the internal event loop
    drogon::app().setDocumentRoot("../../frontend");
    drogon::app().run();
    return 0;
}
