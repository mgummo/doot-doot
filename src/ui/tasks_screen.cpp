#include <sstream>

#include "gui_config.hpp"
#include "tasks_screen.hpp"
#include <audio/dootdoot.hpp>
#include <audio/fanfare.hpp>

void build_header(M5GFX& display, const AppConfig& config);
M5Canvas* build_main(M5GFX& display, const Tasks& tasks);
LGFX_Button* build_button(M5GFX& display, const std::string& title, int row, int col);
bool all_tasks_are_completed(Tasks& tasks);

M5Canvas header(&M5.Lcd);
LGFX_Button** buttons;

void draw_tasks_screen(M5GFX& display, const AppConfig& config, const Tasks& tasks) {
    build_header(display, config);
    build_main(display, tasks);
    draw_footer(display);
}

void build_header(M5GFX& display, const AppConfig& config) {

    M5Canvas header(&display);

    header.setFont(&fonts::FreeSerif24pt7b);

    header.setColorDepth(8);
    header.setFont(&fonts::FreeSerif24pt7b);
    header.setTextWrap(false);
    header.setTextSize(1);
    header.createSprite(display.width(), header_height);

    auto text = config.title;

    header.setTextColor(COLOR_SHADE_1);
    header.fillSprite(COLOR_SHADE_15);
    header.setTextDatum(middle_center);
    header.drawString(text.c_str(), header.width() / 2, header.height() / 2);
    header.pushSprite(&M5.Lcd, 0, 0);
}

M5Canvas* build_main(M5GFX& display, const Tasks& tasks) {
    auto parent = &M5.Lcd;

    auto pCanvas = new M5Canvas(parent);
    M5Canvas& canvas = *pCanvas;

    // canvas.createSprite(display_width, display_height - 2 * header_height);
    // canvas.fillSprite(COLOR_SHADE_0);

    auto count = tasks.size();

    buttons = new LGFX_Button*[count];

    // todo: hack, to make this look good for the current data
    auto col1_size = 5;

    // todo: smarter logic here, full width if only 8 tasks
    for (int i = 0; i < col1_size; i++) {
        buttons[i] = build_button(display, tasks[i]->Title, i, 0);
    }

    for (int i = col1_size; i < count; i++) {
        buttons[i] = build_button(display, tasks[i]->Title, i - col1_size, 1);
    }

    return pCanvas;
}

LGFX_Button* build_button(M5GFX& display, const std::string& title, int row, int col) {

    auto display_width = display.width();

    LGFX_Button* button = new LGFX_Button();

    auto margin_left = 25;
    auto margin_right = 25;
    auto button_height = 60;
    auto margin_top = 25;

    // start drawing buttons after the header
    auto button_group_top = header_height + margin_top;

    auto button_width = (display_width / 2) - margin_left - margin_right;
    // auto button_width = (w-2*margin) / 2;

    auto gap = 20;

    auto x_pos = margin_left + (button_width + margin_right) * col;
    auto y_pos = button_group_top + (button_height + gap) * row;

    auto text_size = 2;

    auto outline = TFT_RED;
    auto fill = TFT_YELLOW;
    auto text_color = TFT_BLACK;

    button->initButtonUL(&M5.Lcd, x_pos, y_pos, button_width, button_height, outline, fill, text_color,
                         title.c_str(), // buggy piece of crap can't display a label longer than 11 chars
                         text_size, text_size);
    button->drawButton(0, title.c_str());

    return button;
}

void handle_touch(const m5::Touch_Class::touch_detail_t t, Tasks& tasks) {
    for (int i = 0; i < tasks.size(); i++) {
        auto task = tasks[i];
        LGFX_Button* button = buttons[i];

        if (button->contains(t.x, t.y)) {

            // button->drawButton(true, task.Title.c_str());
            // button->drawButton(true);

            if (t.state == m5::touch_state_t::touch_begin) {
                auto toggle = !button->isPressed();
                button->press(toggle);
                button->drawButton(toggle, task->Title.c_str());

                if (all_tasks_are_completed(tasks)) {
                    play_fanfare();
                } else {
                    play_doot_doot();
                }
            }

            if (t.state == m5::touch_state_t::flick_end) {
                button->drawButton(button->isPressed(), "flicked");
            }

            if (t.state == m5::touch_state_t::drag_end) {
                button->drawButton(button->isPressed(), "dragged");
            }
        }
    }
}

void draw_footer(M5GFX& display) {

    auto ticks = millis();
    auto time = M5.Rtc.getDateTime();

    auto display_width = display.width();
    auto display_height = display.height();

    M5Canvas canvas(&display);

    // full screen, except for header / footer;
    canvas.createSprite(display_width, header_height);
    canvas.fillSprite(COLOR_SHADE_15);

    bool isCharging = M5.Power.isCharging();
    int32_t batteryLevel = M5.Power.getBatteryLevel(); // 0 - 100 %

    // todo: std::format ?
    std::ostringstream oss;
    oss << "Battery: " << batteryLevel << "%";
    if (isCharging) {
        oss << " (Charging)";
    }

    std::ostringstream formatted_time;
    formatted_time << time.date.year << "/" << time.date.month << time.date.date << " ";
    formatted_time << time.time.hours << ":" << time.time.minutes << ":" << time.time.seconds;

    canvas.setFont(&fonts::FreeMonoBold9pt7b);
    canvas.setTextSize(1);
    canvas.setTextColor(COLOR_SHADE_1);
    // canvas.setTextDatum(middle_left);
    // canvas.drawString(oss.str().c_str(), 0 + margin, canvas.height() / 2);
    canvas.setCursor(0, 0);
    canvas.print(" ");
    canvas.print(oss.str().c_str());
    canvas.print("    ");
    canvas.print(formatted_time.str().c_str());
    canvas.println();
    canvas.print(" ");
    canvas.println(ticks);

    // display under the main content
    canvas.pushSprite(0, display_height - header_height);
}

bool all_tasks_are_completed(Tasks& tasks) {
    // todo: should be checking Tasks model, not ui state
    for (int i = 0; i < tasks.size(); i++) {
        auto task = tasks[i];
        LGFX_Button* button = buttons[i];

        if (!button->isPressed()) {
            return false;
        }
    }
    return true;
}