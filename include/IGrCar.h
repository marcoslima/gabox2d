#pragma once

namespace GUI
{
	class IGrCar
	{
	public:
		virtual ~IGrCar() = default;

		virtual void draw(void *pParams) const = 0;
		virtual void setRoda1(float center_x, float center_y, float radius, float angle, bool touch) = 0;
		virtual void setRoda2(float center_x, float center_y, float radius, float angle, bool touch) = 0;
		virtual void setPeso1(float center_x, float center_y, float radius, bool broke) = 0;
		virtual void setPeso2(float center_x, float center_y, float radius, bool broke) = 0;
		virtual void setCenter(float center_x, float center_y) = 0;
		virtual void setBroke(bool broke) = 0;
	};

}
