#include <stdafx.h>
#include "car.h"

void CCar::_init(void)
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
	_nn.clear();
}

CCar::CCar()
{
	_generate_random_genes();
	_init();
}

CCar::CCar(const char* szGenes)
{
	strcpy(_genes,szGenes);
	_init();
}

void CCar::_destroy(void)
{
	if(_pWorld == NULL)
		return;

	if(_pRoda1 != NULL)
	{
		// Consideraremos que todos os corpos existem se um deles existir
		_pWorld->DestroyBody(_pRoda1);
		_pWorld->DestroyBody(_pRoda2);
		_pWorld->DestroyBody(_pPeso1);
		_pWorld->DestroyBody(_pPeso2);
	}

	// NULLamos tudo:
	_init();
}

void CCar::_generate_random_genes(void)
{
	int i;
	for(i = 0; i < GENES; i++)
	{
		_genes[i] = 'A' + rand()%26;
	}
	_genes[i] = 0;

	return;
}


void CCar::getGenes(string& genes)
{
	genes = string(_genes);
}

string CCar::getGenesString(void)
{
	return string(_genes);
}

void CCar::getGenes(CString& genes)
{
	genes = CString(_genes);
}

CString CCar::getGenesCString(void)
{
	return CString(_genes);
}

double d26[] = 
{
			1,
		   26,
		  676,
		17576,
	   456976,
	 11881376,
	308915776
};

double map_values(double in_min, double in_max, double out_min, double out_max, double val)
{
	double inDelta  = in_max  - in_min ;
	double outDelta = out_max - out_min;
	
	return out_min + (val - in_min)*outDelta/inDelta;
}

double DecodeGen(int nLen, char *genes, double nMin, double nMax)
{
	double dVal = 0;
	for(int i = 0; i < nLen; i++)
	{
		dVal += (genes[i] - 'A') * d26[i];
	}


//	1 dígito: A - Z ou 0 à 25, ou seja, d26[1]-1
//	2 dígitos: Máximo: ZZ que é 25*26 + 25 = 675, ou seja, d26[2]-1

	return map_values(0,d26[nLen]-1,nMin,nMax,dVal);
}

b2_def_t DecodeRoda(char *genes)
{
	b2_def_t def;
	int i = 0;
	int nLen = 4;

	def.bd.position.x = DecodeGen(nLen,&genes[i+=nLen],-8  , 8 );
	def.bd.position.y = DecodeGen(nLen,&genes[i+=nLen], 2  , 8 );
	def.sd.radius	  = DecodeGen(nLen,&genes[i+=nLen], 0.2, 3 );
	def.sd.density    = DecodeGen(nLen,&genes[i+=nLen], 0.1, 10);
	def.sd.friction   = DecodeGen(nLen,&genes[i+=nLen], 0.1, 2 );
	def.sd.restitution= DecodeGen(nLen,&genes[i+=nLen], 0  , 1 );

	return def;
}

