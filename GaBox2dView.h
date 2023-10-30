#ifndef __GABOX2DVIEW_H__
#define __GABOX2DVIEW_H__

#include "ga.h"

typedef struct tagIdInfo
{
	CString		strGenes;
	double		dPoints;
} id_info_t;

typedef vector<id_info_t> vec_info_ids;


// GaBox2dView.h : interface of the CGaBox2dView class
class CGaBox2dView : public CScrollView
{
protected: // create from serialization only
	CGaBox2dView();
	DECLARE_DYNCREATE(CGaBox2dView)

// Attributes
public:
	CGaBox2dDoc* GetDocument() const;
	UINT	m_nSimTimer;

	CBitmap	m_bmpBack;

	UINT	m_nVelocidade;
	CFont	m_fntSmall;
	CFont	m_fntSupersmall;

	CGa		m_ga;
	bool	m_bGaRunning;
	bool	m_bGaExited;

	CString	m_strMelhor;
	CString	m_strQualquer;
	int		m_nGeracao;

	vec_info_ids m_vecInfoIds;
	CRITICAL_SECTION	m_csInfoIds;

	bool	m_bShowInfoId;
	bool	m_bShowInfoGaGenes;


// Operations
public:

// Overrides
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

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
	afx_msg void OnEditEditarch();
};

#ifndef _DEBUG  // debug version in GaBox2dView.cpp
inline CGaBox2dDoc* CGaBox2dView::GetDocument() const
   { return reinterpret_cast<CGaBox2dDoc*>(m_pDocument); }
#endif


#endif //__GABOX2DVIEW_H__