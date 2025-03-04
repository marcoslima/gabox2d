#ifndef __GRCAR_H__
#define __GRCAR_H__

#include "CarDef.h"

#define CImage int
#define Graphics int
#include <SFML/Graphics.hpp>

#define PointF sf::Vector2f

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
	CGrCar();
	~CGrCar();

	// Draw
	void Draw(sf::RenderWindow& window) const;

	using gr_circle_t = struct tagCircle
	{
		PointF	center;
		float	radius{};
	};

	using gr_roda_t = struct tagRoda
	{
		gr_circle_t	circle		;
		bool		touch{}	;
		float		angle{}	;

	};

	using gr_peso_t = struct tagPeso
	{
		gr_circle_t	circle		;
		bool		broke{}	;
	};

	gr_roda_t	_roda1	= {};
	gr_roda_t	_roda2	= {};
	gr_peso_t	_peso1	= {};
	gr_peso_t	_peso2	= {};
	PointF	_cm		= {};
	bool	_broke	= {};
};

}

#endif //__GRCAR_H__