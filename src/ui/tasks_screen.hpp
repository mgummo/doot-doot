#pragma once

#include "../config.hpp"
#include "../model/Task.h"
#include <M5Unified.hpp>

void draw_tasks_screen(M5GFX& display, const AppConfig& config, const Tasks& tasks);
void draw_footer(M5GFX& display);
void handle_touch(const m5::Touch_Class::touch_detail_t touch, Tasks& tasks);