/*
 * flash.cpp
 *
 *  Created on: Oct 11, 2018
 *      Author: LeeChunHei
 */

#include "device_driver/flash.h"
#include "../config/config.h"
#include <assert.h>

namespace DeviceDriver
{

Driver::FlexSpi::Config Flash::GetFlexspiConfig()
{
	Driver::FlexSpi::Config config;
	config.ahb_config.enable_AHB_prefetch = true;
	config.sck = FLASH_FLEXSPI_SCK;
	config.cs = FLASH_FLEXSPI_CS;
	config.sio[0] = FLASH_FLEXSPI_SIO0;
	config.sio[1] = FLASH_FLEXSPI_SIO1;
	config.sio[2] = FLASH_FLEXSPI_SIO2;
	config.sio[3] = FLASH_FLEXSPI_SIO3;
	config.sio[4] = FLASH_FLEXSPI_SIO4;
	config.sio[5] = FLASH_FLEXSPI_SIO5;
	config.sio[6] = FLASH_FLEXSPI_SIO6;
	config.sio[7] = FLASH_FLEXSPI_SIO7;
	config.dqs = FLASH_FLEXSPI_DQS;
	return config;
}

Flash::Flash() : flexspi(GetFlexspiConfig())
{
	Driver::FlexSpi::FlashDeviceConfig flash_config;
	flash_config.flash_size = FLASH_SIZE / 8192;
	flash_config.CS_interval_unit = Driver::FlexSpi::FlashDeviceConfig::CsIntervalCycleUnit::k1SckCycle;
	flash_config.CS_interval = 2;
	flash_config.CS_hold_time = 1;
	flash_config.CS_setup_time = 1;
	flash_config.data_valid_time = 2;
	flash_config.columnspace = 0;
	flash_config.enable_word_address = false;
	flash_config.AWR_seq_index = 13;
	flash_config.ARD_seq_number = 1;
	flash_config.AHB_write_wait_unit = Driver::FlexSpi::FlashDeviceConfig::AHBWriteWaitUnit::k32768AhbCycle;
	flash_config.AHB_write_wait_interval = 17;
	flexspi.SetFlashConfig(&flash_config);
	const uint32_t LUT[FLASH_LUT_SIZE] = FLASH_LUT_CONTENT;
	flexspi.UpdateLUT(0, LUT, FLASH_LUT_SIZE);
	assert(CheckVendorID() == FLASH_JEDECDEVICE_ID);
	//	EnableQuadMode();
}

uint32_t Flash::CheckVendorID()
{
	uint32_t data;
	Driver::FlexSpi::Transfer transfer;
	transfer.device_address = 0;
	transfer.cmd_type = Driver::FlexSpi::Transfer::CommandType::kRead;
	transfer.seq_number = 1;
	transfer.seq_index = FLASH_LUT_READJEDECID;
	transfer.data = &data;
	transfer.data_size = 3;

	flexspi.TransferBlocking(&transfer);
	return ((data & 0xFF) << 16) | (data & 0xFF00) | ((data & 0xFF0000) >> 16);
}

bool Flash::EnableQuadMode()
{
	Driver::FlexSpi::Transfer transfer;
	uint32_t writeValue = 0x40;
	bool status;

	/* Write enable */
	status = WriteEnable();

	if (!status)
	{
		return status;
	}

	/* Enable quad mode. */
	transfer.device_address = 0;
	transfer.cmd_type = Driver::FlexSpi::Transfer::CommandType::kWrite;
	transfer.seq_number = 1;
	transfer.seq_index = FLASH_LUT_WRITESTATUSREG;
	transfer.data = &writeValue;
	transfer.data_size = 1;

	status = flexspi.TransferBlocking(&transfer);
	if (!status)
	{
		return status;
	}

	return WaitBusBusy();
}

bool Flash::WriteEnable()
{
	Driver::FlexSpi::Transfer transfer;

	/* Write neable */
	transfer.device_address = 0;
	transfer.cmd_type = Driver::FlexSpi::Transfer::CommandType::kCommand;
	transfer.seq_number = 1;
	transfer.seq_index = 4;

	return flexspi.TransferBlocking(&transfer);
}

bool Flash::WaitBusBusy()
{
	/* Wait status ready. */
	uint32_t value;
	bool status;
	Driver::FlexSpi::Transfer transfer;

	transfer.device_address = 0;
	transfer.cmd_type = Driver::FlexSpi::Transfer::CommandType::kRead;
	transfer.seq_number = 1;
	transfer.seq_index = 11;
	transfer.data = &value;
	transfer.data_size = 1;

	do
	{
		status = flexspi.TransferBlocking(&transfer);
	} while ((value & 1));

	return status;
}

bool Flash::EraseSector(uint32_t sector_num)
{
	bool status;
	Driver::FlexSpi::Transfer transfer;

	status = WriteEnable();

	if (!status)
	{
		return status;
	}

	transfer.device_address = sector_num * FLASH_SECTOR_SIZE;
	transfer.cmd_type = Driver::FlexSpi::Transfer::CommandType::kCommand;
	transfer.seq_number = 1;
	transfer.seq_index = FLASH_LUT_ERASESECTOR;
	status = flexspi.TransferBlocking(&transfer);

	if (!status)
	{
		return status;
	}

	return WaitBusBusy();
}

bool Flash::PageProgram(uint32_t addr, uint8_t *data, uint8_t size)
{
	bool status;
	Driver::FlexSpi::Transfer transfer;

	/* дʹ�� */
	status = WriteEnable();

	if (!status)
	{
		return status;
	}

	/* ���ô���ṹ�� */
	transfer.device_address = addr;
	transfer.cmd_type = Driver::FlexSpi::Transfer::CommandType::kWrite;
	transfer.seq_number = 1;
	transfer.seq_index = FLASH_LUT_PAGEPROGRAM_QUAD;
	transfer.data = (uint32_t *)data;
	transfer.data_size = size + 1;

	status = flexspi.TransferBlocking(&transfer);

	if (!status)
	{
		return status;
	}

	/* �ȴ�д����� */
	return WaitBusBusy();
}

bool Flash::ReadBuffer(uint32_t addr, uint8_t *data, uint16_t size)
{
	Driver::FlexSpi::Transfer transfer;

	/* ���ô���ṹ�� */
	transfer.device_address = addr;
	transfer.cmd_type = Driver::FlexSpi::Transfer::CommandType::kRead;
	transfer.seq_number = 1;
	transfer.seq_index = FLASH_LUT_READ_FAST_QUAD;
	transfer.data = (uint32_t *)data;
	transfer.data_size = size + 1;

	return flexspi.TransferBlocking(&transfer);
}

} // namespace DeviceDriver
