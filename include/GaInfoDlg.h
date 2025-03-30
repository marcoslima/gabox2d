#pragma once

#include <ga.h>

#include <GaBox2dView.h>
#include <GaInfo.h>
using namespace GA;

namespace GUI
{
    using genes_vector_t = vector<pair<string, string> >;

    class CGaInfoDlg
    {
        const char *_wndName = "GA Info";
        IGaBox2dViewPtr _view;

    public:
        explicit CGaInfoDlg(IGaBox2dViewPtr view);

        ~CGaInfoDlg();

        unsigned m_nGeracao;
        unsigned m_nPopulacao;
        genes_vector_t m_lstGenes;

        void OnInitDialog(CGaInfo *pInfo);

        void Render(CGaInfo *pInfo);
        void show() const;
    };
}; //namespace GUI
