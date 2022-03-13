#include "groupFeedback.hpp"

namespace Amber {

GroupFeedback::GroupFeedback(size_t number_of_modules)
    : internal_(amberGroupFeedbackCreate(number_of_modules)),
      manage_pointer_lifetime_(true), number_of_modules_(number_of_modules) {
  for (size_t i = 0; i < number_of_modules_; i++)
    feedbacks_.emplace_back(amberGroupFeedbackGetModuleFeedback(internal_, i));
}

GroupFeedback::GroupFeedback(AmberGroupFeedbackPtr group_feedback)
    : internal_(group_feedback), manage_pointer_lifetime_(false),
      number_of_modules_(amberGroupFeedbackGetSize(group_feedback)) {
  for (size_t i = 0; i < number_of_modules_; i++)
    feedbacks_.emplace_back(amberGroupFeedbackGetModuleFeedback(internal_, i));
}

GroupFeedback::~GroupFeedback() noexcept {
  if (manage_pointer_lifetime_ && internal_ != nullptr)
    amberGroupFeedbackRelease(internal_);
}

size_t GroupFeedback::size() const { return number_of_modules_; }

const AmberFeedbackPtr &GroupFeedback::operator[](size_t index) const {
  return feedbacks_[index];
}

} // namespace Amber
