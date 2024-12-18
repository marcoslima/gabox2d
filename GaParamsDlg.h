#pragma once
#include "ga_params.h"
#include "GaBox2dView.h"
using namespace std;

namespace GUI
{
    // CGaParamsDlg dialog
    class CGaBox2dView;
    class CGaParamsDlg
    {
        const char *_wndName = "Parâmetros para o GA";
        CGaBox2dView &_view;

    public:
        ga_params_t params;
        explicit CGaParamsDlg(CGaBox2dView &view); // standard constructor
        CGaParamsDlg() = delete;
        void OnInitDialog();

        void show() const;

        void RenderLeftGroup();

        void RenderRightGroup();

        void RenderButtons() const;

        void RenderDialog();
    };
}; //namespace GUI
