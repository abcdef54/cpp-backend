#pragma once

#include <drogon/HttpController.h>
#include "converters/PredictionConverter.h"
#include "middlewares/RequestLoggerMiddleware.h"


namespace Hydrology
{
namespace V1
{
class ModelController : public drogon::HttpController<ModelController>
{
  public:
    METHOD_LIST_BEGIN
    // use METHOD_ADD to add your custom processing function here;
    // METHOD_ADD(ModelController::get, "/{2}/{1}", Get); // path is /Hydrology/V1/ModelController/{arg2}/{arg1}
    // METHOD_ADD(ModelController::your_method_name, "/{1}/{2}/list", Get); // path is /Hydrology/V1/ModelController/{arg1}/{arg2}/list
    // ADD_METHOD_TO(ModelController::your_method_name, "/absolute/path/{1}/{2}/list", Get); // path is /absolute/path/{arg1}/{arg2}/list

    METHOD_ADD(ModelController::predict, "/predict", drogon::Post, "RequestLoggerMiddleware");
    METHOD_ADD(ModelController::status, "/check", drogon::Get, "RequestLoggerMiddleware");

    METHOD_LIST_END
    // your declaration of processing function maybe like this:
    // void get(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int p1, std::string p2);
    // void your_method_name(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, double p1, int p2) const;

    inline void predict(
      Hydrology::V1::PredictionRequest &&pReq,
      std::function<void (const drogon::HttpResponsePtr &)> &&callback
    ) const {
      Hydrology::V1::PredictionResponse result;
      result.prediction = pReq.feature1 * pReq.feature2 - pReq.feature1;
      callback(drogon::HttpResponse::newHttpJsonResponse(result.toJson()));
    }

    inline void status(
      const drogon::HttpRequestPtr &req,
      std::function<void (const drogon::HttpResponsePtr &)> &&callback
    ) const {
      Json::Value body;
      body["status"] = "Performing Inference";

      callback(drogon::HttpResponse::newHttpJsonResponse(body));
    }
};
}
}
