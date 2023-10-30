#include "StdAfx.h"
#include "physcar.h"

namespace PHYS
{
int ID_RODA1 = 1;
int ID_RODA2 = 2;
int ID_PESO1 = 3;
int ID_PESO2 = 4;
int ID_GROUND = 5;


b2Vec2 TranslateVec(vec2_t a)
{
	return b2Vec2(a.x,a.y);
}

b2World* buildWorld(CEnv* env)
{
	vec_vecs_t GroundPoly = env->get_vecs();

	b2World *pWorld;
	b2AABB ab2AABB;
	ab2AABB.lowerBound.Set(env->_tlx,env->_bry);
	ab2AABB.upperBound.Set(env->_brx,env->_tly);

	b2Vec2 gravity(0.0f, -10.0f);
	bool doSleep = true;
	pWorld = new b2World(ab2AABB,gravity,doSleep);

	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, 0.0f);	

	b2Body *pGround = pWorld->CreateBody(&groundBodyDef);

	b2PolygonDef groundShapeDef;
	groundShapeDef.SetAsBox(30.0f, 1.0f);
	groundShapeDef.friction = 1.0;
	groundShapeDef.restitution = 0.0;

	groundShapeDef.vertexCount = 4;

	size_t k,nSize = GroundPoly.size();
	b2Vec2 pt;
	b2Vec2 lpt = TranslateVec(GroundPoly[0]);
	for(k = 1; k < nSize; k++)
	{
		pt = TranslateVec(GroundPoly[k]);
		groundShapeDef.vertices[0] = lpt;
		groundShapeDef.vertices[1].Set(lpt.x,env->_bry);
		groundShapeDef.vertices[2].Set(pt.x,env->_bry);
		groundShapeDef.vertices[3] = pt;

		pGround->CreateShape(&groundShapeDef);

		lpt = pt;
	}

	// Limitadores dos boundary's:
	// Parede direita:

	groundShapeDef.vertices[0].Set(env->_brx-3,env->_tly);
	groundShapeDef.vertices[1].Set(env->_brx-3,env->_bry);
	groundShapeDef.vertices[2].Set(env->_brx,env->_bry);
	groundShapeDef.vertices[3].Set(env->_brx,env->_tly);
	pGround->CreateShape(&groundShapeDef);

	// Teto
	groundShapeDef.vertices[0].Set(env->_tlx,env->_tly-3);
	groundShapeDef.vertices[1].Set(env->_brx,env->_tly-3);
	groundShapeDef.vertices[2].Set(env->_brx,env->_tly);
	groundShapeDef.vertices[3].Set(env->_tlx,env->_tly);
	pGround->CreateShape(&groundShapeDef);

	// Parede esquerda:
	groundShapeDef.vertices[0].Set(env->_tlx,env->_tly);
	groundShapeDef.vertices[1].Set(env->_tlx,env->_bry);
	groundShapeDef.vertices[2].Set(env->_tlx+3,env->_bry);
	groundShapeDef.vertices[3].Set(env->_tlx+3,env->_tly);
	pGround->CreateShape(&groundShapeDef);

	pGround->SetUserData((void*)&ID_GROUND);

	return pWorld;
}


b2_def_t TranslateRoda(const CCarDef::CRoda& roda)
{
	b2_def_t ret;
	ret.bd.position.Set(roda.c.x,roda.c.y);
	ret.sd.radius		= roda.c.raio			;
	ret.sd.density		= roda.b.densidade		;
	ret.sd.friction		= roda.b.friccao		;
	ret.sd.restitution	= roda.b.elasticidade	;

	return ret;
}

CPhysCar::CPhysCar(void)
{
	_init();
}

CPhysCar::~CPhysCar(void)
{
}

