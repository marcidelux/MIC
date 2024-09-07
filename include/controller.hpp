#if !defined(ANIMATION_HANDLER)
#define ANIMATION_HANDLER

#include "panel_handler.hpp"
#include "msgeq7_handler.hpp"
//#include <MSGEQ7.h>
#include "buttons_handler.hpp"

#define PANEL_LEFT          0
#define PANEL_TOP           1
#define PANEL_FRONT         2
#define PANEL_RIGHT         3
#define PANEL_BOTTOM        4
#define PANEL_BACK          5

#define NUM_OF_PANELS       6

#define PANEL_FRONT_PIN     5
#define PANEL_RIGHT_PIN     18
#define PANEL_LEFT_PIN      2
#define PANEL_BACK_PIN      21
#define PANEL_TOP_PIN       4
#define PANEL_BOTTOM_PIN    19

#define MSGEQ7_ANALOG_PIN   15
#define MSGEQ7_STROBE_PIN   16
#define MSGEQ7_RESET_PIN    17
#define MSGEQ7_SMOOTH       100 // Range: 0-255

#define UPDATE_FREQ_1HZ  1000000
#define UPDATE_FREQ_10HZ  100000
#define UPDATE_FREQ_20HZ   50000
#define UPDATE_FREQ_40HZ   25000
#define UPDATE_FREQ_80HZ   12500
#define UPDATE_FREQ_100HZ  10000

enum NormalAnimation_t {
    ANIM_0_NONE,
    ANIM_1_SPEC_ANAL,
    ANIM_2_SPEC_ANAL_MID,
    ANIM_3_DIAG_CIRCLES,
    ANIM_4_MID_CIRCLES,
    ANIM_5_MID_RECTS,
    ANIM_6_MID_RECTS_VU,
    ANIM_7_EDGE_RECTS,
};

// Enum for static animations
enum StaticAnimation_t {
    ANIM_STATIC_1_EXPANDIN_RAINBOW,
    ANIM_STATIC_2_CIRCLE,
    ANIM_STATIC_LAST,
};

class Controller
{
    public:
        Controller();
        ~Controller();
        void Init();
        void CalcAndShow();
    private:
        void stepNormalAnimation(bool direction);
        void stepStaticAnimation(bool direction);
        void setTimeFlags();
        void resetTimeFlags();
        void clear();
        void show();
        
        void animSpectrumAnalyzer();
        void animSpectrumAnalMid();
        void animDiagonalCircles();
        void animMidCircles();
        void animMidRect();
        void animMidRectVolume();
        void animEdgeRects();

        void animStaticExpandingRainbowSquares();
        void animStaticCircle();

        ButtonsHandler btHandler;
        Msgeq7 msgeq7Handler;

        uint32_t cntr_;
        uint32_t cntr_1S;
        uint32_t cntr_100MS;
        uint32_t cntr_10MS;
        uint8_t staticSpeed_;

        PanelHandler *panels_[NUM_OF_PANELS];
        uint8_t curNormalAnim_;
        uint8_t curStaticAnim_;
        bool isNormalAnimation;
        
        unsigned long previousMicros_1ms = 0;    // 1000hz
        unsigned long previousMicros_10ms = 0;   // 100Hz
        unsigned long previousMicros_20ms = 0;   // 50hz
        unsigned long previousMicros_100ms = 0;  // 10hz
        unsigned long previousMicros_1000ms = 0; // 1hz

        bool flag1ms = false;                   //  1000hz
        bool flag10ms = false;                  //  100Hz
        bool flag20ms = false;                  //  50hz
        bool flag100ms = false;                 //  10hz
        bool flag1000ms = false;                //  1hz
};

#endif
