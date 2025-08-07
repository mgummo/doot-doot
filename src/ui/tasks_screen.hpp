#pragma once

#include <M5Unified.hpp>

#include "../config.hpp"
#include "../model/Task.h"
#include "../screen.hpp"

class AppConfig;
class Router;

void draw_tasks_screen(M5GFX& display, const AppConfig& config, const Tasks& tasks);
void draw_footer(M5GFX& display);
void handle_touch(const m5::Touch_Class::touch_detail_t touch, Tasks& tasks);

class TasksScreen : public screen {

  private:
    M5GFX* display;
    AppConfig* config;

    Tasks* tasks;

  public:
    TasksScreen(Router& router, M5GFX& display, AppConfig& config) {
        this->display = &display;
        this->config = &config;
    }

    void bind(Tasks& tasks);

    void on_enter() override;
    void on_loop() override;
    void on_exit() override {}
};