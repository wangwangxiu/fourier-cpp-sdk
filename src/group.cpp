#include "group.hpp"
#include "groupCommand.hpp"
#include "groupFeedback.hpp"

namespace Amber
{

void callbackWrapper(AmberGroupFeedbackPtr group_feedback, void* user_data) {
    reinterpret_cast<Group*>(user_data)->callAttachedHandlers(group_feedback);
}

void Group::callAttachedHandlers(AmberGroupFeedbackPtr group_feedback) {
    // Wrap this:
    GroupFeedback wrapped_fbk(group_feedback);
    // Call handlers:
    std::lock_guard<std::mutex> lock_guard(handler_lock_);
    for (unsigned int i = 0; i < handlers_.size(); i++) {
        GroupFeedbackHandler handler = handlers_[i];
        try {
            handler(wrapped_fbk);
        }
        catch (...) {
        }
    }
}

Group::Group(
    AmberGroupPtr group,
    float initial_feedback_frequency,
    int32_t initial_command_lifetime)
    : internal_(group), number_of_modules_(amberGroupGetSize(internal_))
{
    if (initial_feedback_frequency != 0)
        amberGroupSetFeedbackFrequencyHz(internal_, initial_feedback_frequency);
    if (initial_command_lifetime != 0)
        amberGroupSetCommandLifetime(internal_, initial_command_lifetime);
}

Group::~Group() noexcept
{
    if (internal_ != nullptr) {
        amberGroupRelease(internal_);
    }
}

int Group::size() { return number_of_modules_; }

bool Group::setCommandLifetimeMs(int32_t ms) {
    return (amberGroupSetCommandLifetime(internal_, ms) == AmberStatusSuccess);
}

bool Group::sendCommand(const GroupCommand& group_command) {
    return (amberGroupSendCommand(internal_, group_command.internal_) == AmberStatusSuccess);
}

bool Group::sendFeedbackRequest(AmberFeedbackCode feedbackCode) { 
    return (amberGroupSendFeedbackRequest(internal_, feedbackCode) == AmberStatusSuccess); 
}

bool Group::getNextFeedback(GroupFeedback& feedback, int32_t timeout_ms) {
    return (amberGroupGetNextFeedback(internal_, feedback.internal_, timeout_ms) == AmberStatusSuccess);
}

bool Group::setFeedbackFrequencyHz(float frequency) {
    return (amberGroupSetFeedbackFrequencyHz(internal_, frequency) == AmberStatusSuccess);
}

float Group::getFeedbackFrequencyHz() 
{ 
    return amberGroupGetFeedbackFrequencyHz(internal_); 
}

void Group::addFeedbackHandler(GroupFeedbackHandler handler) 
{
    std::lock_guard<std::mutex> lock_guard(handler_lock_);
    handlers_.push_back(handler);
    if (handlers_.size() == 1) // (i.e., this was the first one)
        amberGroupRegisterFeedbackHandler(internal_, callbackWrapper, reinterpret_cast<void*>(this));
}

void Group::clearFeedbackHandlers() {
    std::lock_guard<std::mutex> lock_guard(handler_lock_);
    amberGroupClearFeedbackHandlers(internal_);
    handlers_.clear();
}

AmberFeedbackErrorPtr Group::getError(int idx)
{
    return amberGroupFeedbackError(internal_, idx);
}
}
