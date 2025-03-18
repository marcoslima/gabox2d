#pragma once
#include "GaCar.h"
#include "PhysCar.h"
#include "GrCar.h"
#include <vector>
#include <list>
#include <memory>

using namespace std;

using ga_car_ptr_t = shared_ptr<GA::IGaCar>;
using phys_car_ptr_t = shared_ptr<PHYS::IPhysCar>;
using gr_car_ptr_t = shared_ptr<GUI::IGrCar>;

class CCar 
{
	ga_car_ptr_t m_ga_car_ptr;
	phys_car_ptr_t m_phys_car_ptr;
	gr_car_ptr_t m_gr_car_ptr;

public:
	CCar();
	CCar(ga_car_ptr_t ga_car_ptr, phys_car_ptr_t phys_car_ptr, gr_car_ptr_t gr_car_ptr);
	~CCar() = default;

	void CreateFromGenes(const char* szGenes = nullptr);
	void CreateRandomCar();
	void DestroyCar();
	bool doStep();
	void UpdateGraphicsData();
	void Medir(b2WorldId WorldId, float max_t) const;
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

	void beginSimulate(b2WorldId b2_world_id);
};

typedef vector<CCar> vec_car_t;
typedef list<CCar> lst_car_t;
CCar createCarFromGenes(const string& genes);
CCar createRandomCar();
