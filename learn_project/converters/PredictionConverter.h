#pragma once

#include <drogon/HttpRequest.h>
#include "dto/PredictionDto.h"

namespace drogon {

template<>
inline Hydrology::V1::PredictionRequest fromRequest(const drogon::HttpRequest &req) {
    auto json = req.getJsonObject();
    Hydrology::V1::PredictionRequest predReq;

    if (json) {
        predReq.feature1 = (*json)["feature1"].asDouble();
        predReq.feature2 = (*json)["feature2"].asDouble();
    }
    return predReq;
}

}