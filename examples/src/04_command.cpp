
#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>

#include "groupCommand.hpp"
#include "groupFeedback.hpp"
#include "lookup.hpp"

using namespace Fourier;

// for test
#ifdef WIN32
#include <WS2tcpip.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#define DllExport __declspec(dllexport)
#else
#include <arpa/inet.h>
#include <sys/time.h>
#include <unistd.h>
#endif

#include <time.h>
const timeval intervalTimeStart() {
  timeval tv_last;
  gettimeofday(&tv_last, NULL);
  return tv_last;
}

double intervalTimeEnd(const timeval &tv_last, const float &frequency) {
  float interval_time;
  if (frequency == 0) {
    interval_time = 0;
  } else {
    interval_time = 1.0 / frequency;
  }
  long double delay_time = interval_time * 1000000;
  long double time = 0;
  timeval now_time;
  while (true) {
    gettimeofday(&now_time, NULL);
    time = ((now_time.tv_sec - tv_last.tv_sec) * 1000000 +
            (now_time.tv_usec - tv_last.tv_usec));
    if (time >= delay_time) {
      break;
    }
    std::this_thread::sleep_for(std::chrono::microseconds(1));
  }
  return time / 1000;
}

#define StartTimeChrono(funName)                                \
  std::chrono::microseconds ms##funName =                       \
      std::chrono::duration_cast<std::chrono::microseconds>(    \
          std::chrono::system_clock::now().time_since_epoch()); \
  long start##funName = ms##funName.count();

#define EndTimeChrono(funName)                                                \
  ms##funName = std::chrono::duration_cast<std::chrono::microseconds>(        \
      std::chrono::system_clock::now().time_since_epoch());                   \
  long end##funName = ms##funName.count();                                    \
  double ms_time = (end##funName - start##funName) / 1000.0;                  \
  std::cout << "The function " << #funName << " runs for " << ms_time << "ms" \
            << std::endl;
// for test

int main() {
  std::string str("192.168.2.255");
  // After construction,start the background thread lookup actuator
  Lookup lookup(&str);
  // Wait 1 seconds for the module list to populate, and then print out its
  // contents
  std::this_thread::sleep_for(std::chrono::seconds(1));
  lookup.setLookupFrequencyHz(0);

  auto group = lookup.getGroupFromFamily("Default");

  if (!group) {
    std::cout << "Group not found! Check that the family and name of a module "
                 "on the network"
              << std::endl
              << "matches what is given in the source file." << std::endl;
    return -1;
  }

  //// Open-loop controller (position)

  // The command struct has fields for various commands and settings; for the
  // actuator, we will primarily use position, velocity, and current.
  //
  // Fields that are not filled in will be ignored when sending.
  GroupCommand group_command(group->size());
  // GroupCommand uses Eigen types for data interchange
  std::vector<float> positions = {0, 0};
  std::vector<float> velocitys = {50000};
  std::vector<float> currents = {1, 1};
  std::vector<float> currents2 = {5, 5};
  // Allocate feedback
  GroupFeedback group_feedback(group->size());

  std::vector<float> enable_status(group->size(),
                                   std::numeric_limits<float>::quiet_NaN());
  for (int i = 0; i < group->size(); ++i) {
    enable_status[i] = 1;
  }
  group_command.enable(enable_status);
  group->sendCommand(group_command);

  //   for (int i = 0; i < 2000; i++) {
  //     // Even though we don't use the feedback, getting feedback conveniently
  //     // limits the loop rate to the feedback frequency
  //     // group->getNextFeedback(group_feedback);

  //     // Update position set point
  //     positions[0] = sin(i * 0.004 * M_PI) * 2000;
  //     positions[1] = sin(i * 0.004 * M_PI) * 2000;
  //     group_command.setPosition(positions);
  //     std::cout << positions[0] << std::endl;
  //     std::cout << positions[1] << std::endl;

  //     group->sendCommand(group_command);
  //     std::this_thread::sleep_for(std::chrono::milliseconds(10));
  //   }

  std::vector<float> zero = {0};
  // group_command.setVelocityRamp(zero);
  // group->sendCommand(group_command);

  group_command.setInputVelocityPt(velocitys);
  // StartTimeChrono(setVelocity);
  group->sendCommand(group_command);
  // EndTimeChrono(setVelocity);
  std::this_thread::sleep_for(std::chrono::milliseconds(5000));

  StartTimeChrono(zero);
  group_command.setInputVelocityPt(zero);
  group->sendCommand(group_command);
  EndTimeChrono(zero);

  // group_command.setCurrent(currents);
  // group->sendCommand(group_command);
  // std::this_thread::sleep_for(std::chrono::milliseconds(3000));
  // group_command.setCurrent(currents2);
  // group->sendCommand(group_command);
  // std::this_thread::sleep_for(std::chrono::milliseconds(3000));
  // group_command.setCurrent(zero);
  // group->sendCommand(group_command);

  std::this_thread::sleep_for(std::chrono::milliseconds(5));
  for (int i = 0; i < group->size(); ++i) {
    enable_status[i] = 0;
  }
  group_command.enable(enable_status);
  group->sendCommand(group_command);

  return 0;
}
