#include <Arduino.h>
#include <hue-group.h>

HueGroup::HueGroup(
    ESPHue &hue,
    std::string name
): api(hue), name(name), brightness(255) {
    groupId = 1;
    hydrateState(api.getGroupInfo(groupId));
    Serial.println("Done for group 1");
}

String HueGroup::extractValueFromJson(
    const String &capturing,
    const String *state
   // const String state
) {
    int startOfSub = (*state).indexOf("\"" + capturing + "\"");
    int endOfSub = (*state).indexOf(",", startOfSub);
    String sub = (*state).substring(startOfSub, endOfSub);

    sub.replace(capturing, "");
    // this only works until there's a `:` in our values. lol
    sub.replace(":", "");
    sub.replace("\"", "");

    return sub;
}

void HueGroup::hydrateState(
    const String &stringState
) {
    sat = extractValueFromJson(String("sat"), &stringState).toInt();
    brightness = extractValueFromJson(String("bri"), &stringState).toInt();
    hue = extractValueFromJson(String("hue"), &stringState).toInt();
    ct = extractValueFromJson(String("ct"), &stringState).toInt();

    if (stringState.indexOf("\"on\":false") != -1) {
      state = false;
    }
    if (stringState.indexOf("\"on\":true") != -1) {
      state = true;
    }

}