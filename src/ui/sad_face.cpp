#include <M5Unified.h>

void draw_sad_face(M5GFX &display)
{
    display.setEpdMode(m5gfx::epd_mode_t::epd_fast);

    display.setColor(6); // Mid-gray tone
    const float thickness = 12;

    int cx = display.width() / 2;
    int cy = display.height() / 2;
    int radius = std::min(cx, cy) - 20;

    // Draw outer circle (the face)
    display.fillEllipseArc(
        cx, cy,
        radius - thickness, radius,
        radius - thickness, radius,
        0, 360);

    // Eyes
    int eyeOffsetX = radius / 3;
    int eyeOffsetY = radius / 3;
    int eyeRadius = radius / 10;

    display.fillCircle(cx - eyeOffsetX, cy - eyeOffsetY, eyeRadius);
    display.fillCircle(cx + eyeOffsetX, cy - eyeOffsetY, eyeRadius);

    const float mouthWidth = radius * 0.9;
    const float mouthHeight = radius * 0.4;

    // Move the mouth below eyes
    const float center_mouth_x = cx;
    const float center_mouth_y = cy + radius / 2;

    // const float mouth_thickness = 3;

    float outerRx = mouthWidth / 2;
    float innerRx = outerRx - thickness;

    float outerRy = mouthHeight / 2;
    float innerRy = outerRy - thickness;

    // float outerRx = mouthWidth / 2;
    // float innerRx = outerRx - thickness;

    // float outerRy = mouthHeight / 2;
    // float innerRy = outerRy - thickness;

    // angle is measured going clockwise
    const int starting_angle = 180;
    const int ending_angle = 360;

    display.fillEllipseArc(
        center_mouth_x, center_mouth_y,
        innerRx, outerRx,
        innerRy, outerRy,
        starting_angle, ending_angle);

    const int margin = 40;
    // display.setTextDatum(middle_center);
    display.setTextSize(3);
    display.setCursor(10, cy + radius + margin);
    display.printf("System Crashed\n");
    // display.printf() cx, cy + radius + margin
    // display.printf("fillEllipseArc(%f, %f, %f, %f, %f, %f)\n",cx, cy + radius + margin")
}
