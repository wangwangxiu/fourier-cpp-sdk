#pragma once

#include "aios.h"
#include <vector>

namespace Amber {

/**
 * @brief A list of Feedback objects that can be received from a Group of
 * modules; the size() must match the number of modules in the group.
 */
class GroupFeedback final {
public:
  /**
   * @brief Create a group feedback with the specified number of modules.
   */
  GroupFeedback(size_t number_of_modules);

  /**
   * TODO:
   */
  GroupFeedback(AmberGroupFeedbackPtr group_feedback);

  /**
   * @brief Destructor cleans up group feedback object as necessary.
   */
  ~GroupFeedback() noexcept;

  /**
   * @brief Returns the number of module feedbacks in this group feedback.
   */
  size_t size() const;

  /**
   * @brief Access the feedback for an individual module.
   */
  const AmberFeedbackPtr &operator[](size_t index) const;

public:
  /**
   * C-style group feedback object.
   * NOTE: this should not be used except by library functions!
   */
  AmberGroupFeedbackPtr internal_;

private:
  /**
   * True if this object is responsible for creating and destroying the
   * underlying C pointer; false otherwise.
   */
  const bool manage_pointer_lifetime_;

  /**
   * The number of modules in this group feedback.
   */
  const size_t number_of_modules_;

  /**
   * The list of Feedback subobjects
   */
  std::vector<AmberFeedbackPtr> feedbacks_;
};

} // namespace Amber
