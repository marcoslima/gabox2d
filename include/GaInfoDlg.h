#pragma once

#include <GaCar.h>
#include <IPanel.h>
#include <ga_status.h>
#include <genoma_image.h>


namespace GUI
{
    class CGaInfoDlg final : public IPanel
    {
        ipc::GaStatus _status;
        CGenomaImage _bestGenoma{static_cast<unsigned int>(CCarDef::bits::len())};
        vector<CGenomaImage> _populationGenomas;
        vector<CGenomaImage> _historyGenomas;

    public:
        CGaInfoDlg();

        void _render_top();

        void _render_bottom();

        void _render_population();

        void _render_history();

        ~CGaInfoDlg() override = default;

        void render() override;
        void set(const ipc::GaStatus& status);
    };
}
