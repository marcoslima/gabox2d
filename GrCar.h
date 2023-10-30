#ifndef __GRCAR_H__
#define __GRCAR_H__

#include "CarDef.h"

namespace GUI
{
static CImage	g_imgRoda;
static CImage	g_imgCrystal;
static CImage	g_imgCracked;
static bool		g_bImgLoaded = false;
static bool		g_bImgFailed = false;

class CGrCar
{
public:
	CGrCar(void);
	~CGrCar(void);

	// Draw
	void Draw(Graphics *pGr);

	// Draw offline (carro n�o criado, apenas com os def's)
	void DrawOffline(Graphics *pGr);
public:
	typedef struct tagCircle
	{
		PointF	c;
		double	r;
	} circle_t;
	typedef struct tagRoda
	{
		circle_t	c		;
		bool		touch	;
		double		angle	;

	} roda_t;

	typedef struct tagPeso
	{
		circle_t	c		;
		bool		broke	;
	} peso_t;

	roda_t	_roda1	;
	roda_t	_roda2	;
	peso_t	_peso1	;
	peso_t	_peso2	;
	PointF	_cm		;
	bool	_broke	;
};

};//namespace GUI

#endif //__GRCAR_H__