#pragma once

#include <optional>
#include <json/json.h>

namespace JsonUtils  {
    inline std::optional<Json::Value> toJson(const std::string &message) {
      Json::Value json;
      Json::CharReaderBuilder reader;
      std::string errors;
      std::istringstream stream(message);

      if (!Json::parseFromStream(reader, stream, &json, &errors)) {
        return std::nullopt;
      }

      return json;
    }
}