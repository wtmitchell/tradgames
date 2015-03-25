//===------------------------------------------------------------*- C++ -*-===//
///
/// \file
/// \brief Defines routines common to all clients
///
//===----------------------------------------------------------------------===//
#ifndef COMMON_CLIENT_H_INCLUDED
#define COMMON_CLIENT_H_INCLUDED

#include <string>
#include <vector>

namespace Common {
/// Reads a line, up to a newline from the server
std::string readMsg();
} // namespace Common

#endif
