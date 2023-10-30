#ifndef __GABOX2DDOC_H__
#define __GABOX2DDOC_H__

#include "car.h"
#include "ga.h"

extern vec_vecs_t	g_vecGroundPoints;
	
// GaBox2dDoc.h : interface of the CGaBox2dDoc class
class CGaBox2dDoc : public CDocument
{
protected: // create from serialization only
	CGaBox2dDoc();
	DECLARE_DYNCREATE(CGaBox2dDoc)

// Attributes
public:

	CCar m_car;
	vec_car_t	m_populacao;

	b2World *m_pWorld;

	b2Body	*m_pGround;


// Operations
public:
	char *RandomCar(car_t& car);

// Overrides
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CGaBox2dDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEditCopy();
};



#endif //__GABOX2DDOC_H__