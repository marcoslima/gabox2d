#pragma once

#include "ga.h"
#include "GaBox2dDoc.h"
#include <SFML/Window/Keyboard.hpp>

using namespace GA;

namespace GUI
{

// GaBox2dView.h : interface of the CGaBox2dView class
class CGaBox2dView final
{
public:
	CGaBox2dView();


// Attributes
private:
	unsigned		m_nSimTimer;
	unsigned		m_nVelocidade;
	bool			m_bGaRunning;
	bool			m_bGaExited;

	bool			m_bShowInfoId;
	bool			m_bShowInfoGaGenes;
	bool			m_bWaitingEvolucao;

	CGaBox2dDoc* _pDocument;

public:
	[[nodiscard]] CGaBox2dDoc* 	GetDocument() const;
	void SetDocument(CGaBox2dDoc* pDoc);

	CGa*			getGa();
	void 			releaseGa();

	void OnDraw();

	// Implementation
	static void _draw_sky(sf::RenderWindow &window, const CEnv &env);

	static void _draw_ground(sf::RenderWindow &window, const CGaBox2dDoc *pDoc);

	static void _draw_border(sf::RenderWindow & window, const MODEL::CEnv & env);

	void Draw(sf::RenderWindow& window) const;

	static void OnSimulaPlay();

	void OnSimulaReset() const;
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

	void OnKeyPressed(sf::Keyboard::Key key) const;
	void OnKeyReleased(sf::Keyboard::Key key) const;
};

}
