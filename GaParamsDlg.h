#pragma once
#include "ga_params.h"
#include "GaBox2dView.h"
using namespace std;

namespace GUI
{
    // CGaParamsDlg dialog
    class CGaBox2dView;
    class CGaParamsDlg : public ga_params_t
    {
        const char *_wndName = "Parâmetros para o GA";
        CGaBox2dView &_view;

    public:
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
