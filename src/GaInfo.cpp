#include <GaInfo.h>

namespace GUI
{
    CGaInfo::CGaInfo()
        : m_geracao(0)
          , m_gps(0)
          , m_maxD(0)
          , m_maxT(0)
          , m_maxVm(0)
          , m_reqMelhores(false)
          , m_reqExtincao(false)
          , m_reqIncludeId(false)
    {
        //        InitializeCriticalSection(&_cs);
    }

    CGaInfo::~CGaInfo()
    {
        //        DeleteCriticalSection(&_cs);
    }

    void CGaInfo::Lock()
    {
        //        EnterCriticalSection(&_cs);
    }

    void CGaInfo::Release()
    {
        //        LeaveCriticalSection(&_cs);
    }
} // namespace GUI
