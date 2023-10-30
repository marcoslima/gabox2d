#ifndef __GABOX2DVIEW_H__
#define __GABOX2DVIEW_H__

#include "ga.h"
#include "GaParamsDlg.h"
#include "IdInfoDlg.h"
#include "GaInfoDlg.h"
namespace GUI
{
// Paramstros para o thread:
class CThreadParams
{
public:
	HANDLE			m_hStopGa	;
	HANDLE			m_hGaStopped;
	HWND			m_wndNotify	;
	ga_params_t		m_Params	;
	CGaInfo*		m_pGaInfo	;
	CEnv			m_env		;
};


// GaBox2dView.h : interface of the CGaBox2dView class
class CGaBox2dView : public CScrollView
{
protected: // create from serialization only
	CGaBox2dView();
	DECLARE_DYNCREATE(CGaBox2dView)

// Attributes
private:
	// UI
	CFont	m_fntSmall;
	CFont	m_fntSupersmall;

	// Outros
	UINT	m_nSimTimer;
	CBitmap	m_bmpBack;
	UINT	m_nVelocidade;
	bool	m_bGaRunning;
	bool	m_bGaExited;

	bool	m_bShowInfoId;
	bool	m_bShowInfoGaGenes;
	CThreadParams	_thread_params;
	CGaInfo	m_GaInfo;
	bool			m_bWaitingEvolucao;

public:
	CGaBox2dDoc* GetDocument() const;

	CGa*		getGa(void);
	void releaseGa(void);

// GUI
public:
	CIdInfoDlg*	m_pdlgIdInfo;
	CGaInfoDlg*	m_pdlgGaInfo;

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	afx_msg	LRESULT OnGaInfo(WPARAM wParma, LPARAM lParam);
	afx_msg	LRESULT OnSimularGene(WPARAM wParma, LPARAM lParam);

// Implementation
public:
	virtual ~CGaBox2dView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	void Draw(CDC *pDc);
	afx_msg void OnSimulaPlay();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnUpdateSimulaPlay(CCmdUI *pCmdUI);
	afx_msg void OnSimulaReset();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnVelocidadeMais();
	afx_msg void OnVelocidadeMenos();
	afx_msg void OnUpdateVelocidadeMais(CCmdUI *pCmdUI);
	afx_msg void OnUpdateVelocidadeMenos(CCmdUI *pCmdUI);
	afx_msg void OnSimulaRepetir();
	afx_msg void OnVelocidade1x();
	afx_msg void OnVelocidade2x();
	afx_msg void OnVelocidade4x();
	afx_msg void OnVelocidade10x();
	afx_msg void OnVelocidade100x();
	afx_msg void OnUpdateVelocidade1x(CCmdUI *pCmdUI);
	afx_msg void OnUpdateVelocidade2x(CCmdUI *pCmdUI);
	afx_msg void OnUpdateVelocidade4x(CCmdUI *pCmdUI);
	afx_msg void OnUpdateVelocidade10x(CCmdUI *pCmdUI);
	afx_msg void OnUpdateVelocidade100x(CCmdUI *pCmdUI);
	afx_msg void OnGaIniciarga();
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateGaIniciarga(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMostrarMelhor(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMostrarQualquer(CCmdUI *pCmdUI);
	afx_msg void OnMostrarMelhor();
	afx_msg void OnMostrarQualquer();
	afx_msg void OnViewCpuinfo();
	afx_msg void OnInformaGagenes();
	afx_msg void OnInformaIndividuoatual();
	afx_msg void OnUpdateInformaGagenes(CCmdUI *pCmdUI);
	afx_msg void OnUpdateInformaIndividuoatual(CCmdUI *pCmdUI);
	afx_msg void OnGaExtins();
	afx_msg void OnGaColar();
	afx_msg void OnUpdateGaExtins(CCmdUI *pCmdUI);
	afx_msg void OnUpdateGaColar(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditEditarch(CCmdUI *pCmdUI);
	afx_msg void OnInformaEvolu();
	afx_msg void OnUpdateInformaEvolu(CCmdUI *pCmdUI);
	afx_msg void OnGaMudarpar();
};

#ifndef _DEBUG  // debug version in GaBox2dView.cpp
inline CGaBox2dDoc* CGaBox2dView::GetDocument() const
   { return reinterpret_cast<CGaBox2dDoc*>(m_pDocument); }
#endif

};//namespace GUI

#endif //__GABOX2DVIEW_H__