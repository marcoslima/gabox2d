#pragma once

#include "ga.h"
#include "WndPreviewChao.h"

void InputUint8(const char* label, uint8_t* v);

namespace GUI
{
// CEditorChaoDlg dialog
class CEditorChaoDlg
{
private:
	const char* _wndName = "Definições do ambiente";

private:
	const double f64_zero = 0.0;
	const int n_zero = 0;
	
	void RenderDimensoes();
	void RenderAmbiente();
	void RenderButtons();
	void RenderDialog();
	void RenderCentralColumn();
	void RenderSeedAndAutoUpdate();
	void RenderParametrosCriacao();
	void RenderOcilador();

public:
	vec_vecs_t m_vecVecs;
	CWndPreviewChao	m_wndPreview;

public:
	CEditorChaoDlg();

public:
	CEnv	m_World;
	float m_fDxMedia;
	float m_fDxStdev;
	float m_fDxOffset;
	float m_fDyMedia;
	float m_fDyStdev;
	float m_fDyOffset;
	float m_fSinPhi;
	float m_fSinOmega;
	float m_fSinA;
	void OnBnClickedCriar();
	void OnBnClickedSalvar();
	void OnBnClickedLer();
	int m_nTlx;
	int m_nTly;
	int m_nBrx;
	int m_nBry;
	unsigned m_nSeed;
	// CSpinButtonCtrl m_cSpinSeed;
	// CSpinButtonCtrl m_cSpinPhi;
	// CSpinButtonCtrl m_cSpinOmega;
	// CSpinButtonCtrl m_cSpinA;
	// CSpinButtonCtrl m_cSpinDxm;
	// CSpinButtonCtrl m_cSpinDxs;
	// CSpinButtonCtrl m_cSpinDxo;
	// CSpinButtonCtrl m_cSpinDym;
	// CSpinButtonCtrl m_cSpinDys;
	// CSpinButtonCtrl m_cSpinDyo;

	void OnInitDialog();
	void show();
	void OnEnChangeSeed();

	bool m_bAutoUpdate;
	// CSliderCtrl m_cSldVert;
	// CSliderCtrl m_cSldHorz;
	// void OnHScroll(unsigned nSBCode, unsigned nPos, CScrollBar* pScrollBar);
	// void OnVScroll(unsigned nSBCode, unsigned nPos, CScrollBar* pScrollBar);

	void OnUpdateData();
	void UpdateData();

	void OnEnChangeDxMedia();
	void OnBnClickedZoomIn();
	void OnBnClickedZoomOut();
	void OnBnClickedAutoUpdate();
	void OnEnChangeSinPhi();
	void OnEnChangeSinOmega();
	void OnEnChangeSinA();
	void OnEnChangeDxStdev();
	void OnEnChangeDxOffset();
	void OnEnChangeDyMedia();
	void OnEnChangeDyStdev();
	void OnEnChangeDyOffset();
	void OnBnClickedOk();
};
};//namespace GUI
