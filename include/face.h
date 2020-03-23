#pragma once

#include "vector3.h"


namespace CS636{

    class Face{
        public:
            CS636::Vector3 v1;
            CS636::Vector3 v2;
            CS636::Vector3 v3;
        public:
            Face(CS636::Vector3 v1, CS636::Vector3 v2 ,CS636::Vector3 v3)
                : v1(v1), v2(v2), v3(v3)
        {
        }

    };

}



