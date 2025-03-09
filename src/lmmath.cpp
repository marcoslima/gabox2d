#if 0


#include "lmmath.h"
#include "primos.h"

// Para agilizar, vamos usar uma tabela
// de n�meros de primos
#include <algorithm>
#include <list>
#include <cmath>
#include <cstdio>

namespace lmmath
{

using namespace std;
list<unsigned long> g_lstPrimos;
void Init(void)
{
	for(unsigned long i = 0; lPrimos[i] > -1; i++)
	{
		g_lstPrimos.push_back(lPrimos[i]);
	}
	return;
}
TFatorado FatoraNumero(unsigned long nNumero)
{
	TFatorado fatorado;

	unsigned long divisor	= 2;
	unsigned long dividendo	= nNumero;

	while(divisor <= dividendo)
	{
		if(dividendo%divisor == 0)
		{
			fatorado.push_back(pair<unsigned long,unsigned long>(dividendo,divisor));
			dividendo /= divisor;
			if(binary_search(g_lstPrimos.begin(), g_lstPrimos.end(), dividendo))
			{
				// � primo! Saimos por tabela!
				fatorado.push_back(pair<unsigned long,unsigned long>(dividendo,dividendo));
				return fatorado;
			}
		}
		else
		{
			divisor++;
		}
	}
	return fatorado;
}

TFracao Fracao(double dVal)
{
	// Quantas casas decimais tem este n�mero
	int nCasas = 0;
	double dInt, dFrac;
	while(true)
	{
		dFrac = modf(dVal,&dInt);
		printf("\nFRACAO::Int: %f, Frac: %f", dInt, dFrac);
		if(dFrac == 0)
			break;
		dVal *= 10.0;
		nCasas++;
	}
	printf(
			"\nFRACAO::N�mero: %f"
			"\nFRACAO::Casas decimais: %d", dVal, nCasas);
	// Fra��o inicial:
	TFracao ret;
	ret.denominador = (unsigned long)pow(10.0,nCasas);
	ret.numerador   = (unsigned long)dVal;

	printf("\nFRACAO::Fracao inicial: %ld/%ld", ret.numerador,ret.denominador);

	TVecLong vecNums;
	vecNums.push_back(ret.denominador);
	vecNums.push_back(ret.numerador);
	unsigned long lComum = Mdc(vecNums);
	ret.denominador /= lComum;
	ret.numerador   /= lComum;

	printf("\nFRACAO::Comum: %ld", lComum);

	return ret;
}

unsigned long Mdc(TVecLong numeros)
{
	// Pegamos o MENOR desses n�meros:
	unsigned long lMin = *min_element(numeros.begin(), numeros.end());

	// Fatoramos eles conjuntamente:
	TVecLong::iterator it;
	bool bComum;
	unsigned long lComum = 1;
	for(unsigned long div = 2; div <= lMin; )
	{
		bComum = true;
		// Cada divisor que dividir todos os n�meros, avan�amos:
		for(it = numeros.begin(); it!= numeros.end(); it++)
		{
			if(*it%div != 0)
			{
				// Este divisor j� n�o � comum. Avan�amos:
				// Descartamos: este divisor n�o divide um deles:
				div++;
				bComum = false;
				break;
			}
		}

		// Se todos eles foram divis�veis por este divisor, entra no comum:
		if(bComum)
		{
			lComum *= div;

			// E dividimos:
			for(it = numeros.begin(); it!= numeros.end(); it++) *it /= div;
		}
	}

	return lComum;
}

unsigned int Fatorial(unsigned int num)
{
	if(num == 0)
		return 1;

	unsigned int ret = 1;
	for(; num > 0; num--)
	{
		ret *= num;
	}
	return ret;
}

unsigned int Binomio(unsigned int row, unsigned int col)
{
	return Fatorial(row)/(Fatorial(col)*Fatorial(row-col));
}

// Obt�m o termo geral de um polinomio da forma (a+b)^n
void TermoGeral(unsigned int expoente, unsigned int zTermo, int*Coef, unsigned int *expoente1, unsigned int *expoente2)
{
	/*
		(a+b)^n =
		( n )           ( n )                      ( n )
		|   | a^n*b^0 + |   | a^(n-1)*b^1 + ... +  |   | a^0*b^n
		( 1 )           ( 2 )                      ( n )
	*/

	if(zTermo > expoente)
	{
		*expoente1 = -1;
		*expoente2 = -1;
		*Coef = -1;
		return;
	}

	*Coef = (int)Binomio(expoente,zTermo);
	*expoente1 = expoente-zTermo;
	*expoente2 = zTermo;
}

void Bilinear(double x, double y, double k[12], double cores[3])
{
	double xy = x*y;
	cores[0] = x*k[0] + y*k[1] + xy*k[ 2] + k[ 3];
	cores[1] = x*k[4] + y*k[5] + xy*k[ 6] + k[ 7];
	cores[2] = x*k[8] + y*k[9] + xy*k[10] + k[11];
}

bool InitBilinear(CBilinearAmostras ams[4], double k[12])
{
	double x[4], y[4], r[4], g[4], b[4];
	for(int i = 0; i < 4; i++)
	{
		x[i] = ams[i].x;
		y[i] = ams[i].y;
		r[i] = ams[i].cores[0];
		g[i] = ams[i].cores[1];
		b[i] = ams[i].cores[2];
	}

	double Delta1 = (-(-x[3] + x[2]) * (-x[1] + x[0]) * y[1] + (-x[3] + x[1]) * (x[0] - x[2]) * y[2] - y[3] * (-x[2] + x[1]) * (x[0] - x[3])) * y[0] + (-(-x[2] + x[1]) * (x[0] - x[3]) * y[2] + y[3] * (-x[3] + x[1]) * (x[0] - x[2])) * y[1] - y[3] * y[2] * (-x[3] + x[2]) * (-x[1] + x[0]);
	double Delta2 = ((-y[3] + y[2]) * (y[0] - y[1]) * x[1] - (y[1] - y[3]) * (-y[2] + y[0]) * x[2] + x[3] * (y[1] - y[2]) * (-y[3] + y[0])) * x[0] + ((y[1] - y[2]) * (-y[3] + y[0]) * x[2] - x[3] * (y[1] - y[3]) * (-y[2] + y[0])) * x[1] + x[2] * x[3] * (-y[3] + y[2]) * (y[0] - y[1]);

	if(Delta1 == 0)
		return false;
	if(Delta2 == 0)
		return false;

	k[ 0] = ((-(-x[1] + x[0]) * (r[2] - r[3]) * y[1] + (x[0] - x[2]) * (r[1] - r[3]) * y[2] - y[3] * (x[0] - x[3]) * (-r[2] + r[1])) * y[0] + (-(-x[2] + x[1]) * (r[0] - r[3]) * y[2] + y[3] * (-x[3] + x[1]) * (r[0] - r[2])) * y[1] - y[3] * y[2] * (-x[3] + x[2]) * (r[0] - r[1])) / Delta1;
	k[ 1] = (((y[0] - y[1]) * (r[2] - r[3]) * x[1] - (-y[2] + y[0]) * (r[1] - r[3]) * x[2] + x[3] * (-y[3] + y[0]) * (-r[2] + r[1])) * x[0] + ((y[1] - y[2]) * (r[0] - r[3]) * x[2] - x[3] * (y[1] - y[3]) * (r[0] - r[2])) * x[1] + x[2] * x[3] * (-y[3] + y[2]) * (r[0] - r[1])) / Delta2;
	k[ 2] = (((r[2] - r[3]) * y[1] + (-r[1] + r[3]) * y[2] + y[3] * (-r[2] + r[1])) * x[0] + ((r[3] - r[2]) * y[0] + (r[0] - r[3]) * y[2] - y[3] * (r[0] - r[2])) * x[1] + ((r[1] - r[3]) * y[0] + (r[3] - r[0]) * y[1] + y[3] * (r[0] - r[1])) * x[2] + ((r[2] - r[1]) * y[0] + (r[0] - r[2]) * y[1] - y[2] * (r[0] - r[1])) * x[3]) / Delta2;
	k[ 3] = ((-(y[0] - y[1]) * (-y[2] * r[3] + y[3] * r[2]) * x[1] + (-y[2] + y[0]) * (y[3] * r[1] - y[1] * r[3]) * x[2] - x[3] * (-y[3] + y[0]) * (y[2] * r[1] - y[1] * r[2])) * x[0] + (-(y[1] - y[2]) * (y[3] * r[0] - y[0] * r[3]) * x[2] + x[3] * (y[1] - y[3]) * (y[2] * r[0] - y[0] * r[2])) * x[1] - x[2] * x[3] * (-y[3] + y[2]) * (y[1] * r[0] - y[0] * r[1])) / Delta2;
	k[ 4] = ((-(-x[1] + x[0]) * (g[2] - g[3]) * y[1] + (x[0] - x[2]) * (-g[3] + g[1]) * y[2] - y[3] * (x[0] - x[3]) * (-g[2] + g[1])) * y[0] + (-(-x[2] + x[1]) * (g[0] - g[3]) * y[2] + y[3] * (-x[3] + x[1]) * (-g[2] + g[0])) * y[1] - y[3] * y[2] * (-x[3] + x[2]) * (-g[1] + g[0])) / Delta1;
	k[ 5] = (((y[0] - y[1]) * (g[2] - g[3]) * x[1] - (-y[2] + y[0]) * (-g[3] + g[1]) * x[2] + x[3] * (-y[3] + y[0]) * (-g[2] + g[1])) * x[0] + ((y[1] - y[2]) * (g[0] - g[3]) * x[2] - x[3] * (y[1] - y[3]) * (-g[2] + g[0])) * x[1] + x[2] * x[3] * (-y[3] + y[2]) * (-g[1] + g[0])) / Delta2;
	k[ 6] = (((g[2] - g[3]) * y[1] + (g[3] - g[1]) * y[2] + y[3] * (-g[2] + g[1])) * x[0] + ((-g[2] + g[3]) * y[0] + (g[0] - g[3]) * y[2] - (-g[2] + g[0]) * y[3]) * x[1] + ((-g[3] + g[1]) * y[0] + (-g[0] + g[3]) * y[1] + y[3] * (-g[1] + g[0])) * x[2] + x[3] * ((g[2] - g[1]) * y[0] + (-g[2] + g[0]) * y[1] - y[2] * (-g[1] + g[0]))) / Delta2;
	k[ 7] = ((-(y[0] - y[1]) * (g[2] * y[3] - g[3] * y[2]) * x[1] + (-y[2] + y[0]) * (y[3] * g[1] - y[1] * g[3]) * x[2] - x[3] * (-y[3] + y[0]) * (-y[1] * g[2] + g[1] * y[2])) * x[0] + (-(y[1] - y[2]) * (y[3] * g[0] - y[0] * g[3]) * x[2] + x[3] * (y[1] - y[3]) * (y[2] * g[0] - y[0] * g[2])) * x[1] - x[2] * x[3] * (-y[3] + y[2]) * (y[1] * g[0] - y[0] * g[1])) / Delta2;
	k[ 8] = ((-(-x[1] + x[0]) * (-b[3] + b[2]) * y[1] + (x[0] - x[2]) * (b[1] - b[3]) * y[2] - y[3] * (x[0] - x[3]) * (b[1] - b[2])) * y[0] + (-(-x[2] + x[1]) * (b[0] - b[3]) * y[2] + y[3] * (-x[3] + x[1]) * (b[0] - b[2])) * y[1] - y[3] * y[2] * (-x[3] + x[2]) * (b[0] - b[1])) / Delta1;
	k[ 9] = (((y[0] - y[1]) * (-b[3] + b[2]) * x[1] - (-y[2] + y[0]) * (b[1] - b[3]) * x[2] + x[3] * (-y[3] + y[0]) * (b[1] - b[2])) * x[0] + ((y[1] - y[2]) * (b[0] - b[3]) * x[2] - x[3] * (y[1] - y[3]) * (b[0] - b[2])) * x[1] + x[2] * x[3] * (-y[3] + y[2]) * (b[0] - b[1])) / Delta2;
	k[10] = (((-b[3] + b[2]) * y[1] + (-b[1] + b[3]) * y[2] + y[3] * (b[1] - b[2])) * x[0] + ((b[3] - b[2]) * y[0] + (b[0] - b[3]) * y[2] - y[3] * (b[0] - b[2])) * x[1] + ((b[1] - b[3]) * y[0] + (b[3] - b[0]) * y[1] + y[3] * (b[0] - b[1])) * x[2] + ((-b[1] + b[2]) * y[0] + (b[0] - b[2]) * y[1] - y[2] * (b[0] - b[1])) * x[3]) / Delta2;
	k[11] = ((-(y[0] - y[1]) * (-y[2] * b[3] + y[3] * b[2]) * x[1] + (-y[2] + y[0]) * (-b[3] * y[1] + y[3] * b[1]) * x[2] - x[3] * (-y[3] + y[0]) * (-b[2] * y[1] + y[2] * b[1])) * x[0] + (-(y[1] - y[2]) * (-y[0] * b[3] + y[3] * b[0]) * x[2] + x[3] * (y[1] - y[3]) * (y[2] * b[0] - y[0] * b[2])) * x[1] - x[2] * x[3] * (-y[3] + y[2]) * (-y[0] * b[1] + b[0] * y[1])) / Delta2;

	return true;
}



#if 0 // Abordagem velha
{
	TFatorado fatorado;
	TFatorado::size_type k;
	vector<TFatorado>				vecFatorados;
	vector<TFatorado::size_type>	vecSizes;
	TFatorado::size_type nMinSize = TFatorado::size_type(LONG_MAX);

	TVecLong::size_type i,nSize = numeros.size();
	for(i = 0; i < nSize; i++)
	{
		fatorado = FatoraNumero(numeros[i]);
		printf("\n-------------------------------\nFatorando %d", numeros[i]);
		for(k = 0; k < fatorado.size(); k++)
		{
			printf("\n%d | %d", fatorado[k].first,fatorado[k].second);
		}
		nMinSize = min(nMinSize,fatorado.size());
		vecFatorados.push_back(fatorado);
	}

	printf("\nMinSize: %d", nMinSize);
	unsigned long lComum = 1;
	for(k = 0; k < nMinSize; k++)
	{
		// Cada elemento da primeira fatora��o ser� procurado
		// nos elementos das demais fatora��es.
		// Se for achado em todas, ele � um comum

		for(i = 1; i < nSize; i++)
		{
			if(!binary_search(vecFatorados[i].begin(),vecFatorados[i].end(),vecFatorados[0][k]))
			{
				// N�o � comum. Podemos passar para o pr�ximo:
			}
			if(vecFatorados[i][k].second != vecFatorados[i+1][k].second)
			{
				goto bailout;
			}
			lComum *= vecFatorados[i][k].second;
		}
	}
bailout:
	return lComum;
}
#endif




} // namespace lmmath

#endif