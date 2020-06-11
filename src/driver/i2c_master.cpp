///*
// * i2c_master.cpp
// *
// *  Created on: Sep 4, 2018
// *      Author: LeeChunHei
// */
//
//#include "driver/i2c_master.h"
//#include "system/clock.h"
//#include "system/systick.h"
//#include "driver/gpio.h"
//#include <assert.h>
//
//using namespace System;
//
//clock_ip_name_t const i2c_ip_clock[] = LPI2C_CLOCKS;
//
//namespace Driver {
//
//uint32_t i2c_clock = 0;
//
//void SetPullDownPins(System::Pinout::Name scl, System::Pinout::Name sda) {
//	GPIO::Config config;
//	config.default_high = false;
//	config.gpio_dir = GPIO::Direction::kDigitalOutput;
//	config.pin_config.pull_keep_config = System::Pinout::Config::PinConfig::PullKeepConfig::kPull;
//	config.pin_config.pull_config = System::Pinout::Config::PinConfig::PullConfig::k100kPullDown;
//	config.pin = scl;
//	GPIO scl_pin(config);
//	config.pin = sda;
//	GPIO sda_pin(config);
//}
//
//void SetPullUpPins(System::Pinout::Name scl, System::Pinout::Name sda) {
//	GPIO::Config config;
//	config.default_high = true;
//	config.gpio_dir = GPIO::Direction::kDigitalOutput;
//	config.pin_config.drive_strength = System::Pinout::Config::PinConfig::DriveStrength::kDSE6;
//	config.pin_config.pull_keep_config = System::Pinout::Config::PinConfig::PullKeepConfig::kPull;
//	config.pin_config.pull_config = System::Pinout::Config::PinConfig::PullConfig::k22kPullUp;
//	config.pin = scl;
//	GPIO scl_pin(config);
//	config.pin = sda;
//	GPIO sda_pin(config);
//	scl_pin.Set(true);
//	sda_pin.Set(false);
//	System::Systick::DelayMS(1);
//	sda_pin.Set(true);
//	System::Systick::DelayMS(1);
//}
//
//I2CMaster::I2CMaster(const Config& config) :
//		send_wait_time(config.send_wait_time), recieve_wait_time(config.recieve_wait_time), scl(config.scl), sda(config.sda) {
//	uint8_t scl_module, sda_module;
//	System::Pinout::Config scl_config, sda_config;
//	scl_config.pin = scl;
//	sda_config.pin = sda;
//	if (!System::Pinout::GetI2CSclPinConfig(scl_config, scl_module) || !System::Pinout::GetI2CSdaPinConfig(sda_config, sda_module) || (scl_module != sda_module)) {
//		return;
//	}
////	SetPullDownPins(scl, sda);
////	SetPullUpPins(scl, sda);
////	SetPullDownPins(scl, sda);
////	SetPullUpPins(scl, sda);
////	System::Systick::DelayMS(1000);
//	System::Pinout::InitPin(scl_config);
//	System::Pinout::InitPin(sda_config);
//
//	i2c_base = (LPI2C_Type*) (0x403F0000u + scl_module*0x4000u);
//
//	/* Ungate the clock. */
//	CLOCK_EnableClock(i2c_ip_clock[(uint8_t) scl_module + 1]);
//
//	/* Reset peripheral before configuring it. */
//	Reset();
//
//	/* Doze bit: 0 is enable, 1 is disable */
//	i2c_base->MCR = LPI2C_MCR_DBGEN(config.debug_enable) | LPI2C_MCR_DOZEN(!(config.enable_doze));
//
//	/* host request */
//	uint32_t reg = i2c_base->MCFGR0;
//	reg &= (~(LPI2C_MCFGR0_HREN_MASK | LPI2C_MCFGR0_HRPOL_MASK | LPI2C_MCFGR0_HRSEL_MASK));
//	reg |= LPI2C_MCFGR0_HREN(config.host_request.enable) | LPI2C_MCFGR0_HRPOL(config.host_request.is_active_high) | LPI2C_MCFGR0_HRSEL(config.host_request.source);
//	i2c_base->MCFGR0 = reg;
//
//	/* pin config and ignore ack */
//	reg = i2c_base->MCFGR1;
//	reg &= ~(LPI2C_MCFGR1_PINCFG_MASK | LPI2C_MCFGR1_IGNACK_MASK);
//	reg |= LPI2C_MCFGR1_PINCFG(config.pin_config);
//	reg |= LPI2C_MCFGR1_IGNACK(config.ignore_ack);
//	i2c_base->MCFGR1 = reg;
//
//	SetWatermarks(0, 0);
//
//	if (!i2c_clock) {
//		i2c_clock = (CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8) / (CLOCK_GetDiv(kCLOCK_Lpi2cDiv) + 1u);
//	}
//
//	SetBaudRate(i2c_clock, config.baud_rate_Hz);
//
//	/* Configure glitch filters and bus idle and pin low timeouts. */
//	uint32_t prescaler = (i2c_base->MCFGR1 & LPI2C_MCFGR1_PRESCALE_MASK) >> LPI2C_MCFGR1_PRESCALE_SHIFT;
//	reg = i2c_base->MCFGR2;
//	if (config.bus_idle_timeout_ns) {
//		uint32_t cycles = GetCyclesForWidth(i2c_clock, config.bus_idle_timeout_ns, (LPI2C_MCFGR2_BUSIDLE_MASK >> LPI2C_MCFGR2_BUSIDLE_SHIFT), prescaler);
//		reg &= ~LPI2C_MCFGR2_BUSIDLE_MASK;
//		reg |= LPI2C_MCFGR2_BUSIDLE(cycles);
//	}
//	if (config.sda_glitch_filter_width_ns) {
//		uint32_t cycles = GetCyclesForWidth(i2c_clock, config.sda_glitch_filter_width_ns, (LPI2C_MCFGR2_FILTSDA_MASK >> LPI2C_MCFGR2_FILTSDA_SHIFT), 1);
//		reg &= ~LPI2C_MCFGR2_FILTSDA_MASK;
//		reg |= LPI2C_MCFGR2_FILTSDA(cycles);
//	}
//	if (config.scl_glitch_filter_width_ns) {
//		uint32_t cycles = GetCyclesForWidth(i2c_clock, config.scl_glitch_filter_width_ns, (LPI2C_MCFGR2_FILTSCL_MASK >> LPI2C_MCFGR2_FILTSCL_SHIFT), 1);
//		reg &= ~LPI2C_MCFGR2_FILTSCL_MASK;
//		reg |= LPI2C_MCFGR2_FILTSCL(cycles);
//	}
//	i2c_base->MCFGR2 = reg;
//	if (config.pin_low_timeout_ns) {
//		uint32_t cycles = GetCyclesForWidth(i2c_clock, config.pin_low_timeout_ns / 256, (LPI2C_MCFGR2_BUSIDLE_MASK >> LPI2C_MCFGR2_BUSIDLE_SHIFT), prescaler);
//		i2c_base->MCFGR3 = (i2c_base->MCFGR3 & ~LPI2C_MCFGR3_PINLOW_MASK) | LPI2C_MCFGR3_PINLOW(cycles);
//	}
//
//	Enable(config.enable_master);
//}
//
//void I2CMaster::SetBaudRate(uint32_t source_clock_Hz, uint32_t baud_rate_Hz) {
//	/* Disable master mode. */
//	bool was_enabled = (i2c_base->MCR & LPI2C_MCR_MEN_MASK) >> LPI2C_MCR_MEN_SHIFT;
//	Enable(false);
//
//	uint32_t best_pre = 0;
//	uint32_t best_clk_hi = 0;
//	uint32_t best_error = 0xffffffffu;
//
//	/* Baud rate = (sourceClock_Hz/2^prescale)/(CLKLO+1+CLKHI+1 + ROUNDDOWN((2+FILTSCL)/2^prescale) */
//	/* Assume CLKLO = 2*CLKHI, SETHOLD = CLKHI, DATAVD = CLKHI/2. */
//	for (uint32_t prescale = 1; (prescale <= 128) && (best_error != 0); prescale = 2 * prescale) {
//		for (uint32_t clk_hi_cycle = 1; clk_hi_cycle < 32; clk_hi_cycle++) {
//			uint32_t computed_rate;
//			if (clk_hi_cycle == 1) {
//				computed_rate = (source_clock_Hz / prescale) / (1 + 3 + 2 + 2 / prescale);
//			} else {
//				computed_rate = (source_clock_Hz / prescale) / (3 * clk_hi_cycle + 2 + 2 / prescale);
//			}
//
//			uint32_t abs_error = baud_rate_Hz > computed_rate ? baud_rate_Hz - computed_rate : computed_rate - baud_rate_Hz;
//
//			if (abs_error < best_error) {
//				best_pre = prescale;
//				best_clk_hi = clk_hi_cycle;
//				best_error = abs_error;
//
//				/* If the error is 0, then we can stop searching because we won't find a better match. */
//				if (abs_error == 0) {
//					break;
//				}
//			}
//		}
//	}
//
//	/* Standard, fast, fast mode plus and ultra-fast transfers. */
//	uint32_t reg = LPI2C_MCCR0_CLKHI(best_clk_hi);
//
//	if (best_clk_hi < 2) {
//		reg |= LPI2C_MCCR0_CLKLO(3) | LPI2C_MCCR0_SETHOLD(2) | LPI2C_MCCR0_DATAVD(1);
//	} else {
//		reg |= LPI2C_MCCR0_CLKLO(2 * best_clk_hi) | LPI2C_MCCR0_SETHOLD(best_clk_hi) | LPI2C_MCCR0_DATAVD(best_clk_hi / 2);
//	}
//
//	i2c_base->MCCR0 = reg;
//
//	for (uint8_t i = 0; i < 8; i++) {
//		if (best_pre == (1U << i)) {
//			best_pre = i;
//			break;
//		}
//	}
//	i2c_base->MCFGR1 = (i2c_base->MCFGR1 & ~LPI2C_MCFGR1_PRESCALE_MASK) | LPI2C_MCFGR1_PRESCALE(best_pre);
//
//	/* Restore master mode. */
//	if (was_enabled) {
//		Enable(true);
//	}
//}
//
//uint32_t I2CMaster::GetCyclesForWidth(uint32_t source_clock_Hz, uint32_t width_ns, uint32_t max_cycles, uint32_t prescaler) {
//	assert(prescaler > 0);
//
//	uint32_t bus_cycle_ns = 1000000 / (source_clock_Hz / prescaler / 1000);
//	uint32_t cycles = 0;
//
//	/* Search for the cycle count just below the desired glitch width. */
//	while ((((cycles + 1) * bus_cycle_ns) < width_ns) && (cycles + 1 < max_cycles)) {
//		++cycles;
//	}
//
//	/* If we end up with zero cycles, then set the filter to a single cycle unless the */
//	/* bus clock is greater than 10x the desired glitch width. */
//	if ((cycles == 0) && (bus_cycle_ns <= (width_ns * 10))) {
//		cycles = 1;
//	}
//
//	return cycles;
//}
//
//bool I2CMaster::CheckForBusyBus() {
//	uint32_t reg = i2c_base->MSR;
//	return !((reg & LPI2C_MSR_BBF_MASK) && (!(reg & LPI2C_MSR_MBF_MASK)));
//}
//
//bool I2CMaster::CheckAndClearError() {
//	/* Check for error. These errors cause a stop to automatically be sent. We must */
//	/* clear the errors before a new transfer can start. */
//	if (i2c_base->MSR & 0b0011110000000000)	//Filter master error flag out and see if there is any error flag
//			{
//		/* Clear the flags. */
//		i2c_base->MSR = i2c_base->MSR & 0b0011110000000000;
//
//		/* Reset fifos. These flags clear automatically. */
//		i2c_base->MCR |= LPI2C_MCR_RRF_MASK | LPI2C_MCR_RTF_MASK;
//		return false;
//	}
//
//	return true;
//}
//
//bool I2CMaster::WaitForTxReady() {
//	uint32_t start_wait_time = Systick::GetTimeInNS();
//	while (1) {
//		/* Check for error flags. */
//		if (!CheckAndClearError()) {
//			return false;
//		}
//		/* Get the number of words in the tx fifo and compute empty slots. */
//		uint8_t tx_count = (i2c_base->MFSR & LPI2C_MFSR_TXCOUNT_MASK) >> LPI2C_MFSR_TXCOUNT_SHIFT;
//		tx_count = 4 - tx_count;
//		if (tx_count)
//			return true;
//		if ((Systick::GetTimeInNS() - start_wait_time) < send_wait_time) {
//			return false;
//		}
//	}
//}
//
//bool I2CMaster::SendByteWithStartCMD(const uint8_t byte) {
//	/* Wait until there is room in the fifo. */
//	if (!WaitForTxReady()) {
//		return false;
//	}
//	/* Issue start command. */
//	i2c_base->MTDR = (uint32_t)(LPI2C_MTDR_CMD(0x4U) | (uint32_t)byte);
//	return true;
//}
//
//bool I2CMaster::SendStopCMD() {
//	/* Wait until all data in fifo had been sent. */
//	if (!WaitForTxReady()) {
//		return false;
//	}
//	i2c_base->MTDR = LPI2C_MTDR_CMD(0x2U);
//	uint32_t start_wait_time = Systick::GetTimeInNS();
//	while (1) {
//		if (!CheckAndClearError()) {
//			return false;
//		}
//		if (i2c_base->MSR & 0b1000000000) {
//			i2c_base->MSR = 0b1000000000;
//			return true;
//		}
//		if (Systick::GetTimeInNS() - start_wait_time > send_wait_time) {
//			return false;
//		}
//	}
//}
//
//bool I2CMaster::SendByte(const uint8_t byte) {
//	/* Wait until there is room in the fifo. */
//	if (!WaitForTxReady()) {
//		return false;
//	}
//	i2c_base->MTDR = byte;
//	return true;
//}
//
//bool I2CMaster::GetByte(uint8_t& byte) {
//	uint32_t start_wait_time = Systick::GetTimeInNS();
//	while (1) {
//		uint32_t value=i2c_base->MRDR;
//		if (!(value & LPI2C_MRDR_RXEMPTY_MASK)) {
//			byte = value & LPI2C_MRDR_DATA_MASK;
//			return true;
//		}
//		if (!CheckAndClearError()) {
//			return false;
//		}
//		if (Systick::GetTimeInNS() - start_wait_time > recieve_wait_time) {
//			return false;
//		}
//	}
//}
//
//bool I2CMaster::SendByte(const uint8_t slave_addr, const uint8_t reg_addr, const uint8_t data) {
//	if (!CheckForBusyBus()) {
//		return false;
//	}
//	/* Clear all flags. */
//	i2c_base->MSR = 0b0111111100000000;
//	/* Turn off auto-stop option. */
//	i2c_base->MCFGR1 &= ~LPI2C_MCFGR1_AUTOSTOP_MASK;
//	return SendByteWithStartCMD((uint32_t)(slave_addr << 1u)) && SendByte(reg_addr) && SendByte(data) && SendStopCMD();
//}
//
//bool I2CMaster::SendByte(const uint8_t slave_addr, const uint16_t reg_addr, const uint8_t data) {
//	if (!CheckForBusyBus()) {
//		return false;
//	}
//	/* Clear all flags. */
//	i2c_base->MSR = 0b0111111100000000;
//	/* Turn off auto-stop option. */
//	i2c_base->MCFGR1 &= ~LPI2C_MCFGR1_AUTOSTOP_MASK;
//	return SendByteWithStartCMD((uint32_t)(slave_addr << 1u)) && SendByte(reg_addr >> 8u) && SendByte(reg_addr && 0xFF) && SendByte(data) && SendStopCMD();
//}
//
//bool I2CMaster::GetByte(const uint8_t slave_addr, const uint8_t reg_addr, uint8_t& data) {
//	if (!CheckForBusyBus()) {
//		return false;
//	}
//	/* Clear all flags. */
//	i2c_base->MSR = 0b0111111100000000;
//	/* Turn off auto-stop option. */
//	i2c_base->MCFGR1 &= ~LPI2C_MCFGR1_AUTOSTOP_MASK;
//	if (!SendByteWithStartCMD((uint8_t)(slave_addr << 1u)) || !SendByte(reg_addr)) {
//		return false;
//	}
//	/* Wait until there is room in the command fifo. */
//	if (!WaitForTxReady()) {
//		return false;
//	}
//	if (!SendByteWithStartCMD((uint8_t)(slave_addr << 1u) | 1u)) {
//		return false;
//	}
//	/* Wait until there is room in the command fifo. */
//	if (!WaitForTxReady()) {
//		return false;
//	}
//	/* Issue command to receive data. */
//	i2c_base->MTDR = 0b100000000 | LPI2C_MTDR_DATA(0);
//	return GetByte(data) && SendStopCMD();
//}
//
//bool I2CMaster::GetByte(const uint8_t slave_addr, const uint16_t reg_addr, uint8_t& data) {
//	if (!CheckForBusyBus()) {
//		return false;
//	}
//	/* Clear all flags. */
//	i2c_base->MSR = 0b0111111100000000;
//	/* Turn off auto-stop option. */
//	i2c_base->MCFGR1 &= ~LPI2C_MCFGR1_AUTOSTOP_MASK;
//	if (!SendByteWithStartCMD((uint8_t)(slave_addr << 1u)) || !SendByte(reg_addr >> 8u) || !SendByte(reg_addr & 0xFF)) {
//		return false;
//	}
//	/* Wait until there is room in the command fifo. */
//	if (!WaitForTxReady()) {
//		return false;
//	}
//	if (!SendByteWithStartCMD((uint8_t)(slave_addr << 1u) | 1u)) {
//		return false;
//	}
//	/* Wait until there is room in the command fifo. */
//	if (!WaitForTxReady()) {
//		return false;
//	}
//	/* Issue command to receive data. */
//	i2c_base->MTDR = 0b100000000 | LPI2C_MTDR_DATA(0);
//	return GetByte(data) && SendStopCMD();
//}
//
//}
//
