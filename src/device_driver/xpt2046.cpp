/*
 * xpt2046.cpp
 *
 *  Created on: 9 Jan 2019
 *      Author: manchak
 */

#include "device_driver/xpt2046.h"
#include "system/pinout/pinout.h"
#include "system/systick.h"
#include <cstdlib>

#ifdef XPT2046_USED

namespace DeviceDriver
{
Driver::SpiMaster::Config XPT2046::GetSpiConfig(const XPT2046::Config &xptConfig)
{
	Driver::SpiMaster::Config config;

	config.baud_rate = 2000000;

	config.sck = xpt2046_pins[xptConfig.id * 5];
	config.sdo = xpt2046_pins[xptConfig.id * 5 + 1];
	config.sdi = xpt2046_pins[xptConfig.id * 5 + 2];
	config.cs = xpt2046_pins[xptConfig.id * 5 + 3];

	config.between_transfer_delay_ns = 200;
	config.last_sck_to_pcs_delay_ns = 200;
	config.pcs_to_sck_delay_ns = 200;

	return config;
}

const uint32_t *XPT2046::GetTransferData()
{
	uint32_t *data = new uint32_t[61];

	for (int i = 0; i < 30; i += 2)
	{
		data[i] = XPT2046_XChannel;
		data[i + 1] = 0;
		data[i + 30] = XPT2046_YChannel;
		data[i + 30 + 1] = 0;
	}

	data[60] = 0;
	return data;
}

void XPT2046::readTouch(long &xSum, long &ySum)
{
	xSum = ySum = 0;
	uint32_t xData[15] = {0};
	uint32_t yData[15] = {0};

	Driver::SpiMaster::Transfer spiTransfer;
	spiTransfer.rx_data_size = 61;
	spiTransfer.tx_data_size = 61;
	spiTransfer.tx_data = spiTransferTxData;
	spiTransfer.rx_data = new uint32_t[61];
	spiTransfer.continuousTransfer = true;

	spi_master.TransferBlocking(spiTransfer);

	// extract data
	uint32_t *rxData = spiTransfer.rx_data;
	for (int i = 0; i < 30; i += 2)
	{
		xData[i / 2] = rxData[i + 1] << 5 | rxData[i + 2] >> 3;
		yData[i / 2] = rxData[i + 31] << 5 | rxData[i + 32] >> 3;
	}

	// sort data
	for (int i = 0; i < 14; ++i)
	{
		for (int j = i + 1; j < 15; ++j)
		{
			if (xData[i] > xData[j])
			{
				uint32_t t = xData[i];
				xData[i] = xData[j];
				xData[j] = t;
			}

			if (yData[i] > yData[j])
			{
				uint32_t t = yData[i];
				yData[i] = yData[j];
				yData[j] = t;
			}
		}
	}

	// take the average of middle values
	for (int i = 5; i < 10; ++i)
	{
		xSum += xData[i];
		ySum += yData[i];
	}

	xSum /= 5;
	ySum /= 5;
	delete[] rxData;
}

XPT2046::XPT2046(const Config &config) : id(config.id), orientation(config.orientation), 										   	//
								   verticalLength(config.verticalLength), horizontalLength(config.horizontalLength),	   	//
								   verticalOffset(config.verticalOffset), horizontalOffset(config.horizontalOffset),	   	//
								   lcdWidth(config.lcdWidth), lcdHeight(config.lcdHeight), lastTouch_x(0), lastTouch_y(0), 	//
								   spi_master(GetSpiConfig(config)), spiTransferTxData(GetTransferData())					//
{

}
}

#endif