void CCar::_create(void)
{
	ASSERT(_pRoda1 == NULL);

	const double dFp = 10;
	int nLen = 4;
	int pos = 0;

	// Obtemos os body's e shape's def's dos genes:
	car_t car_def;

	car_def.R1 = DecodeRoda(&_genes[pos += 18]);
	car_def.R2 = DecodeRoda(&_genes[pos += 18]);
	car_def.P1 = DecodeRoda(&_genes[pos += 18]);
	car_def.P2 = DecodeRoda(&_genes[pos += 18]);
	_trqA = DecodeGen(nLen,&_genes[pos += nLen],-dFp,dFp);
	_trqB = DecodeGen(nLen,&_genes[pos += nLen],-dFp,dFp);
	_trqC = DecodeGen(nLen,&_genes[pos += nLen],-dFp,dFp);
	_trqD = DecodeGen(nLen,&_genes[pos += nLen],-dFp,dFp);

	/*
		O ann será:

		20 20 1

		com 4 entradas

		Assim, temos:
		20 x 4 pesos na camada 1
		20 x 20 pesos na camada 2
		1 x 20 pesos na camada 3

		total: 80 + 400 + 20 = 500 doubles
		Cada double gasta 3 gens -> 1500 genes!!!!

		Que tal outra rede pra começar? :D

		5 10 1, com 4 entradas:

		5 x 4 pesos na camada 1
		10 x 5 pesos na camada 2
		1 x 10 pesos na camada 3
		total: 20 + 50 + 10 = 80
		--> 240 genes.

		Arredondando geral: 300 genes total:
	*/

	{
		_nn.Init(4,5,10,1);

		size_t i,j;
		// Pesos da primeira camada:
		nLen = 3;
		for(i = 0; i < 5; i++)
		{
			for(j = 0; j < 4; j++)
			{
				_nn.setPeso(1,i,j,DecodeGen(nLen,&_genes[pos += nLen],-10,10));
			}
		}

		// Pesos da segunda camada:
		for(i = 0; i < 10; i++)
		{
			for(j = 0; j < 5; j++)
			{
				_nn.setPeso(2,i,j,DecodeGen(nLen,&_genes[pos += nLen],-10,10));
			}
		}

		// Pesos da terceira camada:
		for(j = 0; j < 10; j++)
		{
			_nn.setPeso(3,0,j,DecodeGen(nLen,&_genes[pos += nLen],-10,10));
		}
	}

//	_torque = map_values(0,650,-40,-10,_genes[72]-'A' + (_genes[73]-'A') * 26);

	// Pesos são também sensores:
	car_def.P1.sd.isSensor = true;
	car_def.P2.sd.isSensor = true;

	//////////////////////////////////////////////
	// Criação dos objetos:
	_pRoda1 = _pWorld->CreateBody(&car_def.R1.bd);
	_pRoda1->CreateShape(&car_def.R1.sd);
	_pRoda1->SetMassFromShapes();

	_pRoda2 = _pWorld->CreateBody(&car_def.R2.bd);
	_pRoda2->CreateShape(&car_def.R2.sd);
	_pRoda2->SetMassFromShapes();

	_pPeso1 = _pWorld->CreateBody(&car_def.P1.bd);
	_pPeso1->CreateShape(&car_def.P1.sd);
	_pPeso1->SetBullet(true);
	_pPeso1->SetMassFromShapes();

	_pPeso2 = _pWorld->CreateBody(&car_def.P2.bd);
	_pPeso2->CreateShape(&car_def.P2.sd);
	_pPeso2->SetBullet(true);
	_pPeso2->SetMassFromShapes();

	/////////////////
	// Joints:
	float32 damp = 1.0;
	b2DistanceJointDef jd;
	jd.Initialize(_pRoda1, _pRoda2, _pRoda1->GetWorldCenter(), _pRoda2->GetWorldCenter());
	jd.collideConnected = true;
	jd.dampingRatio = damp;
	
	_pJc1c2 = _pWorld->CreateJoint(&jd);

	jd.Initialize(_pRoda1, _pPeso1, _pRoda1->GetWorldCenter(), _pPeso1->GetWorldCenter());
	jd.dampingRatio = damp;
	_pJc1p1 = _pWorld->CreateJoint(&jd);

	jd.Initialize(_pRoda1, _pPeso2, _pRoda1->GetWorldCenter(), _pPeso2->GetWorldCenter());
	jd.dampingRatio = damp;
	_pJc1p2 = _pWorld->CreateJoint(&jd);

	jd.Initialize(_pRoda2, _pPeso1, _pRoda2->GetWorldCenter(), _pPeso1->GetWorldCenter());
	jd.dampingRatio = damp;
	_pJc2p1 = _pWorld->CreateJoint(&jd);

	jd.Initialize(_pRoda2, _pPeso2, _pRoda2->GetWorldCenter(), _pPeso2->GetWorldCenter());
	jd.dampingRatio = damp;
	_pJc2p2 = _pWorld->CreateJoint(&jd);

	jd.Initialize(_pPeso1, _pPeso2, _pPeso1->GetWorldCenter(), _pPeso2->GetWorldCenter());
	jd.dampingRatio = damp;
	_pJp1p2 = _pWorld->CreateJoint(&jd);
}