void CPhysCar::_create(b2World *pWorld, CCarDef carro)
{
	_verificar_step();
	if(_pWorld != NULL)
		_destroy();

	_car_def.R1 = TranslateRoda(carro._roda1);
	_car_def.R2 = TranslateRoda(carro._roda2);
	_car_def.P1 = TranslateRoda(carro._peso1);
	_car_def.P2 = TranslateRoda(carro._peso2);
	for(int i = 0; i < 6; i++)
	{
		if(i < 4) _car_def.torque[i] = carro._torque[i];
		_car_def.freq[i] = carro._freq[i];
		_car_def.damp[i] = carro._damp[i];
	}

	// Pesos são também sensores:
//	_car_def.P1.sd.isSensor = true;
//	_car_def.P2.sd.isSensor = true;


	_pWorld = pWorld;

	//////////////////////////////////////////////
	// Criação dos objetos:
	_pRoda1 = _pWorld->CreateBody(&_car_def.R1.bd);
	_pRoda1->CreateShape(&_car_def.R1.sd);
	_pRoda1->SetMassFromShapes();
	_pRoda1->SetUserData((void *)&ID_RODA1);

	_pRoda2 = _pWorld->CreateBody(&_car_def.R2.bd);
	_pRoda2->CreateShape(&_car_def.R2.sd);
	_pRoda2->SetMassFromShapes();
	_pRoda2->SetUserData((void *)&ID_RODA2);

	_pPeso1 = _pWorld->CreateBody(&_car_def.P1.bd);
	_pPeso1->CreateShape(&_car_def.P1.sd);
	_pPeso1->SetBullet(true);
	_pPeso1->SetMassFromShapes();
	_pPeso1->SetUserData((void *)&ID_PESO1);

	_pPeso2 = _pWorld->CreateBody(&_car_def.P2.bd);
	_pPeso2->CreateShape(&_car_def.P2.sd);
	_pPeso2->SetBullet(true);
	_pPeso2->SetMassFromShapes();
	_pPeso2->SetUserData((void *)&ID_PESO2);


	/////////////////
	// Joints:
	b2DistanceJointDef jd;
	int j = 0;
	jd.Initialize(_pRoda1, _pRoda2, _pRoda1->GetWorldCenter(), _pRoda2->GetWorldCenter());
	jd.collideConnected = true;
	jd.dampingRatio = _car_def.damp[j];
	jd.frequencyHz  = _car_def.freq[j++];
	
	_pJc1c2 = _pWorld->CreateJoint(&jd);

	jd.Initialize(_pRoda1, _pPeso1, _pRoda1->GetWorldCenter(), _pPeso1->GetWorldCenter());
	jd.dampingRatio = _car_def.damp[j];
	jd.frequencyHz  = _car_def.freq[j++];
	_pJc1p1 = _pWorld->CreateJoint(&jd);

	jd.Initialize(_pRoda1, _pPeso2, _pRoda1->GetWorldCenter(), _pPeso2->GetWorldCenter());
	jd.dampingRatio = _car_def.damp[j];
	jd.frequencyHz  = _car_def.freq[j++];
	_pJc1p2 = _pWorld->CreateJoint(&jd);

	jd.Initialize(_pRoda2, _pPeso1, _pRoda2->GetWorldCenter(), _pPeso1->GetWorldCenter());
	jd.dampingRatio = _car_def.damp[j];
	jd.frequencyHz  = _car_def.freq[j++];
	_pJc2p1 = _pWorld->CreateJoint(&jd);

	jd.Initialize(_pRoda2, _pPeso2, _pRoda2->GetWorldCenter(), _pPeso2->GetWorldCenter());
	jd.dampingRatio = _car_def.damp[j];
	jd.frequencyHz  = _car_def.freq[j++];
	_pJc2p2 = _pWorld->CreateJoint(&jd);

	jd.Initialize(_pPeso1, _pPeso2, _pPeso1->GetWorldCenter(), _pPeso2->GetWorldCenter());
	jd.dampingRatio = _car_def.damp[j];
	jd.frequencyHz  = _car_def.freq[j++];
	_pJp1p2 = _pWorld->CreateJoint(&jd);

	// Torques:
	_trqA = _car_def.torque[0];
	_trqB = _car_def.torque[1];
	_trqC = _car_def.torque[2];
	_trqD = _car_def.torque[3];
}


