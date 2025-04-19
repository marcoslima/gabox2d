#pragma once

#include <boost/beast/core/detail/base64.hpp>
#include <string>

std::string binary_to_base64(const std::string& binary_data);
std::string base64_to_binary(const std::string& base64_data);
