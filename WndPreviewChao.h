#ifndef __WNDPREVIEWCHAO_H__
#define __WNDPREVIEWCHAO_H__

// CWndPreviewChao
#include "ga.h"
#include "env.h"
namespace GUI
{
class CWndPreviewChao : public CWnd
{
	DECLARE_DYNAMIC(CWndPreviewChao)

public:
	CWndPreviewChao();
	virtual ~CWndPreviewChao();
	void SetCenter(int x, int y);

	CEnv		m_env;
	b2Vec2		m_vecTl;
	b2Vec2		m_vecBr;
	CPoint		m_ptCenter;
	int			m_zoom;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};


};//namespace GUI
#endif //__WNDPREVIEWCHAO_H__