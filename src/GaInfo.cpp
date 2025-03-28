#include <GaInfo.h>
namespace GUI
{
    CGaInfo::CGaInfo()
    {
//        InitializeCriticalSection(&_cs);
        m_reqExtincao = false;
        m_reqMelhores = false;
        m_reqIncludeId = false;
    }

    CGaInfo::~CGaInfo()
    {
//        DeleteCriticalSection(&_cs);
    }

    void CGaInfo::Lock(void)
    {
//        EnterCriticalSection(&_cs);
    }

    void CGaInfo::Release(void)
    {
//        LeaveCriticalSection(&_cs);
    }
} // namespace GUI