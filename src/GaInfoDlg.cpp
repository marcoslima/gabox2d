#include <imgui.h>
#if 1
#include <GaInfoDlg.h>
#include <GaBox2dView.h>


namespace GUI
{
    CGaInfoDlg::CGaInfoDlg(IGaBox2dView &view)
        : _view(view)
          , m_nGeracao(0)
          , m_nPopulacao(0) {}

    CGaInfoDlg::~CGaInfoDlg() {}
    
    void CGaInfoDlg::OnInitDialog(CGaInfo *pInfo)
    {
        // m_lstGenes.InsertColumn(0, "Pts");
        // m_lstGenes.InsertColumn(1, "Genes");
        // m_lstGenes.AdjustColumns();
        //
        // return TRUE;
        ImGui::BeginPopup(_wndName, ImGuiWindowFlags_Popup);
        Render(pInfo);
        // ImGui::EndPopup();

    }
    void CGaInfoDlg::show() const
    {
        ImGui::OpenPopup(_wndName);
    }

    void CGaInfoDlg::Render(CGaInfo *pInfo)
    {
        ImGui::BeginGroup();
        ImGui::Text("Geração: %d", pInfo->m_geracao);
        ImGui::Text("População: %ld", pInfo->m_populacao.size());

        ImGui::EndGroup();
#if 0
        m_nGeracao = pInfo->m_geracao;
        m_nPopulacao = (UINT) pInfo->m_populacao.size();

        m_lstGenes.SetRedraw(FALSE);
        m_lstGenes.DeleteAllItems();

        int iItem;
        lst_car_t::iterator it;
        for (it = pInfo->m_populacao.begin();
             it != pInfo->m_populacao.end();
             it++)
        {
            iItem = m_lstGenes.InsertItem();
            m_lstGenes.SetItemData(iItem, 0, (int) it->_pontos);
            m_lstGenes.SetItemData(iItem, 1, it->_genes.c_str());
        }
        m_lstGenes.AdjustColumns();
        m_lstGenes.SetRedraw(TRUE);

        UpdateData(FALSE);
#endif
    }

    // void CGaInfoDlg::OnNMDblclkGenes(NMHDR *pn, LRESULT *pResult)
    // {
    //     *pResult = 0;
    //     NMITEMACTIVATE *pni = (NMITEMACTIVATE *) pn;
    //
    //     if (pni->iItem == -1)
    //         return;
    //     int iItem = pni->iItem;
    //
    //     CString strGenes = m_lstGenes.GetItemData(iItem, 1).GetString();
    //     char *szGenes = new char[strGenes.GetLength() + 5];
    //     strcpy(szGenes, strGenes);
    //
    //     m_pView->SendMessage(IDM_SIMULAR_GENE, 0, (LPARAM) szGenes);
    //
    //     delete szGenes;
    // }
}; //namespace GUI


#endif
