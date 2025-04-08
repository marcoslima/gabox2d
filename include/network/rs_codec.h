#pragma once
#include <string>
#include <ReedSolomon.hpp>


class RsCodec
{
    std::string _decoded_data;
    size_t _chunk_size;
    size_t _redundancy;
    RS::ReedSolomon _rs;

    void _divide_to_chunks(const std::string &message, std::vector<std::vector<unsigned char>> &chunks) const;
    void _encode_chunks(std::vector<std::vector<unsigned char>> chunks, std::vector<std::vector<RS::RSWord>> &encoded_chunks) const;
    static void _reassemble_encoded(const std::vector<std::vector<RS::RSWord>>& encoded_chunks, std::string &encoded_data) ;
    void _divide_encoded_into_chunks(const std::string &data, std::vector<std::vector<unsigned char>> &chunks) const;
    bool _decode_chunks(std::vector<std::vector<unsigned char>> chunks, std::vector<std::vector<RS::RSWord>> &decoded_chunks);
    void _reassemble_decoded(const std::vector<std::vector<RS::RSWord>>& decoded_chunks);

public:
    RsCodec(size_t chunk_size, size_t redundancy);


    [[nodiscard]] std::string encode(const std::string &message) const;
    bool decode(const std::string &data);
    [[nodiscard]] std::string get_decoded_data() { return std::move(_decoded_data); }
};
