#ifndef AvKavLcd_h
#define AvKavLcd_h

#include "XPLPro.h"    
#include "KAV_A3XX_FCU_LCD.h"

class AvKavLcd {
public:
    enum DataRefType {
        POWER,      // autopilot powered 1 (yes) or 0 (no)
        HDG,        // Heading or Track
        AIRSPEED,   // Airspeed 
        MACH,       // Mach
        ALT,        // Altitude
        VERT_SPEED, // Vertical Speed 
        FPA,   // Flight Path Angle
        SPD_MODE,   // Speed Mode (0 for Knots, 1 for Mach)
        NAV_MODE    // Navigation Mode (0 for HDG/VS, 1 for TRK/FPA)
    };

    AvKavLcd(uint8_t CS, uint8_t CLK, uint8_t DATA, XPLPro& xpl)
    : _lcd(CS, CLK, DATA), _xpl(xpl) {
        int numElements = sizeof(_refHandles) / sizeof(_refHandles[0]);
        for (int i = 0; i < numElements; ++i) {
            _refHandles[i] = XPL_HANDLE_INVALID;
        }
    }

    void begin() {
        _lcd.begin();
        _lcd.clearLCD();
    }
    void end(){
        _lcd.clearLCD();
    }

    void setDataRef(XPString_t* dataref, DataRefType type, int updateInterval = 100, float resolution = 1.0, int index = -1) {
        int handle = _xpl.registerDataRef(dataref);
        if (handle != XPL_HANDLE_INVALID) {
            _refHandles[type] = handle;
            if (index != -1){
                _xpl.requestUpdates(handle, updateInterval, resolution, index);
            }else{
                _xpl.requestUpdates(handle, updateInterval, resolution);
            }
        } else {
            _xpl.sendDebugMessage("Failed to register DataRef");
        }
    }

    void refresh_display(){
        display_speed_mach();

    }

    void display_speed_mach(){
        if (_machMode == 1){
            _lcd.setSpeedLabel(0);
            _lcd.setMachLabel(1);
            _lcd.setMachMode(_mach_value);
        }else{
            _lcd.setMachLabel(0);
            _lcd.setSpeedLabel(1);
            _lcd.showSpeedValue(_mach_value);
        }
    } 
    void display_nav_hdg(){
        if (_navMode == 1){
            _lcd.setTrackMode();
        }else{
            _lcd.setHeadingMode();
        }
    }

    void update(inStruct *inData) {
        if (inData->handle == _refHandles[POWER]) {
            _power_on = static_cast<int>(inData->inLong) > 0;  //  power received
            if (_power_on){
                _lcd.setStartLabels();
                refresh_display();
            }else{
                _lcd.clearLCD();
            }
        }else if (inData->handle == _refHandles[SPD_MODE]) {
            _machMode = static_cast<int>(inData->inLong);  // Speed/Mach mode 1: MACH, 0: SPD
            if (_power_on){
                display_speed_mach();
            }
        } else if (inData->handle == _refHandles[NAV_MODE]) {
            _navMode = static_cast<int>(inData->inLong);  // Navigation mode
            if (_power_on){
                display_nav_hdg();
            }

        } else if (_machMode == 1 && inData->handle == _refHandles[MACH]) {// MACH
            _mach_value = static_cast<uint16_t>(inData->inLong);
            if (_power_on) _lcd.setMachMode(_mach_value);
        } else if (_machMode == 0 && inData->handle == _refHandles[AIRSPEED]) {// MACH
            _speed_value = static_cast<uint16_t>(inData->inLong);
            if (_power_on) _lcd.showSpeedValue(_mach_value);
        } else if (inData->handle == _refHandles[HDG]) {
             if (_power_on) _lcd.showHeadingValue(static_cast<uint16_t>(inData->inLong));
        } else if (inData->handle == _refHandles[ALT]) {
            if (_power_on) _lcd.showAltitudeValue(static_cast<int16_t>(inData->inLong));
        }
    }

private:

    KAV_A3XX_FCU_LCD _lcd;
    XPLPro& _xpl;
    int _refHandles[7];  // Dataref handles
    int _machMode;       // 0 for knots, 1 for Mach
    int _navMode;         // 0 for HDG/VS, 1 for TRK/FPA
    int _mach_value;
    int _speed_value;
    boolean _power_on = false;
};

#endif // AvKavLcd_h
