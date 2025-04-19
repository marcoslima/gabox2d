#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filter/bzip2.hpp>
#include <boost/iostreams/copy.hpp>
#include <iostream>
#include <sstream>
#include <string>

// Function to compress string data using zlib
std::string compress_string_zlib(const std::string& data) {
    std::stringstream compressed;
    std::stringstream origin(data);

    // Set up compression filter
    boost::iostreams::filtering_streambuf<boost::iostreams::input> out;
    out.push(boost::iostreams::zlib_compressor());  // Use zlib algorithm
    out.push(origin);

    // Compress data
    boost::iostreams::copy(out, compressed);

    return compressed.str();
}

// Function to decompress zlib-compressed data
std::string decompress_string_zlib(const std::string& compressed_data) {
    std::stringstream decompressed;
    std::stringstream compressed(compressed_data);

    boost::iostreams::filtering_streambuf<boost::iostreams::input> in;
    in.push(boost::iostreams::zlib_decompressor());
    in.push(compressed);

    boost::iostreams::copy(in, decompressed);

    return decompressed.str();
}

