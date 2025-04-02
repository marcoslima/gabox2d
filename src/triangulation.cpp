#include <functional>

#include "triangulation.h"

#include <iostream>
#include <map>



/**
 * Computes a hash value for a vector of vectors (ground shape)
 *
 * @param vecGround The vector containing 2D points representing ground vertices
 * @return A size_t hash value uniquely identifying the ground shape
 */
size_t hashGround(const MODEL::vec_vecs_t& vecGround)
{
    size_t hash = 0;

    // Combine hashes of all vertices in a deterministic order
    for (const auto& point : vecGround)
    {
        // Hash the x coordinate
        const size_t xHash = std::hash<float>{}(point.x);

        // Hash the y coordinate
        const size_t yHash = std::hash<float>{}(point.y);

        // Combine hashes: rotate bits of the running hash and XOR with point hash
        hash ^= xHash + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        hash ^= yHash + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    }

    return hash;
}

using ground_triangulated_t = std::map<size_t, triangulation_t>;

ground_triangulated_t ground_triangulated_cache;

triangulation_t triangularize(const MODEL::vec_vecs_t &vecGround)
{
    const auto groundHash = hashGround(vecGround);
    const auto it = ground_triangulated_cache.find(groundHash);
    if (it != ground_triangulated_cache.end())
    {
        // Found a cached triangulation
        return it->second;
    }

    CDT::Triangulation<float> cdt;
    vector<CDT::V2d<float> > vecVertices;
    for (const auto &v: vecGround)
    {
        vecVertices.emplace_back(v.x, v.y);
    }
    cdt.insertVertices(vecVertices);
    vector<CDT::Edge> vecEdges;
    vecEdges.reserve(vecGround.size());
    for (int i = 0; i < vecGround.size(); i++)
    {
        vecEdges.emplace_back(i, (i + 1) % vecGround.size());
    }
    cdt.insertEdges(vecEdges);
    cdt.eraseOuterTrianglesAndHoles();

    triangulation_t result{cdt, vecVertices};
    ground_triangulated_cache[groundHash] = result;
    return result;
}
