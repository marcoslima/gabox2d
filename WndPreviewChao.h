#pragma once


// CWndPreviewChao
#include "ga.h"

class CWndPreviewChao : public CWnd
{
	DECLARE_DYNAMIC(CWndPreviewChao)

public:
	CWndPreviewChao();
	virtual ~CWndPreviewChao();

	vec_vecs_t	m_vecGround;
	b2Vec2		m_vecTl;
	b2Vec2		m_vecBr;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};


