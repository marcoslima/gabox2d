#pragma once
#include <IPanel.h>
#include <vector>


class CFitnessGraph final : public IPanel
{
public:
    using data_set_t = std::vector<std::pair<size_t, float>>;

    CFitnessGraph() = default;
    ~CFitnessGraph() override = default;

    void render() override;
    void set(const data_set_t &data);
private:
    data_set_t _data;
};
