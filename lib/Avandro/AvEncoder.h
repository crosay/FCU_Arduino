
#ifndef AvEncoder_h_
#define AvEncoder_h_
#include <XPLPro.h>
#include "direct_pin_read.h"

class AvEncoder {
public:
    AvEncoder(int pinA, int pinB, XPLPro& xplPro, bool useInternalPullUp = false, int stepsPerNotch = 1)
    : _pinA(pinA), _pinB(pinB), _xplPro(xplPro), _use_internal_pullup(useInternalPullUp), _step(stepsPerNotch), _cmdCW(-1), _cmdCCW(-1) {
    }

	void begin(){
		pinMode(_pinA, _use_internal_pullup ? INPUT_PULLUP : INPUT);
        pinMode(_pinB, _use_internal_pullup ? INPUT_PULLUP : INPUT);
        if (_use_internal_pullup) {
            digitalWrite(_pinA, HIGH);  
            digitalWrite(_pinB, HIGH);  
	        // Allow time for any passive R-C filters to stabilize
	        delayMicroseconds(2000);
        }else{
            digitalWrite(_pinA, LOW);  
            digitalWrite(_pinB, LOW);  

		}
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
                _xplPro.commandTrigger( _use_internal_pullup? _cmdCCW : _cmdCW);
			}else{
                _xplPro.commandTrigger(_use_internal_pullup? _cmdCW : _cmdCCW);
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
	bool _use_internal_pullup = false;
	volatile IO_REG_TYPE * _pinA_register;
	volatile IO_REG_TYPE * _pinB_register;
	IO_REG_TYPE _pinA_bitmask;
	IO_REG_TYPE _pinB_bitmask;
};

#endif
