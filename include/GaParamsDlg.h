#pragma once

#include <BasicModalDlg.h>
#include <functional>

#include "ga_params.h"


using namespace std;

namespace GUI
{
    class CGaParamsDlg final : public BasicModalDlg, public ga_params_t
    {
        void RenderLeftGroup();
        void RenderRightGroup();
        void RenderButtons();

    protected:
        void RenderContent() override;

    public:
        using callback_t = function<void(const ga_params_t&)>;
        callback_t _callback;

        explicit CGaParamsDlg(const callback_t &fn_callback);
        CGaParamsDlg() = delete;
    };
}
