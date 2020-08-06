#include <Arduino.h>
#include <./hue.h>
#include <./hue-group.h>

class ButtonController {
    protected:
        static const long longPressDelay = 500; 
        static const long burstPressDelay = 150;
        static const int longCycleInterval = 5000;
        
        ESPHue hue;
        bool status;
        bool shortPress;
        int burstCount;
        bool longPress;
        long pressedAt;
        long lastShortPress;
        std::string groupName;
        int pinIn;
        HueGroup hueGroup;
        int longDimTo;

        void toggleGroup();
        void turnOnMaxBrightness();
        void stopTransition();
        void startTransition(bool toOn);
        int calculateLongPress(long now, long pressedAt);

    public:
        ButtonController(ESPHue &hue_api, std::string groupName, int pin_in):
            hue(hue_api),
            status(0), 
            shortPress(false), 
            burstCount(0), 
            longPress(false), 
            pressedAt(0), 
            lastShortPress(0),
            groupName(groupName),
            pinIn(pin_in),
            hueGroup(HueGroup(hue_api, groupName))
             {}


        void handleButton();
};