void CCar::setGenes(const char *genes)
{
	if(genes == NULL || strlen(genes) != GENES)
		_generate_random_genes();
	else
		strcpy(_genes,genes);
}

void CCar::CreateCar(const char *genes)
{
	setGenes(genes);
}

b2Vec2 CCar::getCenter(void)
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
	pos[4] *= 1.0/(massa[0]+massa[1]+massa[2]+massa[3]);

	return pos[4];
}

void DrawJoint(CDC *pDc, b2Joint* pJ)
{
	// Joints:
	b2Vec2 j1,j2;
	j1 = pJ->GetAnchor1();
	j2 = pJ->GetAnchor2();

	j1 *= 100;
	j2 *= 100;

	pDc->MoveTo(j1.x,j1.y);
	pDc->LineTo(j2.x,j2.y);
}

void DrawRoda(CDC *pDc, b2Body *pRoda, int nOb)
{
	CRect rcBall;
	float32 r,fAngle;
	b2Vec2 x;
	b2CircleShape *circle;
	b2Vec2 vPos;

	circle = (b2CircleShape*)pRoda->GetShapeList();
	vPos = pRoda->GetPosition();
	x = vPos + circle->GetLocalPosition();
	r = circle->GetRadius();

	x *= 100;
	r *= 100;
	rcBall = CRect(x.x-r,x.y-r,x.x+r,x.y+r);

	if(!g_bImgFailed && !g_bImgLoaded)
	{
		HRESULT hres = g_imgRoda.Load("roda.png");
		if(hres == 0)
		{
			g_bImgLoaded = true;
	
			hres = g_imgCrystal.Load("crystal.png");
			if(hres != 0)
			{
				g_bImgLoaded = false;
				g_bImgFailed = true;
			}
			else
			{
				hres = g_imgCracked.Load("cracked.png");
				if(hres != 0)
				{
					g_bImgLoaded = false;
					g_bImgFailed = true;
				}
			}
		}
		else
		{
			g_bImgFailed = true;
		}
	}

	if(g_bImgLoaded)
	{
		switch(nOb)
		{
		case 0:	// Roda 1
		case 1: // Roda 2
		{
			XFORM xf;
			double cs = cos((double)pRoda->GetAngle());
			double sn = sin((double)pRoda->GetAngle());
			xf.eM11 = xf.eM22 = cs;
			xf.eM12 = sn;
			xf.eM21 = -sn;
			xf.eDx = rcBall.CenterPoint().x;
			xf.eDy = rcBall.CenterPoint().y;
			
			rcBall -= CPoint(rcBall.TopLeft());
			rcBall -= CPoint(rcBall.Width()/2,rcBall.Height()/2);

			int nRes = pDc->SaveDC();
			SetGraphicsMode(pDc->m_hDC,GM_ADVANCED);
			SetWorldTransform(pDc->m_hDC,&xf);

			g_imgRoda.AlphaBlend(pDc->m_hDC,rcBall,CRect(CPoint(0,0),CSize(g_imgRoda.GetWidth(),g_imgRoda.GetHeight())));
			pDc->RestoreDC(nRes);

			break;
		}

		case 2: // Peso
			g_imgCrystal.AlphaBlend(pDc->m_hDC,rcBall,CRect(CPoint(0,0),CSize(g_imgCrystal.GetWidth(),g_imgCrystal.GetHeight())),100);
			break;
		case 3: // Peso quebrado
			g_imgCracked.AlphaBlend(pDc->m_hDC,rcBall,CRect(CPoint(0,0),CSize(g_imgCracked.GetWidth(),g_imgCracked.GetHeight())),100);
			break;
		}
	}
	else
	{
		pDc->Ellipse(rcBall);
	}

	if(!g_bImgLoaded)
	{
		fAngle = pRoda->GetAngle();
		pDc->MoveTo(x.x,x.y);
		pDc->LineTo(x.x + r * cos(fAngle), x.y + r * sin(fAngle));
	}
}

