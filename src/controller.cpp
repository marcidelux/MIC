#include "controller.hpp"

Controller::Controller()
{
}

Controller::~Controller()
{
}

void Controller::Init()
{
    Serial.println("init msgeq7");
    curNormalAnim_ = ANIM_0_NONE;
    curStaticAnim_ = ANIM_STATIC_1_EXPANDIN_RAINBOW;
    isNormalAnimation = true;
    cntr_ = 0;
    staticSpeed_ = 1;

    panels_[PANEL_LEFT] = new PanelHandler(PANEL_LEFT_PIN, STARTING_POINT_BOTTOM_LEFT_Y_GOES_RIGHT_X_GOES_UP);
    panels_[PANEL_TOP] = new PanelHandler(PANEL_TOP_PIN, STARTING_POINT_TOP_LEFT_X_GOES_RIGHT_Y_GOES_DOWN);
    panels_[PANEL_FRONT] = new PanelHandler(PANEL_FRONT_PIN, STARTING_POINT_BOTTOM_LEFT_Y_GOES_RIGHT_X_GOES_UP);
    panels_[PANEL_RIGHT] = new PanelHandler(PANEL_RIGHT_PIN, STARTING_POINT_TOP_LEFT_Y_GOES_RIGHT_X_GOES_DOWN);
    panels_[PANEL_BOTTOM] = new PanelHandler(PANEL_BOTTOM_PIN, STARTING_POINT_TOP_RIGHT_X_GOES_LEFT_Y_GOES_DOWN);
    panels_[PANEL_BACK] = new PanelHandler(PANEL_BACK_PIN, STARTING_POINT_BOTTOM_LEFT_X_GOES_RIGHT_Y_GOES_UP);
    Serial.println("clear");
    clear();
    Serial.println("show");
    show();
}

void Controller::stepNormalAnimation(bool direction)
{
    isNormalAnimation = true;
    cntr_ = 0;

    if (direction) {
        if(curNormalAnim_ == ANIM_7_EDGE_RECTS) {
            curNormalAnim_ = ANIM_1_SPEC_ANAL;
        } else {
            curNormalAnim_ ++;
        }
    } else {
        if(curNormalAnim_ == ANIM_1_SPEC_ANAL) {
            curNormalAnim_ = ANIM_7_EDGE_RECTS;
        } else {
            curNormalAnim_--;
        }
    }
}

void Controller::stepStaticAnimation(bool direction)
{
    isNormalAnimation = false;
    cntr_ = 0;
    
    if (direction) {
        if(curStaticAnim_ == ANIM_STATIC_2_CIRCLE) {
            curStaticAnim_ = ANIM_STATIC_1_EXPANDIN_RAINBOW;
        } else {
            curStaticAnim_++;
        }
    } else {
        if(curStaticAnim_ == ANIM_STATIC_1_EXPANDIN_RAINBOW) {
            curStaticAnim_ = ANIM_STATIC_2_CIRCLE;
        } else {
            curStaticAnim_--;
        }
    }
}

