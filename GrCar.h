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

	using circle_t = struct tagCircle
	{
		PointF	c;
		float	r{};
	};

	using roda_t = struct tagRoda
	{
		circle_t	c		;
		bool		touch{}	;
		float		angle{}	;

	};

	using peso_t = struct tagPeso
	{
		circle_t	c		;
		bool		broke{}	;
	};

	roda_t	_roda1	= {};
	roda_t	_roda2	= {};
	peso_t	_peso1	= {};
	peso_t	_peso2	= {};
	PointF	_cm		= {};
	bool	_broke	= {};
};

}

#endif //__GRCAR_H__