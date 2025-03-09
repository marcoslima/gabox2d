#include "car.h"
#include <car_helpers.h>
#include <iostream>


CCar::CCar()
    : m_ga_car()
    , m_phys_car()
    , m_gr_car() {}

CCar::CCar(const char *szGenes)
    : m_ga_car(szGenes) {}

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
    m_phys_car.destroy();

    m_phys_car.init();

    m_ga_car.CreateCarFromGenes(szGenes);
}

void CCar::CreateCar()
{
    m_phys_car.reset();
    m_ga_car.CreateCar();
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

string CCar::getGenes(void)
{
    return m_ga_car.getGenes();
}

b2Vec2 CCar::getCenter() const
{
    return m_phys_car.getCenter();
}

void CCar::calc_fitness(double max_t)
{
	float pts;
	float c1,c2,v,d,t;
	float p1,p2,p3,p4,p5;

    c1 = m_phys_car.m_contatoR1;
    c2 = m_phys_car.m_contatoR2	;
    v  = m_phys_car.m_vm			;
    d  = m_phys_car.m_distancia	;
    t  = m_phys_car.getT()			; 

    /*
        A pontuação é meio difícil porque, para ser absoluta, não pode
        depender da população.
        Mas se não depender, é muito difícil normalizar as partes (c1, c2, v, d e t).
        Sem normalizar, a distância, por exemplo, que pode ter valores grandes,
        será mais importante que os outros parâmetros de avaliação.
        
        Para resolver isso, vamos fazer o fitness como sendo a distância
        euclidiana de um vetor composto pelos parâmetros de avaliação a um 
        vetor constante ideal.
        
        O vetor será (c1, c2, v, d, t).
        O vetor objetivo ideal será: (t_max, t_max, 1000, 1000, 0).
        Ou seja, 
            . o tempo de contato das rodas é o máximo possível
            . A velocidade é a máxima possível
            . A distância percorrida é a máxima possível
            . O tempo gasto é o mínimo. No caso, nem é possível, pois é zero.
        
        Para não gastar um sqrt à toa, faremos o quadrado da distância.		*/

    p1 = max_t - c1;
    p2 = max_t - c2;
    p3 = 1000 - v;
    p4 = 1000 - d;
    p5 = t; // 0 - t = -t, mas como ser� ao quadrado, deixa t mesmo.

    pts = (p1*p1 + p2*p2 + p3*p3 + p4*p4 + p5*p5);
    
    // Se quebrou, vale um d�cimo de um que n�o quebrou:
    if(m_phys_car.m_bDead) pts *= 10;

    m_ga_car.setPontos(pts);
}

string CCar::getGenesString() const
{
    return m_ga_car.getGenes();
}