void Controller::CalcAndShow()
{
    setTimeFlags();

    // 10Hz read
    if(flag100ms)
    {
        if (btHandler.Measure()) {
            if(btHandler.buttons[BUTTON_RED] == BT_STATE_RELEASED) {
                stepNormalAnimation(true);
            } else if (btHandler.buttons[BUTTON_YELLOW] == BT_STATE_RELEASED) {
                stepNormalAnimation(false);
            } else if (btHandler.buttons[BUTTON_BLUE] == BT_STATE_RELEASED) {
                stepStaticAnimation(true);
            } else if (btHandler.buttons[BUTTON_GREEN] == BT_STATE_RELEASED) {
                stepStaticAnimation(false);
            }

            Serial.printf("Button state: %d %d %d %d\n", btHandler.buttons[BUTTON_RED], btHandler.buttons[BUTTON_YELLOW], btHandler.buttons[BUTTON_BLUE], btHandler.buttons[BUTTON_GREEN]);
            Serial.printf("Current normal anim: %d\n", curNormalAnim_);

            if (btHandler.potmeterChange[POTMETER_RED])
            {
                float brightness = (btHandler.potmeters[POTMETER_RED] / 255.0f) * BIRGHTNESS_DIFF + BRIGHTNESS_MIN;
                for (uint8_t i = 0; i < NUM_OF_PANELS; i++)
                {
                    panels_[i]->SetBrightness(brightness);
                }
                Serial.printf("Brightness: %f\n", brightness);
            }

            if (btHandler.potmeterChange[POTMETER_BLUE])
            {
                staticSpeed_ = btHandler.potmeters[POTMETER_BLUE] / 32;
                float sensitivity = (btHandler.potmeters[POTMETER_BLUE] / 255.0f) * SENSITIVITY_DIFF + SENSITIVITY_MIN;
                for (uint8_t i = 0; i < NUM_OF_PANELS; i++)
                {
                    msgeq7Handler.SetSensitivity(sensitivity);
                }
                Serial.printf("Sensitivity: %f\n", sensitivity);
            }
        }
    }

    // 50Hz read
    if(flag20ms)
    {
        msgeq7Handler.Measure();

        if(isNormalAnimation) {
            switch (curNormalAnim_)
            {
                case ANIM_1_SPEC_ANAL:
                    animSpectrumAnalyzer();
                    break;
                case ANIM_2_SPEC_ANAL_MID:
                    animSpectrumAnalMid();
                    break;
                case ANIM_3_DIAG_CIRCLES:
                    animDiagonalCircles();
                    break;
                case ANIM_4_MID_CIRCLES:
                    animMidCircles();
                    break;
                case ANIM_5_MID_RECTS:
                    animMidRect();
                    break;
                case ANIM_6_MID_RECTS_VU:
                    animMidRectVolume();
                    break;
                case ANIM_7_EDGE_RECTS:
                    animEdgeRects();
                    break;
                default:
                    break;
            }
        } else {
            switch (curStaticAnim_)
            {
                case ANIM_STATIC_1_EXPANDIN_RAINBOW:
                    animStaticExpandingRainbowSquares();
                    break;
                case ANIM_STATIC_2_CIRCLE:
                    animStaticCircle();
                    break;
                default:
                    break;
            }
        }
        panels_[PANEL_BOTTOM]->Clear();
        show();
    }

    // 100Hz
    if (flag10ms)
    {
        
    }

   resetTimeFlags();
}

void Controller::setTimeFlags()
{
    unsigned long currentMicros = micros();

    // Check if 1ms (1000 microseconds) has passed
    if (currentMicros - previousMicros_1ms >= 1000) {
        flag1ms = true;  // Set the flag
        previousMicros_1ms = currentMicros;  // Reset timer
    }

    // Check if 10ms (10000 microseconds) has passed
    if (currentMicros - previousMicros_10ms >= 10000) {
        flag10ms = true;  // Set the flag
        previousMicros_10ms = currentMicros;  // Reset timer
    }

    // Check if 20ms (20000 microseconds) has passed (for 50Hz)
    if (currentMicros - previousMicros_20ms >= 20000) {
        flag20ms = true;
        previousMicros_20ms = currentMicros;
    }

    // Check if 100ms (100000 microseconds) has passed
    if (currentMicros - previousMicros_100ms >= 100000) {
        flag100ms = true;  // Set the flag
        previousMicros_100ms = currentMicros;  // Reset timer
    }

    // Check if 1000ms (1000000 microseconds) has passed
    if (currentMicros - previousMicros_1000ms >= 1000000) {
        flag1000ms = true;  // Set the flag
        previousMicros_1000ms = currentMicros;  // Reset timer
    }
}

void Controller::resetTimeFlags()
{
    if(flag1ms)
    {
        flag1ms = false;
    }
    if(flag10ms)
    {
        flag10ms = false;
    }
    if(flag20ms)
    {
        flag20ms = false;
    }
    if(flag100ms)
    {
        flag100ms = false;
    }
    if(flag1000ms)
    {
        flag1000ms = false;
    }
}

void Controller::clear()
{
    for (uint8_t i = 0; i < NUM_OF_PANELS; i++)
    {
        panels_[i]->Clear();
    }
}

void Controller::show()
{
    for (uint8_t i = 0; i < NUM_OF_PANELS; i++)
    {
        panels_[i]->Show();
    }
}

