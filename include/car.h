#pragma once
#include "GaCar.h"
#include "PhysCar.h"
#include "GrCar.h"
#include <vector>
#include <list>

using namespace std;

class CCar 
{
private:
	GA::CGaCar m_ga_car; 
	PHYS::CPhysCar m_phys_car;
	GUI::CGrCar m_gr_car;

public:
	CCar(GA::CGaCar ga_car, PHYS::CPhysCar phys_car, GUI::CGrCar gr_car);

	~CCar()= default;

	void beginSimulate(b2WorldId WorldId);
	void endSimulate();
	void CreateFromGenes(const char* szGenes = nullptr);
	void CreateRandomCar();
	void DestroyCar();
	bool doStep();
	void UpdateGraphicsData();
	void Medir(b2WorldId WorldId, double max_t);
	string getGenes(void);
	[[nodiscard]] b2Vec2 getCenter() const;
	void calc_fitness(double max_t);


	///////////////////////////////////
	// Down Interfaces
	double getPontuacao() const;
	string getGenes() const;
	string deadReason() const;
	double getT() const;
	void Draw(sf::RenderWindow &window) const;	
	void CreateCarFromGenes(const char *genes);
};

typedef vector<CCar> vec_car_t;
typedef list<CCar> lst_car_t;
CCar createCarFromGenes(string genes, 
					    GA::CGaCar ga_car = GA::CGaCar(), 
					    PHYS::CPhysCar phys_car = PHYS::CPhysCar(), 
					    GUI::CGrCar gr_car = GUI::CGrCar());
CCar createRandomCar(GA::CGaCar ga_car = GA::CGaCar(), 
					 PHYS::CPhysCar phys_car = PHYS::CPhysCar(), 
					 GUI::CGrCar gr_car = GUI::CGrCar());