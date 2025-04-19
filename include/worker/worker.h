#pragma once
#include <ga.h>
using namespace GA;

map_measures_results_t _do_measures(
    const env_data_t &env_data,
    const map_individuals_t &individuals,
    const float max_t);

void do_work(const string& encoded_data);