#pragma once

#include <math.h>
#include <string>
#include <vector>

#define SQUAREDX(a)	(a^2.0)
#define SQRTX(a)	(a^0.5)
#define ZEROX doublex(0.0,0.0)

namespace LmFisica
{
class doublex
{
	double dV;	// Valor
	double dS;	// Incerteza
public:
	// Constructors:
	doublex();

	explicit doublex(double aVal, double aS = 0.0);

	// Copy constructor:
	doublex(const doublex& other);

// Operações

	doublex operator +(const doublex &other) const;
	doublex operator -(const doublex &other) const;
	doublex operator *(const doublex &other) const;
	doublex operator /(const doublex &other) const;
	doublex operator ^(const doublex &other) const;

	// <operator>=
	doublex operator +=(const doublex& other);
	doublex operator -=(const doublex& other);
	doublex operator *=(const doublex& other);
	doublex operator /=(const doublex& other);
	doublex operator ^=(const doublex& other);

// Operações com double:
	doublex operator +(double other) const;
	doublex operator -(double other) const;
	doublex operator *(double other) const;
	doublex operator /(double other) const;
	doublex operator ^(double other) const;

	doublex operator += (double other);
	doublex operator -= (double other);
	doublex operator *= (double other);
	doublex operator /= (double other);
	doublex operator ^= (double other);

// Unário:
	doublex operator -();

// Atribuições:
	doublex& operator =(const doublex &other);
	doublex& operator =(double other);
	doublex& operator =(long   other);
	doublex& operator =(int    other);

// Comparações:
	bool operator < (const doublex &other) const;
	bool operator > (const doublex& other) const;
	bool operator <=(const doublex& other) const;
	bool operator >=(const doublex& other) const;
	bool operator ==(const doublex& other) const;
	bool operator !=(const doublex& other) const;

// Conversão para double:
//	operator double(){return dV;} // Melhor deixar explicito.
// Conversão para int:
	explicit operator int(){return static_cast<int>(dV);}

// Perfumaria:
	// String
	std::string str() const;

	double S() const {return dS;}
	double V() const {return dV;}
};

////////////////////////////////////////////////////////
// Overloads fora da classe:
doublex sqrt(const doublex&  aVal);
doublex sin (const doublex&  aVal);
doublex cos (const doublex&  aVal);
doublex tan (const doublex&  aVal);
doublex asin(const doublex&  aVal);
doublex acos(const doublex&  aVal);
doublex atan(const doublex&  aVal);
doublex sinh(const doublex&  aVal);
doublex cosh(const doublex&  aVal);
doublex tanh(const doublex&  aVal);

doublex floor(const doublex& aVal);
doublex ceil (const doublex& aVal);

doublex signi(const doublex& aVal);
doublex mod(const doublex& left, const doublex& right);
doublex log(const doublex& numero, double base = 10.0);

// Operadores com double à esquerda:
// FIXME: terminar os operadores
doublex operator *(double left, const doublex &right);
bool operator <(double left, doublex right);
bool operator >(double left, doublex right);


// Funções especializadas doublex:
doublex Somatorio (const std::vector<doublex> &vecValores);
doublex Somatorio2(const std::vector<doublex> &vecValores);
doublex Media     (const std::vector<doublex> &vec);
doublex StdDev	  (const std::vector<doublex> &vec);
}
