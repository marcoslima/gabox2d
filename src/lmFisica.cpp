#include <lmfisica.h>
#include <doublex.h>
#include <cmath>
#include <limits>
#include <cstdint>
#include <ranges>
#include <bits/ranges_algo.h>

using namespace LmFisica;

namespace LmFisica
{

#if 0
double Average(VECDBL& vec)
{
	double aTot(0.0);
	size_t i,nSize = vec.size();

	for(i = 0; i < nSize; i++)
	{
		aTot += vec[i];
	}

	return aTot / (double)nSize;
}

// Desvio padr�o
double StdDev (VECDBL& vec)
{
	double aMed = Average(vec);
	double aTot(0.0);
	size_t i,nSize = vec.size();

	for(i = 0; i < nSize; i++)
	{
		aTot += pow(vec[i]-aMed,2);
	}

	return sqrt(aTot / (nSize-1));
}
#endif

doublex CAngulo::GetDeg()
{
	/*
		 pi     rad             rad * 180
		---- = ------ -> deg = -----------
		180     deg                 pi
	*/
	return m_degVal;
}

doublex CAngulo::GetRad()
{
	return m_Val;
}

void CAngulo::SetDeg(const doublex &aDeg)
{
	m_degVal = aDeg;
	m_Val = aDeg * M_PI / 180.0;
}

void CAngulo::SetRad(const doublex &aRad)
{
	m_Val = aRad;
	m_degVal = aRad * 180.0 / M_PI;
}
doublex CAngulo::GetSin()
{
	return sin(m_Val);
}
doublex CAngulo::GetCos()
{
	return cos(m_Val);
}
doublex CAngulo::GetTan()
{
	return tan(m_Val);
}

//////////////////////////////////////
// Arredondamento na física:
// Para aaaaab.cdddd:
// Se cdddd <  5; mantém b.
// Se cdddd >  5; b+1
// Se cdddd == 5: se b deve ficar par.
//////////////////////////////////////
// Vuolo, José Henrrique,
// Fundamentos da Teoria de Erros,
// 2ª edição,
// Editora Edgard Blücher Ltda,
// São Paulo (1996).
// (pág. 71)
///////////////////////////////////////////////
double Round(double num, const int nCasas)
{
	double dRet = 0;

	// Multiplicamos para que fique com o n�mero de casas
	// decimais desejado exatamente na v�rgula:
	if(nCasas != 0)
	    num *= pow(10,nCasas);

	// Isolamos o n�mero que determinar� o arredondamento:
	double dInt;
	const double dFrac = modf(num, &dInt);

	if(dFrac < 0.5)
		dRet = dInt;
	else if(dFrac > 0.5)
		dRet = dInt+1;
	else
	{
		if( static_cast<int64_t>(dInt) % 2 == 1 )
		if( static_cast<int64_t>(dInt) % 2 == 1 )
			dRet = dInt + 1;
		else
			dRet = dInt;
	}

	// Voltamos o número à sua ordem real:
	if(nCasas != 0)
	    return dRet / pow(10,nCasas);
	return dRet;
}

int Significativos(double& grandeza, double& incerteza, const bool cientifico)
{
    // Expoente da potência de 10
    double dExp10 = pow(10, std::floor(log10(incerteza)));

	// Reduzimos os números à ordem zero ( 1 <= x < 10 )
    double grd0 = grandeza  / dExp10;
    if(fabs(grd0) == numeric_limits<double>::infinity())
	{
		// Passamos a ignorar a incerteza, pois est� gerando erro
		// Bateu nos limites.
		dExp10 = pow(10, std::floor(log10(grandeza)));
		grd0 = grandeza / dExp10;
	}
    const double inc0 = incerteza / dExp10;

    // Limitamos a incerteza aos algarismos significativos:
	incerteza = Round(inc0, inc0 < 3.0 ? 1 : 0);

    // Limitamos também a grandeza:
	grandeza = Round(grd0, incerteza < 3.0 ? 1 : 0);

    if(!cientifico)
    {
		// Retornamos eles à ordem original:
		grandeza *= dExp10;
		incerteza *= dExp10;
    }

    return static_cast<int>(log10(dExp10));
}

double SigniVal(const double& aV, const double& aS)
{
	double v(aV),s(aS);
	Significativos(v,s,false);
	return v;
}
double SigniS(const double& aS)
{
	double v(0.0),s(aS);
	Significativos(v,s,false);
	return s;
}

void Inclinacao(vec_pair_dbl_t vecValores, double& a,  double& b)
{
	double sx(0), sy(0), sxy(0), sx2(0);
	const auto n = static_cast<double>(vecValores.size());
	// for(auto it = vecValores.begin(); it!= vecValores.end(); ++it)
	ranges::for_each(vecValores, [&](const auto& it)
	{
		sx  += it.first;
		sy  += it.second;
		sxy += it.first*it.second;
		sx2 += it.first*it.first;
	});

	const double xm = sx / n;
	const double ym = sy / n;
	a = (sxy - n * xm * ym)/(sx2-n*(xm*xm));
	b = ym - a*xm;
}

void MediaPonderada(vector<double>&vecValores, vector<double>&vecIncertezas, double& outMedia, double& outIncerteza)
{
	double aTot(0.0);
	const size_t nSize = vecValores.size();
	if(vecIncertezas.size() != nSize)
	{
		throw invalid_argument("Vetor de pesos tem tamanho diferente do vetor de valores");
	}

	double sumPesos = 0.0;
	for(size_t i = 0; i < nSize; i++)
	{
		const double peso = 1.0 / (vecIncertezas[i] * vecIncertezas[i]);
		sumPesos += peso;
		aTot += vecValores[i] * peso;
	}

	outMedia	 = aTot / sumPesos;
	outIncerteza = ::sqrt(1.0/sumPesos);
}

} // namespace LmFisica
