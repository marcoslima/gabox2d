// GaParamsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GaBox2d.h"
#include "GaParamsDlg.h"


// Codificação genética do carro:
/*

Para a base 26 temos: 

  dígitos	números
	1			 26
	2			676
	3		 17.576
	4		456.976
	5	 11.881.376
	6	308.915.776


@@@ @@@ @@@ @@@ @@@ @@@ @@@ @@@ @@@ @@@ @@@ @@@ @@@ @@@ @@@ @@@ @@@ @@@ @@@ @@@ @@@ @@@ @@@ @@@ @@
--- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 x   y   r   d   f  res  x   y   r   d   f  res  x   y   r   d   f  res  x   y   r   d   f  res TQ
	R O D A   1			    R O D A   2			    P E S O   1			    P E S O  2			


@@@@@@@@@@
@@@@@@@@@@
@@@@@@@@@@
@@@@@@@@@@
@@@@@@@@@@
@@@@@@@@@@
@@@@@@@@@@
@@@@

78 digitos

x e y da roda 1:
-5 <= x <= 5
Precisão: 3 casas decimais: [-5000, 5000] -> 10000 unique numbers

raio: entre 0.2 e 1 -> 4 casas -> 2000 - 10000 = 8000
densidade: entre 0.1 e 1 -> 4 casas
friction: entre 0 e 1 -> 4 casas
restitution: entre 0 e 1 -> 4 casas
Torque: entre 1 e 20 -> 1 casa: 10 - 200 = 190
*/


// CGaParamsDlg dialog

IMPLEMENT_DYNAMIC(CGaParamsDlg, CDialog)
CGaParamsDlg::CGaParamsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGaParamsDlg::IDD, pParent)
	, m_nPopulacao(60)
	, m_strCrossover("75")
	, m_nElitismo(2)
	, m_strMutacao("65")
	, m_nAlienismo(0)
	, m_nMutInt(10)
	, m_dMaxT(60.0)
{
}

CGaParamsDlg::~CGaParamsDlg()
{
}

void CGaParamsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_POPULACAO, m_nPopulacao);
	DDX_Text(pDX, IDC_CROSSOVER, m_strCrossover);
	DDX_Text(pDX, IDC_ELITISMO, m_nElitismo);
	DDX_Text(pDX, IDC_MUTACAO, m_strMutacao);
	DDX_Text(pDX, IDC_ALIENISMO, m_nAlienismo);
	DDX_Text(pDX, IDC_MUT_INT, m_nMutInt);
	DDX_Text(pDX, IDC_MAX_T, m_dMaxT);
}


BEGIN_MESSAGE_MAP(CGaParamsDlg, CDialog)
END_MESSAGE_MAP()


// CGaParamsDlg message handlers
