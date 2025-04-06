#pragma once

#include <IPanel.h>
#include <ga_ipc.h>


namespace GUI
{
    class CGaInfoDlg final : public IPanel
    {
        ipc::GaStatus _status;

    public:
        CGaInfoDlg();
        void render() override;
        void set(const ipc::GaStatus& status);

        ~CGaInfoDlg() override = default;
    };
}
