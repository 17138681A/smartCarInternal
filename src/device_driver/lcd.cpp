/*
 * lcd.cpp
 *
 *  Created on: Sep 25, 2018
 *      Author: LeeChunHei
 */

#include "device_driver/lcd.h"
#include <cstdlib>

namespace DeviceDriver
{

void Lcd::DrawCircle(const uint32_t xc, const uint32_t yc, const uint32_t radius, const uint32_t boundary_color, const uint32_t width, const bool color_filled, const uint32_t fill_color)
{
	if (color_filled)
	{
		if (fill_color)
		{ // Adding useless approach to cancel compiling warning before finishing the function
		}
	}
	else
	{
		int xo = radius + (width >> 1);
		int xi = radius - (width >> 1) - 1;
		int y = 0;
		int erro = 1 - xo;
		int erri = 1 - xi;

		while (xo >= y)
		{
			DrawLine(xc + xi, yc + y, xc + xo, yc + y, boundary_color);
			DrawLine(xc + y, yc + xi, xc + y, yc + xo, boundary_color);
			DrawLine(xc - xo, yc + y, xc - xi, yc + y, boundary_color);
			DrawLine(xc - y, yc + xi, xc - y, yc + xo, boundary_color);
			DrawLine(xc - xo, yc - y, xc - xi, yc - y, boundary_color);
			DrawLine(xc - y, yc - xo, xc - y, yc - xi, boundary_color);
			DrawLine(xc + xi, yc - y, xc + xo, yc - y, boundary_color);
			DrawLine(xc + y, yc - xo, xc + y, yc - xi, boundary_color);

			y++;

			if (erro < 0)
			{
				erro += 2 * y + 1;
			}
			else
			{
				xo--;
				erro += 2 * (y - xo + 1);
			}

			if (y > (int)(radius - (width >> 1)))
			{
				xi = y;
			}
			else
			{
				if (erri < 0)
				{
					erri += 2 * y + 1;
				}
				else
				{
					xi--;
					erri += 2 * (y - xi + 1);
				}
			}
		}
	}
}

void Lcd::DrawLine(const uint32_t start_x, const uint32_t start_y, const uint32_t end_x, const uint32_t end_y, const uint32_t color, const uint32_t width)
{
	if (start_x == end_x)
	{
		FillColor(DeviceDriver::Lcd::Rect(start_x - (width >> 1), start_y, width, std::abs((int)(end_y - start_y))), color);
	}
	else if (start_y == end_y)
	{
		FillColor(DeviceDriver::Lcd::Rect(start_x, start_y - (width >> 1), std::abs((int)(end_x - start_x)), width), color);
	}
	else
	{
		//		deltaX = endpoint.X - origin.X
		//		deltaY = endpoint.Y - origin.Y
		//		error = 0
		//
		//		// Note the below fails for completely vertical lines.
		//		int delta_rror = std::abs()
		//		absoluteValue(deltaY / deltaX)
		//
		//		Y = origin.Y
		//	for (X from origin.X to endpoint.X) {
		//		surface.PlotPixel(X, Y)
		//		error = error + deltaError
		//		if (error >= 0.5) {
		//			++Y;
		//			error -= 1.0
		//		}
		//	}
	}
}

} // namespace DeviceDriver
