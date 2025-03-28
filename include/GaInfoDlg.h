#pragma once

#include <ga.h>

#include <GaBox2dView.h>
#include <GaInfo.h>
using namespace GA;

namespace GUI
{
    class CGaInfoDlg
    {
        const char *_wndName = "GA Info";
        IGaBox2dView &_view;

    public:
        explicit CGaInfoDlg(IGaBox2dView &view);

        ~CGaInfoDlg();

        unsigned m_nGeracao;
        unsigned m_nPopulacao;
        vector<pair<string, string> > m_lstGenes;

        void OnInitDialog(CGaInfo *pInfo);

        void Render(CGaInfo *pInfo);
        void show() const;
    };
}; //namespace GUI
