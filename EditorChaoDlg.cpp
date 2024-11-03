// EditorChaoDlg.cpp : implementation file
//
#include <imgui.h>

#include "EditorChaoDlg.h"
#include <fstream>
#include <sstream>
#include "devutils.h"
using namespace std;
using namespace DevUtils;


namespace GUI
{
    // CEditorChaoDlg dialog
    CEditorChaoDlg::CEditorChaoDlg()
        : m_World(), m_dDxMedia(0)
          , m_dDxStdev(10)
          , m_dDxOffset(50)
          , m_dDyMedia(0)
          , m_dDyStdev(3)
          , m_dDyOffset(0)
          , m_dSinPhi(0)
          , m_dSinOmega(0)
          , m_dSinA(0)
          , m_nTlx(-100)
          , m_nTly(500)
          , m_nBrx(500)
          , m_nBry(-50)
          , m_nSeed(0)
          , m_bAutoUpdate(false) {}

    // BEGIN_MESSAGE_MAP(CEditorChaoDlg, CDialog)
    // 	ON_BN_CLICKED(IDC_CRIAR, OnBnClickedCriar)
    // 	ON_BN_CLICKED(IDC_SALVAR, OnBnClickedSalvar)
    // 	ON_BN_CLICKED(IDC_LER, OnBnClickedLer)
    // 	ON_EN_CHANGE(IDC_SEED, OnEnChangeSeed)
    // 	ON_EN_CHANGE(IDC_DX_MEDIA, OnEnChangeDxMedia)
    // 	ON_WM_HSCROLL()
    // 	ON_WM_VSCROLL()
    // 	ON_BN_CLICKED(IDC_ZOOM_IN, OnBnClickedZoomIn)
    // 	ON_BN_CLICKED(IDC_ZOOM_OUT, OnBnClickedZoomOut)
    // 	ON_BN_CLICKED(IDC_AUTO_UPDATE, OnBnClickedAutoUpdate)
    // 	ON_EN_CHANGE(IDC_SIN_PHI, OnEnChangeSinPhi)
    // 	ON_EN_CHANGE(IDC_SIN_OMEGA, OnEnChangeSinOmega)
    // 	ON_EN_CHANGE(IDC_SIN_A, OnEnChangeSinA)
    // 	ON_EN_CHANGE(IDC_DX_STDEV, OnEnChangeDxStdev)
    // 	ON_EN_CHANGE(IDC_DX_OFFSET, OnEnChangeDxOffset)
    // 	ON_EN_CHANGE(IDC_DY_MEDIA, OnEnChangeDyMedia)
    // 	ON_EN_CHANGE(IDC_DY_STDEV, OnEnChangeDyStdev)
    // 	ON_EN_CHANGE(IDC_DY_OFFSET, OnEnChangeDyOffset)
    // 	ON_BN_CLICKED(IDOK, OnBnClickedOk)
    // END_MESSAGE_MAP()


    void CEditorChaoDlg::OnUpdateData() {}
    void CEditorChaoDlg::UpdateData() {}

    // CEditorChaoDlg message handlers
    void CEditorChaoDlg::OnBnClickedCriar()
    {
        UpdateData();
        // double  dxMedia		,
        // 		dxStdev		,
        // 		dxOffset	,
        // 		dyMedia		,
        // 		dyStdev		,
        // 		dyOffset	,
        // 		sinPhi		,
        // 		sinOmega	,
        // 		sinA		,
        // 		tlx			,
        // 		tly			,
        // 		brx			,
        // 		bry;

        // sinPhi			= str2dbl(m_strSinPhi	);
        // sinOmega		= str2dbl(m_strSinOmega	);
        // if(sinOmega != 0) sinOmega = 1.0 / sinOmega;
        // sinA			= str2dbl(m_strSinA		)/10.0;
        // tlx				= str2dbl(m_strTlx		);
        // tly				= str2dbl(m_strTly		);
        // brx				= str2dbl(m_strBrx		);
        // bry				= str2dbl(m_strBry		);

        m_World.set(m_nSeed,
                    m_dDxMedia, m_dDxStdev, m_dDxOffset,
                    m_dDyMedia, m_dDyStdev, m_dDyOffset,
                    m_dSinPhi, m_dSinOmega, m_dSinA,
                    m_nTlx, m_nTly, m_nBrx, m_nBry);

        m_vecVecs.clear();

        m_wndPreview.m_env = m_World;
        m_wndPreview.m_vecTl = b2Vec2(m_nTlx, m_nTly);
        m_wndPreview.m_vecBr = b2Vec2(m_nBrx, m_nBry);

        m_wndPreview.Invalidate();
    }

