#ifndef __LMFISICA_H__
#define __LMFISICA_H__
// M�dulo de f�sica da LibMarcos: LmFisica
////////////////////////////////////////////

#include "lmmath.h"
#include <stdexcept>

// Tipo doublex: double com incerteza
#include "doublex.h"

namespace LmFisica
{

double Round(double num, int nCasas = 0);

// Defines de c�lculos muito utilizados:
// Quadratura:
#define QUADRATURA(a,b) sqrt((double)((a)*(a))+((b)*(b)))

// Compatibilidade de valores com incertezas diferentes:
#define COMPATIVEL(v1,i1,v2,i2) (abs(dVal1-dVal2) / QUADRATURA(i1,i2) <= 3)

#define VECDBL vector<double>
typedef vector<pair<double,double> > TVecPairDouble;

// Soma dos elementos
template<class T> T Somatorio(const vector<T>&vecValores)
{
	T aTot(0);
	size_t i,nSize = vecValores.size();

	for(i = 0; i < nSize; i++)
	{
		aTot += vecValores[i];
	}

	return aTot;
}

// Soma dos elementos ao quadrado (cada um ao quadrado, e ent�o somados)
template<class T> T Somatorio2(vector<T>&vecValores)
{
	T aTot(0);
	size_t i,nSize = vecValores.size();

	for(i = 0; i < nSize; i++)
	{
		aTot += (vecValores[i]*vecValores[i]);
	}

	return aTot;
}

template<class T, class R > R Media(const vector<T>&vec)
{
	return (R)Somatorio<T>(vec)/vec.size();
}

// Desvio padr�o:
template<class T, class R> R StdDev(const vector<T> &vec)
{
	R aMed = Media<T,R>(vec);
	R aTot(0.0);
	size_t i,nSize = vec.size();

	if(nSize <= 1)
		return 0;

	for(i = 0; i < nSize; i++)
	{
		aTot += (vec[i]-aMed)*(vec[i]-aMed);
	}

	return (R)(::sqrt((R)(aTot / (R)(nSize-1))));
}
template<class T, class R> R Mediana(const vector<T>& vec)
{
	list<T> lst;
	typename vector<T>::size_type i,size = vec.size();
	switch(size)
	{
	case 0:
		return (R) (0);
	case 1:
		return (R) vec[0];
	case 2:
		return (R) ((vec[0] + vec[1])/2.0);
	}
	// Se chegar aqui, o tamanho � maior que 2:

	for(i = 0; i < size; i++)
	{
		lst.push_back(vec[i]);
	}
	lst.sort();

	if(size % 2 == 0)
	{
		size /= 2;
		return (R) ((vec[size]+vec[size+1])/2.0);
	}
	else
	{
		size = (vector<T>::size_type) ((size - 1) / 2.0) + 1;
		return (R) (vec[size]);
	}
}

// Media ponderada: recebe vetor com valores (v = vecValores[i], e vetor com pesos (p = vecPesos[i])
//                                             i                                     i
/*
         n
        ====
        \
         >    p  v
        /      i  i
        ====
        i = 1
mp = ------------------
         n
        ====
        \
         >    p
        /      i
        ====
        i = 1
*/
template<class T, class R>
 R MediaPonderada(vector< T>&vecValores, vector< T>&vecPesos)
{
	 R aTot(0);
	size_t i,nSize = vecValores.size();
	if(vecPesos.size() != nSize)
	{
		throw invalid_argument("Vetor de pesos tem tamanho diferente do vetor de valores");
		throw length_error("Comprimento do vetor de pesos diferente do vetor de valores");
	}

	for(i = 0; i < nSize; i++)
	{
		aTot += vecValores[i] * vecPesos[i];
	}

	return aTot / Somatorio< T>(vecPesos);
}

// domingo, 13 de dezembro de 2009 10:16:13 StdPonderada parece estar incompleta
template<class T, class R>
 R StdevPonderada(vector< T>&vecValores, vector< T>&vecPesos)
{
	 R aTot(0);
	size_t i,nSize = vecValores.size();
	if(vecPesos.size() != nSize)
	{
		throw invalid_argument("Vetor de pesos tem tamanho diferente do vetor de valores");
		throw length_error("Comprimento do vetor de pesos diferente do vetor de valores");
	}

	for(i = 0; i < nSize; i++)
	{
		aTot += vecValores[i] * vecPesos[i];
	}

	return aTot / Somatorio< T>(vecPesos);
}

void MediaPonderada(vector<double>&vecValores, vector<double>&vecIncertezas, double& outMedia, double& outIncerteza);

// Obt�m M�dia e Desvio padr�o de um std::vector, ambos num tipo indicado (class R):
template<class T, class R> void Stats(vector< T>&vecValores,  R& retMedia,  R& retStdDev)
{
	retMedia  = Media<T,R> (vecValores);
	retStdDev = StdDev<T,R>(vecValores);
	return;
}

// Mant�m apenas algarismos significativos:
int Significativos(double& grandeza, double& incerteza, bool cientifico = false);
double SigniVal(const double& aV, const double& aS);
double SigniS(const double& aS);

// Regress�o linear simples
void Inclinacao(TVecPairDouble vecValores, double& a, double& b);






class CAngulo
{
private:
	doublex m_Val; // radianos
	doublex m_degVal;	// Graus
public:
	doublex GetDeg(void);
	doublex GetRad(void);
	void SetDeg(doublex aDeg);
	void SetRad(doublex aRad);
	doublex GetSin(void);
	doublex GetCos(void);
	doublex GetTan(void);
};


} // namespace LmFisica
#endif //__LMFISICA_H__
