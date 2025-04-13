#include "GaCar.h"
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <random_genes_generator.h>


namespace GA
{
    CGaCar::CGaCar()
        : _pontos(0)
    {
        _generate_random_genes();
    }

    CGaCar::CGaCar(const char *szGenes)
        : _genes(szGenes)
        , _pontos(0) {}

    void CGaCar::_generate_random_genes()
    {
        generate_random_genes(GENES, _genes);
    }

    string CGaCar::getGenes() const
    {
        return _genes;
    }

    float CGaCar::getPontuacao() const
    {
        return _pontos;
    }

    ga_car_ptr_t CGaCar::clone()
    {
        auto car = make_shared<CGaCar>();
        car->_genes = _genes;
        car->_pontos = _pontos;
        return car;
    }

    void CGaCar::CreateCarFromGenes(const string &genes)
    {
        if (genes.size() != GENES)
        {
            stringstream ss;
            ss << "Invalid genes size: " << genes.size();
            throw length_error(ss.str().c_str());
        }

        _genes = genes;
    }

    void CGaCar::CreateRandomCar()
    {
        _generate_random_genes();
    }

    void CGaCar::calc_fitness(const fitness_params_t fitness_params, const float max_t)
    {
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

            Para não gastar um sqrt à toa, faremos o quadrado da distância.
        */
        const float p1 = max_t - fitness_params.contact1;
        const float p2 = max_t - fitness_params.contact2;
        const float p3 = 1000 - fitness_params.velocity;
        const float p4 = 1000 - fitness_params.distance;
        const float p5 = fitness_params.time; // 0 - t = -t, mas como será ao quadrado, deixa t mesmo.

        float pts = p1*p1 + p2*p2 + p3*p3 + p4*p4 + p5*p5;

        // Se quebrou, vale um décimo de um que não quebrou:
        if(fitness_params.is_dead) pts *= 10;

        _pontos = pts;
    }


}
