#include "groupCommand.hpp"

namespace Amber {

GroupCommand::GroupCommand(size_t number_of_modules)
    : internal_(amberGroupCommandCreate(number_of_modules)),
      number_of_modules_(number_of_modules) {
  for (size_t i = 0; i < number_of_modules_; i++)
    commands_.emplace_back(amberGroupCommandGetModuleCommand(internal_, i));
}

GroupCommand::~GroupCommand() noexcept {
  if (internal_ != nullptr)
    amberGroupCommandRelease(internal_);
}

size_t GroupCommand::size() const { return number_of_modules_; }

AmberCommandPtr GroupCommand::operator[](size_t index) {
  return commands_[index];
}

const AmberCommandPtr GroupCommand::operator[](size_t index) const {
  return commands_[index];
}

void GroupCommand::clear() { amberGroupCommandClear(internal_); }

void GroupCommand::setPosition(const std::vector<float> &position) {
  if (position.size() != number_of_modules_)
    return;
  for (size_t i = 0; i < number_of_modules_; ++i) {
    commands_[i]->float_fields_ = position[i];
    commands_[i]->commandEnum = AmberCommandPosition;
  }
}

void GroupCommand::setTrapezoidalMove(const std::vector<float> &val) {
  if (val.size() != number_of_modules_)
    return;
  for (size_t i = 0; i < number_of_modules_; ++i) {
    commands_[i]->float_fields_ = val[i];
    commands_[i]->commandEnum = AmberCommandTrapezoidal;
  }
}
void GroupCommand::setVelocity(const std::vector<float> &velocity) {
  if (velocity.size() != number_of_modules_)
    return;
  for (size_t i = 0; i < number_of_modules_; ++i) {
    commands_[i]->float_fields_ = velocity[i];
    commands_[i]->commandEnum = AmberCommandVelocity;
  }
}
void GroupCommand::setVelocityRamp(const std::vector<float> &velocity) {
  if (velocity.size() != number_of_modules_)
    return;
  for (size_t i = 0; i < number_of_modules_; ++i) {
    commands_[i]->float_fields_ = velocity[i];
    commands_[i]->commandEnum = AmberCommandVelocityRamp;
  }
}
void GroupCommand::setCurrent(const std::vector<float> &current) {
  if (current.size() != number_of_modules_)
    return;
  for (size_t i = 0; i < number_of_modules_; ++i) {
    commands_[i]->float_fields_ = current[i];
    commands_[i]->commandEnum = AmberCommandCurrent;
  }
}

void GroupCommand::enable(const std::vector<float> &status) {
  if (status.size() != number_of_modules_)
    return;
  for (size_t i = 0; i < number_of_modules_; ++i) {
    commands_[i]->float_fields_ = status[i];
    commands_[i]->commandEnum = AmberCommandEnable;
  }
}

void GroupCommand::reboot(const std::vector<bool> &flag) {
  if (flag.size() != number_of_modules_)
    return;
  for (size_t i = 0; i < number_of_modules_; ++i) {
    commands_[i]->bool_fields_ = flag[i];
    commands_[i]->commandEnum = AmberCommandReboot;
  }
}

void GroupCommand::getError(const std::vector<bool> &flag) {
  if (flag.size() != number_of_modules_)
    return;
  for (size_t i = 0; i < number_of_modules_; ++i) {
    commands_[i]->bool_fields_ = flag[i];
    commands_[i]->commandEnum = AmberCommandGetError;
  }
}

void GroupCommand::clearError(const std::vector<bool> &flag) {
  if (flag.size() != number_of_modules_)
    return;
  for (size_t i = 0; i < number_of_modules_; ++i) {
    commands_[i]->bool_fields_ = flag[i];
    commands_[i]->commandEnum = AmberCommandClearError;
  }
}

void GroupCommand::resetLinearCount(const std::vector<float> &linearCount) {
  if (linearCount.size() != number_of_modules_)
    return;
  for (size_t i = 0; i < number_of_modules_; ++i) {
    commands_[i]->float_fields_ = linearCount[i];
    commands_[i]->commandEnum = AmberCommandResetLinearCount;
  }
}

void GroupCommand::setMontionCtrlConfig(
    const std::vector<MotionControllerConfig *> &config) {
  if (config.size() != number_of_modules_)
    return;
  for (size_t i = 0; i < number_of_modules_; ++i) {
    commands_[i]->motion_ctrl_config = config[i];
    commands_[i]->commandEnum = AmberCommandMontionControllerConfig;
  }
}

void GroupCommand::setMotorConfig(const std::vector<MotorConfig *> &config) {
  if (config.size() != number_of_modules_)
    return;
  for (size_t i = 0; i < number_of_modules_; ++i) {
    commands_[i]->motor_config = config[i];
    commands_[i]->commandEnum = AmberCommandMotorConfig;
  }
}

void GroupCommand::setTrapTraj(const std::vector<TrapTraj *> &config) {
  if (config.size() != number_of_modules_)
    return;
  for (size_t i = 0; i < number_of_modules_; ++i) {
    commands_[i]->trap_traj = config[i];
    commands_[i]->commandEnum = AmberCommandTrapTraj;
  }
}

void GroupCommand::saveConfig(const std::vector<bool> &flag) {
  if (flag.size() != number_of_modules_)
    return;
  for (size_t i = 0; i < number_of_modules_; ++i) {
    commands_[i]->bool_fields_ = flag[i];
    commands_[i]->commandEnum = AmberCommandSaveConfig;
  }
}

void GroupCommand::setNetworkSetting(
    const std::vector<NetworkSetting *> &config) {
  if (config.size() != number_of_modules_)
    return;
  for (size_t i = 0; i < number_of_modules_; ++i) {
    commands_[i]->network_setting = config[i];
    commands_[i]->commandEnum = AmberCommandNetworkSetting;
  }
}

void GroupCommand::setLatencyTest(const std::vector<bool> &flag) {
  if (flag.size() != number_of_modules_)
    return;
  for (size_t i = 0; i < number_of_modules_; ++i) {
    commands_[i]->bool_fields_ = flag[i];
    commands_[i]->commandEnum = AmberCommandLatencyTest;
  }
}

// Ctrl Box
void GroupCommand::ctrlBoxEnable(const std::vector<float> &status) {
  if (status.size() != number_of_modules_)
    return;
  for (size_t i = 0; i < number_of_modules_; ++i) {
    commands_[i]->float_fields_ = status[i];
    commands_[i]->commandEnum = AmberCommandCtrlBoxPower;
  }
}

} // namespace Amber
