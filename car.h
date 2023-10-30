#ifndef __CAR_H__
#define __CAR_H__
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
// Interface
public:
	// Construtoras
	CCar();
	CCar(const char* szGenes);
	
	void beginSimulate(b2World *pWorld);
	void endSimulate(void);
	void CreateCar(const char* szGenes = NULL);
	void DestroyCar(void);
	bool doStep(void);
	void Phys2Gr(void);
	void Medir(b2World* pWorld, double max_t);
};

typedef vector<CCar> vec_car_t;
typedef list<CCar> lst_car_t;


#endif //__CAR_H__