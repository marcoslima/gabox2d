#pragma once
#include "GaCar.h"
#include "PhysCar.h"
#include "GrCar.h"
#include <vector>
#include <list>

using namespace std;

class CCar 
{
public:
	GA::CGaCar m_ga_car; 
	PHYS::CPhysCar m_phys_car;
	GUI::CGrCar m_gr_car;

public:
	CCar();

	explicit CCar(const char* szGenes);
	~CCar()= default;
	void beginSimulate(b2WorldId WorldId);

	void endSimulate();
	void CreateCar(const char* szGenes = nullptr);
	void DestroyCar();
	bool doStep();
	void UpdateGraphicsData();
	void Medir(b2WorldId WorldId, double max_t);

	///////////////////////////////////
	// Down Interfaces
	double getPontuacao() const
	{
		return m_ga_car.getPontuacao();
	}

	const char* getGenes() const
	{
		return m_ga_car.getGenes();
	}
};

typedef vector<CCar> vec_car_t;
typedef list<CCar> lst_car_t;
