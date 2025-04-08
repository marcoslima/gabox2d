#include <network/rs_codec.h>
#include <ReedSolomon.hpp>
#include <boost/asio/detail/thread_info_base.hpp>


RsCodec::RsCodec(const size_t chunk_size, const size_t redundancy)
    : _chunk_size(chunk_size)
    , _redundancy(redundancy)
    , _rs(8 * sizeof(RS::RSWord), redundancy)
{}


void RsCodec::_divide_to_chunks(const std::string &message, std::vector<std::vector<unsigned char>> &chunks) const
{
    const std::vector<uint8_t> data(message.begin(), message.end());
    chunks = RS::DataChunker::ChunkData(data, _chunk_size);
}

void RsCodec::_encode_chunks(std::vector<std::vector<unsigned char>> chunks, std::vector<std::vector<RS::RSWord>> &encoded_chunks) const
{
    std::ranges::transform(chunks, std::back_inserter(encoded_chunks),
                           [this](const auto &chunk)
                           {
                               return _rs.Encode(std::vector<RS::RSWord>(chunk.begin(), chunk.end()));
                           });
}

void RsCodec::_reassemble_encoded(const std::vector<std::vector<RS::RSWord>>& encoded_chunks, std::string &encoded_data)
{
    for (const auto &encoded_chunk : encoded_chunks)
    {
        const auto ch_size = encoded_chunk.size() * sizeof(RS::RSWord);
        encoded_data.append(reinterpret_cast<const char *>(encoded_chunk.data()), ch_size);
    }
}

std::string RsCodec::encode(const std::string &message) const // NOLINT(*-convert-member-functions-to-static)
{
    std::vector<std::vector<unsigned char> > chunks;
    std::vector<std::vector<RS::RSWord> > encoded_chunks;
    std::string encoded_data;

    _divide_to_chunks(message, chunks);
    _encode_chunks(chunks, encoded_chunks);
    _reassemble_encoded(encoded_chunks, encoded_data);

    return encoded_data;
}

void RsCodec::_divide_encoded_into_chunks(const std::string &data, std::vector<std::vector<unsigned char>> &chunks) const
{
    const auto real_chunk_size = _chunk_size + _redundancy;
    chunks = RS::DataChunker::ChunkData(std::vector<uint8_t>(data.begin(), data.end()), real_chunk_size);
}

bool RsCodec::_decode_chunks(std::vector<std::vector<unsigned char>> chunks, std::vector<std::vector<RS::RSWord>> &decoded_chunks) {
    try
    {
        std::ranges::transform(chunks, std::back_inserter(decoded_chunks),
                               [this](const auto &chunk)
                               {
                                   return _rs.Decode(chunk);
                               });
        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
}

void RsCodec::_reassemble_decoded(const std::vector<std::vector<RS::RSWord>>& decoded_chunks)
{
    _decoded_data.clear();
    for (const auto &decoded_chunk : decoded_chunks)
    {
        _decoded_data.append(reinterpret_cast<const char *>(decoded_chunk.data()), decoded_chunk.size() * sizeof(RS::RSWord));
    }
}

bool RsCodec::decode(const std::string &data)
{
    std::vector<std::vector<unsigned char> > chunks;
    std::vector<std::vector<RS::RSWord> > decoded_chunks;

    _divide_encoded_into_chunks(data, chunks);
    if (!_decode_chunks(chunks, decoded_chunks)) return false;
    _reassemble_decoded(decoded_chunks);

    return true;
}
