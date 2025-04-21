#include <boost/beast/core/detail/base64.hpp>
#include <string>

std::string binary_to_base64(const std::string& binary_data) {
    std::string result;
    // Calculate the size needed for the base64 output
    result.resize(boost::beast::detail::base64::encoded_size(binary_data.size()));

    // Encode the data
    size_t written = boost::beast::detail::base64::encode(
        result.data(),
        binary_data.data(),
        binary_data.size()
    );

    result.resize(written);
    return result;
}

std::string base64_to_binary(const std::string& base64_data) {
    std::string result;
    // Calculate the size needed for the binary output
    result.resize(boost::beast::detail::base64::decoded_size(base64_data.size()));

    // Decode the data
    auto [written, success] = boost::beast::detail::base64::decode(
        result.data(),
        base64_data.data(),
        base64_data.size()
    );

    if (!success) {
        throw std::runtime_error("Invalid base64 input");
    }

    result.resize(written);
    return result;
}