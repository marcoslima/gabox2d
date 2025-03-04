//
// Created by marcos on 11/3/24.
//

#ifndef CGROUND_H
#define CGROUND_H
#include <box2d/id.h>

namespace PHYS {

    class CWorld
    {
    public:
        b2WorldId m_WorldId = b2_nullWorldId;
        b2BodyId m_GroundId = b2_nullBodyId;
        b2ChainId m_ChainId = b2_nullChainId;
    };

} // PHYS

#endif //CGROUND_H
