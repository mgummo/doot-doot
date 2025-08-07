#include <vector>

#include <Arduino.h>
#include <LittleFS.h>
#include <M5Unified.hpp>

#include "gui_config.hpp"

#include "config.hpp"
#include "model/Task.h"
#include "router.hpp"
#include "ui/sad_face.hpp"
#include "ui/splash_screen.hpp"
#include "ui/tasks_screen.hpp"

// ui callbacks
// void handle_touch();
void handle_error();

int32_t display_width;
int32_t display_height;

Tasks tasks;
AppConfig config;
auto display = M5.Display;

Router router(display, config);

// todo: load from wifi
void init_clock() {
    tm dt;
    dt.tm_year = 2025;
    dt.tm_mon = 8;
    dt.tm_wday = 7;
    dt.tm_hour = 0;
    dt.tm_min = 0;
    dt.tm_sec = 0;
    M5.Rtc.setDateTime(dt);
}

void setup(void) {

    auto cfg = M5.config();
    M5.begin(cfg);

    Serial.begin(115200);

    try {
        Serial.println("Starting...");

        display_width = M5.Lcd.width();
        display_height = M5.Lcd.height();

        // default is upside down so rotate 90 degrees twice
        M5.Lcd.setRotation(2);
        M5.Lcd.setEpdMode(epd_mode_t::epd_fastest);
        M5.Lcd.setColorDepth(24);
        M5.Lcd.clear();

        if (!LittleFS.begin()) {
            Serial.println("💥 Failed to mount filesystem.");
            throw std::runtime_error("Failed to mount filesystem.");
        }

        config = load_config();
        tasks = config.tasks;

        // ugly
        {
            auto task_screen = (TasksScreen*)screens::tasks;
            task_screen->bind(tasks);
        }

        init_clock();

        router.switch_screens(screens::splash);

        Serial.println("Started.");
    } catch (const std::exception& e) {
        Serial.println("💥 Startup failed: ");
        Serial.println(e.what());
        handle_error();

        throw;
    }
}

void loop(void) {
    M5.update();

    router.on_loop();

    delay(100);
}

void log_touch(const m5::Touch_Class::touch_detail_t& t) {
    static m5::touch_state_t prev_state;
    if (prev_state == t.state) {
        return;
    }

    prev_state = t.state;
    static constexpr const char* state_name[16] = {
        "none", "touch", "touch_end", "touch_begin", "___", "hold", "hold_end", "hold_begin",
        "___",  "flick", "flick_end", "flick_begin", "___", "drag", "drag_end", "drag_begin"};

    // M5.Display.setTextDatum(middle_center);
    // M5.Display.setTextFont(&fonts::FreeSans12pt7b);
    // M5.Display.setTextSize(1);
    // M5.Display.drawString("{x} {y} {state}", M5.Display.width() / 2,
    // M5.Display.height() - 50); const char *con_str = button.contains(w / 2, h /
    // 2) ? "Yes" : "No";// coordinate: (w / 2, h / 2)
    // M5.Display.drawString(con_str, M5.Display.width() / 2, M5.Display.height()
    // - 20);

    M5_LOGI("%s", state_name[t.state]);
}

// void handle_touch() {
//     auto t = M5.Touch.getDetail();
//     log_touch(t);

//     handle_touch(t, tasks);
// }

void handle_error() {
    draw_sad_face(M5.Display);
    M5.Power.powerOff();
}
