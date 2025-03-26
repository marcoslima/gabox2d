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
	icar_ptr_t m_car;

public:
	bool m_bQuit = false;
	bool m_IsSimulating = false;

	vec_car_t	m_populacao;

	PHYS::CWorld m_World;

	CEnv m_env;
	vec_vecs_t	m_vecGround;

// Operations
	void	BeginSimulation();
	void	EndSimulation();

	[[nodiscard]] PointF	GetCenter() const;

	void Quit();

	const icar_ptr_t& GetCar();

	bool OnNewDocument(const CEnv& env);

// Implementation
	~CGaBox2dDoc();

	void _start_world();

	void OnEditEditarch();

};

};//namespace GUI
