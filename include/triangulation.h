#pragma once
#undef CDT_USE_AS_COMPILED_LIBRARY
#include <CDT.hpp>

#include <vector>

#include "env.h"

using namespace std;


using vec_triangles_t = vector<CDT::V2d<float> >;
using cdt_vertices_t = vector<CDT::V2d<float> >;
using triangulation_t = pair<CDT::Triangulation<float>, cdt_vertices_t>;

triangulation_t triangularize(const MODEL::vec_vecs_t &vecGround);
