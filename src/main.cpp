#include <XPLPro.h>
#include <AvPushButton.h>
#include <AvEncoder.h>
#include <AvPotentiometer.h>
#include <AvLED.h>
#include <Arduino.h>
#include "AvKavLcd.h"

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
AvLED gearDownLED(11, xplPro);
AvLED gearOpLED(12, xplPro);
AvKavLcd lcdDisplay(8,9,10, xplPro);  //  pins for CS, CLK, DATA



void registerWithXPlane() {
    headingEncoder.updateCommandHandles(xplPro.registerCommand(F("sim/autopilot/heading_up")), xplPro.registerCommand(F("sim/autopilot/heading_down")));
    pushButtonSetHDG.updateCommandHandle(xplPro.registerCommand(F("sim/autopilot/heading")));
    obsEncoder.updateCommandHandles(xplPro.registerCommand(F("sim/radios/obs_HSI_up")), xplPro.registerCommand(F("sim/radios/obs_HSI_down")));
    pushButtonSetNAV.updateCommandHandle(xplPro.registerCommand(F("sim/autopilot/NAV")));
    pushButtonUp.updateCommandHandle(xplPro.registerCommand(F("sim/flight_controls/flaps_up")));
    pushButtonUp.updateCommandHandle(xplPro.registerCommand(F("sim/flight_controls/flaps_up")));
    pushButtonDown.updateCommandHandle(xplPro.registerCommand(F("sim/flight_controls/flaps_down")));
    cowlFlapControl0.setDataRef(F("sim/cockpit2/engine/actuators/cowl_flap_ratio"), 0);
    cowlFlapControl1.setDataRef(F("sim/cockpit2/engine/actuators/cowl_flap_ratio"), 1);
    gearDownLED.setDataRef(F("sim/flightmodel2/gear/deploy_ratio"), 100, 1, 0); // Nose gear
    gearOpLED.setDataRef(F("laminar/b58/annun/gear_in_transit"), 100, 0); // Gears are in transit
    lcdDisplay.setDataRef(F("sim/cockpit/autopilot/heading"), AvKavLcd::HDG, 100, 1.0);
    lcdDisplay.setDataRef(F("sim/cockpit2/autopilot/airspeed_dial_kts_mach"), AvKavLcd::AIRSPEED, 100, 0.01);
    lcdDisplay.setDataRef(F("sim/cockpit2/autopilot/altitude_dial_ft"), AvKavLcd::ALT, 100, 10.0);
    lcdDisplay.setDataRef(F("sim/cockpit2/autopilot/airspeed_dial_kts_mach"), AvKavLcd::MACH, 100, 0.01);
    lcdDisplay.setDataRef(F("sim/cockpit2/autopilot/vvi_dial_fpm"), AvKavLcd::VERT_SPEED, 100, 10.0);
    lcdDisplay.setDataRef(F("sim/cockpit2/autopilot/flight_director_pitch_deg"), AvKavLcd::FPA, 100, 0.1);
        // Register datarefs for speed and navigation modes
    lcdDisplay.setDataRef(F("sim/cockpit2/autopilot/airspeed_is_mach"), AvKavLcd::SPD_MODE);
    lcdDisplay.setDataRef(F("sim/cockpit2/autopilot/airspeed_kts_mach"), AvKavLcd::AIRSPEED);
    lcdDisplay.setDataRef(F("sim/cockpit2/autopilot/airspeed_kts_mach"), AvKavLcd::MACH);

    // Register datarefs for heading/track and vertical speed/flight path angle
    lcdDisplay.setDataRef(F("sim/cockpit2/autopilot/trk_fpa"), AvKavLcd::NAV_MODE);
    lcdDisplay.setDataRef(F("sim/cockpit2/autopilot/heading_mag"), AvKavLcd::HDG);
    lcdDisplay.setDataRef(F("sim/cockpit2/autopilot/altitude_dial_ft"), AvKavLcd::ALT);
    lcdDisplay.setDataRef(F("sim/cockpit2/autopilot/vertical_velocity_fpm"), AvKavLcd::VERT_SPEED);
    lcdDisplay.setDataRef(F("sim/cockpit2/autopilot/flight_director_pitch_deg"), AvKavLcd::FPA);

}

void onStop() {
    xplPro.sendDebugMessage("X-Plane has stopped or changed the aircraft.");
}

void inboundHandler(inStruct *inData) {
    gearDownLED.update(inData);  // Update LED state based on the inbound data
    gearOpLED.update(inData);  // Update LED state based on the inbound data
    lcdDisplay.update(inData);  //update the KavFCU 
}

void setup() {
    Serial.begin(XPL_BAUDRATE);
    while (!Serial) ;  // Wait for serial port to connect

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
}
