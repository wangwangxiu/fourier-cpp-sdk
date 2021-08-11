
#include <iostream>
#include <chrono>
#include <thread>
#include <cmath>
#include "lookup.hpp"
#include "groupCommand.hpp"
#include "groupFeedback.hpp"

using namespace Amber;

#define M_PI 3.1415926

int main()
{
    // Get group
    Lookup lookup;
    lookup.setLookupFrequencyHz(0);

    auto group = lookup.getGroupFromFamily("Default");

    if (!group)
    {
        std::cout
            << "Group not found! Check that the family and name of a module on the network" << std::endl
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
    std::vector<float> velocitys = {20000, 20000};
    std::vector<float> currents = {1, 1};
    std::vector<float> currents2 = {5, 5};
    // Allocate feedback
    GroupFeedback group_feedback(group->size());

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


    std::vector<float> zero = {0, 0};
    for (int i = 0; i < 200; i++) {
        group_command.setVelocity(velocitys);
        group->sendCommand(group_command);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    group_command.setVelocity(zero);
    group->sendCommand(group_command);



    group_command.setCurrent(currents);
    group->sendCommand(group_command);
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    group_command.setCurrent(currents2);
    group->sendCommand(group_command);
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    group_command.setCurrent(zero);
    group->sendCommand(group_command);

    return 0;
}
