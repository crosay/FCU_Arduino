

/*
 * 
 * FCI with XPLPro
 */

#include <arduino.h>
#include <XPLPro.h>              //  include file for the X-plane direct interface 
#include <AvKavLcd.h>
XPLPro XP(&Serial);      // create an instance of it


long int startTime;

int drefParkingBrake;   // this stores a handle to the parking brake dataref 
AvKavLcd fcu(8, 9, 10, XP);
/*
 * callback function we specified that will be called any time our requested data is sent to us.
 * handle is the handle to the dataref.  The following variables within the plugin are how we receive the data:
 * 
 * inStruct *inData is a pointer to a structure that contains information about the incoming dataref. 
 * 
 *  int inData->handle          The handle of the incoming dataref
 *  int inData->element         If the dataref is an array style dataref, this is the element of the array
 *  long inData->inLong         long value for datarefs that are long values
 *  float inData->inFloat       float value for datarefs that are float values
 */
 void xplInboundHandler(inStruct *inData){
  fcu.update(inData);  // Update FCU
}

/*
 *  callback for when the plugin is ready to receive dataref and command bindings requests
 *  It could be called multiple times if for instance xplane is restarted or loads a different aircraft
 */
void xplRegister(){
  fcu.setDataRef(F("sim/cockpit2/autopilot/autopilot_has_power"), AvKavLcd::POWER, 100, 0.0);
  fcu.setDataRef(F("avandro/fcu/is_mach"), AvKavLcd::SPD_MODE, 100, 1.0);
  fcu.setDataRef(F("avandro/fcu/is_trkfpa"), AvKavLcd::NAV_MODE, 100, 1.0);
  fcu.setDataRef(F("avandro/fcu/hdg"), AvKavLcd::HDG, 100, 1.0);
  fcu.setDataRef(F("avandro/fcu/spd"), AvKavLcd::AIRSPEED, 100, 1);
  fcu.setDataRef(F("avandro/fcu/mach"), AvKavLcd::MACH, 100, 0.01);
  fcu.setDataRef(F("avandro/fcu/alt"), AvKavLcd::ALT, 100, 10.0);
}

void xplShutdown(){
  // turn off the FCU.
  fcu.end();
}
void setup() {
  XP.begin("FCU", &xplRegister, &xplShutdown, &xplInboundHandler);
  fcu.begin();
}


void loop() {
 
  XP.xloop();  //  needs to run every cycle.  

/************************************************************************************************************************************
 * everything after the next line will only occur every 100ms.  You can also utilize other time values.
 * This helps maintain serial data flow and also helps with switch debounce.
 * do NOT add delays anywhere in the loop cycle, it will interfere with the reception of serial data from xplane and the plugin.
 ************************************************************************************************************************************
*/

  if (millis() - startTime > 100) startTime = millis();   else return;          

}


