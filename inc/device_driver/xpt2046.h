/*
 * xpt2046.h
 *
 *  Created on: 9 Jan 2019
 *      Author: manchak
 */

#ifndef XPT2046_H_
#define XPT2046_H_

#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "system/pinout/pinout.h"
#include "device_driver/led.h"
#include <functional>
#include "../config/config.h"

#ifdef XPT2046_USED

namespace DeviceDriver
{

#define XPT2046_XChannel (uint8_t)0xD0
#define XPT2046_YChannel (uint8_t)0x90

class XPT2046
{
  public:
	struct Config
	{
		// id of the st7735 it is attached to
		uint8_t id;

		// Orientation of the screen (four direction: 0 1 2 3, count in clockwise)
		uint8_t orientation = 0;

		uint32_t verticalLength = 23399;
		uint32_t horizontalLength = 23399;
		uint32_t verticalOffset = 189;
		uint32_t horizontalOffset = 181;

		uint32_t lcdWidth = 130;
		uint32_t lcdHeight = 161;
	};

	XPT2046(const Config &config);

	~XPT2046()
	{
		delete[] spiTransferTxData;
	}

	bool isTouched(uint32_t &out_x, uint32_t &out_y)
	{
		// parsing data
		long x, y, x0, x1, y0, y1;
		readTouch(x0, y0);
		readTouch(x1, y1);

		x = (x0 + x1) / 2;
		y = (y0 + y1) / 2;

		bool isInvalid = (x0 < 200) | (x1 < 200) | (abs(x0 - x1) > 100);
		isInvalid |= (y0 < 200) | (y1 < 200) | (abs(y0 - y1) > 100);

		if (isInvalid)
		{
			out_x = lastTouch_x;
			out_y = lastTouch_y;
			return false;
		}

		lastTouch_x = (1000 * (x - horizontalOffset)) / horizontalLength;
		lastTouch_y = (1000 * (y - verticalOffset)) / verticalLength;

		switch (this->orientation)
		{
		case 0:
			lastTouch_x = lcdWidth - lastTouch_x;
			lastTouch_y = lcdHeight - lastTouch_y;
			break;
		case 1:
		{
			uint32_t temp = lastTouch_x;
			lastTouch_x = lastTouch_y;
			lastTouch_y = lcdHeight - temp;
		}
		break;
		case 2:
			break;
		case 3:
		{
			uint32_t temp = lastTouch_x;
			lastTouch_x = lcdWidth - lastTouch_y;
			lastTouch_y = temp;
		}
		break;
		}

		out_x = lastTouch_x;
		out_y = lastTouch_y;
		return true;
	}

  private:
	const uint8_t id, orientation;
	const uint32_t verticalLength, horizontalLength, verticalOffset, horizontalOffset, lcdWidth, lcdHeight;
	const System::Pinout::Name xpt2046_pins[XPT2046_USED * 5] = XPT2046_PINS;
	uint32_t lastTouch_x, lastTouch_y;
	Driver::SpiMaster spi_master;
	const uint32_t *spiTransferTxData;

	void readTouch(long &xSum, long &ySum);
	Driver::SpiMaster::Config GetSpiConfig(const Config &);
	const uint32_t *GetTransferData();
};
} // namespace DeviceDriver

#endif

#endif /* XPT2046_H_ */
