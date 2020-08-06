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
            longDimTo = hueGroup.brightness;
            Serial.println("Starting transition UP");
            startTransition(true);
		}

	} else {
        // We're not pushing the button
		if (status == true) {
            // IF we JUST let go of the physical button
			if (longPress == true) {
                stopTransition();
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
        long time = now - (pressedAt + longPressDelay);
        long doubleDelay = longCycleInterval * 2;

        // We're wanting to go up 0 -> 255
        if ( time % doubleDelay < longCycleInterval ) {
            if ((time - 210) % doubleDelay > longCycleInterval) {
                // Start the transition up
                Serial.println("Starting transition UP");
                startTransition(true);
            }
        } else {
            if ((time - 210) % doubleDelay < longCycleInterval) {
                // Start the transition down
                Serial.println("Starting transition DOWN");
                startTransition(false);
            }
        }

        delay(200);
    }

}

void ButtonController::startTransition(bool toOn) {
    int dimTo;

    if (toOn) { dimTo = 254; }
    else { dimTo = -254; }

    hueGroup.state = true;
    hue.setGroupBriInc(1, dimTo, longCycleInterval/100);
}

void ButtonController::stopTransition() {
    Serial.println("Stopping transition");
    hueGroup.state = true;
    hue.setGroupBriInc(1, 0, longCycleInterval/100);
}

void ButtonController::toggleGroup() {
    hue.setGroupPower(1, !hueGroup.state);
    hueGroup.state = !hueGroup.state;
}

void ButtonController::turnOnMaxBrightness() {
    hue.setGroup(1, 1, hueGroup.sat, 254, hueGroup.hue);
    hueGroup.state = 1;
    Serial.println("ayyy");
}
