#include "devutils.h"


namespace DevUtils
{
    std::string dbl2str(const double d, const char* fmt)
    {
        char buffer[100];
        sprintf(buffer, fmt, d);
        return {buffer};
    }
}
