#pragma once
#include <ga.h>
using namespace GA;

map_measures_results_t do_measures_(
    const env_data_t &env_data,
    const map_individuals_t &individuals,
    float max_t
);

void do_work(const string& encoded_data);
