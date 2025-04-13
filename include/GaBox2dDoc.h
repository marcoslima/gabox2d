#pragma once
#include <IGaBox2dDoc.h>
#include "car.h"
#include "ga.h"
#include "env.h"

namespace GUI
{
extern vec_vecs_t	g_vecGroundPoints;

class CGaBox2dDoc final : public IGaBox2dDoc
{
	bool _isSimulating = false;
	PHYS::IWorldPtr _world;

public:
	CGaBox2dDoc();
	~CGaBox2dDoc() override;

	[[nodiscard]] bool isSimulating() const override;
	[[nodiscard]] PHYS::IWorldPtr GetWorld() const override;
	[[nodiscard]] bool isQuit() const override;
	[[nodiscard]] vec_vecs_t GetGround() const override;
	[[nodiscard]] CEnv GetEnv() const override;

	// Attributes
private:
	icar_ptr_t m_car;

public:
	bool m_bQuit = false;

	vec_car_t	m_populacao;


	CEnv m_env;
	vec_vecs_t	m_vecGround;

// Operations
	void BeginSimulation() override;
	void EndSimulation() override;
	void Quit() override;
	const icar_ptr_t& GetCar() override;
	bool OnNewDocument(const CEnv& env) override;
	void _start_world() override;
	void OnEditEditarch() override;
};

}