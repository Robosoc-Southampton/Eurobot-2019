
/* Connection test
 *     requires connection for meaningful output
 *
 * Tests the bluetooth/serial connection from the Pi to the Arduino
 * Should log "Connected" and "Connected (flash)" when connected
 * Should log "Forward" or "Backward" for 'forward' messages
 * Should log "Turn" for 'turn' messages
 * Should log "Do" for 'do' messages
 * Should log "Other" for all other messages
 * Should echo all messages sent from the Pi as a response message,
 *   with the payload being the original message's payload
 */

#pragma once

#include <Arduino.h>
#include "../../messages.h"
