
#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>

#include "groupFeedback.hpp"
#include "lookup.hpp"

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

int main(int argc, char *argv[]) {
  // Try and get the requested group.
  std::shared_ptr<Fourier::Group> group;
  {
    std::string str("192.168.2.255");
    Fourier::Lookup lookup(&str);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    lookup.setLookupFrequencyHz(0);
    group = lookup.getGroupFromFamily("Default");
    if (!group) {
      std::cout << "No group found!" << std::endl;
      return -1;
    }
  }

  int num_modules = group->size();

  // Create a group feedback object; this will be filled in during the request.
  Fourier::GroupFeedback feedback(num_modules);

  // In a loop, send requests for feedback to the group and wait for responses.
  long timeout_ms = 1000;
  float period_s = 0.25f;
  group->setFeedbackFrequencyHz(500);
  for (int i = 0; i < 500; i++) {
    StartTimeChrono(main);
    // group->sendFeedbackRequest(FourierFeedbackCVP);
    // group->sendFeedbackRequest();
    auto t = intervalTimeStart();
    if (group->getNextFeedback(feedback, timeout_ms)) {
      for (size_t mod_idx = 0; mod_idx < feedback.size(); ++mod_idx) {
        if (feedback[mod_idx]->position !=
            std::numeric_limits<float>::quiet_NaN()) {
          std::cout << "i:" << i << "  " << feedback[mod_idx]->position << "  "
                    << feedback[mod_idx]->velocity << "  "
                    << feedback[mod_idx]->current << "  ";
        }
        std::cout << std::endl;
      }
    } else {
      std::cout << "Received no feedback from group!" << std::endl;
    }
    intervalTimeEnd(t, 500);
    EndTimeChrono(main);
  }

  group->setFeedbackFrequencyHz(0);
  std::this_thread::sleep_for(std::chrono::milliseconds(2000));

  group->setFeedbackFrequencyHz(100);
  std::this_thread::sleep_for(std::chrono::milliseconds(30));

  // group->setFeedbackFrequencyHz(0);
  // NOTE: destructors automatically clean up remaining objects
  return 0;
}