#define ISBODY(s1,s2,id) (s1 == id  || s2 == id)
void CContactListener::Add(const b2ContactPoint* point)
{
	int s1 = *((int *)point->shape1->GetBody()->GetUserData());
	int s2 = *((int *)point->shape2->GetBody()->GetUserData());

	bool bGround = ISBODY(s1,s2,ID_GROUND);
	bool bR1	 = ISBODY(s1,s2,ID_RODA1 );
	bool bR2	 = ISBODY(s1,s2,ID_RODA2 );
	bool bP1	 = ISBODY(s1,s2,ID_PESO1 );
	bool bP2	 = ISBODY(s1,s2,ID_PESO2 );

	if((bGround && (bP1 || bP2)) || (bR1 && bR2) )
		m_bDead = true;

	m_cVel = point->velocity;
	m_cPos = point->position;
/*
	if(point->velocity.Length() > 100.0)
	{
		m_bDead = true;
	}
*/
}

void CContactListener::Persist(const b2ContactPoint* point)
{
	int s1 = *((int *)point->shape1->GetBody()->GetUserData());
	int s2 = *((int *)point->shape2->GetBody()->GetUserData());

	bool bGround = ISBODY(s1,s2,ID_GROUND);
	bool bR1	 = ISBODY(s1,s2,ID_RODA1 );
	bool bR2	 = ISBODY(s1,s2,ID_RODA2 );

	if(bGround)
	{
		m_bContactR1 |= bR1;
		m_bContactR2 |= bR2;
	}
}

// Executa um passo da simulação e retorna false se o carro morreu.
bool CPhysCar::_simulation_step(void)
{
	_bInStep = true;
	_simulation_pre_tick();

	_cl.m_bDead      = false;
	_cl.m_bContactR1 = false;
	_cl.m_bContactR2 = false;

	_pWorld->Step(_timeStep,_iterations);

	// Está vivo ainda?
	m_bDead |= _cl.m_bDead;
	_cVel = _cl.m_cVel;
	_cPos = _cl.m_cPos;
	m_bContactR1 = _cl.m_bContactR1;
	m_bContactR2 = _cl.m_bContactR2;

	// Contato das rodas:
	if(m_bContactR1)
	{
		m_acum_contatoR1 += _timeStep;
		_last_contact_r1 = _t;
	}
	if(m_bContactR2)
	{
		m_acum_contatoR2 += _timeStep;
		_last_contact_r2 = _t;
	}

	_no_contact_time_r1 = _t - _last_contact_r1;
	_no_contact_time_r2 = _t - _last_contact_r2;

	if(_no_contact_time_r1 > 2 || _no_contact_time_r2 > 2)
	{
		m_bDead = true;
	}

	if(m_bDead && _pJp1p2)
	{
		_pWorld->DestroyJoint(_pJc1c2);
		_pWorld->DestroyJoint(_pJc1p1);
		_pWorld->DestroyJoint(_pJc1p2);
		_pWorld->DestroyJoint(_pJc2p1);
		_pWorld->DestroyJoint(_pJc2p2);
		_pWorld->DestroyJoint(_pJp1p2);

		_pJc1c2 = NULL;
		_pJc1p1 = NULL;
		_pJc1p2 = NULL;
		_pJc2p1 = NULL;
		_pJc2p2 = NULL;
		_pJp1p2 = NULL;
	}

	m_contatoR1 = m_acum_contatoR1;
	m_contatoR2 = m_acum_contatoR2;

	b2Vec2 x = getCenter();
	m_distancia = x.x - _x0.x;
	if(m_distancia < 0)
		m_distancia = 0;


	if(_t != 0)
	{
		m_vm = m_distancia / _t;
	}
	else
	{
		m_vm = 0;
	}

	_t += _timeStep;

	_bInStep = false;
	return !m_bDead;	
}

