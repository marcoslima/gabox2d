#include <ranges>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <msgpack/msgpack.hpp>
#include <string>
#include <ReedSolomon.hpp>
using namespace std;
using namespace RS;

struct data_envelope_t
{
    std::string data;

    template<class T>
    void pack(T &pack)
    {
        pack(data);
    }
};

TEST_CASE("ReedSolomon", "[reedsolomon]")
{
    // Data to be encoded
    const string test_string = "dfiug sdufygv suoyd bouasgdf 8o23g 87o3r gouysg fuoyg wo7r 8g7 gfd897w e8 d8gweq87dfg "
            "wqe876dg 8q6 td87w euyfg wq786ef dg7q86wd 687q td876wf e8w7g ef7d6qwf ged786t q876d "
            "w8e7f g8w6g df8q76twd 876qt d76wg fe786wgq e796fdt q76fd 9q786 gdf87wq6gf 796qwt "
            "dwef796q dw976qfg wd76qfg fd6gq 76dwg 7q6wgd76qgw76qw gd796qwgd 796qwg d76q wgdq7 "
            "w6dg q7c6gd 976qwfg d76qwgd97q6wg df796qwf dg76qw fd7q6wfd97qw6f d76qdw "
            "wqe876dg 8q6 td87w euyfg wq786ef dg7q86wd 687q td876wf e8w7g ef7d6qwf ged786t q876d "
            "w8e7f g8w6g df8q76twd 876qt d76wg fe786wgq e796fdt q76fd 9q786 gdf87wq6gf 796qwt "
            "dwef796q dw976qfg wd76qfg fd6gq 76dwg 7q6wgd76qgw76qw gd796qwgd 796qwg d76q wgdq7 "
            "w6dg q7c6gd 976qwfg d76qwgd97q6wg df796qwf dg76qw fd7q6wfd97qw6f d76qdw "
            "wqe876dg 8q6 td87w euyfg wq786ef dg7q86wd 687q td876wf e8w7g ef7d6qwf ged786t q876d "
            "w8e7f g8w6g df8q76twd 876qt d76wg fe786wgq e796fdt q76fd 9q786 gdf87wq6gf 796qwt "
            "dwef796q dw976qfg wd76qfg fd6gq 76dwg 7q6wgd76qgw76qw gd796qwgd 796qwg d76q wgdq7 "
            "w6dg q7c6gd 976qwfg d76qwgd97q6wg df796qwf dg76qw fd7q6wfd97qw6f d76qdw ";
    data_envelope_t data_envelope{test_string};
    // cout << "Test string size: " << test_string.size() << endl;
    // cout << "Test string: " << test_string << endl;

    // Pack with msgpack:
    auto data = msgpack::pack(data_envelope);
    // cout << "msgpack serialized size: " << std::dec << data.size() << endl;
    // for (auto &byte: data)
    // {
    //     cout << std::hex << static_cast<int>(byte) << " ";
    // }
    // cout << std::dec << endl;

    SECTION("Without errors")
    {
        // Divide into chunks
        const auto chunks = RS::DataChunker::ChunkData(data, 100);
        // cout << "Number of chunks: " << chunks.size() << endl;
        // for (auto &chunk: chunks)
        // {
        //     cout << "Chunk size: " << std::dec << chunk.size() << endl;
        //     for (auto &byte: chunk)
        //     {
        //         cout << std::hex << static_cast<int>(byte) << " ";
        //     }
        //     cout << endl;
        // }

        // Encode chunks with Reed-Solomon
        ReedSolomon rs(8 * sizeof(RSWord), 5);
        vector<vector<RSWord>> encoded_chunks;
        ranges::transform(chunks, std::back_inserter(encoded_chunks),
                          [&rs](const auto &chunk)
                          {
                              return rs.Encode(vector<RSWord>(chunk.begin(), chunk.end()));
                          });
        // cout << "Encoded chunks size: " << std::dec << encoded_chunks.size() << endl;

        // Decode chunks with Reed-Solomon
        vector<vector<RSWord>> decoded_chunks;
        ranges::transform(encoded_chunks, std::back_inserter(decoded_chunks),
                          [&rs](const auto &chunk)
                          {
                              return rs.Decode(chunk);
                          });

        // Reassemble chunks
        const auto reassembled = RS::DataChunker::AssembleChunks(decoded_chunks);

        // Unpack with msgpack:
        auto unpacked_data_envelope = msgpack::unpack<data_envelope_t>(reassembled.data(), reassembled.size());

        // Compare with original message:
        REQUIRE(unpacked_data_envelope.data == test_string);
    }

    SECTION("With errors")
    {
        // Divide into chunks
        const auto chunks = RS::DataChunker::ChunkData(data, 100);
        // cout << "Number of chunks: " << chunks.size() << endl;
        // for (auto &chunk: chunks)
        // {
        //     cout << "Chunk size: " << std::dec << chunk.size() << endl;
        //     for (auto &byte: chunk)
        //     {
        //         cout << std::hex << static_cast<int>(byte) << " ";
        //     }
        //     cout << endl;
        // }

        // Encode chunks with Reed-Solomon
        ReedSolomon rs(8 * sizeof(RSWord), 5);
        vector<vector<RSWord>> encoded_chunks;
        ranges::transform(chunks, std::back_inserter(encoded_chunks),
                          [&rs](const auto &chunk)
                          {
                              return rs.Encode(vector<RSWord>(chunk.begin(), chunk.end()));
                          });
        // cout << "Encoded chunks size: " << std::dec << encoded_chunks.size() << endl;

        // Introduce some errors:
        // Corrupt message with random letters (errors with unknown position)
        encoded_chunks[0][2] = 'X';
        encoded_chunks[0][10] = 'X';
        encoded_chunks[1][0] = 'Q';
        encoded_chunks[1][15] = 'Q';
        encoded_chunks[5][5] = 'Q';
        encoded_chunks[5][7] = 'Q';
        // encoded_chunks[5][23] = 'Q';
        vector corrupted_chunks{0, 1, 5};
        constexpr auto expected_errors = 6;

        // Verify if chunks are corrupted
        for (const auto i : ranges::views::iota(0u, encoded_chunks.size()))
        {
            REQUIRE(rs.IsMessageCorrupted(encoded_chunks[i]) == (std::find(corrupted_chunks.begin(), corrupted_chunks.end(), i) != corrupted_chunks.end()));
        }

        // Decode chunks with Reed-Solomon
        vector<vector<RSWord>> decoded_chunks;
        uint64_t total_errors_found = 0;
        ranges::transform(encoded_chunks, std::back_inserter(decoded_chunks),
                          [&rs, &total_errors_found](const auto &chunk)
                          {
                              uint64_t errors_found = 0;
                              auto result = rs.Decode(chunk, nullptr, &errors_found);
                              total_errors_found += errors_found;
                              return result;
                          });

        // cout << "Total errors found: " << total_errors_found << endl;
        REQUIRE(total_errors_found == expected_errors);

        // Reassemble chunks
        const auto reassembled = RS::DataChunker::AssembleChunks(decoded_chunks);

        // Unpack with msgpack:
        auto unpacked_data_envelope = msgpack::unpack<data_envelope_t>(reassembled.data(), reassembled.size());

        // Compare with original message:
        REQUIRE(unpacked_data_envelope.data == test_string);
    }
}
