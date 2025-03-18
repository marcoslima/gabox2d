#pragma once

namespace GA
{
    using fitness_params_t = struct tagFitnessParams
    {
        float contact1;
        float contact2;
        float velocity;
        float distance;
        float time;
        bool is_dead;
    };
}