    void CEditorChaoDlg::OnBnClickedSalvar()
    {
        // CString strFile;

        // CFileDialog dlgFile (
        // 	FALSE,
        // 	_T("ada"),
        // 	strFile,
        // 	OFN_ENABLESIZING|OFN_EXPLORER|OFN_LONGNAMES|OFN_NOREADONLYRETURN|OFN_OVERWRITEPROMPT|OFN_PATHMUSTEXIST,
        // 	_T("Arquivos de defini��o de ambiente (*.ada)|*.ada|Todos os arquivos (*.*)|*.*|"),
        // 	this );
        // dlgFile.m_ofn.lpstrTitle = _T("Salvar configura��o de ambiente");

        // if(dlgFile.DoModal() != IDOK)
        // 	return;

        // strFile = dlgFile.GetPathName();

        // UpdateData();

        // ofstream fOut(strFile);
        // fOut << m_World.get();
    }

    void CEditorChaoDlg::OnBnClickedLer()
    {
        // CString strFile;

        // CFileDialog dlgFile (
        // 	TRUE,
        // 	_T("ada"),
        // 	strFile,
        // 	OFN_ENABLESIZING|OFN_EXPLORER|OFN_LONGNAMES|OFN_NOREADONLYRETURN|OFN_OVERWRITEPROMPT|OFN_PATHMUSTEXIST,
        // 	_T("Arquivos de defini��o de ambiente (*.ada)|*.ada|Todos os arquivos (*.*)|*.*|"),
        // 	this );
        // dlgFile.m_ofn.lpstrTitle = _T("Salvar configura��o de ambiente");

        // if(dlgFile.DoModal() != IDOK)
        // 	return;

        // strFile = dlgFile.GetPathName();

        // string strParams;

        // ifstream fIn(strFile);
        // while(fIn)
        // 	fIn >> strParams;

        // UpdateData();
        // m_World.set(strParams);

        // m_strDxMedia  = dbl2str(m_World._dxm*10);
        // m_strDxStdev  = dbl2str(m_World._dxs*10);
        // m_strDxOffset = dbl2str(m_World._dxo*10);

        // m_strDyMedia  = dbl2str(m_World._dym*10);
        // m_strDyStdev  = dbl2str(m_World._dys*10);
        // m_strDyOffset = dbl2str(m_World._dyo*10);

        // m_strSinPhi   = dbl2str(m_World._phi	);
        // m_strSinOmega = dbl2str((m_World._omega == 0)?(0):(1.0/m_World._omega));
        // m_strSinA	  = dbl2str(m_World._a*10	);


        // m_strTlx = dbl2str(m_World._tlx);
        // m_strTly = dbl2str(m_World._tly);
        // m_strBrx = dbl2str(m_World._brx);
        // m_strBry = dbl2str(m_World._bry);

        // m_nSeed = m_World._seed;

        // UpdateData(FALSE);
        // OnBnClickedCriar();
    }

    void DragDouble(const char *label, void *p_data, float v_speed, const void *p_min, const void *p_max,
                    const char *format)
    {
        ImGui::PushItemWidth(-1.0e-38f);
        ImGui::DragScalar(label, ImGuiDataType_Double, p_data, v_speed, p_min, p_max, format, 1.0f);
        ImGui::PopItemWidth();
    }

    void DragSigned(const char *label, void *p_data, float v_speed, const void *p_min, const void *p_max,
                    const char *format)
    {
        ImGui::PushItemWidth(-1.0e-38f);
        ImGui::DragScalar(label, ImGuiDataType_S32, p_data, v_speed, p_min, p_max, format, 1.0f);
        ImGui::PopItemWidth();
    }

