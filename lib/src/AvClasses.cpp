#include "AvClasses.h"

// // Implementation for AvClass (abstract base)
// char* AvClass::toString() {
//   return "AvClass";
// }

// // Implementation for AvPinAble
// uint8_t AvPinAble::getPin() {
//   return _pin;
// }

// // Implementation for AvReadAble (intentionally left empty, should be overridden)
// int AvReadAble::read(bool force) {
//   return -1;
// }

// // Implementation for AvWriteAble (intentionally left empty, should be overridden)
// void AvWriteAble::set(int value) {
//   // This should set the pin value to high or low depending on `value`.
// }

// // Implementation for AvDigitalPin
// char* AvDigitalPin::toString() {
//   static char buffer[50];
//   snprintf(buffer, 50, "Digital Pin: %d, Mode: %d", _pin, _mode);
//   return buffer;
// }

// int AvReadAble::read(bool force) {
//   int currentValue = digitalRead(_pin);
//   if (_lastValue != currentValue || force) {
//     _lastValue = currentValue;
//     if (_callback != nullptr) {
//       _callback(currentValue);
//     }
//   }
//   return currentValue;
// }

// // For AvDigitalPullup, AvReadAble provides read, which is now specified.
// int AvDigitalPullup::read(bool force) {
//   int currentValue = digitalRead(_pin);
//   if (_lastValue != currentValue || force) {
//     _lastValue = currentValue;
//     callCallback(currentValue);
//   }
//   return currentValue;
// }

// // AvLed specific methods can be added here, if there are any specifics other than what's inherited
// void AvLed::set(int value) {
//   digitalWrite(_pin, value);
//   _oldValue = value;
// }

// // AvButton does not need any specific implementations beyond what is inherited from AvDigitalPullup.
