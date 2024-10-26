#pragma once

#include <iostream>
using namespace std;

namespace GUI
{
// CGaParamsDlg dialog
class ga_params_t
{
public:
	uint8_t		m_nPopulacao	;
	float		m_fCrossover	;
	uint8_t		m_nElitismo		;
	float 		m_fMutacao		;
	uint8_t		m_nAlienismo	;
	uint8_t		m_nMutInt		;
	float		m_fMaxT			;

public:
	ga_params_t(
		uint8_t	nPopulacao	,
		float	fCrossover	,
		uint8_t	nElitismo	,
		float	fMutacao	,
		uint8_t	nAlienismo	,
		uint8_t	nMutInt		,
		float	fMaxT		)
	{
		m_nPopulacao	= nPopulacao	;
		m_fCrossover	= fCrossover	;
		m_nElitismo		= nElitismo		;
		m_fMutacao		= fMutacao	;
		m_nAlienismo	= nAlienismo	;
		m_nMutInt		= nMutInt		;
		m_fMaxT			= fMaxT			;
	}

	ga_params_t(){}
};

class CGaParamsDlg : public ga_params_t
{
public:
	CGaParamsDlg();   // standard constructor

	void show(void);
	void draw(void);
};

};//namespace GUI
