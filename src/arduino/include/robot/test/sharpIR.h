
/* MD25 ticks test
 *     requires connection for meaningful output
 * 
 * Should log "Sensor connected to A0" when connected
 * Should repeatedly log reading and voltage, depending on config
 * 
 * Change config using config-key and config-set messages:
 *   config-key 0 - sets reading output to false
 *   config-key 1 - sets reading output to true
 *   config-key 2 - sets voltage output to false
 *   config-key 3 - sets voltage output to true
 *   config-set X - sets delay between outputs to X milliseconds
 */

#pragma once

#include "../../component/SharpIR.h"
#include "../../debug.h"
#include "../../messages.h"
