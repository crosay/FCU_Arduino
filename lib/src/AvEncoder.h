
#ifndef AvEncoder_h_
#define AvEncoder_h_
#include <XPLPro.h>
#include "direct_pin_read.h"

class AvEncoder {
public:
    AvEncoder(int pinA, int pinB, XPLPro& xplPro, bool useInternalPullUp = true, int stepsPerNotch = 1)
    : _pinA(pinA), _pinB(pinB), _xplPro(xplPro), _step(stepsPerNotch), _cmdCW(-1), _cmdCCW(-1) {
        pinMode(_pinA, useInternalPullUp ? INPUT_PULLUP : INPUT);
        pinMode(_pinB, useInternalPullUp ? INPUT_PULLUP : INPUT);
        if (useInternalPullUp) {
            digitalWrite(_pinA, HIGH);  // Ensure pull-up is enabled (redundant in most cases)
            digitalWrite(_pinB, HIGH);  // Ensure pull-up is enabled (redundant in most cases)
	        // Allow time for any passive R-C filters to stabilize
	        delayMicroseconds(2000);
        }
		_inverted = useInternalPullUp;
        initializeState();
    }

    void initializeState() {
        _pinA_register = PIN_TO_BASEREG(_pinA);
        _pinA_bitmask = PIN_TO_BITMASK(_pinA);
        _pinB_register = PIN_TO_BASEREG(_pinB);
        _pinB_bitmask = PIN_TO_BITMASK(_pinB);
        uint8_t s = 0;
        if (DIRECT_PIN_READ(_pinA_register, _pinA_bitmask)) s |= 1;
        if (DIRECT_PIN_READ(_pinB_register, _pinB_bitmask)) s |= 2;
        _lastState = s;
    }

    void update() {
		uint8_t p1val = DIRECT_PIN_READ(_pinA_register, _pinA_bitmask);
		uint8_t p2val = DIRECT_PIN_READ(_pinB_register, _pinB_bitmask);
		uint8_t state = _lastState & 3;
		if (p1val) state |= 4;
		if (p2val) state |= 8;
		_lastState = (state >> 2);
		// State transition handling based on encoder outputs
		switch (state) {
			case 1: case 3: case 7: case 8: case 12: case 14:
				_mem++;  // Increment memory on CW rotation states
				break;
			case 2: case 4: case 6: case 9: case 11: case 13:
				_mem--;  // Decrement memory on CCW rotation states
				break;
		}
		if (_step - abs(_mem) <= 0){
			int8_t current = _mem > 0 ? 1 : -1;
			_mem = 0;
			if (current == 1){
                _xplPro.commandTrigger(_inverted? _cmdCCW : _cmdCW);
                _xplPro.sendDebugMessage("Encoder CW rotation detected.");
			}else{
                _xplPro.commandTrigger(_inverted? _cmdCW : _cmdCCW);
                _xplPro.sendDebugMessage("Encoder CCW rotation detected.");
			}

		}

    }

    void updateCommandHandles(int newCmdCW, int newCmdCCW) {
        _cmdCW = newCmdCW;
        _cmdCCW = newCmdCCW;
    }

private:
	int8_t _pinA;
	int8_t _pinB;    
	XPLPro& _xplPro;
	int8_t _mem;
	int8_t _step = 1;
    int _cmdCW;
    int _cmdCCW;
    uint8_t _lastState;
	bool _inverted = false;
	volatile IO_REG_TYPE * _pinA_register;
	volatile IO_REG_TYPE * _pinB_register;
	IO_REG_TYPE _pinA_bitmask;
	IO_REG_TYPE _pinB_bitmask;
};

#endif
