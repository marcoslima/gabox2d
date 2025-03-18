#include "car.h"
#include <car_helpers.h>
#include <iostream>
#include <utility>


CCar::CCar()
{
    m_ga_car_ptr = make_shared<GA::CGaCar>();
    m_phys_car_ptr = make_shared<PHYS::CPhysCar>();
    m_gr_car_ptr = make_shared<GUI::CGrCar>();
}

CCar::CCar(ga_car_ptr_t ga_car_ptr, phys_car_ptr_t phys_car_ptr, gr_car_ptr_t gr_car_ptr)
    : m_ga_car_ptr(std::move(ga_car_ptr))
    , m_phys_car_ptr(std::move(phys_car_ptr))
    , m_gr_car_ptr(std::move(gr_car_ptr))
    {}

void CCar::CreateFromGenes(const char *szGenes)
{
    m_phys_car_ptr->reset();
    m_ga_car_ptr->CreateCarFromGenes(szGenes);
}

void CCar::CreateRandomCar()
{
    m_phys_car_ptr->reset();
    m_ga_car_ptr->CreateRandomCar();
}

void CCar::DestroyCar()
{
    m_phys_car_ptr->destroy();
}

bool CCar::doStep()
{
    const bool bRet = m_phys_car_ptr->simulation_step();

    UpdateGraphicsData();

    return bRet;
}

void CCar::UpdateGraphicsData()
{
    m_phys_car_ptr->fill_gr_car(*m_gr_car_ptr);
    // m_gr_car_ptr->_roda1 = TranslateRoda(m_phys_car_ptr->m_Roda1Id, m_phys_car_ptr->m_bContactR1);
    // m_gr_car_ptr->_roda2 = TranslateRoda(m_phys_car_ptr->m_Roda2Id, m_phys_car_ptr->m_bContactR2);
    // m_gr_car_ptr->_peso1 = TranslatePeso(m_phys_car_ptr->m_Peso1Id, m_phys_car_ptr->isDead());
    // m_gr_car_ptr->_peso2 = TranslatePeso(m_phys_car_ptr->m_Peso2Id, m_phys_car_ptr->isDead());
    // const auto [x, y] = m_phys_car_ptr->getMassCenter();
    // m_gr_car_ptr->_cm = PointF(x, y);
    // m_gr_car_ptr->_broke = m_phys_car_ptr->m_bDead;
}

void CCar::Medir(const b2WorldId WorldId, const float max_t) const
{
    m_ga_car_ptr->decode();
    m_phys_car_ptr->measure(WorldId, m_ga_car_ptr->getCarro(), max_t);
}

// OLD IMPLEMENTATION using doStep (that update graphics!!! so wrong!)
// void CCar::Medir(const b2WorldId WorldId, const double max_t)
// {
//     beginSimulate(WorldId);
//     const float x0 = m_phys_car_ptr->getCurrentX();
//
//     while(m_phys_car_ptr->_t < max_t && doStep()){};
//
//     const float x = m_phys_car_ptr->getCurrentX();
//     m_phys_car_ptr->m_t = m_phys_car_ptr->_t;
//     m_phys_car_ptr->Destroy();
//
//     m_phys_car_ptr->m_distancia = x - x0;
// }

string CCar::getGenes()
{
    return m_ga_car_ptr->getGenes();
}

b2Vec2 CCar::getCenter() const
{
    return m_phys_car_ptr->getMassCenter();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void CCar::calc_fitness(const float max_t)
{
    m_ga_car_ptr->calc_fitness(m_phys_car_ptr->get_ga_fitness_params(), max_t);
}

double CCar::getPontuacao() const
{
    return m_ga_car_ptr->getPontuacao();
}

string CCar::getGenes() const
{
    return m_ga_car_ptr->getGenes();
}

string CCar::deadReason() const
{
    return m_phys_car_ptr->deadReason();
}

double CCar::getT() const
{
    return m_phys_car_ptr->getT();
}

void CCar::Draw(sf::RenderWindow &window) const
{
    m_gr_car_ptr->draw(&window);
}

void CCar::CreateCarFromGenes(const char *genes)
{
    m_ga_car_ptr->CreateCarFromGenes(genes);
}

void CCar::beginSimulate(b2WorldId b2_world_id)
{
    m_ga_car_ptr->decode();
    m_phys_car_ptr->create(b2_world_id, m_ga_car_ptr->getCarro());
}

CCar createCarFromGenes(const string& genes)
{
    CCar car;
    car.CreateFromGenes(genes.c_str());
    return car;
}

CCar createRandomCar()
{
    CCar car;
    car.CreateRandomCar();
    return car;
}
