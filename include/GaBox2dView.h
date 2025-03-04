#pragma once

#include <thread>

#include "ga.h"
#include "GaBox2dDoc.h"
#include <SFML/Window/Keyboard.hpp>

#include "GaParamsDlg.h"
#include "ga_params.h"

using namespace GA;

namespace GUI
{
// Paramstros para o thread:
class CThreadParams
{
public:
	atomic<bool>	m_bStopGa	;
	ga_params_t m_Params	;
	CEnv		m_env		;

	// HANDLE		m_hStopGa	;	// TODO: Substituir por mutex?
	// HANDLE		m_hGaStopped;	// TODO: Substituir por mutex?
	// HWND			m_wndNotify	;
	// ga_params_t		m_Params	;
	// CGaInfo*		m_pGaInfo	;
};

class CGaParamsDlg;

// GaBox2dView.h : interface of the CGaBox2dView class
class CGaBox2dView final
{

public:
	CGaBox2dView();


	[[nodiscard]] unsigned getVelocidade() const
	{return m_nVelocidade;}
	void setVelocidade(const unsigned nVelocidade) // NOLINT(*-convert-member-functions-to-static)
	{m_nVelocidade = nVelocidade;}

	[[nodiscard]] bool isGaRunning() const;
// Attributes
private:
	unsigned		m_nVelocidade;
	bool			m_bGaRunning;
	bool			m_bGaExited;

	bool			m_bShowInfoId;
	bool			m_bShowInfoGaGenes;

	CThreadParams   _thread_params;
	bool			m_bWaitingEvolucao;
	thread			_ga_thread;


	// UI:
	float m_ZoomFactor = 1.0f;
	bool m_bZoomIn = false;
	bool m_bZoomOut = false;
	sf::Vector2f m_view_pos = {0.0f, 0.0f};
	bool m_bMoveLeft = false;
	bool m_bMoveRight = false;
	bool m_bMoveUp = false;
	bool m_bMoveDown = false;
	bool m_bShowHelp = false;
	bool m_bDrawDebugGround = false;

	CGaBox2dDoc* _pDocument;
	bool m_bFollowCar = false;

public:
	[[nodiscard]] CGaBox2dDoc* 	GetDocument() const;
	void SetDocument(CGaBox2dDoc* pDoc);

	CGa*			getGa();
	void 			releaseGa();
	[[nodiscard]] bool isShowHelp() const { return m_bShowHelp; }
	[[nodiscard]] bool isDebugGround() const {return m_bDrawDebugGround; }
	void toggleDrawDebugGround(){m_bDrawDebugGround = !m_bDrawDebugGround; }

	void ShowHelp();

	[[nodiscard]] bool isFollowCar() const;

	void toggleFollowCar();

	// Implementation
	static void _draw_sky(sf::RenderWindow &window, const CEnv &env);

	static void _debug_draw_ground(sf::RenderWindow &window, const vec_vecs_t &vecGround);

	void _draw_ground(sf::RenderWindow &window) const;

	static void _draw_border(sf::RenderWindow & window, const MODEL::CEnv & env);

	void Draw(sf::RenderWindow& window);

	void OnSimulaPlay() const;

	void OnSimulaReset() const;
	void OnVelocidadeMais();
	void OnVelocidadeMenos();
	void OnSimulaRepetir() const;
	void OnVelocidade1x();
	void OnVelocidade2x();
	void OnVelocidade4x();
	void OnVelocidade10x();
	void OnVelocidade100x();

	void _stop_ga();

	void _start_ga(const CGaParamsDlg &dlgParams);

	void _show_start_ga_params();

	void OnGaIniciarga();
	void OnEditCopy() const;
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

	void OnKeyPressed(sf::Keyboard::Key key);
	void OnKeyReleased(sf::Keyboard::Key key);

	[[nodiscard]] string getDeadReason() const
	{
		return GetDocument()->GetCar().m_dead_reason;
	}
};

}
