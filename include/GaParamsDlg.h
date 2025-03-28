#pragma once
#include "ga_params.h"
#include <IGaBox2dView.h>

using namespace std;

namespace GUI
{
    class CGaParamsDlg
    {
        const char *_wndName = "Parâmetros para o GA";
        IGaBox2dView &_view;

    public:
        ga_params_t m_params;
        explicit CGaParamsDlg(IGaBox2dView &view); // standard constructor
        CGaParamsDlg() = delete;
        void OnInitDialog();

        void show() const;

        void RenderLeftGroup();

        void RenderRightGroup();

        void RenderButtons() const;

        void RenderDialog();
    };
}; //namespace GUI
