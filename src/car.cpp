#include "car.h"
#include <utility>


CCar::CCar()
{
    m_ga_car_ptr = make_shared<GA::CGaCar>();
    m_phys_car_ptr = make_shared<PHYS::CPhysCar>();
    m_gr_car_ptr = make_shared<GUI::CGrCar>();
}

CCar::CCar(GA::ga_car_ptr_t ga_car_ptr, PHYS::phys_car_ptr_t phys_car_ptr, GUI::gr_car_ptr_t gr_car_ptr)
    : m_ga_car_ptr(std::move(ga_car_ptr))
    , m_phys_car_ptr(std::move(phys_car_ptr))
    , m_gr_car_ptr(std::move(gr_car_ptr))
    {}

icar_ptr_t CCar::clone()
{
    auto car = make_unique<CCar>();
    car->m_ga_car_ptr = m_ga_car_ptr->clone();
    car->m_phys_car_ptr = m_phys_car_ptr->clone();
    car->m_gr_car_ptr = m_gr_car_ptr->clone();
    return car;
}

void updateGraphicsData_(const PHYS::phys_car_ptr_t &phys_car, const GUI::gr_car_ptr_t &gr_car)
{
    phys_car->fill_gr_car(*gr_car);
}

bool CCar::doStepGetContinue()
{
    const bool bRet = m_phys_car_ptr->simulation_step_get_dead();
    updateGraphicsData_(m_phys_car_ptr, m_gr_car_ptr);

    return bRet;
}

void CCar::doStep()
{
    m_phys_car_ptr->simulation_step();
    updateGraphicsData_(m_phys_car_ptr, m_gr_car_ptr);
}

void CCar::setShowRodaParams(const bool show)
{
    m_gr_car_ptr->setShowRodaParams(show);
}

void CCar::Medir(const PHYS::IWorldPtr world, const float max_t)
{
    const CCarDef car(m_ga_car_ptr->getGenes());
    m_phys_car_ptr->measure(world, car, max_t);
}

string CCar::getGenes() const
{
    return m_ga_car_ptr->getGenes();
}

vec2f_t CCar::getCenter() const
{
    return m_phys_car_ptr->getMassCenter();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void CCar::calc_fitness(const GA::fitness_params_t& params, const float max_t)
{
    m_ga_car_ptr->calc_fitness(params, max_t);
}

float CCar::getFitness() const
{
    return m_ga_car_ptr->getPontuacao();
}

void CCar::resetPhysCar()
{
    m_phys_car_ptr->reset();
}

void CCar::createGaRandomCar()
{
    m_ga_car_ptr->CreateRandomCar();
}

void CCar::createGaFromGenes(const string &genes)
{
    m_ga_car_ptr->CreateCarFromGenes(genes);
}

bool CCar::operator<(const ICar &rhs) const
{
    return getFitness() < rhs.getFitness();
}

string CCar::deadReason() const
{
    return m_phys_car_ptr->deadReason();
}

float CCar::getT() const
{
    return m_phys_car_ptr->getT();
}

void CCar::draw(void *pParams) const
{
    m_gr_car_ptr->draw(pParams);
}

void CCar::beginSimulate(const PHYS::IWorldPtr world)
{
    const CCarDef car(m_ga_car_ptr->getGenes());
    m_phys_car_ptr->create(world, car);
}

CCar::~CCar() = default;

icar_ptr_t CCarFactory::createCarFromGenes(const std::string &genes)
{
    icar_ptr_t car = make_unique<CCar>();
    car->createGaFromGenes(genes);
    return car;
}
icar_ptr_t CCarFactory::createRandomCar()
{
    icar_ptr_t car = make_unique<CCar>();
    car->resetPhysCar();
    car->createGaRandomCar();
    return car;
}
