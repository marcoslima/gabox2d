#pragma once

#include <IGrCar.h>

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

class CGrCar final : public IGrCar
{
	void Draw(sf::RenderWindow& window) const;

public:
	CGrCar();
	~CGrCar() override;

	void draw(void *pParams) const override;

	void setRoda1(float center_x, float center_y, float radius, float angle, bool touch) override;
	void setRoda2(float center_x, float center_y, float radius, float angle, bool touch) override;
	void setPeso1(float center_x, float center_y, float radius, bool broke) override;
	void setPeso2(float center_x, float center_y, float radius, bool broke) override;
	void setCenter(float center_x, float center_y) override;
	void setBroke(bool broke) override;
	[[nodiscard]] gr_car_ptr_t clone() override;

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
