#include <button-controller.h>
#include <Arduino.h>

/**
 * Checks the status of the physical button and sends API requests
 * as expected
 */
void ButtonController::handleButton() {
    long now = millis();

    // We're pushing in
	if (digitalRead(pinIn) == HIGH) {

		if (status == false) {
			status = true;
			pressedAt = now;
		}

        // We've been pushing in for more than the threshold, we're a long press
		if ((now - pressedAt > longPressDelay) && (longPress == false)) {
			longPress = true;
            Serial.println("Going to long press");
		}

	} else {
        // We're not pushing the button
		if (status == true) {
            // IF we JUST let go of the physical button
			if (longPress == true) {
                // if we were long pressing, stop it
				longPress = false;
                status = false;
                shortPress = false;
			} else {
                if (lastShortPress > 0 && (now - lastShortPress < burstPressDelay)) {
                    // If its been less than burstdelay since last we short pressed, increment our burst count
                    burstCount++;
                }

                if (burstCount > 0) {
                    Serial.println("Burst pressed");
                    burstCount = 0;
                    status = false;
                    shortPress = false;
                    turnOnMaxBrightness();
                    delay(500);
                } else {
                    lastShortPress = now;
                    status = false;
                    shortPress = true;
                }
  
			}
		}

        // We might have let go for a while
        if (shortPress && (now - lastShortPress > burstPressDelay) && lastShortPress > 0) {
            // If we were due for a short press, do it
            status = false;
            shortPress = false;
            Serial.println("Short pressed");
            toggleGroup();
        }
	}

    if (longPress) {
        // we're currently long pressing
        Serial.println("Long pressing");

    }

}

void ButtonController::dimGroup(int dimTo) {

}

void ButtonController::toggleGroup() {
    hue.setGroupPower(1, !hueGroup.state);
    hueGroup.state = !hueGroup.state;
}

void ButtonController::turnOnMaxBrightness() {
    hue.setGroup(1, 1, hueGroup.sat, 100, hueGroup.hue, 0);
    Serial.println('ayyy');
}
