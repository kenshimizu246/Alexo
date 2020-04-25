
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

#include "njm2670d2.hpp"

using namespace rapidjson;



int main() {
  std::cout << "test" << std::endl;

unsigned int p = 21;

  njm2670d2 drive{21,22,23,24};

//  wiringPiSetupGpio();
  wiringPiSetup();

  pinMode(p, OUTPUT);

  digitalWrite(p, HIGH);

//  drive.initMode();

//  drive.forward();


  return 0;
}
