#pragma once

#include "ga.h"
#include "GaParamsDlg.h"
#include "IdInfoDlg.h"
#include "GaInfoDlg.h"
#include "GaBox2dDoc.h"
#include <SFML/Window/Keyboard.hpp>

namespace GUI
{
// Paramstros para o thread:
// class CThreadParams
// {
// public:
// 	HANDLE			m_hStopGa	;
// 	HANDLE			m_hGaStopped;
// 	ga_params_t		m_Params	;
// 	CGaInfo*		m_pGaInfo	;
// 	CEnv			m_env		;
// };


// GaBox2dView.h : interface of the CGaBox2dView class
class CGaBox2dView final
{
public:
	CGaBox2dView();


// Attributes
private:
	// UI
	// CFont		m_fntSmall;
	// CFont		m_fntSupersmall;

	// Outros	
	unsigned		m_nSimTimer;

	unsigned		m_nVelocidade;
	bool			m_bGaRunning;
	bool			m_bGaExited;

	bool			m_bShowInfoId;
	bool			m_bShowInfoGaGenes;
	// CThreadParams	_thread_params;
	// CGaInfo			m_GaInfo;
	bool			m_bWaitingEvolucao;

private:
	CGaBox2dDoc* _pDocument;

public:
	[[nodiscard]] CGaBox2dDoc* 	GetDocument() const;
	void SetDocument(CGaBox2dDoc* pDoc);

	CGa*			getGa();
	void 			releaseGa();

// GUI
public:
	CIdInfoDlg*	m_pdlgIdInfo;
	CGaInfoDlg*	m_pdlgGaInfo;

// Overrides
public:
	void OnDraw();
// virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	void OnInitialUpdate(); // called first time after construct
	// afx_msg	LRESULT OnGaInfo(WPARAM wParma, LPARAM lParam);
	// afx_msg	LRESULT OnSimularGene(WPARAM wParma, LPARAM lParam);

// Implementation
public:
	virtual ~CGaBox2dView();

public:
	static void _draw_sky(sf::RenderWindow &window, const CEnv &env);

	static void _draw_ground(sf::RenderWindow &window, const CGaBox2dDoc *pDoc);

	static void _draw_border(sf::RenderWindow & window, const MODEL::CEnv & env);

	void Draw(sf::RenderWindow& window) const;
	void OnSimulaPlay();

	void OnSimulaReset();
	void OnVelocidadeMais();
	void OnVelocidadeMenos();
	void OnSimulaRepetir();
	void OnVelocidade1x();
	void OnVelocidade2x();
	void OnVelocidade4x();
	void OnVelocidade10x();
	void OnVelocidade100x();
	void OnGaIniciarga();
	void OnEditPaste();
	void OnMostrarMelhor();
	void OnMostrarQualquer();
	void OnViewCpuinfo();
	void OnInformaGagenes();
	void OnInformaIndividuoatual();
	void OnGaExtins();
	void OnGaColar();
	void OnInformaEvolu();
	void OnGaMudarpar();

public:
	void OnKeyPressed(const sf::Keyboard::Key key) const;

	void OnKeyReleased(sf::Keyboard::Key key) const;
};

};//namespace GUI
