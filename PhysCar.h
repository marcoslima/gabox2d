#ifndef __PHYSCAR_H__
#define __PHYSCAR_H__

#include <box2d/box2d.h>
#include "CarDef.h"
#include "env.h"
using namespace MODEL;

namespace PHYS
{

typedef struct tagB2Def
{
	b2CircleShape sd;
	b2BodyDef	bd;
} b2_def_t;

typedef struct tagCarParams
{
	// Roda 1
	b2_def_t R1;

	// Roda 2
	b2_def_t R2;

	// Peso 1
	b2_def_t P1;

	// Peso 2
	b2_def_t P2;

	double	torque[4];

	// Para que os distance joints fa�am parte do genoma,
	// teremos a frequencia de resposta e o damp deles codificados
	// tamb�m.
	double	freq[6];
	double damp[6];	
} car_t;

class CPhysCar;
class CContactListener : public b2ContactListener
{
public:
	void Add(const b2Contact* point);
	void Persist(const b2Contact* point);

public:
	bool		m_bDead		;
	bool		m_bContactR1;
	bool		m_bContactR2;
	b2Vec2		m_cVel		;
	b2Vec2		m_cPos		;
};

class CPhysCar
{
friend class CContactListener;
public:
	CPhysCar(void);
	~CPhysCar(void);

public:
// Defini��es do carro (decodificados dos genes)
	car_t	_car_def;

// Instanciamento do carro no box2d
public:
	b2Body*	_pRoda1;
	b2Body*	_pRoda2;
	b2Body*	_pPeso1;
	b2Body*	_pPeso2;

	b2Joint* _pJc1c2;
	b2Joint* _pJc1p1;
	b2Joint* _pJc1p2;
	b2Joint* _pJc2p1;
	b2Joint* _pJc2p2;
	b2Joint* _pJp1p2;

	b2World* _pWorld;

	bool	_bBroke;

// semi-constantes
protected:
	double _timeStep;
	int32   _iterations;

	// Tempor�rias durante simula��o:
	b2Vec2	_x0	;
	double	_t;
	bool	m_bContactR1;
	bool	m_bContactR2;
	double	_last_contact_r1;
	double	_last_contact_r2;
	double	_no_contact_time_r1;
	double	_no_contact_time_r2;
	b2Vec2 _cVel;
	b2Vec2 _cPos;

// Dados ef�meros. S� existem durante a medi��o/simula��o do carro.
public:
	double _trqA;
	double _trqB;
	double _trqC;
	double _trqD;

	double	_angle;

private:
	void _simulation_pre_tick	(void			);
	CContactListener _cl;
	bool _bInStep;
	void _verificar_step(void);

protected:
	void _init(void);

	// Usa as defini��es decodificadas para criar o objeto em si no box2d
	void _create				(b2World* pWorld, CCarDef carro);
	void _destroy				(void			);
	void _phys_begin_simulate	(void			);
	void _phys_end_simulate		(void			);
	bool _simulation_step		(void			);


// Suporte � simula��o no Box2d
public:
	b2Vec2 getCenter(void);
	// Queries
	b2Body* getR1(void){return _pRoda1;}
	b2Body* getR2(void){return _pRoda2;}
	b2Body* getP1(void){return _pPeso1;}
	b2Body* getP2(void){return _pPeso2;}
	double	getT(void){return _t;}

	void Destroy(void){_destroy();}

	bool	m_bDead				;
	double	m_distancia			;
	double	m_contatoR1			;
	double	m_contatoR2			;
	double	m_acum_contatoR1	;
	double	m_acum_contatoR2	;
	double	m_vm				;
	double	m_t					;

};

b2World* buildWorld(CEnv* env);

};//namespace PHYS

#endif //__PHYSCAR_H__