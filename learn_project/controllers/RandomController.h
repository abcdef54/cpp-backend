#pragma once
#include <random>

#include <drogon/HttpSimpleController.h>


class RandomController : public drogon::HttpSimpleController<RandomController>
{
  public:
    inline void asyncHandleHttpRequest(const drogon::HttpRequestPtr& req, std::function<void (const drogon::HttpResponsePtr &)> &&callback) override {
      std::random_device rd;
      std::mt19937 gen(rd());
      std::uniform_int_distribution<int> distrib(1,100);

      int random_num = distrib(gen);

      Json::Value body;
      body["random_num"] = random_num;
      callback(drogon::HttpResponse::newHttpJsonResponse(body));
    }
    PATH_LIST_BEGIN
    // list path definitions here;
    // PATH_ADD("/path", "filter1", "filter2", HttpMethod1, HttpMethod2...);
    PATH_ADD("/random", drogon::Get);
    PATH_LIST_END
};
