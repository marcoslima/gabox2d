#pragma once

#include <math.h>
#include <float.h>
#include <string>
#include <vector>
#include <list>

#define SQUAREDX(a)	(a^2.0)
#define SQRTX(a)	(a^0.5)
#define ZEROX doublex(0.0,0.0)

namespace LmFisica
{
class doublex
{
private:
	double dV;	// Valor
	double dS;	// Incerteza
public:
	// Constructors:
	doublex();
	doublex(double aVal, double aS = 0.0);

	// Copy constructor:
	doublex(const doublex& other);

// Opera��es

	doublex operator +(doublex other) const;
	doublex operator -(doublex other) const;
	doublex operator *(doublex other) const;
	doublex operator /(doublex other) const;
	doublex operator ^(doublex other) const;

	// <operator>=
	doublex operator +=(const doublex& other);
	doublex operator -=(const doublex& other);
	doublex operator *=(const doublex& other);
	doublex operator /=(const doublex& other);
	doublex operator ^=(const doublex& other);

// Opera��es com double:
	doublex operator +(const double other) const;
	doublex operator -(const double other) const;
	doublex operator *(const double other) const;
	doublex operator /(const double other) const;
	doublex operator ^(const double other) const;

	doublex operator += (const double other);
	doublex operator -= (const double other);
	doublex operator *= (const double other);
	doublex operator /= (const double other);
	doublex operator ^= (const double other);

// Un�rio:
	doublex operator -(void);

// Atribui��es:
	doublex operator =(const doublex other);
	doublex operator =(const double other);
	doublex operator =(const long   other);
	doublex operator =(const int    other);

// Compara��es:
//	bool operator < (const doublex  other);
	bool operator > (const doublex& other);
	bool operator <=(const doublex& other);
	bool operator >=(const doublex& other);
	bool operator ==(const doublex& other);
	bool operator !=(const doublex& other);

// Convers�o para double:
//	operator double(){return dV;} // Melhor deixar explicito.
// Convers�o para int:
	operator int(){return int(dV);}

// Perfumaria:
	// String
	std::string str(void);
public:
	double S(void) const {return dS;}
	double V(void) const {return dV;}
};

////////////////////////////////////////////////////////
// Overloads fora da classe:
doublex sqrt(doublex  aVal);
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

// Operadores com double � esquerda:
// FIXME: terminar os operadores
doublex operator *(double left, doublex right);
bool operator <(double left, doublex right);
bool operator >(double left, doublex right);

bool operator <(doublex left, doublex right);


// Fun��es especializadas doublex:
doublex Somatorio (const std::vector<doublex> &vecValores);
doublex Somatorio2(const std::vector<doublex> &vecValores);
doublex Media     (const std::vector<doublex> &vec);
doublex StdDev	  (const std::vector<doublex> &vec);
};//namespace LmFisica
