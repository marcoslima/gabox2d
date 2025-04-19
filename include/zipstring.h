#pragma once
#include <string>


std::string compress_string_zlib(const std::string& data);
std::string decompress_string_zlib(const std::string& compressed_data);
