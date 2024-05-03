#ifndef AvLED_h
#define AvLED_h

#include <Arduino.h>
#include <XPLPro.h>

class AvLED {
public:
    AvLED(uint8_t pin, XPLPro& xpl, float triggerValue = 1.0)
    : _pin(pin), _xplPro(xpl), _triggerValue(triggerValue), _currentState(LOW) {
        pinMode(_pin, OUTPUT);
        digitalWrite(_pin, LOW); // Start with the LED off
        _dataRefHandle = XPL_HANDLE_INVALID; // Initialize handle as invalid
    }

    // Method to register the dataref and request updates
    void setDataRef(XPString_t* datarefName, int updateInterval = 100, float resolution = 1.0, int index = -1) {
        _dataRefHandle = _xplPro.registerDataRef(datarefName);
        _index= index;
        if (_dataRefHandle != XPL_HANDLE_INVALID) {
            if (index == -1){
                _xplPro.requestUpdates(_dataRefHandle, updateInterval, resolution);
            }else{
                _xplPro.requestUpdates(_dataRefHandle, updateInterval, resolution, index);
            }
        } else {
            _xplPro.sendDebugMessage("Failed to register DataRef for LED");
        }
    }

    // Update LED based on data received via inboundHandler
    void update(inStruct *inData) {
        if (inData->handle == _dataRefHandle && (_index == -1 || inData->element==_index)){
            bool newState = (inData->inFloat >= _triggerValue) ? HIGH : LOW;
            if (newState != _currentState) {
                char debugMsg[50];  // Make sure this buffer is large enough 
                sprintf(debugMsg, "pin %d set to state %d", _pin, newState);
                _xplPro.sendDebugMessage(debugMsg);
                digitalWrite(_pin, newState);
                _currentState = newState;
            }
        }
    }

private:
    uint8_t _pin;                   // GPIO pin number where the LED is connected
    XPLPro& _xplPro;                // Reference to the XPLPro instance to communicate with X-Plane
    int _dataRefHandle;             // Handle for the registered dataref
    uint8_t _index;                   // index in case the dataref is an array
    float _triggerValue;            // Threshold value to activate the LED
    bool _currentState;             // Current state of the LED (ON or OFF)
};

#endif
