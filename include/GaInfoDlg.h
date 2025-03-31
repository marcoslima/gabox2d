#pragma once

#include <ga.h>
#include <GaBox2dView.h>


using namespace GA;


namespace GUI
{
    using genes_vector_t = vector<pair<string, string> >;

    class CGaInfoDlg
    {
        const char *_wndName;
        IGaBox2dViewPtr _view;

    public:
        explicit CGaInfoDlg(const IGaBox2dViewPtr &view);

        ~CGaInfoDlg();

        unsigned m_nGeracao;
        unsigned m_nPopulacao;
        genes_vector_t m_lstGenes;

        void OnInitDialog();

        void Render();
    };
}; //namespace GUI
