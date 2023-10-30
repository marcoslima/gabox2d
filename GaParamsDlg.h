#pragma once

#include <iostream>
using namespace std;

namespace GUI
{
// CGaParamsDlg dialog
class ga_params_t
{
public:
	uint64_t	m_nPopulacao	;
	float		m_fCrossover	;
	uint64_t	m_nElitismo		;
	float 		m_fMutacao		;
	uint64_t	m_nAlienismo	;
	uint64_t	m_nMutInt		;
	float		m_fMaxT			;

public:
	ga_params_t(
		uint64_t	nPopulacao	,
		float		fCrossover	,
		uint64_t	nElitismo	,
		float		fMutacao	,
		uint64_t	nAlienismo	,
		uint64_t	nMutInt		,
		float		fMaxT		)
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
