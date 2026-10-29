#pragma once

#include <drogon/HttpController.h>
#include <json/json.h>

class SessionController : public drogon::HttpController<SessionController> {

public:
    METHOD_LIST_BEGIN
    METHOD_ADD(SessionController::visit, "/visit", drogon::Get);
    METHOD_ADD(SessionController::clear, "/reset", drogon::Delete);
    METHOD_LIST_END

    void visit(
        const drogon::HttpRequestPtr &req,
        std::function<void (const drogon::HttpResponsePtr &)> &&cb
    ) const {
        auto session = req->session();

        if(session == nullptr) {
            Json::Value body;
            body["error"] = "Session support is unavailable";
            LOG_INFO << "Request from " << req->peerAddr().toIp() << " | Method: " << req->getMethodString() << " | Status: Session Not Found";
            auto resp = drogon::HttpResponse::newHttpJsonResponse(body);
            resp->setStatusCode(drogon::k500InternalServerError);
            cb(resp);
            return;
        }

        if (session->find("visit_count")) {
            int visitCount = 0;
            session->modify<int>("visit_count", [&visitCount](int& storedCount){ visitCount = ++storedCount; });
            
            Json::Value body;
            body["message"] = "Welcome back! Visit count: " + std::to_string(visitCount);
            body["visit_count"] = visitCount;

            LOG_INFO << "Session ID: " << session->sessionId() << " visit count + 1. Total visit count: " << visitCount;

            cb(drogon::HttpResponse::newHttpJsonResponse(body));
        }
        else {
            session->insert("visit_count", 1);
            Json::Value body;
            body["message"] = "Welcome user!";
            body["visit_count"] = 1;

            LOG_INFO << "A new session has been created. Session ID: " << session->sessionId();

            cb(drogon::HttpResponse::newHttpJsonResponse(body));
        }
    }

    void clear(
        const drogon::HttpRequestPtr &req,
        std::function<void (const drogon::HttpResponsePtr &)> &&cb
    ) const {
        auto session = req->session();

        if(session == nullptr) {
            Json::Value body;
            body["error"] = "Session support is unavailable";
            LOG_INFO << "Request from " << req->peerAddr().toIp() << " | Method: " << req->getMethodString() << " | Status: Session Not Found";
            auto resp = drogon::HttpResponse::newHttpJsonResponse(body);
            resp->setStatusCode(drogon::k500InternalServerError);
            cb(resp);
            return;
        }

        if (session->find("visit_count")) {
            session->erase("visit_count");

            Json::Value body;
            body["message"] = "Visit count for this session has been reseted.";

            LOG_INFO << "Session ID: " << session->sessionId() << " visit count reseted.";

            cb(drogon::HttpResponse::newHttpJsonResponse(body));
        }
        else {
            LOG_INFO << "Attempted to perform a reset on a session without the visit_count key";
            Json::Value body;
            body["message"] = "Visit count is already reset.";
            cb(drogon::HttpResponse::newHttpJsonResponse(body));
        }
    }
};