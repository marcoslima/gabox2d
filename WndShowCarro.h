#ifndef __WNDSHOWCARRO_H__
#define __WNDSHOWCARRO_H__

#include "ga.h"
#include "car.h"

using namespace GA;

namespace GUI
{
// CWndShowCarro
class CWndShowCarro : public CStatic
{
	DECLARE_DYNAMIC(CWndShowCarro)

public:
	CWndShowCarro();
	virtual ~CWndShowCarro();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	CCar m_Carro;
};


};//namespace GUI
#endif //__WNDSHOWCARRO_H__