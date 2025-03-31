#pragma once


class IPanel
{
public:
    virtual ~IPanel() {}
    virtual void render() = 0;
};
