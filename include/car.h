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
	~CCar() override;

	// Overriden methods
	void calc_fitness(float max_t) override;
	void Medir(PHYS::IWorldPtr world, float max_t) override;
	void resetPhysCar() override;
	void createGaFromGenes(const string &genes) override;
	void createGaRandomCar() override;
	void beginSimulate(PHYS::IWorldPtr world) override;
	void draw(void *pParams) const override;
	void doStep() override;
	[[nodiscard]] float getFitness() const override;
	[[nodiscard]] float getT() const override;
	[[nodiscard]] bool operator<(const ICar &rhs) const override;
	[[nodiscard]] bool doStepGetContinue() override;
	[[nodiscard]] string deadReason() const override;
	[[nodiscard]] icar_ptr_t clone() override;
	[[nodiscard]] string getGenes() const override;
	[[nodiscard]] vec2f_t getCenter() const override;
};

class CCarFactory final : public ICarFactory
{
public:
	icar_ptr_t createCarFromGenes(const std::string &genes) override;
	icar_ptr_t createRandomCar() override;
};
