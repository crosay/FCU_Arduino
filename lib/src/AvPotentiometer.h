#ifndef AvPotentiometer_h
#define AvPotentiometer_h

#include <Arduino.h>
#include <XPLPro.h>

class AvPotentiometer {
public:
    // Constructor without immediate dataref registration
    AvPotentiometer(uint8_t pin, XPLPro& xpl, float a = 0.0, float b = 1.0, float c = 0.0)
    : _pin(pin), _xplPro(xpl), _coeffA(a), _coeffB(b), _coeffC(c), _dataRefHandle(-1), _lastValue(-1.0) {
        pinMode(_pin, INPUT);
    }

    // Method to set the dataref name and register it
    void setDataRef(XPString_t *datarefName, int index=-1) {
        _index = index;
        _dataRefName = datarefName;
        _dataRefHandle = _xplPro.registerDataRef(datarefName);
        if (_dataRefHandle == -1) {
            _xplPro.sendDebugMessage("Failed to register DataRef");
        }
    }

    void update() {
        int raw = readRaw();
        float transformed = applyTransformation(raw);
        if (fabs(transformed - _lastValue) > _epsilon) {
            if (_dataRefHandle != -1) {
                if (_index == -1){
                    _xplPro.datarefWrite(_dataRefHandle, transformed);
                }else{
                    _xplPro.datarefWrite(_dataRefHandle, transformed, _index);
                }
                _lastValue = transformed;
            }
        }
    }

    void setTransformationCoefficients(float a, float b, float c) {
        _coeffA = a;
        _coeffB = b;
        _coeffC = c;
    }

private:
    int _index;
    uint8_t _pin;
    XPLPro& _xplPro;
    XPString_t* _dataRefName; // Name of the dataref
    int _dataRefHandle;       // Handle for the dataref
    float _coeffA, _coeffB, _coeffC;
    float _lastValue;
    const float _epsilon = 0.01;

    int readRaw() {
        return analogRead(_pin);
    }

    float applyTransformation(int raw) {
        return _coeffA * raw * raw + _coeffB * raw + _coeffC;
    }
};

#endif
