#ifndef HUE_GROUP_H
#define HUE_GROUP_H
#include <Arduino.h>
#include <hue.h>


/**
 * Describes a group (ie: room) of HUE lights
 */
class HueGroup {
    protected:
        ESPHue api;

        void hydrateState(const String &state);
        String extractValueFromJson(const String &capturing, const String *state);

    public:
        /** name of the group */
        std::string name;
        /** hue id of the group */
        int groupId;
        /** current known brightness of group */
        int brightness;
        /** current known hue of this group */
        int hue;
        /** current known sat of this group */
        int sat;
        /** current known ct of this group */
        int ct;
        /** current known state of group */
        bool state;

        HueGroup(
            ESPHue &hue,
            std::string name
        );
};
#endif