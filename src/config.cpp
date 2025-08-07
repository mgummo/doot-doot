#include <ArduinoJson.h>
#include <FS.h>
#include <LittleFS.h>

#include "config.hpp"

std::string get(const JsonDocument& doc, std::string_view key, std::string_view default_value);
void load_tasks(const JsonVariant& schedule, AppConfig& config);

AppConfig load_config() {
    const std::string path = "/config.json";

    if (!LittleFS.begin()) {
        Serial.println("💥 Failed to mount filesystem.");
        throw std::runtime_error("Failed to load config.");
    }

    File file = LittleFS.open(path.c_str(), "r");
    if (!file) {
        Serial.printf("💥 Failed to open config file: %s\n", path);
        throw std::runtime_error("Failed to load config.");
    }

    JsonDocument doc;
    auto error = deserializeJson(doc, file);
    file.close();

    if (error) {
        Serial.println("💥 Failed to parse config file:");
        Serial.println(error.c_str());
        throw std::runtime_error("Failed to load config.");
    }

    AppConfig config;
    config.title = get(doc, "title", "Tasks");

    Serial.println("👣 Reading schedules...");

    auto schedules = doc["schedules"];
    if (schedules.isNull()) {
        throw std::runtime_error("Cannot read schedules from config.");
    }

    // todo: support multiple schedules
    auto schedule = schedules[0];

    load_tasks(schedule, config);
    Serial.printf("✅ Loaded %d tasks from config.\n", config.tasks.size());

    return config;
}

void load_tasks(const JsonVariant& schedule, AppConfig& config) {
    auto tasks = schedule["tasks"];
    if (tasks.isNull()) {
        // Empty config is unusual, but allowed.
        return;
    }

    for (auto dict : tasks.as<JsonArray>()) {
        auto t = new Task{
            .Title = dict["title"],
        };

        config.tasks.push_back(t);
    }
}

std::string get(const JsonDocument& doc, std::string_view key, std::string_view default_value) {
    auto val = doc[key.data()];
    if (val.isNull()) {
        return std::string(default_value);
    }

    return val.as<std::string>();
}