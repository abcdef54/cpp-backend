#pragma once

#include <drogon/HttpMiddleware.h>
#include <chrono>
#include <trantor/utils/Logger.h>
#include <utility>

class RequestLoggerMiddleware : public drogon::HttpMiddleware<RequestLoggerMiddleware> {

public:
    RequestLoggerMiddleware() {}

    inline void invoke(
        const drogon::HttpRequestPtr &req,
        drogon::MiddlewareNextCallback &&ncb,
        drogon::MiddlewareCallback &&cb
    ) override {
        LOG_INFO << "Received HTTP " << req->methodString() << " method from adress: " << req->peerAddr().toIp();
        const auto start = std::chrono::steady_clock::now();

        const std::string &origin = req->getHeader("origin");
        if (origin.find("https://www.google.com") != std::string::npos) {
            cb(drogon::HttpResponse::newNotFoundResponse(req));
            return;
        }

        ncb([cb = std::move(cb), start](const drogon::HttpResponsePtr &resp) {
            const auto end = std::chrono::steady_clock::now();
            const auto elapsedMs = std::chrono::duration<double, std::milli>(end - start).count();
            const auto status = resp->statusCode();
            const auto logLevel = elapsedMs > 100.0 ? trantor::Logger::kWarn : trantor::Logger::kInfo;

            if (logLevel >= trantor::Logger::logLevel()) {
                trantor::Logger(__FILE__, __LINE__, logLevel).stream()
                    << "Request completed in " << elapsedMs
                    << " ms | Status Code: " << static_cast<int>(status);
            }

            cb(resp);
        });
    }

};