void CCar::Draw(CDC *pDc)
{
	// Rodas 1 e 2
	CBrush bshRoda,bshRodaC;
	CPen penRoda (PS_SOLID,10,RGB(0,0,255));
	CPen penRodaC(PS_SOLID,10,RGB(0,0,  0));

	bshRoda .CreateSolidBrush(RGB(255,255,255));
	bshRodaC.CreateSolidBrush(RGB(255,200,200));

	// Roda 1
	pDc->SelectObject((m_bContactR1)?(&penRodaC):(&penRoda));
	pDc->SelectObject((m_bContactR1)?(&bshRodaC):(&bshRoda));
	DrawRoda(pDc,_pRoda1,0);

	pDc->SelectObject((m_bContactR2)?(&penRodaC):(&penRoda));
	pDc->SelectObject((m_bContactR2)?(&bshRodaC):(&bshRoda));
	DrawRoda(pDc,_pRoda2,1);

	// Pesos 1 e 2
	CBrush bshNull;
	CPen penPeso(PS_DOT,2,RGB(255,0,0));
	bshNull.CreateStockObject(NULL_BRUSH);
	pDc->SelectObject(&bshNull);
	pDc->SelectObject(&penPeso);

	DrawRoda(pDc,_pPeso1,(!m_bDead)?(2):(3));
	DrawRoda(pDc,_pPeso2,(!m_bDead)?(2):(3));

	// Joints:
	CPen penJoint(PS_SOLID,1,RGB(200,200,200));
	pDc->SelectObject(&penJoint);

	DrawJoint(pDc,_pJc1c2);
	DrawJoint(pDc,_pJc1p1);
	DrawJoint(pDc,_pJc1p2);
	DrawJoint(pDc,_pJc2p1);
	DrawJoint(pDc,_pJc2p2);
	DrawJoint(pDc,_pJp1p2);

	// Centro de massa:
	CPen penCm(PS_SOLID,0,RGB(0,0,0));
	pDc->SelectObject(&penCm);
	pDc->SelectObject(&bshNull);

	b2Vec2 vCm = getCenter();
	CPoint ptCm(vCm.x*100,vCm.y*100);
	pDc->MoveTo(ptCm.x-50,ptCm.y-50);
	pDc->LineTo(ptCm.x+50,ptCm.y+50);
	pDc->MoveTo(ptCm.x-50,ptCm.y+50);
	pDc->LineTo(ptCm.x+50,ptCm.y-50);

	CRect rcCm(ptCm.x-25,ptCm.y-25,ptCm.x+25,ptCm.y+25);
	pDc->Ellipse(rcCm);
	rcCm.InflateRect(10,10);
	pDc->Ellipse(rcCm);

}

void CCar::preTick(void)
{
	float32 dx,dy;
	float32 fAngulo,fAngulo2,fAngulo3;

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


//	fAngulo2 = fAngulo * fAngulo;
//	fAngulo3 = fAngulo2 * fAngulo;

//	_torque = _trqA * fAngulo3 + _trqB * fAngulo2 + _trqC * fAngulo + _trqD;
//	_torque = _trqA * exp(fAngulo * _trqB) + _trqC * exp(fAngulo * _trqD) ;

	dado_t entrada;
	entrada.push_back(fAngulo/M_PI);
	entrada.push_back(_pRoda1->GetLinearVelocity().Length()/100.0);
	entrada.push_back(m_bContactR1?1.0:-1.0);
	entrada.push_back(m_bContactR2?1.0:-1.0);
	_torque = _nn.execNet(entrada)[0]*40.0;


	if(m_bContactR1)
		_pRoda1->ApplyTorque(_torque * _pRoda1->GetMass());

	if(m_bContactR2)
		_pRoda2->ApplyTorque(_torque * _pRoda2->GetMass());
}

