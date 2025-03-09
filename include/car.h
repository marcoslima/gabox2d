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
	void CreateFromGenes(const char* szGenes = nullptr);
	void CreateCar();
	void DestroyCar();
	bool doStep();
	void UpdateGraphicsData();
	void Medir(b2WorldId WorldId, double max_t);
	string getGenes(void);
	[[nodiscard]] b2Vec2 getCenter() const;
	void calc_fitness(double max_t);
	string getGenesString() const;


	///////////////////////////////////
	// Down Interfaces
	double getPontuacao() const
	{
		return m_ga_car.getPontuacao();
	}

	string getGenes() const
	{
		return m_ga_car.getGenes();
	}
};

typedef vector<CCar> vec_car_t;
typedef list<CCar> lst_car_t;
