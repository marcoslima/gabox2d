#ifndef __CAR_H__
#define __CAR_H__
#include <box2d.h>
#include <vector>
#include <string>
using namespace std;
#include <atlimage.h>

#define GENES 200
#define GENES_LEN (GENES+1)

typedef struct tagB2Def
{
	b2CircleDef sd;
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

	double	 torque;
} car_t;

static CImage	g_imgRoda;
static CImage	g_imgCrystal;
static CImage	g_imgCracked;
static bool		g_bImgLoaded = false;
static bool		g_bImgFailed = false;

class CCar;
class CContactListener : public b2ContactListener
{
public:
	void Add(const b2ContactPoint* point);
	void Persist(const b2ContactPoint* point);

public:
	CCar   *m_pCar;
	b2Body *m_pGround;
	bool	m_bDead;
};

class CCar
{
public:
	char _genes[GENES_LEN];

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

	float32 _trqA;
	float32 _trqB;
	float32 _trqC;
	float32 _trqD;

	float32 _torque;

	double	_angle;

private:
	float32 _timeStep;
	int32   _iterations;


private:
	void _init(void);
	void _generate_random_genes(void);
	void _create(void);
	void _destroy(void);
public:
	CCar();
	CCar(const char* szGenes);

	void CreateCar(const char *genes = NULL);
	void Destroy(void){_destroy();}

	// Queries
	void getGenes(string& genes);
	void getGenes(CString& genes);
	string getGenesString(void);
	CString getGenesCString(void);
	b2Vec2 getCenter(void);
	b2Body* getR1(void){return _pRoda1;}
	b2Body* getR2(void){return _pRoda2;}
	b2Body* getP1(void){return _pPeso1;}
	b2Body* getP2(void){return _pPeso2;}
	float32 getTorque(void){return _torque;}
	double getPontuacao(void){return _pontos;}
	

	// Sets
	void setGenes(const char *genes);
	void setPontos(double pontos){_pontos = pontos;}

	// Draw
	void Draw(CDC *pDc);

	// Simulate
	void beginSimulate(b2World *pWorld);
	void preTick(void);
	bool doStep(void);
	void endSimulate(void);

	// Estados:
	bool	m_bContactR1;
	bool	m_bContactR2;

private:
	CContactListener _cl;

public:
	// Ga:
	bool	m_bDead		;
	double	m_distancia	;

	double	m_contatoR1	;
	double	m_contatoR2	;

	double	m_acum_contatoR1	;
	double	m_acum_contatoR2	;

	double	m_vm		;
	double	m_t			;

	void Medir(b2World *pWorld, double max_t);
public:
	double	_pontos;

	// Temporária durante simulação:
	b2Vec2	_x0	;
	double	_t;

};

typedef vector<CCar> vec_car_t;



#endif //__CAR_H__