void CCar::Medir(b2World *pWorld, double max_t)
{
	float32 x0,x;
	int k;
	
	beginSimulate(pWorld);
	x0 = getCenter().x;
	for(k = 0;  _t < max_t; k++)
	{
		if(!doStep())
			break;
	}
	x = getCenter().x;
	m_t = _t;
	Destroy();

	m_distancia = x - x0;
}

void CContactListener::Add(const b2ContactPoint* point)
{
	if
	( 
		(
			point->shape1->GetBody() == m_pGround 
			&&
			(
				point->shape2->GetBody() == m_pCar->getP1() 
				||
				point->shape2->GetBody() == m_pCar->getP2()
			)
		)
		||
		(
			point->shape2->GetBody() == m_pGround 
			&&
			(
				point->shape1->GetBody() == m_pCar->getP1()
				||
				point->shape1->GetBody() == m_pCar->getP2()
			)
		)
	)
	{
		m_bDead = true;
	}
	else if(point->shape1->GetBody() == m_pCar->getR1() &&
			point->shape2->GetBody() == m_pCar->getR2())
	{
		m_bDead = true;	
	}
	else if(point->shape2->GetBody() == m_pCar->getR1() &&
			point->shape1->GetBody() == m_pCar->getR2())
	{
		m_bDead = true;	
	}

}

void CContactListener::Persist(const b2ContactPoint* point)
{
	if(point->shape1->GetBody() == m_pGround)
	{
		if(point->shape2->GetBody() == m_pCar->getR1())
		{
			m_pCar->m_bContactR1 |= true;
		}
		else if(point->shape2->GetBody() == m_pCar->getR2())
		{
			m_pCar->m_bContactR2 |= true;
		}
	}
	else if(point->shape2->GetBody() == m_pGround)
	{
		if(point->shape1->GetBody() == m_pCar->getR1())
		{
			m_pCar->m_bContactR1 |= true;
		}
		else if(point->shape1->GetBody() == m_pCar->getR2())
		{
			m_pCar->m_bContactR2 |= true;
		}
	}
}


void CCar::beginSimulate(b2World *pWorld)
{
	if(_pWorld != NULL)
		_destroy();

	_pWorld = pWorld;
	_cl.m_pGround = pWorld->GetBodyList();

	_create();
	_x0 = getCenter();
	_t	 = 0;

	_cl.m_pCar =  this;

	_pWorld->SetContactListener(&_cl);

	m_contatoR1 = 0;
	m_contatoR2 = 0;
	m_acum_contatoR1 = 0;
	m_acum_contatoR2 = 0;

	m_vm = 0;
	m_distancia = 0;
}

// Executa um passo da simulação e retorna false se o carro morreu.
bool CCar::doStep(void)
{
	preTick();

	_cl.m_bDead  = false;
	m_bContactR1 = false;
	m_bContactR2 = false;

	_pWorld->Step(_timeStep,_iterations);
	_t += _timeStep;

	// Está vivo ainda?
	m_bDead = _cl.m_bDead;
	
	// Contato das rodas:
	if(m_bContactR1)
	{
		m_acum_contatoR1 += _timeStep;
	}
	if(m_bContactR2)
	{
		m_acum_contatoR2 += _timeStep;
	}

	m_contatoR1 = (_t != 0)?(m_acum_contatoR1/_t):(0);
	m_contatoR2 = (_t != 0)?(m_acum_contatoR2/_t):(0);

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

	return !m_bDead;	
}

void CCar::endSimulate(void)
{
	_pWorld->SetContactListener(NULL);
}
