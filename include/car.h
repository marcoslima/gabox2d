#pragma once
#include "GaCar.h"
#include "PhysCar.h"
#include "GrCar.h"
#include <vector>
#include <list>

using namespace std;

class CCar : 
	public GA::CGaCar	 , 
	public PHYS::CPhysCar, 
	public GUI::CGrCar
{
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
};

typedef vector<CCar> vec_car_t;
typedef list<CCar> lst_car_t;