    void CEditorChaoDlg::RenderDimensoes()
    {
        ImGuiTableFlags flags1 = ImGuiTableFlags_Borders
                                 | ImGuiTableFlags_NoBordersInBodyUntilResize
                                 | ImGuiTableFlags_NoHostExtendX;

        ImGui::BeginGroup();
        if (ImGui::BeginTable("table", 4, flags1))
        {
            ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 20.0f);
            ImGui::TableSetupColumn("mean", ImGuiTableColumnFlags_WidthFixed, 50.0f);
            ImGui::TableSetupColumn("stdev", ImGuiTableColumnFlags_WidthFixed, 50.0f);
            ImGui::TableSetupColumn("offset", ImGuiTableColumnFlags_WidthFixed, 50.0f);
            ImGui::TableHeadersRow();

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("dx");
            ImGui::TableNextColumn();
            DragDouble("##a", &m_dDxMedia, 0.01f, &f64_zero, nullptr, "%.1f");
            ImGui::TableNextColumn();
            DragDouble("##b", &m_dDxStdev, 0.01f, &f64_zero, nullptr, "%.1f");
            ImGui::TableNextColumn();
            DragDouble("##c", &m_dDxOffset, 0.01f, nullptr, nullptr, "%.1f");

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("dy");
            ImGui::TableNextColumn();
            DragDouble("##d", &m_dDyMedia, 0.01f, nullptr, nullptr, "%.1f");
            ImGui::TableNextColumn();
            DragDouble("##e", &m_dDyStdev, 0.01f, &f64_zero, nullptr, "%.1f");
            ImGui::TableNextColumn();
            DragDouble("##f", &m_dDyOffset, 0.01f, nullptr, nullptr, "%.1f");
            ImGui::EndTable();
        }
        ImGui::EndGroup();
    }

    void CEditorChaoDlg::RenderParametrosCriacao()
    {
        ImGui::BeginGroup();
        ImGui::Text("Parâmetros de criação");
        RenderDimensoes();
        RenderOcilador();
        ImGui::EndGroup();
    }

    void CEditorChaoDlg::RenderOcilador()
    {
        ImGuiTableFlags flags1 = ImGuiTableFlags_Borders
                                 | ImGuiTableFlags_NoBordersInBodyUntilResize
                                 | ImGuiTableFlags_NoHostExtendX;

        ImGui::BeginGroup();
        if (ImGui::BeginTable("ocilador", 4, flags1))
        {
            ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 20.0f);
            ImGui::TableSetupColumn("phi", ImGuiTableColumnFlags_WidthFixed, 50.0f);
            ImGui::TableSetupColumn("omega-", ImGuiTableColumnFlags_WidthFixed, 50.0f);
            ImGui::TableSetupColumn("A", ImGuiTableColumnFlags_WidthFixed, 50.0f);
            ImGui::TableHeadersRow();

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("sen");
            ImGui::TableNextColumn();
            DragDouble("##g", &m_dSinPhi, .5f, nullptr, nullptr, "%.1f");
            ImGui::TableNextColumn();
            DragDouble("##h", &m_dSinOmega, .5f, nullptr, nullptr, "%.1f");
            ImGui::TableNextColumn();
            DragDouble("##i", &m_dSinA, .5f, nullptr, nullptr, "%.1f");
            ImGui::EndTable();
        }
        ImGui::EndGroup();
    }

    void CEditorChaoDlg::RenderAmbiente()
    {
        ImGuiTableFlags flags1 = ImGuiTableFlags_Borders
                                 | ImGuiTableFlags_NoBordersInBodyUntilResize
                                 | ImGuiTableFlags_NoHostExtendX;

        ImGui::BeginGroup();
        ImGui::Text("Ambiente");
        if (ImGui::BeginTable("ambiente", 3, flags1))
        {
            ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 80.0f);
            ImGui::TableSetupColumn("x", ImGuiTableColumnFlags_WidthFixed, 50.0f);
            ImGui::TableSetupColumn("y", ImGuiTableColumnFlags_WidthFixed, 50.0f);
            ImGui::TableHeadersRow();

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("TopLeft");
            ImGui::TableSetColumnIndex(1);
            DragSigned("##g", &m_nTlx, .5f, nullptr, nullptr, "%d");
            ImGui::TableSetColumnIndex(2);
            DragSigned("##h", &m_nTly, .5f, nullptr, nullptr, "%d");

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("BottomRight");
            ImGui::TableSetColumnIndex(1);
            DragSigned("##i", &m_nBrx, .5f, nullptr, nullptr, "%d");
            ImGui::TableSetColumnIndex(2);
            DragSigned("##j", &m_nBry, .5f, nullptr, nullptr, "%d");
            ImGui::EndTable();
        }
        ImGui::EndGroup();
    }

    void CEditorChaoDlg::RenderButtons()
    {
        ImVec2 size = ImVec2(100, 0);
        ImGui::BeginGroup();
        ImGui::Button("Criar", size);
        ImGui::Button("Salvar", size);
        ImGui::Button("Ler", size);
        ImGui::Button("Usar", size);
        if (ImGui::Button("Cancelar", size))
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndGroup();
    }

    void RenderHorizontalSpace(float size = 10)
    {
        ImGui::SameLine();
        ImGui::Dummy(ImVec2(size, 0));
        ImGui::SameLine();
    }

    void CEditorChaoDlg::RenderSeedAndAutoUpdate()
    {
        ImGui::Text("Seed");
        ImGui::SameLine();
        ImGui::PushItemWidth(80);
        ImGui::InputInt("##Seed", (int *) &m_nSeed);
        ImGui::PopItemWidth();
        ImGui::SameLine();
        ImGui::Checkbox("Auto-criar", &m_bAutoUpdate);
    }

    void CEditorChaoDlg::RenderCentralColumn()
    {
        ImGui::BeginGroup();
        RenderAmbiente();
        ImGui::Dummy(ImVec2(0, 10));
        RenderSeedAndAutoUpdate();
        ImGui::EndGroup();
    }

    void CEditorChaoDlg::RenderDialog()
    {
        RenderParametrosCriacao();

        RenderHorizontalSpace();

        RenderCentralColumn();

        RenderHorizontalSpace();

        RenderButtons();
    }

    void CEditorChaoDlg::OnInitDialog()
    {
        static ImGuiTableFlags flags1 = ImGuiTableFlags_Borders | ImGuiTableFlags_NoBordersInBodyUntilResize;

        if (ImGui::BeginPopupModal(_wndName, NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            RenderDialog();

            ImGui::EndPopup();
        }

        return;
    }

    void CEditorChaoDlg::show()
    {
        ImGui::OpenPopup(_wndName);
    }

    void CEditorChaoDlg::OnEnChangeSeed()
    {
        if (m_bAutoUpdate)
        {
            UpdateData();
            OnBnClickedCriar();
        }
    }

    void CEditorChaoDlg::OnEnChangeDxMedia()
    {
        if (m_bAutoUpdate) OnBnClickedCriar();
    }

    void CEditorChaoDlg::OnBnClickedZoomIn()
    {
        m_wndPreview.m_zoom++;
        m_wndPreview.Invalidate();
    }

    void CEditorChaoDlg::OnBnClickedZoomOut()
    {
        m_wndPreview.m_zoom--;
        m_wndPreview.Invalidate();
    }

    void CEditorChaoDlg::OnBnClickedAutoUpdate()
    {
        UpdateData();
    }

    void CEditorChaoDlg::OnEnChangeSinPhi()
    {
        if (m_bAutoUpdate) OnBnClickedCriar();
    }

    void CEditorChaoDlg::OnEnChangeSinOmega()
    {
        if (m_bAutoUpdate) OnBnClickedCriar();
    }

    void CEditorChaoDlg::OnEnChangeSinA()
    {
        if (m_bAutoUpdate) OnBnClickedCriar();
    }

    void CEditorChaoDlg::OnEnChangeDxStdev()
    {
        if (m_bAutoUpdate) OnBnClickedCriar();
    }

    void CEditorChaoDlg::OnEnChangeDxOffset()
    {
        if (m_bAutoUpdate) OnBnClickedCriar();
    }

    void CEditorChaoDlg::OnEnChangeDyMedia()
    {
        if (m_bAutoUpdate) OnBnClickedCriar();
    }

    void CEditorChaoDlg::OnEnChangeDyStdev()
    {
        if (m_bAutoUpdate) OnBnClickedCriar();
    }

    void CEditorChaoDlg::OnEnChangeDyOffset()
    {
        if (m_bAutoUpdate) OnBnClickedCriar();
    }

    void CEditorChaoDlg::OnBnClickedOk()
    {
        // OnOK();
    }
}; //namespace GUI
