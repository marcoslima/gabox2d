#include "doublex.h"
#include "lmfisica.h"
#include <cstdio>
#include <limits>

namespace LmFisica
{
doublex::doublex()
{
	dV = 0.0;
	dS = DBL_EPSILON;
}

// Se aS for zero, consideramos o menor valor poss�vel para double
// Assim a incerteza ser� o mais pr�xima poss�vel de zero, mas n�o ser� zero.
doublex::doublex(double aVal, double aS)
{
	dV = aVal;

	if(aS == 0.0)
		dS = DBL_EPSILON;
	else
		dS = aS;
}

doublex::doublex(const doublex& other)
{
	dV = other.dV;
	dS = other.dS;
}

// Adi��o
doublex doublex::operator +(doublex other) const
{
	return doublex(dV+other.dV,::sqrt((double)pow(dS,2)+pow(other.dS,2)));
}

// Subtra��o
doublex doublex::operator -(doublex other) const
{
	return doublex(dV-other.dV,::sqrt((double)pow(dS,2)+pow(other.dS,2)));
}

// Multiplica��o
doublex doublex::operator *(doublex other) const
{
	return doublex
		   (
				dV*other.dV,
				::sqrt
				(
					pow(other.dV,2.0) * pow(dS,2.0)
					+
					pow(dV,2.0) * pow(other.dS,2.0)
				)
			);
}

// Divis�o
doublex doublex::operator /(doublex other) const
{
	return	doublex
			(
				dV / other.dV,
				::sqrt
				(
					pow(dS/other.dV,2.0)
					+
					pow(dV*other.dS,2.0)/pow(other.dV,4.0)
				)
			);
}

// Pot�ncia:
doublex doublex::operator^(doublex other) const
{
	return	doublex(pow(dV,other.dV),
				::sqrt
				(
					pow(dS * other.dV * pow(dV,other.dV-1),2.0)
					+
					pow(pow(dV,other.dV)*::log(dV)*other.dS, 2.0)
				)
			);
}

doublex doublex::operator +=(const doublex& other)
{
	*this  = *this + other;
	return *this;
}

doublex doublex::operator -=(const doublex& other)
{
	*this = *this - other;
	return *this;
}

doublex doublex::operator *=(const doublex& other)
{
	*this = *this * other;
	return *this;
}

doublex doublex::operator /=(const doublex& other)
{
	*this = *this / other;
	return *this;
}

doublex doublex::operator ^=(const doublex& other)
{
	*this = *this ^ other;
	return *this;
}

// Opera��es com n�meros sem incerteza (double)
doublex doublex::operator +(const double other) const
{
	return doublex(dV+other,dS);
}

doublex doublex::operator -(const double other) const
{
	return doublex(dV-other,dS);
}

doublex doublex::operator *(const double other) const
{
	return doublex(dV*other,dS*other);
}

doublex doublex::operator /(const double other) const
{
	return doublex(dV/other,dS/other);
}
doublex doublex::operator ^(const double other) const
{
	return doublex(pow(dV,other), other * pow(dV,other-1) * dS);
}

////////////////////////
doublex doublex::operator += (const double other)
{
	*this = *this + other;
	return *this;
}

doublex doublex::operator -= (const double other)
{
	*this = *this - other;
	return *this;
}

doublex doublex::operator *= (const double other)
{
	*this = *this * other;
	return *this;
}

doublex doublex::operator /= (const double other)
{
	*this = *this / other;
	return *this;
}

doublex doublex::operator ^= (const double other)
{
	*this = *this ^ other;
	return *this;
}

doublex doublex::operator -(void)
{
	return doublex(-dV,dS);
}

////////////////
// Atribui��es:
doublex doublex::operator =(const doublex other)
{
	dV = other.dV;
	dS = other.dS;
	return *this;
}

doublex doublex::operator =(const double other)
{
	dV = other;
	dS = DBL_EPSILON;
	return *this;
}

doublex doublex::operator =(const long   other)
{
	dV = (double)other;
	dS = DBL_EPSILON;
	return *this;
}

doublex doublex::operator =(const int    other)
{
	dV = (double)other;
	dS = DBL_EPSILON;
	return *this;
}

// Compara��es:
bool doublex::operator <(const doublex other)
{
	return dV < other.dV;
}
bool doublex::operator >(const doublex& other)
{
	return dV > other.dV;
}
bool doublex::operator <=(const doublex& other)
{
	return dV < other.dV || *this == other;
}
bool doublex::operator >=(const doublex& other)
{
	return dV > other.dV || *this == other;
}
bool doublex::operator ==(const doublex& other)
{
	return fabs(dV-other.dV)/::sqrt(pow(dS,2.0)+pow(other.dS,2.0)) <= 3.0;
}
bool doublex::operator !=(const doublex& other)
{
	return fabs(dV-other.dV)/::sqrt(pow(dS,2.0)+pow(other.dS,2.0)) > 3.0;
}

////////////////////////////////////////////////////////
// Overloads fora da classe:
doublex sqrt(doublex aVal)
{
	return doublex(::sqrt(aVal.V()),aVal.S()*::sqrt(1.0/aVal.V()));
}

// Trigonom�tricas:
doublex sin(const doublex& aVal)
{
	return doublex(::sin(aVal.V()),fabs(::cos(aVal.V())*aVal.S()));
}

doublex cos(const doublex& aVal)
{
	return doublex(::cos(aVal.V()),fabs(::cos(aVal.V())*aVal.S()));
}

doublex tan(const doublex& aVal)
{
	return doublex(::tan(aVal.V()),fabs((1/pow(::cos(aVal.V()),2.0))*aVal.S()));
}

doublex asin(const doublex& aVal)
{
	return doublex(::asin(aVal.V()),fabs(aVal.S()/::sqrt(1-(aVal.V()*aVal.V()))));
}

doublex acos(const doublex& aVal)
{
	return doublex(::acos(aVal.V()),fabs(aVal.S()/::sqrt(1-(aVal.V()*aVal.V()))));
}

doublex atan(const doublex& aVal)
{
	return doublex(::atan(aVal.V()),fabs(aVal.S()/(1+aVal.V()*aVal.V())));
}
doublex sinh(const doublex&  aVal)
{
	return doublex(::sinh(aVal.V()),fabs(::cosh(aVal.V())*aVal.S()));
}
doublex cosh(const doublex&  aVal)
{
	return doublex(::cosh(aVal.V()),fabs(::sinh(aVal.V())*aVal.S()));
}
doublex tanh(const doublex&  aVal)
{
	return doublex(::tanh(aVal.V()),fabs((1-::tanh(aVal.V())*::tanh(aVal.V()))*aVal.S()) );
}

doublex floor(const doublex& aVal)
{
	return doublex(::floor(aVal.V()),aVal.S()+1);
}

doublex ceil (const doublex& aVal)
{
	return doublex(::ceil(aVal.V()),aVal.S()+1);
}


doublex signi(const doublex& aVal)
{
	double dV, dS;
	dV = aVal.V();
	dS = aVal.S();
	Significativos(dV,dS,false);
	return doublex(dV,dS);
}

doublex operator *(double left, doublex right)
{
	return right * left;
}

doublex mod(const doublex& left, const doublex& right)
{
	return doublex
	(
		fmod(left.V(),right.V()),
		::sqrt(pow(left.S(), 2.0) + pow(right.S(), 2.0))
	);
}

doublex log(const doublex& numero, double base)
{
	return doublex(	::log(numero.V()) / ::log(base),
		::fabs( numero.S() / (numero.V() * ::log(base)) )	 );
}

string doublex::str(void)
{
	static char szFmt[64];
	sprintf(szFmt,"%f ± %f", dV, dS);
	return string(szFmt);
}

// Fun��es de estat�stica, copiados de lmfisica.h, adaptados do template para o doublex
doublex Somatorio(const vector<doublex> &vecValores)
{
	doublex aTot(0,0);
	size_t i,nSize = vecValores.size();

	for(i = 0; i < nSize; i++)
	{
		aTot += vecValores[i];
	}

	return aTot;
}

// Soma dos elementos ao quadrado (cada um ao quadrado, e ent�o somados)
doublex Somatorio2(const vector<doublex> &vec)
{
	doublex aTot(0,0);
	size_t i,nSize = vec.size();

	for(i = 0; i < nSize; i++)
	{
		aTot += (vec[i]*vec[i]);
	}

	return aTot;
}

doublex Media(const vector<doublex> &vec)
{
	return Somatorio(vec) / (double)vec.size();
}

// Desvio padr�o:
doublex StdDev(const vector<doublex> &vec)
{
	doublex aMed = Media(vec);
	doublex aTot(0.0,0.0);
	size_t i,nSize = vec.size();

	if(nSize <= 1)
		return ZEROX;

	for(i = 0; i < nSize; i++)
	{
		aTot += (vec[i]-aMed)*(vec[i]-aMed);
	}

	doublex tmp = aTot / doublex(nSize-1,0);
	return sqrt(tmp);
}

/////////////////////////////////////////////////////////
}//namespace LmFisica
