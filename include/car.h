#pragma once
#include "GaCar.h"
#include "PhysCar.h"
#include "GrCar.h"
#include <memory>
#include <icar.h>

using namespace std;


class CCar final : public ICar
{
	GA::ga_car_ptr_t m_ga_car_ptr;
	PHYS::phys_car_ptr_t m_phys_car_ptr;
	GUI::gr_car_ptr_t m_gr_car_ptr;

public:
	CCar();
	CCar(GA::ga_car_ptr_t ga_car_ptr, PHYS::phys_car_ptr_t phys_car_ptr, GUI::gr_car_ptr_t gr_car_ptr);

	// Overriden methods
	void calc_fitness(float max_t) override;
	void Medir(PHYS::IWorld &world, float max_t) override;
	void resetPhysCar() override;
	void createGaFromGenes(const string &genes) override;
	void beginSimulate(PHYS::IWorld &world) override;
	void draw(void *pParams) const override;
	void mutate() override;
	[[nodiscard]] float getFitness() const override;
	[[nodiscard]] float getT() const override;
	[[nodiscard]] bool operator<(const ICar &rhs) const override;
	[[nodiscard]] bool doStep() override;
	[[nodiscard]] string deadReason() const override;
	[[nodiscard]] icar_ptr_t crossover(const icar_ptr_t &rhs, size_t crosspoint) const override;
	[[nodiscard]] icar_ptr_t clone() override;

	// Rest: verify after all done
	void CreateFromGenes(const char* szGenes = nullptr);
	void CreateRandomCar();
	void DestroyCar();
	void UpdateGraphicsData();
	string getGenes();
	[[nodiscard]] IVec2f getCenter() const override;

	///////////////////////////////////
	// Down Interfaces
	[[nodiscard]] string getGenes() const;

	void Draw(sf::RenderWindow &window) const;
	void createGaRandomCar() override;

	~CCar() override;
};

inline void CCar::mutate()
{
	m_ga_car_ptr->mutate();
}

class CCarFactory final : public ICarFactory
{
public:
	icar_ptr_t createCarFromGenes(const std::string &genes) override;
	icar_ptr_t createRandomCar() override;
};
