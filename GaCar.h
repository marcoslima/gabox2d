#ifndef __GACAR_H__
#define __GACAR_H__

#include <string>
#include "CarDef.h"

using namespace std;
using namespace MODEL;

#define GENES 300
#define GENES_LEN (GENES+1)

namespace GA
{

class CGaCar
{
// Tipos internos:
public:

// O carro em si
public:
	// Codificado
	string _genes;

	// Decodificado
	CCarDef	_carro;

// Suporte ao algoritmo gen�tico:
public:
	double	_pontos				;

// Internas
protected:
	void _generate_random_genes(void);

	// Decodifica o carro dos genes criando-o nas defini��es
	void _decode(void);

public:
	CGaCar();
	CGaCar(const char* szGenes);
	~CGaCar(void);
	void getGenes(string& genes);
	inline const char *getGenes(void){return _genes.c_str();}
	inline char getGene(size_t nIndex){return _genes[nIndex];}
	inline void setGene(size_t nIndex, char gen){_genes[nIndex] = gen;}
	string getGenesString(void);
	double getPontuacao(void) const {return _pontos;}
	void setGenes(const char *genes);
	void setPontos(double pontos){_pontos = pontos;}
	void CreateCar(const char *genes = NULL);

	// Processo evolutivo:
	void Crossover(CGaCar& other);

};

}; // namespace GA

#endif //__GACAR_H__