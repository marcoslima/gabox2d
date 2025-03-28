#pragma once

#include <ga_params.h>

class IGaBox2dView
{
public:
    virtual ~IGaBox2dView() = default;

    virtual void startGa(ga_params_t params) = 0;
};
