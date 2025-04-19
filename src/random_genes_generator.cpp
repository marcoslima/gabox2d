#include <random_genes_generator.h>

#include "CRandom.h"


void generate_random_genes(const size_t genes_len, std::string &genes)
{
    const CRandom random(static_cast<unsigned>(time(nullptr)));

    genes.clear();
    genes.reserve(genes_len);
    std::generate_n(std::back_inserter(genes), genes_len,
                    [&]
                    {
                        return random.discrete_random('0', '1');
                    });
}


std::string generate_random_genes(const size_t genes_len)
{
    std::string genes;
    generate_random_genes(genes_len, genes);
    return genes;
}
