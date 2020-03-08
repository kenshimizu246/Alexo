
#include <iostream>
#include <memory>

#include "pca9685.hpp"
#include "njm2670d2.hpp"

#ifndef _command_hpp
#define _command_hpp


enum drive_command_type {
  FORWARD,
  BACKWARD,
  RIGHT,
  LEFT,
  STOP,
  AUTO
};

class command {
public:
  virtual void doCommand() = 0;
};

class drive_command : public command {
public:
  drive_command(njm2670d2& m, drive_command_type s): motor(m), type(s) {}
  drive_command(njm2670d2& m, std::string& str): motor(m) {
    const std::string kFORWARD = "FORWARD";
    const std::string kBACKWARD = "BACKWARD";
    const std::string kRIGHT = "RIGHT";
    const std::string kLEFT = "LEFT";
    const std::string kSTOP = "STOP";
    const std::string kAUTO = "AUTO";

    if (str == kFORWARD) {
      type = drive_command_type::FORWARD;
    } else if (str == kBACKWARD) {
      type = drive_command_type::BACKWARD;
    } else if (str == kRIGHT) {
      type = drive_command_type::RIGHT;
    } else if (str == kLEFT) {
      type = drive_command_type::LEFT;
    } else if (str == kSTOP) {
      type = drive_command_type::STOP;
    } else if (str == kAUTO) {
      type = drive_command_type::AUTO;
    } else {
      std::stringstream ss;
      ss << "No supported command Error : " << str;
      //throw message_handler_error(ss.str().c_str());
    }
  }

  drive_command_type getType(){ return type; }
  void doCommand(){
    if (getType() == drive_command_type::FORWARD){
      std::cout << "forward selected!" << std::endl;
      motor.forward();
    }else if (getType() == drive_command_type::BACKWARD){
      std::cout << "backward selected!" << std::endl;
      motor.backward();
    }else if (getType() == drive_command_type::RIGHT){
      std::cout << "right selected!" << std::endl;
      motor.right();
    }else if (getType() == drive_command_type::LEFT){
      std::cout << "left selected!" << std::endl;
      motor.left();
    }else if (getType() == drive_command_type::STOP){
      std::cout << "stop selected!" << std::endl;
      motor.stop();
    }else if (getType() == drive_command_type::AUTO){
      std::cout << "else selected!" << std::endl;
    }
  }
private:
  njm2670d2& motor;
  drive_command_type type;
};

class servo_command : public command {
public:
  servo_command(pca9685& s, unsigned int id, int value): servo(s), id(id), value(value) {}
  unsigned int getId() { return id;}
  unsigned int getValue() {return value; }
  void doCommand(){
    servo.PwmWrite(id, 0, value);
  }
private:
  pca9685& servo;
  unsigned int id;
  unsigned int value;
};

class command_factory {
public:
  command_factory(pca9685& s, njm2670d2& m): servo(s), motor(m) {}
  void set(pca9685& s){
    servo = s;
  }
  void set(njm2670d2& m){
    motor = m;
  }
  std::shared_ptr<command> createDriveCommand(std::string& s){
    return std::shared_ptr<command>(new drive_command(motor, s));
  }
  std::shared_ptr<command> createDriveCommand(drive_command_type s){
    return std::shared_ptr<command>(new drive_command(motor, s));
  }
  std::shared_ptr<command> createServoCommand(int id, unsigned int value){
    return std::shared_ptr<command>(new servo_command(servo, id, value));
  }
private:
  pca9685& servo;
  njm2670d2& motor;
};

#endif

