#pragma once

#include <IPanel.h>
#include <ga_ipc.h>


using namespace GA;


namespace GUI
{
    class CGaInfoDlg final : public IPanel
    {
        ipc::GaStatus _status{};

    public:
        void render() override;
        void set(const ipc::GaStatus& status);

        ~CGaInfoDlg() override = default;
    };
}
