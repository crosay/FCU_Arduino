#ifndef AvKavLcd_h
#define AvKavLcd_h

#include "KAV_A3XX_FCU_LCD.h"
#include "XPLPro.h"    


class AvKavLcd {
public:
    enum DataRefType {
        HDG,        // Heading or Track
        AIRSPEED,   // Airspeed 
        MACH,       // Mach
        ALT,        // Altitude
        VERT_SPEED, // Vertical Speed 
        FPA,        // Flight Path Angle
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
        _lcd.setStartLabels();
    }

    void setDataRef(XPString_t* dataref, DataRefType type, int updateInterval = 100, float resolution = 1.0, int index = -1) {
        int handle = _xpl.registerDataRef(dataref);
        if (handle != XPL_HANDLE_INVALID) {
            _refHandles[type] = handle;
            _xpl.requestUpdates(handle, updateInterval, resolution, index);
        } else {
            _xpl.sendDebugMessage("Failed to register DataRef");
        }
    }

    void update(inStruct *inData) {
        if (inData->handle == _refHandles[SPD_MODE]) {
            _speedMode = static_cast<int>(inData->inLong);  // Speed mode
        } else if (inData->handle == _refHandles[NAV_MODE]) {
            _navMode = static_cast<int>(inData->inLong);  // Navigation mode
        } else if (inData->handle == _refHandles[AIRSPEED]) {
            if (_speedMode == 0) {  // Knots
                _lcd.setSpeedMode(static_cast<uint16_t>(inData->inFloat));
            }  // Mach mode is handled with MACH dataref
        } else if (inData->handle == _refHandles[HDG]) {
            if (_navMode == 0) {  // HDG
                _lcd.showHeadingValue(static_cast<uint16_t>(inData->inFloat));
            }  // Track mode is handled with TRK dataref
        } else if (inData->handle == _refHandles[VERT_SPEED]) {
            if (_navMode == 0) {  // VS
                _lcd.showVerticalValue(static_cast<int16_t>(inData->inFloat));
            }  // FPA mode is handled with FPA dataref
        } else if (inData->handle == _refHandles[MACH] && _speedMode == 1) {
            _lcd.setMachMode(static_cast<uint16_t>(inData->inFloat * 100));  // Mach number
        } else if (inData->handle == _refHandles[FPA] && _navMode == 1) {
            _lcd.showFPAValue(static_cast<int8_t>(inData->inFloat));  // FPA
        }
    }

private:

    KAV_A3XX_FCU_LCD _lcd;
    XPLPro& _xpl;
    int _refHandles[7];  // Dataref handles
    int _speedMode;       // 0 for knots, 1 for Mach
    int _navMode;         // 0 for HDG/VS, 1 for TRK/FPA
};

#endif // AvKavLcd_h
