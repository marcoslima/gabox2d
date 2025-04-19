
#pragma once
// Módulo matemático da LibMarcos: LmMath
//////////////////////////////////////////

#include <vector>
using namespace std;
namespace lmmath
{
// Fatora um numero:
typedef vector< pair<unsigned long,unsigned long> > TFatorado;
TFatorado FatoraNumero(unsigned long nNumero);

// Transforma número de ponto flutuante em fração:
typedef struct tagFracao
{
	bool		  negative;
	unsigned long numerador;
	unsigned long denominador;
} TFracao;
TFracao Fracao(double dVal);

typedef vector<unsigned long> TVecLong;
unsigned long Mdc(TVecLong numeros);

// Fatorial:
unsigned int Fatorial(unsigned int num);

// Obt�m o valor de um bin�mio de Newton:
unsigned int Binomio(unsigned int row, unsigned int col);

// Obt�m o termo geral de um polinomio da forma (a+b)^n
void TermoGeral(unsigned int expoente, unsigned int zTermo, int*Coef, unsigned int *expoente1, unsigned int *expoente2);

// Faz interpola��o bilinear de 4 pontos dados:
class CBilinearAmostras
{
public:
	double x;
	double y;
	double cores[3];
	CBilinearAmostras(): x(0), y(0), cores{} {}

	CBilinearAmostras(const double ax, const double ay, const double r, const double g, const double b)
	{x = ax; y = ay; cores[0] = r; cores[1] = g; cores[2] = b;}
};

bool InitBilinear(CBilinearAmostras ams[4], double k[12]);
void Bilinear(double x, double y, double k[12], double cores[3]);

// Mapeamento afim:
// [x' y' 1] = [x y 1]	[ a c  0]
//						[ b d  0]
//						[ox oy 1]
class CLinearMap
{
	double	_a;
	double	_b;
	double	_c;
	double	_d;
	double	_ox;
	double	_oy;

public:
	CLinearMap()
	{
		_a = 1.0;
		_b = 0.0;
		_c = 0.0;
		_d = 1.0;
		_ox = 0.0;
		_oy = 0.0;
	}

	CLinearMap(const double a, const double b, const double c, const double d, const double ox, const double oy)
	{
		_a = a;
		_b = b;
		_c = c;
		_d = d;
		_ox = ox;
		_oy = oy;
	}

/*
a x1 + b y1 + ox = x1'
a x2 + b y2 + ox = x2'
a x3 + b y3 + ox = x3'

|x1 y1 1|
|x2 y2 1| = Delta
|x3 y3 1|

|x1l y1 1|    1
|x2l y2 1| . --- = a
|x3l y3 1|    D

|x1 x1l 1|    1
|x2 x2l 1| . --- = b
|x3 x3l 1|    D

|x1 y1 x1l|    1
|x2 y2 x2l| . --- = ox
|x3 y3 x3l|    D

*/
	CLinearMap(const double x1, const double y1, const double x1l, const double y1l,
				const double x2, const double y2, const double x2l, const double y2l,
				const double x3, const double y3, const double x3l, const double y3l)
	{
		set(x1,y1,x1l,y1l,
			x2,y2,x2l,y2l,
			x3,y3,x3l,y3l);
	}

	// H� casos em que sabemos que n�o h� influ�ncia do y na transforma��o de x
	// nem de x em y.
	// Tamb�m acontece de, nesta situa��o, termos pontos que produzem delta zerado.
	// Neste caso, fa�amos uma transforma��o mais direta (menos gen�rica) considerando isso:
	CLinearMap(const double cx , // Largura no espa�o de entrada
				const double cy , // Altura no espa�o de entrada
				const double cxl, // Largura no espa�o resultante
				const double cyl, // Altura no espa�o resultante
				const double x  , // Este ponto no espa�o de entrada �...
				const double y  ,
				const double xl , // Este ponto no espa�o resultante.
				const double yl )
	{
		// cxl = a cx
		// a = cxl / cx
		_a = cxl / cx;
		_b = 0.0;
		_c = 0.0;
		_d = cyl / cy;

		// ax + _ox = xl
		// _ox = xl - ax
		_ox = xl - _a * x;
		_oy = yl - _d * y;
	}

	void set(const double x1, const double y1, const double x1l, const double y1l,
				const double x2, const double y2, const double x2l, const double y2l,
				const double x3, const double y3, const double x3l, const double y3l)
	{
		_a = 1.0;
		_b = 0.0;
		_c = 0.0;
		_d = 1.0;
		_ox = 0.0;
		_oy = 0.0;

		const double delta = x1 * y2 + y1 * x3 + x2 * y3 - x1*y3 - y1*x2 - y2*x3;
		if(delta != 0)
		{
			_a = (x1l * y2 + y1*x3l + x2l*y3 - x1l*y3 - y1*x2l - y2*x3l)/delta;
			_b = (x1 * x2l + x1l * x3 + x2 * x3l - x1 * x3l - x1l * x2 - x2l * x3)/delta;
			_ox= (x1*y2*x3l + y1*x2l*x3 + x1l*x2*y3 - x1*x2l*y3 - y1*x2*x3l - x1l*y2*x3)/delta;

			_c = (y1l * y2 + y1*y3l + y2l*y3 - y1l*y3 - y1*y2l - y2*y3l)/delta;
			_d = (x1 * y2l + y1l * x3 + x2 * y3l - x1 * y3l - y1l * x2 - y2l * x3)/delta;
			_oy= (x1*y2*y3l + y1*y2l*x3 + y1l*x2*y3 - x1*y2l*y3 - y1*x2*y3l - y1l*y2*x3)/delta;
		}
		else
		{

		}
	}

	// Mapeamento afim:
	// [x' y' 1] = [x y 1]	[ a c  0]
	//						[ b d  0]
	//						[ox oy 1]
	double mapX(const double x, const double y) const
	{
		return _a * x + _b * y + _ox;
	}
	double mapY(const double x, const double y) const
	{
		return _c * x + _d * y + _oy;
	}
	void map(const double x, const double y, double& xl, double& yl) const
	{
		xl = _a * x + _b * y + _ox;
		yl = _c * x + _d * y + _oy;
	}
	void map(double& x, double& y) const
	{
		const double tx = x;
		const double ty = y;
		x = _a * tx + _b * ty + _ox;
		y = _c * tx + _d * ty + _oy;
	}

	/*

	xl = a x + b y + ox
	yl = c x + d y + oy

	a x + b y = xl - ox
	c x + d y = yl - oy

	|a b|
	|c d| = delta

	|xl-ox b|
	|yl-oy d|. 1/delta = x

	|a xl-ox|
	|c yl-oy|. 1/delta = y

	*/
	void unmap(double& x, double& y) const
	{
		const double delta = _a*_d - _b*_c;
		if(delta != 0)
		{
			const double xl = x;
			const double yl = y;
			x = ((xl-_ox)*_d - (yl-_oy)*_b)/delta;
			y = ((yl-_oy)*_a - (xl-_ox)*_c)/delta;
		}
	}
};

}// namespace lmmath
