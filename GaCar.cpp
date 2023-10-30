#include <mersenne.h>
extern MTRand mrand;

#include "GaCar.h"
namespace GA
{

double d26[] = 
{
			1,
		   26,
		  676,
		17576,
	   456976,
	 11881376,
	308915776
};

double map_values(double in_min, double in_max, double out_min, double out_max, double val)
{
	double inDelta  = in_max  - in_min ;
	double outDelta = out_max - out_min;
	
	return out_min + (val - in_min)*outDelta/inDelta;
}

double DecodeGen(int nLen, const char *genes, double nMin, double nMax, size_t& nPos)
{
	double dVal = 0;
	char ch;
	for(int i = 0; i < nLen; i++)
	{
		ch = *(genes+(nPos++));
		ch -= 'A';
		dVal += ch * d26[i];
		//dVal += (*(genes+(nPos++)) - 'A') * d26[i];
	}


//	1 d�gito: A - Z ou 0 � 25, ou seja, d26[1]-1
//	2 d�gitos: M�ximo: ZZ que � 25*26 + 25 = 675, ou seja, d26[2]-1

	return (double)map_values(0,d26[nLen]-1,nMin,nMax,dVal);
}

CCarDef::CRoda DecodeRoda(const char *genes, size_t& nPos)
{
	const int nLen = 4;
	return CCarDef::CRoda(	DecodeGen(nLen,genes,-8  , 8 ,nPos),
							DecodeGen(nLen,genes, 2  , 8 ,nPos),
							DecodeGen(nLen,genes, 0.2, 3 ,nPos),
							DecodeGen(nLen,genes, 0.1, 10,nPos),
							DecodeGen(nLen,genes, 0.1, 2 ,nPos),
							DecodeGen(nLen,genes, 0  , 1 ,nPos));
}

CGaCar::CGaCar()
{
	_generate_random_genes();
}

CGaCar::CGaCar(const char* szGenes)
{
	_genes = szGenes;
}

CGaCar::~CGaCar(void)
{
}

void CGaCar::_generate_random_genes(void)
{
	int i;
	_genes.clear();

	mrand.seed();

	for(i = 0; i < GENES; i++)
	{
		_genes.push_back('A' + mrand.randInt(26));
	}

	return;
}


void CGaCar::getGenes(string& genes)
{
	genes = string(_genes);
}

string CGaCar::getGenesString(void)
{
	return string(_genes);
}

void CGaCar::setGenes(const char *genes)
{
	if(genes == NULL || strlen(genes) != GENES)
		_generate_random_genes();
	else
		_genes = genes;
}

void CGaCar::CreateCar(const char *genes)
{
	setGenes(genes);
}
void CGaCar::_decode(void)
{
	const double dFp = 50;
	const double dMinFreq = 0.1;
	const double dMaxFreq = 30.0;
	const double dMinDamp = 0.0;
	const double dMaxDamp = 2.0;

	const int nLen = 4;
	int i;
	size_t	nPos = 0;

	// Obtemos os body's e shape's def's dos genes:
	_carro._roda1 = DecodeRoda(_genes.c_str(),nPos);
	_carro._roda2 = DecodeRoda(_genes.c_str(),nPos);
	_carro._peso1 = DecodeRoda(_genes.c_str(),nPos);
	_carro._peso2 = DecodeRoda(_genes.c_str(),nPos);

	for(i = 0; i < 6; i++)
	{
		if(i < 4) _carro._torque[i] = DecodeGen(nLen,_genes.c_str(),-dFp,dFp,nPos);
		_carro._freq[i] = DecodeGen(nLen,_genes.c_str(),dMinFreq,dMaxFreq,nPos);
		_carro._damp[i] = DecodeGen(nLen,_genes.c_str(),dMinDamp,dMaxDamp,nPos);
	}

//	TRACE1("\r\nTamanho da sequ�ncia gen�tica: %d", nPos);_asm int 3;

	return;
}

void CGaCar::Crossover(CGaCar& other)
{
	size_t i,nCross;
	nCross = 1 + mrand.randInt(GENES-2);

	char tmp;
	for(i = nCross; i < GENES; i++)
	{
		tmp = getGene(i);
		setGene(i,other.getGene(i));
		other.setGene(i,tmp);
	}
}

}; // namespace GA
