#include <Arduino.h>
#include <AvPushButton.h>
#include <AvEncoder.h>
#include <AvPotentiometer.h>
#include <AvLED.h>
#include "AvKavLcd.h"
#include <XPLPro.h>

long int startTime;
XPLPro xplPro(&Serial);

// Initialize the encoder
AvEncoder headingEncoder(2, 3, xplPro, 1);
AvPushButton pushButtonSetHDG(4, xplPro);  // HDG mode
AvEncoder obsEncoder(5, 6, xplPro, 1);
AvPushButton pushButtonSetNAV(7, xplPro);  // NAV Mode
// Initialize push buttons
AvPushButton pushButtonUp(14, xplPro);  // Flaps up
AvPushButton pushButtonDown(15, xplPro);  // Flaps down
AvPotentiometer cowlFlapControl0(A0, xplPro, 0.0, 1.0/1023.0, 0.0);
AvPotentiometer cowlFlapControl1(A0, xplPro, 0.0, 1.0/1023.0, 0.0);
AvLED greenLED(11, xplPro);
AvLED redLED(13, xplPro);
AvKavLcd lcdDisplay(8,9,10, xplPro);  //  pins for CS, CLK, DATA



void registerWithXPlane() {

    pushButtonSetHDG.updateCommandHandle(xplPro.registerCommand(F("sim/autopilot/heading")));
    obsEncoder.updateCommandHandles(xplPro.registerCommand(F("sim/radios/obs_HSI_up")), xplPro.registerCommand(F("sim/radios/obs_HSI_down")));
    pushButtonSetNAV.updateCommandHandle(xplPro.registerCommand(F("sim/autopilot/NAV")));
    pushButtonUp.updateCommandHandle(xplPro.registerCommand(F("sim/flight_controls/flaps_up")));
    pushButtonUp.updateCommandHandle(xplPro.registerCommand(F("sim/flight_controls/flaps_up")));
    pushButtonDown.updateCommandHandle(xplPro.registerCommand(F("sim/flight_controls/flaps_down")));
    cowlFlapControl0.setDataRef(F("sim/cockpit2/engine/actuators/cowl_flap_ratio"), 0);
    cowlFlapControl1.setDataRef(F("sim/cockpit2/engine/actuators/cowl_flap_ratio"), 1);
    greenLED.setDataRef(F("sim/flightmodel2/gear/deploy_ratio"), 100, 1, 0); // Nose gear
    redLED.setDataRef(F("sim/cockpit2/autopilot/autopilot_has_power"), 100, 0); // Gears are in transit
    lcdDisplay.setDataRef(F("sim/cockpit2/autopilot/autopilot_has_power"), AvKavLcd::POWER, 100, 1.0);
    lcdDisplay.setDataRef(F("avandro/fcu/is_mach"), AvKavLcd::SPD_MODE, 100, 1.0);
    lcdDisplay.setDataRef(F("avandro/fcu/is_trkfpa"), AvKavLcd::NAV_MODE, 100, 1.0);
    lcdDisplay.setDataRef(F("avandro/fcu/hdg"), AvKavLcd::HDG, 100, 1.0);
    lcdDisplay.setDataRef(F("avandro/fcu/spd"), AvKavLcd::AIRSPEED, 100, 1);
    lcdDisplay.setDataRef(F("avandro/fcu/mach"), AvKavLcd::MACH, 100, 0.01);
    lcdDisplay.setDataRef(F("avandro/fcu/alt"), AvKavLcd::ALT, 100, 10.0);

}

void onStop() {
    xplPro.sendDebugMessage("X-Plane has stopped or changed the aircraft.");
}

void inboundHandler(inStruct *inData) {
    greenLED.update(inData);  // Update LED state based on the inbound data
    redLED.update(inData);  // Update LED state based on the inbound data
    lcdDisplay.update(inData);  //update the KavFCU 
}

void setup() {
    Serial.begin(XPL_BAUDRATE);
    while (!xplPro.connectionStatus()) ;  // Wait for serial port to connect

    xplPro.begin("Av_FCU", registerWithXPlane, onStop, inboundHandler);
    lcdDisplay.begin();

}

void loop() {
    xplPro.xloop();
    // Update button states
    pushButtonUp.update();
    pushButtonDown.update();
    headingEncoder.update();
    obsEncoder.update();
    cowlFlapControl0.update();
    cowlFlapControl1.update();
    pushButtonSetHDG.update();
    pushButtonSetNAV.update();
    if (millis() - startTime > 100) startTime = millis();   else return; 
}
