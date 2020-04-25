
#include <iostream>
#include <sstream>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/filewritestream.h>

//#include <time.h>

#include <ctime>
#include <iostream>
#include <locale>
  
#include <wiringPi.h>

#include "pca9685.hpp"

using namespace rapidjson;



int main() {
  std::cout << "test" << std::endl;

unsigned int p = 21;

  pca9685 servo;

//  wiringPiSetupGpio();
  wiringPiSetup();

  servo.PwmSetup(0x40, 50);
  servo.PwmWrite(0, 0, 350);



  return 0;
}
