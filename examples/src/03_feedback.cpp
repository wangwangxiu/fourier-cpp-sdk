
#include "lookup.hpp"
#include "groupFeedback.hpp"
#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>

int main(int argc, char *argv[])
{
    // Try and get the requested group.
    std::shared_ptr<Amber::Group> group;
    {
        Amber::Lookup lookup;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        lookup.setLookupFrequencyHz(0);

        group = lookup.getGroupFromFamily("Default");

        if (!group)
        {
            std::cout << "No group found!" << std::endl;
            return -1;
        }
    }

    int num_modules = group->size();

    // Create a group feedback object; this will be filled in during the request.
    Amber::GroupFeedback feedback(num_modules);

    // In a loop, send requests for feedback to the group and wait for responses.
    long timeout_ms = 1000;
    float period_s = 0.25f;
    for (int i = 0; i < 2000; i++)
    {
        group->sendFeedbackRequest();
        if (group->getNextFeedback(feedback, timeout_ms))
        {
            for (size_t mod_idx = 0; mod_idx < feedback.size(); mod_idx++)
            {
                if (feedback[mod_idx]->position != std::numeric_limits<float>::quiet_NaN())
                {
                    std::cout 
                        << feedback[mod_idx]->position << "  "
                        << feedback[mod_idx]->velocity << "  "
                        << feedback[mod_idx]->current << "  "
                        << feedback[mod_idx]->voltage << "  "
                        << feedback[mod_idx]->motor_temp_m1 << "  "
                        << feedback[mod_idx]->inverter_temp_m1 << "  ";
                }
                std::cout << std::endl;
            }
        }
        else
            std::cout << "Received no feedback from group!" << std::endl;

        // std::this_thread::sleep_for(std::chrono::milliseconds((long int) (period_s * 1000)));
    }

    // NOTE: destructors automatically clean up remaining objects
    return 0;
}
