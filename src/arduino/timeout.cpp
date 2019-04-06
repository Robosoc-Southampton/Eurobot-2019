
#include "include/timeout.h"

namespace robot {
	int32_t start_time = 0;
	bool started = false;
	bool pull_cord_inserted = false;

	bool wait_for_pullcord(uint8_t pin) {
		digitalWrite(LED_BUILTIN, HIGH);

		if (!pull_cord_inserted && !digitalRead(pin)) {
			rlogf("Pull cord inserted");
			pull_cord_inserted = true;
		}

		if (pull_cord_inserted && digitalRead(pin)) {
			rlogf("Pull cord pulled");
			digitalWrite(LED_BUILTIN, LOW);
			notify_start();
			pull_cord_inserted = false;
			return true;
		}

		return false;
	}

	void notify_start() {
		started = true;
		start_time = millis();
	}

	void check_timeout() {
		if (started && (millis() - start_time) >= TIMEOUT) {
			digitalWrite(LED_BUILTIN, HIGH);
			rlogf("Stopping robot due to timeout");

			while (true);
		}
	}
}