void CPhysCar::_phys_begin_simulate(void)
{
	ASSERT(_pWorld != NULL);
	ASSERT(_pRoda1 != NULL);

	_x0 = getCenter();
	_t	 = 0;

	_pWorld->SetContactListener(&_cl);

	m_contatoR1 = 0;
	m_contatoR2 = 0;
	m_acum_contatoR1 = 0;
	m_acum_contatoR2 = 0;

	m_vm = 0;
	m_distancia = 0;

	_last_contact_r1 = 0;
	_last_contact_r2 = 0;
}

void CPhysCar::_phys_end_simulate(void)
{
	_pWorld->SetContactListener(NULL);
}


void CPhysCar::_simulation_pre_tick(void)
{
	float32 dx,dy;
	float32 fAngulo;

	dx = _pRoda2->GetPosition().x - _pRoda1->GetPosition().x;
	dy = _pRoda2->GetPosition().y - _pRoda1->GetPosition().y;
	if(dx == 0)
	{
		fAngulo = M_PI_2 * (dy > 0)?(1):(-1);
	}
	else
	{
		fAngulo = atan(dy/dx);	
	}

	if(fAngulo > M_PI_2)
		fAngulo -= M_PI_2;
	if(fAngulo < -M_PI_2)
		fAngulo += M_PI_2;

	_angle = fAngulo * (180 / M_PI);

	if(!m_bDead)
	{
		if(m_bContactR1)
			_pRoda1->ApplyTorque((_trqA+_trqB) * _pRoda1->GetMass());

		if(m_bContactR2)
			_pRoda2->ApplyTorque((_trqC+_trqD) * _pRoda2->GetMass());
	}
}


b2Vec2 CPhysCar::getCenter(void)
{
	b2Vec2 pos[5];
	float32 massa[4];

	pos[0] = _pRoda1->GetPosition();
	pos[1] = _pRoda2->GetPosition();
	pos[2] = _pPeso1->GetPosition();
	pos[3] = _pPeso2->GetPosition();

	massa[0] = _pRoda1->GetMass();
	massa[1] = _pRoda2->GetMass();
	massa[2] = _pPeso1->GetMass();
	massa[3] = _pPeso2->GetMass();

	pos[4] = (massa[0]*pos[0] + massa[1]*pos[1] + massa[2]*pos[2] + massa[3]*pos[3]);
	pos[4] *= (float32)1.0/(float32)(massa[0]+massa[1]+massa[2]+massa[3]);

	return pos[4];
}

void CPhysCar::_init(void)
{
	_pRoda1 = NULL;
	_pRoda2 = NULL;
	_pPeso1 = NULL;
	_pPeso2 = NULL;

	_pJc1c2 = NULL;
	_pJc1p1 = NULL;
	_pJc1p2 = NULL;
	_pJc2p1 = NULL;
	_pJc2p2 = NULL;
	_pJp1p2 = NULL;

	_pWorld = NULL;

	_timeStep = 1.0f/50.0f;
	_iterations = 10;
	_bInStep = false;
	_bBroke = false;
	m_bDead = false;
}

void CPhysCar::_destroy(void)
{
	if(_pWorld == NULL)
		return;

	_verificar_step();

	// Consideraremos que todos os corpos existem se um deles existir
	_pWorld->DestroyBody(_pRoda1);
	_pWorld->DestroyBody(_pRoda2);
	_pWorld->DestroyBody(_pPeso1);
	_pWorld->DestroyBody(_pPeso2);
	_pWorld->SetContactListener(NULL);

	// NULLamos tudo:
	_init();
}

void CPhysCar::_verificar_step(void)
{
	if(_bInStep)
	{
		// Aguardamos até 5 segundos antes de prosseguir:
		for(int i = 0; i < 500 || !_bInStep; i++)
		{
			Sleep(10);
		}
	}
}

};//namespace PHYS
