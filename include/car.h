#pragma once
#include "GaCar.h"
#include "PhysCar.h"
#include "GrCar.h"
#include <vector>
#include <list>

using namespace std;

class CCar 
{
	GA::CGaCar m_ga_car;
	PHYS::CPhysCar m_phys_car;
	GUI::CGrCar m_gr_car;

public:
	CCar();
	~CCar() = default;

	void beginSimulate(b2WorldId WorldId);
	void endSimulate();
	void CreateFromGenes(const char* szGenes = nullptr);
	void CreateRandomCar();
	void DestroyCar();
	bool doStep();
	void UpdateGraphicsData();
	void Medir(b2WorldId WorldId, double max_t);
	string getGenes();
	[[nodiscard]] b2Vec2 getCenter() const;
	void calc_fitness(float max_t);

	///////////////////////////////////
	// Down Interfaces
	[[nodiscard]] double getPontuacao() const;
	[[nodiscard]] string getGenes() const;
	[[nodiscard]] string deadReason() const;
	[[nodiscard]] double getT() const;
	void Draw(sf::RenderWindow &window) const;	
	void CreateCarFromGenes(const char *genes);
};

typedef vector<CCar> vec_car_t;
typedef list<CCar> lst_car_t;
CCar createCarFromGenes(const string& genes);
CCar createRandomCar();
