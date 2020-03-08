
#ifndef _pca9685_hpp
#define _pca9685_hpp

#include <stdexcept>

class pca9685_error : public std::runtime_error{
public:
  pca9685_error(const char *_message)
      : std::runtime_error(_message) {}
};

class pca9685 {
public:
  void PwmSetup(const int i2cAddress/* = 0x40*/, float freq/* = 50*/);
  void PwmFreq(float freq);
  void PwmReset();
  void PwmWrite(int pin, int on, int off);
  void PwmRead(int pin, int *on, int *off);
  void PwmFullOn(int pin, int tf);
  void PwmFullOff(int pin, int tf);

private:
  bool initialized = false;
  int fd;

  int baseReq(int pin);
};

#endif



