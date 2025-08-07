#pragma once

#include <M5Unified.hpp>

static auto header_height = 72;

const auto COLOR_SHADE_16 = M5.Lcd.color888(0,0,0);
const auto COLOR_SHADE_15 = M5.Lcd.color888(256/16*1,256/16*1,256/16*1);
const auto COLOR_SHADE_14 = M5.Lcd.color888(256/16*2,256/16*2,256/16*2);
const auto COLOR_SHADE_2 = M5.Lcd.color888(256/16*14,256/16*14,256/16*14);
const auto COLOR_SHADE_1 = M5.Lcd.color888(256/16*15,256/16*15,256/16*15);
const auto COLOR_SHADE_0 = M5.Lcd.color888(255,255,255);