// ############### ANIMATIONS ######################
void Controller::animSpectrumAnalyzer()
{
    static Rectangle rects[7] = {
        {Cord{0,0},1,0,BASE_COLORS[0]},
        {Cord{2,0},1,0,BASE_COLORS[1]},
        {Cord{4,0},1,0,BASE_COLORS[2]},
        {Cord{6,0},1,0,BASE_COLORS[3]},
        {Cord{8,0},1,0,BASE_COLORS[4]},
        {Cord{10,0},1,0,BASE_COLORS[5]},
        {Cord{12,0},1,0,BASE_COLORS[6]},
    };

    clear();

    for (uint8_t i = 0; i < NUM_OF_PANELS; i++)
    {
        for (uint8_t ri = 0; ri < 7; ri++)
        {
            uint8_t height = uint8_t(msgeq7Handler.values[ri] / 16);
            rects[ri].height = height;
            panels_[i]->DrawRectangle(rects[ri]);
        }
    }
}

void Controller::animSpectrumAnalMid()
{
    static Rectangle rects[7] = {
        {Cord{0,0},1,0,Color{255,0,0}},
        {Cord{2,0},1,0,Color{255,255,0}},
        {Cord{4,0},1,0,Color{0,255,0}},
        {Cord{6,0},1,0,Color{0,255,255}},
        {Cord{8,0},1,0,Color{0,0,255}},
        {Cord{10,0},1,0,Color{255,0,255}},
        {Cord{12,0},1,0,Color{255,255,255}},
    };

    clear();

    for (uint8_t i = 0; i < NUM_OF_PANELS; i++)
    {
        for (uint8_t ri = 0; ri < 7; ri++)
        {
            uint8_t height = uint8_t(msgeq7Handler.values[ri] / 16);
            rects[ri].height = height;
            panels_[i]->DrawRectangle(rects[ri]);
        }
    }
}

void Controller::animDiagonalCircles()
{
    clear();

    for(uint8_t i = 0; i < 7; i++) {
        uint8_t height = uint8_t(msgeq7Handler.values[i] / 16);
        uint8_t height2 = uint8_t(msgeq7Handler.values[6-i] / 16);
        panels_[PANEL_FRONT]->DrawCircle(Cord{(uint8_t)(14-i*2),(uint8_t)(14-i*2)}, height, BASE_COLORS[i]);
        panels_[PANEL_BACK]->DrawCircle(Cord{(uint8_t)(14-i*2),(uint8_t)(14-i*2)}, height, BASE_COLORS[i]);
        panels_[PANEL_LEFT]->DrawCircle(Cord{(uint8_t)(14-i*2),(uint8_t)(14-i*2)}, height, BASE_COLORS[i]);
        panels_[PANEL_RIGHT]->DrawCircle(Cord{(uint8_t)(14-i*2),(uint8_t)(14-i*2)}, height, BASE_COLORS[i]);
        panels_[PANEL_TOP]->DrawCircle(Cord{(uint8_t)(14-i*2),(uint8_t)(14-i*2)}, height, BASE_COLORS[i]);
    }
}

void Controller::animMidCircles()
{
    const Cord midCord{7,7};
    static float hue = 0.0; // Hue in range [0.0, 1.0]
    static Color currentColor = HSVtoRGB(hue, 1.0, 1.0);
    currentColor = HSVtoRGB(hue, 1.0, 1.0);

    clear();

    for (uint8_t i = 0; i < NUM_OF_PANELS; i++)
    {
        uint8_t size = uint8_t(msgeq7Handler.values[i] / 16);
        panels_[i]->DrawCircle(midCord, size, currentColor);
    }
    hue += 0.01; // Adjust the step size for speed of color change
    if (hue >= 1.0) {
        hue = 0.0; // Loop back to start
    }
}

