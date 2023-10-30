#ifndef __GABOX2DDOC_H__
#define __GABOX2DDOC_H__

#include "car.h"
#include "ga.h"
#include "env.h"

namespace GUI
{
//extern vec_vecs_t	g_vecGroundPoints;
	
// GaBox2dDoc.h : interface of the CGaBox2dDoc class
class CGaBox2dDoc : public CDocument
{
protected: // create from serialization only
	CGaBox2dDoc();
	DECLARE_DYNCREATE(CGaBox2dDoc)



// Attributes
private:
	CCar m_car;

public:

	vec_car_t	m_populacao;

	b2World *m_pWorld;
	b2Body	*m_pGround;

	CEnv m_env;
	vec_vecs_t	m_vecGround;

// Operations
public:
	void	BeginSimulation(void);
	void	EndSimulation(void);
	PointF	GetCenter(void);
	CCar&	GetCar(void){return m_car;}

//	char *RandomCar(car_t& car);

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
	afx_msg void OnEditEditarch();

};

};//namespace GUI

#endif //__GABOX2DDOC_H__