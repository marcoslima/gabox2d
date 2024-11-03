#pragma once
#include "car.h"
#include "ga.h"
#include "env.h"

namespace GUI
{
extern vec_vecs_t	g_vecGroundPoints;
	
// GaBox2dDoc.h : interface of the CGaBox2dDoc class
class CGaBox2dDoc final
{
public:
	CGaBox2dDoc();


// Attributes
private:
	CCar m_car;

public:

	vec_car_t	m_populacao;

	PHYS::CWorld m_World;

	CEnv m_env;
	vec_vecs_t	m_vecGround;

// Operations
public:
	void	BeginSimulation();

	static void	EndSimulation();
	PointF	GetCenter();
	CCar&	GetCar(){return m_car;}

//	char *RandomCar(car_t& car);

// Overrides
	public:
	virtual bool OnNewDocument(CEnv& env);

// Implementation
public:
	virtual ~CGaBox2dDoc();

public:
	void OnEditCopy() const;

	void _start_world();

	void OnEditEditarch();

};

};//namespace GUI
