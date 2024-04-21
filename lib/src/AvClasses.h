/*
  project avandro 
  library unoAvandro 
 created 2019 c.rosay for avandro
 */
 
#ifndef AvClasses_h
#define AvClasses_h
// #include "Arduino.h"

// class AvClass{
//   public:
//     virtual int read(bool force){return -1;};
//     virtual void loop(){};
//     virtual char* toString();
// };


// // Define a mixin class for pin setup
// class AvPinAble {
// public:
//     AvPinAble(int pin):_pin(pin) {};
//     uint8_t getPin(){return _pin;};
// protected:
//   uint8_t _pin;
// };

// // Define a mixin class for callback functionality
// class AvCallbackAble {
// public:
//     AvCallbackAble(void (*callback)(int)) : _callback(callback) {};

// protected:
//     void callCallback(int value) {
//         if (_callback != nullptr) {
//             _callback(value);
//         }
//     }
// private:
//     void (*_callback)(int);
// };


// /**a class to manipulate single digital pin (LEDs, switches)*/
// class AvDigitalPin : public AvClass, public AvPinAble{
// public:
//     /**creator of the class
//      * parameters:
//      * int pin  : the digital pin on the arduino board 
//      * int mode : INPUT / OUTPUT / INPUT_PULLUP
//     */
//     AvDigitalPin(int pin, int mode) : AvClass(), AvPinAble(pin), _mode(mode) {};

//     virtual char* toString();

// private:
//     int _mode;
//     int _lastValue;
//     bool _isPWM = false;
// };


// // Define a mixin class for readable classes
// class AvReadAble : public AvDigitalPin {
// public:
//     /**
//      * reads the value of the pin and if the value changed, call the callback
//      * passed as parameter in the creator
//     */
//     virtual int read(bool force){};
//     virtual void loop() {};
// protected:
//   int _lastValue = -1;
// };


// class AvWriteAble  : public AvDigitalPin{
// public:
//     /**set the value of the digital pin*/
//     virtual void set(int value);
// private:
//   int _oldValue;
// };

// /**a class to manipulate single analog pin */
// class AvAnalogPin: public AvClass, public AvCallbackAble, public AvPinAble{
// public:
//    /**creator of the class
//     * int pin : the digital pin on the arduino board 
//    */
//     AvAnalogPin(int pin, void (*callback)(int)) : AvClass(), AvCallbackAble(callback), AvPinAble(pin) {};
// };

// class AvDigitalPullup: public AvCallbackAble, AvReadAble{
// public:
//   AvDigitalPullup(int pin, void (*callback)(int)) : AvDigitalPin(pin, INPUT_PULLUP), AvCallbackAble(callback){};
// };

// /**
//  * a class definition for single LED*/
// class AvLed: public AvDigitalPin, public AvWriteAble{
// public:
//   AvLed(int pin):AvDigitalPin(pin, OUTPUT){};
// };

// /**
//  * a class definition for single buton or switch*/
// class AvButton: public AvDigitalPullup{
// public:
//   AvButton(int pin, void (*callback)(int)):AvDigitalPullup(pin, callback){
//            digitalWrite(_pin, HIGH);
//   };
// };

#endif
