#pragma once

namespace Hydrology::V1 {

struct PredictionRequest {
    double feature1 = 0;
    double feature2 = 0;
};

struct PredictionResponse {
    double prediction = 0;

    Json::Value toJson() const {
        Json::Value body;
        body["prediction"] = prediction;
        return body;
    }
};

}