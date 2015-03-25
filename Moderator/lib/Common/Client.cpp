#include "Common/Client.h"

#include <iostream>
#include <string>

#include "Common/String.h"

namespace Common {
std::string readMsg() {
  std::string msg;
  std::getline(std::cin, msg);
  msg = Common::rtrim(msg);
  return msg;
}

} // namespace Common
