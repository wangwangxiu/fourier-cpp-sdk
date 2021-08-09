#pragma once

#include <string>

// clang-format off
#define AMBER_DISABLE_COPY_MOVE(Class) \
/* Disable copy constructor. */ \
Class(const Class& other) = delete; \
/* Disable move constructor. */ \
Class(Class&& other) = delete; \
/* Disable copy assigment operator. */ \
Class& operator= (const Class& other) = delete; \
/* Disable move assigment operator. */ \
Class& operator= (Class&& other) = delete;

#define AMBER_DISABLE_COPY(Class) \
/* Disable copy constructor. */ \
Class(const Class& other) = delete; \
/* Disable copy assigment operator. */ \
Class& operator= (const Class& other) = delete;
// clang-format on