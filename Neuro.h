#ifndef __NEURO_H__
#define __NEURO_H__

#include <vector>
using namespace std;

// Um neurônio tem:
// 1. o seu próprio valor
// 2. os pesos de si para os neurônios da 
//    camada anterior.

// Pesos: vector de double, normal:
typedef vector<double> pesos_t;

// Neurônio:
typedef pair<double,pesos_t> neuronio_t;

// Uma camada de neurônios tem x neurônios:
typedef vector<neuronio_t> camada_t;


// A entrada e a saida da rede é um vetor
typedef vector<double> dado_t;

class CNeuroNet
{
private:
	// Camadas 1, 2 e 3, e a entrada, no indice zero
	camada_t	_camadas[4];
	size_t		_nEntradas;

public:
	void	Init(size_t nEntrada, size_t nCamada1, size_t nCamada2, size_t nCamada3);
	dado_t	execNet(dado_t entrada);
	double	getPeso(size_t camada, size_t neuronio, size_t peso);
	void	setPeso(size_t camada, size_t neuronio, size_t peso, double dPeso);
	void	clear(void);

};

#endif //__NEURO_H__