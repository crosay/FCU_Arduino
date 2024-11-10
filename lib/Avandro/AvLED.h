#ifndef AvLED_h
#define AvLED_h

#include <Arduino.h>
#include <XPLPro.h>

class AvLED {
public:
    // Constructor that initializes just with a pin
    AvLED(uint8_t pin)
    : _pin(pin), _xplPro(nullptr), _dataRefHandle(XPL_HANDLE_INVALID), _index(-1), _triggerValue(1.0), _currentState(LOW) {
        pinMode(_pin, OUTPUT);
        digitalWrite(_pin, LOW);  // Start with the LED off
    }

    // Constructor that initializes with a pin and XPLPro object
    AvLED(uint8_t pin, XPLPro& xpl, float triggerValue = 1.0)
    : _pin(pin), _xplPro(&xpl), _dataRefHandle(XPL_HANDLE_INVALID), _index(-1), _triggerValue(triggerValue), _currentState(LOW) {
        pinMode(_pin, OUTPUT);
        digitalWrite(_pin, LOW);  // Start with the LED off
    }

    void set(uint8_t value) {
        digitalWrite(_pin, value);
    }

    // Method to register the dataref and request updates
    void setDataRef(XPString_t* datarefName, int updateInterval = 100, float resolution = 1.0, int index = -1) {
        if (_xplPro != nullptr) {
            _dataRefHandle = _xplPro->registerDataRef(datarefName);
            _index = index;
            if (_dataRefHandle != XPL_HANDLE_INVALID) {
                if (index == -1) {
                    _xplPro->requestUpdates(_dataRefHandle, updateInterval, resolution);
                } else {
                    _xplPro->requestUpdates(_dataRefHandle, updateInterval, resolution, index);
                }
            } else {
                _xplPro->sendDebugMessage("Error: Dataref handle is invalid.");
            }
        } else {
            Serial.println("Error: XPLPro object is not initialized.");
        }
    }

    // Update LED based on data received via inboundHandler
    void update(inStruct *inData) {
        if (inData->handle == _dataRefHandle && (_index == -1 || inData->element == _index)) {
            bool newState = (inData->inFloat >= _triggerValue) ? HIGH : LOW;
            if (newState != _currentState) {
                digitalWrite(_pin, newState);
                _currentState = newState;
            }
        }
    }

private:
    uint8_t _pin;               // GPIO pin number where the LED is connected
    XPLPro* _xplPro;            // Pointer to the XPLPro instance to communicate with X-Plane
    int _dataRefHandle;         // Handle for the registered dataref
    int _index;                 // Index in case the dataref is an array
    float _triggerValue;        // Threshold value to activate the LED
    bool _currentState;         // Current state of the LED (ON or OFF)
};

#endif

