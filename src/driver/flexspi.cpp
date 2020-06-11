/*
 * flexspi.cpp
 *
 *  Created on: Oct 6, 2018
 *      Author: LeeChunHei
 */

#include "driver/flexspi.h"
#include "system/clock.h"
#include <assert.h>

namespace Driver
{

using namespace System;
static const clock_ip_name_t s_flexspiClock[] = FLEXSPI_CLOCKS;
uint32_t flexspi_clock = 0;

FlexSpi::FlexSpi(const Config &config)
{
	/* Enable the flexspi clock */
	CLOCK_EnableClock(s_flexspiClock[0]);

	if (!flexspi_clock)
	{
		flexspi_clock = 198000000;
	}

	System::Pinout::Config pin_config;
	pin_config.pin = config.sck;
	assert(System::Pinout::GetFlexSpiSckPinConfig(pin_config));
	Pinout::InitPin(pin_config);
	for (uint8_t i = 0; i < 8; i++)
	{
		if (config.sio[i] == System::Pinout::Name::kDisable)
		{
			break;
		}
		else
		{
			pin_config.pin = config.sio[i];
			assert(System::Pinout::GetFlexSpiSioPinConfig(pin_config));
			Pinout::InitPin(pin_config);
		}
	}
	pin_config.pin = config.cs;
	uint8_t cs;
	assert(System::Pinout::GetFlexSpiCSPinConfig(pin_config, cs));
	Pinout::InitPin(pin_config);
	port = (Port)cs;
	if (config.dqs != System::Pinout::Name::kDisable)
	{
		pin_config.pin = config.dqs;
		assert(System::Pinout::GetFlexSpiDQSPinConfig(pin_config));
		Pinout::InitPin(pin_config);
	}

	flexspi_base = ((FLEXSPI_Type *)0x402A8000u);

	/* Reset peripheral before configuring it. */
	flexspi_base->MCR0 &= ~FLEXSPI_MCR0_MDIS_MASK;
	Reset();

	/* Configure MCR0 configuration items. */
	flexspi_base->MCR0 =
		FLEXSPI_MCR0_RXCLKSRC(
			config.rx_sample_clock) |
		FLEXSPI_MCR0_DOZEEN(config.enable_doze) | FLEXSPI_MCR0_IPGRANTWAIT(config.ip_grant_timeout_cycle) | FLEXSPI_MCR0_AHBGRANTWAIT(config.ahb_config.ahb_grant_timeout_cycle) | FLEXSPI_MCR0_SCKFREERUNEN(config.enable_sck_free_running) | FLEXSPI_MCR0_HSEN(config.enable_half_speed_access) | FLEXSPI_MCR0_COMBINATIONEN(config.enable_combination) | FLEXSPI_MCR0_ATDFEN(config.ahb_config.enable_AHB_write_ip_tx_fifo) | FLEXSPI_MCR0_ARDFEN(config.ahb_config.enable_AHB_write_ip_rx_fifo) | FLEXSPI_MCR0_MDIS_MASK;

	/* Configure MCR1 configurations. */
	flexspi_base->MCR1 = FLEXSPI_MCR1_SEQWAIT(config.seq_timeout_cycle) | FLEXSPI_MCR1_AHBBUSWAIT(config.ahb_config.ahb_bus_timeout_cycle);

	/* Configure MCR2 configurations. */
	uint32_t config_value = flexspi_base->MCR2;
	config_value &= ~(FLEXSPI_MCR2_RESUMEWAIT_MASK | FLEXSPI_MCR2_SCKBDIFFOPT_MASK | FLEXSPI_MCR2_SAMEDEVICEEN_MASK | FLEXSPI_MCR2_CLRAHBBUFOPT_MASK);
	config_value |=
		FLEXSPI_MCR2_RESUMEWAIT(
			config.ahb_config.resume_wait_cycle) |
		FLEXSPI_MCR2_SCKBDIFFOPT(config.enable_sckB_diff_opt) | FLEXSPI_MCR2_SAMEDEVICEEN(config.enable_same_config_for_all) | FLEXSPI_MCR2_CLRAHBBUFOPT(config.ahb_config.enable_clear_AHB_buffer_opt);

	flexspi_base->MCR2 = config_value;

	/* Configure AHB control items. */
	config_value = flexspi_base->AHBCR;
	config_value &=
		~(FLEXSPI_AHBCR_READADDROPT_MASK | FLEXSPI_AHBCR_PREFETCHEN_MASK | FLEXSPI_AHBCR_BUFFERABLEEN_MASK | FLEXSPI_AHBCR_CACHABLEEN_MASK);
	config_value |=
		FLEXSPI_AHBCR_READADDROPT(
			config.ahb_config.enable_read_address_opt) |
		FLEXSPI_AHBCR_PREFETCHEN(config.ahb_config.enable_AHB_prefetch) | FLEXSPI_AHBCR_BUFFERABLEEN(config.ahb_config.enable_AHB_bufferable) | FLEXSPI_AHBCR_CACHABLEEN(config.ahb_config.enable_AHB_cachable);
	flexspi_base->AHBCR = config_value;

	/* Configure AHB rx buffers. */
	for (uint8_t i = 0; i < 4; i++)
	{
		config_value = flexspi_base->AHBRXBUFCR0[i];

		config_value &= ~(FLEXSPI_AHBRXBUFCR0_PREFETCHEN_MASK | FLEXSPI_AHBRXBUFCR0_PRIORITY_MASK | FLEXSPI_AHBRXBUFCR0_MSTRID_MASK | FLEXSPI_AHBRXBUFCR0_BUFSZ_MASK);
		config_value |=
			FLEXSPI_AHBRXBUFCR0_PREFETCHEN(
				config.ahb_config.buffer[i].enable_prefetch) |
			FLEXSPI_AHBRXBUFCR0_PRIORITY(config.ahb_config.buffer[i].priority) | FLEXSPI_AHBRXBUFCR0_MSTRID(config.ahb_config.buffer[i].master_index) | FLEXSPI_AHBRXBUFCR0_BUFSZ(config.ahb_config.buffer[i].buffer_size * 8);
		flexspi_base->AHBRXBUFCR0[i] = config_value;
	}

	/* Configure IP Fifo watermarks. */
	flexspi_base->IPRXFCR &= ~FLEXSPI_IPRXFCR_RXWMRK_MASK;
	flexspi_base->IPRXFCR |= FLEXSPI_IPRXFCR_RXWMRK(config.rx_watermark / 8 - 1);
	flexspi_base->IPTXFCR &= ~FLEXSPI_IPTXFCR_TXWMRK_MASK;
	flexspi_base->IPTXFCR |= FLEXSPI_IPTXFCR_TXWMRK(config.tx_watermark / 8 - 1);
}

void FlexSpi::SetFlashConfig(FlashDeviceConfig *config)
{
	/* Wait for bus idle before change flash configuration. */
	while (!GetBusIdleStatus())
		;

	uint8_t index = (uint8_t)port >> 1; /* PortA with index 0, PortB with index 1. */

	/* Configure flash size. */
	flexspi_base->FLSHCR0[index] = 0;
	flexspi_base->FLSHCR0[(uint8_t)port] = config->flash_size;

	/* Configure flash parameters. */
	flexspi_base->FLSHCR1[(uint8_t)port] =
		FLEXSPI_FLSHCR1_CSINTERVAL(
			config->CS_interval) |
		FLEXSPI_FLSHCR1_CSINTERVALUNIT((uint8_t)config->CS_interval_unit) | FLEXSPI_FLSHCR1_TCSH(config->CS_hold_time) | FLEXSPI_FLSHCR1_TCSS(config->CS_setup_time) | FLEXSPI_FLSHCR1_CAS(config->columnspace) | FLEXSPI_FLSHCR1_WA(config->enable_word_address);

	/* Configure AHB operation items. */
	uint32_t config_value = flexspi_base->FLSHCR2[(uint8_t)port];

	config_value &= ~(FLEXSPI_FLSHCR2_AWRWAITUNIT_MASK | FLEXSPI_FLSHCR2_AWRWAIT_MASK | FLEXSPI_FLSHCR2_AWRSEQNUM_MASK | FLEXSPI_FLSHCR2_AWRSEQID_MASK | FLEXSPI_FLSHCR2_ARDSEQNUM_MASK | FLEXSPI_FLSHCR2_AWRSEQID_MASK);

	config_value |= FLEXSPI_FLSHCR2_AWRWAITUNIT(config->AHB_write_wait_unit) | FLEXSPI_FLSHCR2_AWRWAIT(config->AHB_write_wait_interval);

	if (config->AWR_seq_number > 0U)
	{
		config_value |= FLEXSPI_FLSHCR2_AWRSEQID(config->AWR_seq_index) | FLEXSPI_FLSHCR2_AWRSEQNUM(config->AWR_seq_number - 1U);
	}

	if (config->ARD_seq_number > 0U)
	{
		config_value |= FLEXSPI_FLSHCR2_ARDSEQID(config->ARD_seq_index) | FLEXSPI_FLSHCR2_ARDSEQNUM(config->ARD_seq_number - 1U);
	}

	flexspi_base->FLSHCR2[(uint8_t)port] = config_value;

	/* Configure DLL. */
	flexspi_base->DLLCR[index] = ConfigureDll(config);

	/* Configure write mask. */
	if (config->enable_write_mask)
	{
		flexspi_base->FLSHCR4 &= ~FLEXSPI_FLSHCR4_WMOPT1_MASK;
	}
	else
	{
		flexspi_base->FLSHCR4 |= FLEXSPI_FLSHCR4_WMOPT1_MASK;
	}

	if (index == 0) /*PortA*/
	{
		flexspi_base->FLSHCR4 &= ~FLEXSPI_FLSHCR4_WMENA_MASK;
		flexspi_base->FLSHCR4 |= FLEXSPI_FLSHCR4_WMENA(config->enable_write_mask);
	}
	else
	{
		flexspi_base->FLSHCR4 &= ~FLEXSPI_FLSHCR4_WMENB_MASK;
		flexspi_base->FLSHCR4 |= FLEXSPI_FLSHCR4_WMENB(config->enable_write_mask);
	}

	/* Exit stop mode. */
	flexspi_base->MCR0 &= ~FLEXSPI_MCR0_MDIS_MASK;
}

void FlexSpi::UpdateLUT(uint32_t index, const uint32_t *cmd, uint32_t count)
{
	assert(index < 64);

	/* Wait for bus idle before change flash configuration. */
	while (!GetBusIdleStatus())
		;

	/* Unlock LUT for update. */
	flexspi_base->LUTKEY = 0x5AF05AF0ul;
	flexspi_base->LUTCR = 0x02;

	volatile uint32_t *lutBase = &flexspi_base->LUT[index];
	for (uint8_t i = index; i < count; i++)
	{
		*lutBase++ = *cmd++;
	}

	/* Lock LUT. */
	flexspi_base->LUTKEY = 0x5AF05AF0ul;
	flexspi_base->LUTCR = 0x01;
}

bool FlexSpi::CheckAndClearError(uint32_t status)
{
	bool result = true;

	/* Check for error. */
	status &= FLEXSPI_INTEN_SEQTIMEOUTEN_MASK | FLEXSPI_INTEN_IPCMDERREN_MASK | FLEXSPI_INTEN_IPCMDGEEN_MASK;
	if (status)
	{
		/* Select the correct error code.. */
		if (status & FLEXSPI_INTEN_SEQTIMEOUTEN_MASK)
		{
			result = false;
		}
		else if (status & FLEXSPI_INTEN_IPCMDERREN_MASK)
		{
			result = false;
		}
		else if (status & FLEXSPI_INTEN_IPCMDGEEN_MASK)
		{
			result = false;
		}
		else
		{
			assert(false);
		}

		/* Clear the flags. */
		flexspi_base->INTR |= status;

		/* Reset fifos. These flags clear automatically. */
		flexspi_base->IPTXFCR |= FLEXSPI_IPTXFCR_CLRIPTXF_MASK;
		flexspi_base->IPRXFCR |= FLEXSPI_IPRXFCR_CLRIPRXF_MASK;
	}

	return result;
}

bool FlexSpi::WriteBlocking(uint32_t *buffer, uint32_t size)
{
	uint8_t tx_watermark = ((flexspi_base->IPTXFCR & FLEXSPI_IPTXFCR_TXWMRK_MASK) >> FLEXSPI_IPTXFCR_TXWMRK_SHIFT) + 1;
	uint32_t status;
	bool result = true;

	/* Send data buffer */
	while (size)
	{
		/* Wait until there is room in the fifo. This also checks for errors. */
		while (!((status = flexspi_base->INTR) & FLEXSPI_INTEN_IPTXWEEN_MASK))
			;

		result = CheckAndClearError(status);

		if (!result)
		{
			return result;
		}

		/* Write watermark level data into tx fifo . */
		if (size >= 8 * tx_watermark)
		{
			for (uint32_t i = 0; i < 2 * tx_watermark; i++)
			{
				flexspi_base->TFDR[i] = *buffer++;
			}

			size = size - 8 * tx_watermark;
		}
		else
		{
			for (uint32_t i = 0; i < (size / 4 + 1); i++)
			{
				flexspi_base->TFDR[i] = *buffer++;
			}
			size = 0;
		}

		/* Push a watermark level datas into IP TX FIFO. */
		flexspi_base->INTR |= FLEXSPI_INTEN_IPTXWEEN_MASK;
	}

	return result;
}

bool FlexSpi::ReadBlocking(uint32_t *buffer, uint32_t size)
{
	uint8_t rx_watermark = ((flexspi_base->IPRXFCR & FLEXSPI_IPRXFCR_RXWMRK_MASK) >> FLEXSPI_IPRXFCR_RXWMRK_SHIFT) + 1;
	bool result = true;

	/* Send data buffer */
	while (size)
	{
		if (size >= 8 * rx_watermark)
		{
			uint32_t status;
			/* Wait until there is room in the fifo. This also checks for errors. */
			while (!((status = flexspi_base->INTR) & FLEXSPI_INTEN_IPRXWAEN_MASK))
			{
				result = CheckAndClearError(status);

				if (!result)
				{
					return result;
				}
			}
		}
		else
		{
			/* Wait fill level. This also checks for errors. */
			while (size > ((((flexspi_base->IPRXFSTS) & FLEXSPI_IPRXFSTS_FILL_MASK) >> FLEXSPI_IPRXFSTS_FILL_SHIFT) * 8U))
			{
				result = CheckAndClearError(flexspi_base->INTR);

				if (!result)
				{
					return result;
				}
			}
		}

		result = CheckAndClearError(flexspi_base->INTR);

		if (!result)
		{
			return result;
		}

		/* Read watermark level data from rx fifo . */
		if (size >= 8 * rx_watermark)
		{
			for (uint32_t i = 0; i < 2 * rx_watermark; i++)
			{
				*buffer++ = flexspi_base->RFDR[i];
			}

			size = size - 8 * rx_watermark;
		}
		else
		{
			for (uint32_t i = 0; i < (size / 4 + 1); i++)
			{
				*buffer++ = flexspi_base->RFDR[i];
			}
			size = 0;
		}

		/* Pop out a watermark level datas from IP RX FIFO. */
		flexspi_base->INTR |= FLEXSPI_INTEN_IPRXWAEN_MASK;
	}

	return result;
}

bool FlexSpi::TransferBlocking(Transfer *xfer)
{
	bool result = true;

	/* Clear sequence pointer before sending data to external devices. */
	flexspi_base->FLSHCR2[(uint8_t)port] |= FLEXSPI_FLSHCR2_CLRINSTRPTR_MASK;

	/* Clear former pending status before start this tranfer. */
	flexspi_base->INTR |= FLEXSPI_INTR_AHBCMDERR_MASK | FLEXSPI_INTR_IPCMDERR_MASK | FLEXSPI_INTR_AHBCMDGE_MASK | FLEXSPI_INTR_IPCMDGE_MASK;

	/* Configure flexspi_base addresss. */
	flexspi_base->IPCR0 = xfer->device_address;

	/* Reset fifos. */
	flexspi_base->IPTXFCR |= FLEXSPI_IPTXFCR_CLRIPTXF_MASK;
	flexspi_base->IPRXFCR |= FLEXSPI_IPRXFCR_CLRIPRXF_MASK;

	uint32_t config_value = 0;

	/* Configure data size. */
	if ((xfer->cmd_type == Transfer::CommandType::kRead) || (xfer->cmd_type == Transfer::CommandType::kWrite) || (xfer->cmd_type == Transfer::CommandType::kConfig))
	{
		config_value = FLEXSPI_IPCR1_IDATSZ(xfer->data_size);
	}

	/* Configure sequence ID. */
	config_value |= FLEXSPI_IPCR1_ISEQID(xfer->seq_index) | FLEXSPI_IPCR1_ISEQNUM(xfer->seq_number - 1);
	flexspi_base->IPCR1 = config_value;

	/* Start Transfer. */
	flexspi_base->IPCMD |= FLEXSPI_IPCMD_TRG_MASK;

	if ((xfer->cmd_type == Transfer::CommandType::kWrite) || (xfer->cmd_type == Transfer::CommandType::kConfig))
	{
		result = WriteBlocking(xfer->data, xfer->data_size);
	}
	else if (xfer->cmd_type == Transfer::CommandType::kRead)
	{
		result = ReadBlocking(xfer->data, xfer->data_size);
	}
	else
	{
	}

	/* Wait for bus idle. */
	while (!GetBusIdleStatus())
		;

	if (xfer->cmd_type == Transfer::CommandType::kCommand)
	{
		result = CheckAndClearError(flexspi_base->INTR);
	}

	return result;
}

uint32_t FlexSpi::ConfigureDll(FlashDeviceConfig *config)
{
	bool is_unified_config = true;
	uint32_t flexspi_dll_value;
	uint32_t dll_value;

	switch (((flexspi_base->MCR0 & FLEXSPI_MCR0_RXCLKSRC_MASK) >> FLEXSPI_MCR0_RXCLKSRC_SHIFT))
	{
	case (uint8_t)Config::ReadSampleClock::kLoopbackInternally:
	case (uint8_t)Config::ReadSampleClock::kLoopbackFromDqsPad:
	case (uint8_t)Config::ReadSampleClock::kLoopbackFromSckPad:
		is_unified_config = true;
		break;
	case (uint8_t)Config::ReadSampleClock::kExternalInputFromDqsPad:
		if (config->is_sck2_enabled)
		{
			is_unified_config = true;
		}
		else
		{
			is_unified_config = false;
		}
		break;
	default:
		break;
	}

	if (is_unified_config)
	{
		flexspi_dll_value = 0x100UL; /* 1 fixed delay cells in DLL delay chain) */
	}
	else
	{
		if (flexspi_clock >= 100 * 1000000)
		{
			/* DLLEN = 1, SLVDLYTARGET = 0xF, */
			flexspi_dll_value = FLEXSPI_DLLCR_DLLEN(1) | FLEXSPI_DLLCR_SLVDLYTARGET(0x0F);
		}
		else
		{
			uint32_t temp = config->data_valid_time * 1000; /* Convert data valid time in ns to ps. */
			dll_value = temp / 75;
			if (dll_value * 75 < temp)
			{
				dll_value++;
			}
			flexspi_dll_value = FLEXSPI_DLLCR_OVRDEN(1) | FLEXSPI_DLLCR_OVRDVAL(dll_value);
		}
	}
	return flexspi_dll_value;
}

} // namespace Driver
