#include "StdAfx.h"
#include ".\neuro.h"
#include <math.h>

void CNeuroNet::Init(size_t nEntrada, size_t nCamada1, size_t nCamada2, size_t nCamada3)
{
	// Inicializamos as camadas:
	_camadas[0].resize(nEntrada);
	_camadas[1].resize(nCamada1);
	_camadas[2].resize(nCamada2);
	_camadas[3].resize(nCamada3);

	// Cada camada deve ter o número de pesos igual ao número de neurônios
	// da camada anterior:
	size_t i,j;

	// Para cada camada:
	for(i = 1; i < 4; i++)
	{
		// Para cada neurônio desta camada:
		for(j = 0; j < _camadas[i].size(); j++)
		{
			// Reservamos o espaço para os pesos:
			_camadas[i][j].second.resize(_camadas[i-1].size());
		}
	}
}

dado_t CNeuroNet::execNet(dado_t entrada)
{
	size_t i,j,k;

	ASSERT(entrada.size() == _camadas[0].size());

	// Inicializamos a camada zero com a entrada:
	for(i = 0; i < entrada.size(); i++)
	{
		_camadas[0][i].first = entrada[i];
	}

	// Realizamos a soma ponderada de cada neurônio de cada camada:

	// Para cada camada
	for(i = 1; i < 4; i++)
	{
		// Para cada neurônio desta camada
		for(j = 0; j < _camadas[i].size(); j++)
		{
			// Zeramos valor inicial do neurônio:
			_camadas[i][j].first = 0;

			ASSERT(_camadas[i][j].second.size() == _camadas[i-1].size());
			// Para cada neuronio da camada anterior:
			for(k = 0; k < _camadas[i-1].size(); k++)
			{
				_camadas[i][j].first += 
					_camadas[i][j].second[k] * _camadas[i-1][k].first;
				//  |<----- peso --------->|   |<-saida neuro anter->|
			}

			// Por fim aplicamos a função de transferência:
			_camadas[i][j].first = tanh(_camadas[i][j].first);
		}
	}
	// Retornamos a última camada como resposta:
	dado_t saida;
	saida.resize(_camadas[3].size());
	for(i = 0; i < saida.size(); i++)
		saida[i] = _camadas[3][i].first;

	return saida;
}

double CNeuroNet::getPeso(size_t camada, size_t neuronio, size_t peso)
{
	ASSERT(camada >= 0 && camada <= 3);
	ASSERT(neuronio < _camadas[camada].size());
	ASSERT(peso     < _camadas[camada][neuronio].second.size());

	return _camadas[camada][neuronio].second[peso];
}

void   CNeuroNet::setPeso(size_t camada, size_t neuronio, size_t peso, double dPeso)
{
	ASSERT(camada >= 0 && camada <= 3);
	ASSERT(neuronio < _camadas[camada].size());
	ASSERT(peso     < _camadas[camada][neuronio].second.size());
	_camadas[camada][neuronio].second[peso] = dPeso;
}

void	CNeuroNet::clear(void)
{
	TRACE3("\r\nCamadas: %d,%d,%d", _camadas[3].size(),_camadas[2].size(),_camadas[1].size());
	_camadas[3].clear();
	_camadas[2].clear();
	_camadas[1].clear();
	_camadas[0].clear();
}
