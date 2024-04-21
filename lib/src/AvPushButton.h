#include <XPLPro.h>

class AvPushButton {
public:
    AvPushButton(int pin, XPLPro& xplPro) : _pin(pin), _xplPro(xplPro), _commandHandle(-1), _lastState(HIGH) {
        pinMode(_pin, INPUT_PULLUP);
        _lastDebounceTime = 0;
        _buttonState = HIGH;
    }

    void updateCommandHandle(int newCommandHandle) {
        _commandHandle = newCommandHandle;
    }

    void update() {
        int currentState = digitalRead(_pin);
        if (currentState != _lastState) {
            _lastDebounceTime = millis();  // reset debounce timer
        }

        if ((millis() - _lastDebounceTime) > debounceDelay) {
            if (currentState != _buttonState) {
                _buttonState = currentState;
                if (_buttonState == LOW && _commandHandle != -1) {
                    _xplPro.commandTrigger(_commandHandle);
                    _xplPro.sendDebugMessage("Command triggered via push button.");
                }
            }
        }
        _lastState = currentState;
    }

private:
    int _pin;
    int _commandHandle;
    XPLPro& _xplPro;
    int _lastState;
    int _buttonState;
    unsigned long _lastDebounceTime;
    static const long debounceDelay = 50; // Debounce time in milliseconds
};