void Controller::animMidRect()
{
    // Static variable to hold the hue and color
    static float hue = 0.0; // Hue in range [0.0, 1.0]
    static Color currentColor = HSVtoRGB(hue, 1.0, 1.0);
    
    currentColor = HSVtoRGB(hue, 1.0, 1.0);

    const Cord midCord{7,7};

    clear();

    for (uint8_t i = 0; i < NUM_OF_PANELS; i++)
    {
        uint8_t size = uint8_t(msgeq7Handler.values[i] / 16);
        panels_[i]->DrawFilledSquare(midCord, size, currentColor);
    }

    hue += 0.01; // Adjust the step size for speed of color change
    if (hue >= 1.0) {
        hue = 0.0; // Loop back to start
    }
}

void Controller::animMidRectVolume()
{
    // Static variable to hold the hue and color
    static float hue = 0.0; // Hue in range [0.0, 1.0]
    static Color currentColor = HSVtoRGB(hue, 1.0, 1.0);
    
    currentColor = HSVtoRGB(hue, 1.0, 1.0);

    const Cord midCord{7,7};

    clear();

    uint8_t size = uint8_t(msgeq7Handler.volume / 16);
    for (uint8_t i = 0; i < NUM_OF_PANELS; i++)
    {
        panels_[i]->DrawFilledSquare(midCord, size, currentColor);
    }

    hue += 0.01; // Adjust the step size for speed of color change
    if (hue >= 1.0) {
        hue = 0.0; // Loop back to start
    }
}

void Controller::animEdgeRects()
{
    const Cord LT{0,0};
    const Cord RT{15,0};
    const Cord LB{0,15};
    const Cord RB{15,15};

    static float hues[6] = {0.0, 0.166, 0.333, 0.5, 0.666, 0.833};  // Values in [0.0, 1.0]
    // Create an array to store the six colors
    static Color sideColors[6];

    // Initialize each color with a different starting hue
    for (int i = 0; i < 6; ++i) {
        sideColors[i] = HSVtoRGB(hues[i], 1.0, 1.0);  // Full saturation and brightness
    }
    clear();

    // Go througth the freqz
    for (uint8_t i = 0; i < NUM_OF_PANELS; i++)
    {
        uint8_t size = uint8_t(msgeq7Handler.values[i] / 16);
        panels_[i]->DrawFilledSquare(LT, size, sideColors[i]);
        panels_[i]->DrawFilledSquare(RT, size, sideColors[i]);
        panels_[i]->DrawFilledSquare(LB, size, sideColors[i]);
        panels_[i]->DrawFilledSquare(RB, size, sideColors[i]);

        hues[i] += 0.01; // Adjust the step size for speed of color change
        if (hues[i] >= 1.0) {
            hues[i] = 0.0; // Loop back to start
        }
    }
}

void Controller::animStaticExpandingRainbowSquares()
{
    static float hue = 0.0;
    static uint8_t size = 1;
    static uint8_t lscntr = 0;

    // Slow down animation speed
    if(lscntr < staticSpeed_) {
        lscntr++;
        return;
    }
    lscntr = 0;

    // Clear all panels
    for(uint8_t pidx = 0; pidx < NUM_OF_PANELS; pidx++) {
        panels_[pidx]->Clear();
    }

    // Increment counter and reset size if too large
    cntr_++;
    if (cntr_ == 30) {
        cntr_ = 0;
        size = 1;
    }
    size++;

    // Update the hue for rainbow effect
    hue += 0.01;
    if (hue >= 1.0) hue = 0.0;
    Color color = HSVtoRGB(hue, 1.0, 1.0);

    // Draw expanding squares on each panel
    for(uint8_t pidx = 0; pidx < NUM_OF_PANELS; pidx++) {
        Cord center = {7, 7};  // Center of the panel
        panels_[pidx]->DrawFilledSquare(center, size, color);
    }
}

void Controller::animStaticCircle()
{
    static Color RED{255,0,0};
    static Cord center{7,7};
    static uint8_t radius  = 1;
    static uint8_t lscntr = 0;

    if(lscntr < staticSpeed_) {
        lscntr++;
        return;
    }
    lscntr = 0;

    cntr_++;
    if(cntr_ == 15)
    {
        cntr_ = 0;
        radius = 1;
    }
    radius++;

    clear();
    
    for(uint8_t pidx = 0; pidx < NUM_OF_PANELS; pidx++)
    {
        panels_[pidx]->DrawCircle(center, radius, RED);
    }
}
