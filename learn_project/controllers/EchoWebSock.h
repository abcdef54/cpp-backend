#pragma once

#include <drogon/WebSocketController.h>
#include <random>
#include "utils/utils.h"


class EchoWebSock : public drogon::WebSocketController<EchoWebSock>
{
  public:
     inline void handleNewMessage(
      const drogon::WebSocketConnectionPtr &wsConnPtr,
      std::string &&message,
      const drogon::WebSocketMessageType &type
    ) override {
      
      if (type != drogon::WebSocketMessageType::Text) {
        return;
      }

      auto json = JsonUtils::toJson(message);

      if (!json || !json->isObject()) {
        wsConnPtr->send(R"({"error":"Invalid JSON"})");
        return;
      }

      if (!(*json)["action"].isString()) {
        wsConnPtr->send(R"({"error":"Action must be a string"})");
        return;
      }

      std::string action = (*json)["action"].asString();

      if (action == "echo") {
        wsConnPtr->send(message);
      }
      else if (action == "generate_random") {

        if (!json->isMember("count") || !(*json)["count"].isInt()) {
          wsConnPtr->send(R"({"error":"Count must be an integer"})");
          return;
        }

        int count = (*json)["count"].asInt();
        
        if (count <= 0 || count > 100) {
          wsConnPtr->send(R"({"error":"Count must be between 1 and 100"})");
          return;
        }

        static thread_local std::mt19937 generator{
          std::random_device{}()
        };
        std::uniform_int_distribution<int> distribution(1, 100);

        for (int i = 0; i < count; i++) {
          wsConnPtr->send(std::to_string(distribution(generator)));
        }
      }
      else {
        wsConnPtr->send(R"({"error":"Unknown action"})");
      }
    }

    inline void handleNewConnection(
      const drogon::HttpRequestPtr &req,
      const drogon::WebSocketConnectionPtr &wsConnPtr
    ) override  {
      LOG_INFO << "New WebSocket connection from " << req->peerAddr().toIp();
    }

    inline void handleConnectionClosed(const drogon::WebSocketConnectionPtr &wsConnPtr) override {
      LOG_INFO << "Websocket connection closed";
    }

    WS_PATH_LIST_BEGIN
    // list path definitions here;
    // WS_PATH_ADD("/path", "filter1", "filter2", ...);
    WS_PATH_ADD("/echo");
    WS_PATH_LIST_END
};
