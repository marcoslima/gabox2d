#include "car.h"
#include <car_helpers.h>
#include <iostream>


CCar::CCar()
    : m_ga_car()
    , m_phys_car()
    , m_gr_car()
{
}

void CCar::beginSimulate(const b2WorldId WorldId)
{
    // Desinstanciamento
    m_phys_car.destroy();

    // Decodificamos os genes (genes -> carro | string -> CCarDef)
    m_ga_car.decode();

    // Instanciamento
    m_phys_car.create(WorldId, m_ga_car._carro);

    // Inicializamos a simulação física:
    m_phys_car.init_simulation_vars();
}

void CCar::endSimulate()
{
    // Liberamos os recursos da phys:
    m_phys_car.phys_end_simulate();
}

void CCar::CreateFromGenes(const char *szGenes)
{
    m_phys_car.reset();
    m_ga_car.CreateCarFromGenes(szGenes);
}

void CCar::CreateRandomCar()
{
    m_phys_car.reset();
    m_ga_car.CreateRandomCar();
}

void CCar::DestroyCar()
{
    m_phys_car.destroy();
}

bool CCar::doStep()
{
    const bool bRet = m_phys_car.simulation_step();

    UpdateGraphicsData();

    return bRet;
}

void CCar::UpdateGraphicsData()
{
    m_gr_car._roda1 = TranslateRoda(m_phys_car.m_Roda1Id, m_phys_car.m_bContactR1);
    m_gr_car._roda2 = TranslateRoda(m_phys_car.m_Roda2Id, m_phys_car.m_bContactR2);
    m_gr_car._peso1 = TranslatePeso(m_phys_car.m_Peso1Id, m_phys_car.m_bDead);
    m_gr_car._peso2 = TranslatePeso(m_phys_car.m_Peso2Id, m_phys_car.m_bDead);
    const auto [x, y] = m_phys_car.getCenter();
    m_gr_car._cm = PointF(x, y);
    m_gr_car._broke = m_phys_car.m_bDead;
}

void CCar::Medir(const b2WorldId WorldId, const double max_t)
{
    beginSimulate(WorldId);
    const float x0 = m_phys_car.getCenter().x;
    for (int k = 0; m_phys_car._t < max_t; k++)
    {
        if (!doStep())
            break;
    }
    const float x = m_phys_car.getCenter().x;
    m_phys_car.m_t = m_phys_car._t;
    m_phys_car.Destroy();

    m_phys_car.m_distancia = x - x0;
}

string CCar::getGenes()
{
    return m_ga_car.getGenes();
}

b2Vec2 CCar::getCenter() const
{
    return m_phys_car.getCenter();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void CCar::calc_fitness(const float max_t)
{
    const float c1 = m_phys_car.m_contatoR1;
    const float c2 = m_phys_car.m_contatoR2;
    const float v = m_phys_car.m_vm;
    const float d = m_phys_car.m_distancia;
    const float t = m_phys_car.getT();

    m_ga_car.calc_fitness(c1, c2, v, d, t, max_t, m_phys_car.m_bDead);
}

double CCar::getPontuacao() const
{
    return m_ga_car.getPontuacao();
}

string CCar::getGenes() const
{
    return m_ga_car.getGenes();
}

string CCar::deadReason() const
{
    return m_phys_car.m_dead_reason;
}

double CCar::getT() const
{
    return m_phys_car.getT();
}

void CCar::Draw(sf::RenderWindow &window) const
{
    m_gr_car.Draw(window);
}

void CCar::CreateCarFromGenes(const char *genes)
{
    m_ga_car.CreateCarFromGenes(genes);
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
