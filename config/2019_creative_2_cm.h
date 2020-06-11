/*
 * 2019_creative_2_cm.h
 *
 *  Created on: May 29, 2019
 *      Author: LeeChunHei, Joseph Yim
 */

#ifndef CONFIG_2019_CREATIVE_2_CM_H_
#define CONFIG_2019_CREATIVE_2_CM_H_

#define __TARGET_FPU_VFP

#define CPU_XTAL_CLK_HZ 24000000UL // value of external crystal

#define SWITCH_USED 2
// #define FIVE_WAY_SWITCH_USED 0
#define ST7735R_USED 1
#define XPT2046_USED 1
#define MT9V034_USED 1
#define MPU6050_USED 0
#define LED_USED 2
// #define RGB_USED 0
// #define ALTERNATE_MOTOR_USED 4
// #define ENCODER_USED 4
// #define DIR_ENCODER_USED 4
#define USE_LVGL 1
// #define SERVO_USED 4
#define BLUETOOTH_USED 1

#define MOTOR_FREQ 100000 // 10kHz

/*
 * Pin order: switch0, switch1, switch1...
 */
#define SWITCH_PIN	\
	{                \
	}

/*
 * Pin order: (id==0)(five_way_switch_select, five_way_switch_up, five_way_switch_down, five_way_switch_left, five_way_switch_right)
 * 				(id==1)(five_way_switch_select, five_way_switch_up, five_way_switch_down, five_way_switch_left, five_way_switch_right)
 */
#define FIVE_WAY_SWITCH_PINS \
	{                          \
	}

/*
 * Pin order: (id==0)(ST7735R_SCL, ST7735R_SDA, ST7735R_CS, ST7735R_DC, ST7735R_RESET, ST7735R_BL),
 * 				(id==1)(ST7735R_SCL, ST7735R_SDA, ST7735R_CS, ST7735R_DC, ST7735R_RESET, ST7735R_BL)...
 * If no BL pin assign it as System::Pinout::Name::kDisable
 */
#define ST7735R_SOFT_SPI
#define ST7735R_PINS                                   \
	{                                                    \
				System::Pinout::Name::kGPIO_SD_B0_00,			/*	SCL	*/  \
				System::Pinout::Name::kGPIO_SD_B0_02,	/*	SDA	*/  \
				System::Pinout::Name::kGPIO_SD_B0_01,	/*	CS	*/   \
				System::Pinout::Name::kGPIO_SD_B0_04, /*	DC	*/   \
				System::Pinout::Name::kGPIO_SD_B0_05, /* RESET*/ \
				System::Pinout::Name::kDisable		/*	BL	*/   \
	}

#define XPT2046_PINS                                      \
	{                                                       \
				System::Pinout::Name::kGPIO_SD_B0_00,		 /*	SCL		*/     \
				System::Pinout::Name::kGPIO_SD_B0_02, /*	SDA		*/     \
				System::Pinout::Name::kGPIO_SD_B0_03, /*	SDI		*/     \
				System::Pinout::Name::kGPIO_EMC_41, /* touch CE	*/ \
				System::Pinout::Name::kDisable		 /* touch	*/    \
	}

#define MT9V034_DATABUS 8
/*
 * Pin order: (id==0)(MT9V034_SDA, MT9V034_SCL, MT9V034_VSYNC, MT9V034_HSYNC, MT9V034_PCLK, MT9V034_D0 - MT9V034_D9)
 * 				(id==1)(MT9V034_SDA, MT9V034_SCL, MT9V034_VSYNC, MT9V034_HSYNC, MT9V034_PCLK, MT9V034_D0 - MT9V034_D9)
 * Not in use pin assign System::Pinout::Name::kDisable
 */
#define MT9V034_PINS                                     \
	{                                                      \
		System::Pinout::Name::kGPIO_AD_B0_13,			/*	SDA	*/  \
				System::Pinout::Name::kGPIO_AD_B0_12, /*	SCL	*/  \
				System::Pinout::Name::kGPIO_AD_B1_06, /*VSYNC	*/ \
				System::Pinout::Name::kGPIO_AD_B1_07, /*HSYNC	*/ \
				System::Pinout::Name::kGPIO_AD_B1_04, /* PCLK	*/ \
				System::Pinout::Name::kDisable,				/*	D0	*/   \
				System::Pinout::Name::kDisable,				/*	D1	*/   \
				System::Pinout::Name::kGPIO_AD_B1_15, /*	D2	*/   \
				System::Pinout::Name::kGPIO_AD_B1_14, /*	D3	*/   \
				System::Pinout::Name::kGPIO_AD_B1_13, /*	D4	*/   \
				System::Pinout::Name::kGPIO_AD_B1_12, /*	D5	*/   \
				System::Pinout::Name::kGPIO_AD_B1_11, /*	D6	*/   \
				System::Pinout::Name::kGPIO_AD_B1_10, /*	D7	*/   \
				System::Pinout::Name::kGPIO_AD_B1_09, /*	D8	*/   \
				System::Pinout::Name::kGPIO_AD_B1_08, /*	D9	*/   \
	}

#define MT9V034_SDA(id) id == 0 ? System::Pinout::Name::kGPIO_AD_B0_13 : System::Pinout::Name::kDisable
#define MT9V034_SCK(id) id == 0 ? System::Pinout::Name::kGPIO_AD_B0_12 : System::Pinout::Name::kDisable
#define MT9V034_VSYNC(id) id == 0 ? System::Pinout::Name::kGPIO_AD_B1_06 : System::Pinout::Name::kDisable
#define MT9V034_HSYNC(id) id == 0 ? System::Pinout::Name::kGPIO_AD_B1_07 : System::Pinout::Name::kDisable
#define MT9V034_PCLK(id) id == 0 ? System::Pinout::Name::kGPIO_AD_B1_04 : System::Pinout::Name::kDisable
#define MT9V034_D2(id) id == 0 ? System::Pinout::Name::kGPIO_AD_B1_15 : System::Pinout::Name::kDisable
#define MT9V034_D3(id) id == 0 ? System::Pinout::Name::kGPIO_AD_B1_14 : System::Pinout::Name::kDisable
#define MT9V034_D4(id) id == 0 ? System::Pinout::Name::kGPIO_AD_B1_13 : System::Pinout::Name::kDisable
#define MT9V034_D5(id) id == 0 ? System::Pinout::Name::kGPIO_AD_B1_12 : System::Pinout::Name::kDisable
#define MT9V034_D6(id) id == 0 ? System::Pinout::Name::kGPIO_AD_B1_11 : System::Pinout::Name::kDisable
#define MT9V034_D7(id) id == 0 ? System::Pinout::Name::kGPIO_AD_B1_10 : System::Pinout::Name::kDisable
#define MT9V034_D8(id) id == 0 ? System::Pinout::Name::kGPIO_AD_B1_09 : System::Pinout::Name::kDisable
#define MT9V034_D9(id) id == 0 ? System::Pinout::Name::kGPIO_AD_B1_08 : System::Pinout::Name::kDisable

#define MPU6050_SDA(id) id == 0 ? System::Pinout::Name::kGPIO_AD_B1_01 : System::Pinout::Name::kDisable
#define MPU6050_SCK(id) id == 0 ? System::Pinout::Name::kGPIO_AD_B1_00 : System::Pinout::Name::kDisable

/*
 * Pin order: led0 pin, led1 pin, led2 pin...
 */
#define LED_PIN                              \
	{                                          \
				System::Pinout::Name::kGPIO_EMC_40,    \
				System::Pinout::Name::kGPIO_B1_15, \
	}

/*
 * Pin order: bluetooth0 tx pin, bluetooth0 rx pin, bluetooth1 tx pin, bluetooth1 rx pin...
 */
#define BLUETOOTH_PINS                                          \
	{                                                             \
		System::Pinout::Name::kGPIO_B1_12,			/*	TX LPUART 5	*/ \
				System::Pinout::Name::kGPIO_B1_13, /*	RX LPUART 5	*/ \
	}

/*
 * Pin order: R0 pin, G0 pin, B0 pin, R1 pin, G1 pin, B1 pin...
 */
#define RGB_LED_PINS \
	{                  \
	}

/*
 * Pin order: Motor0 PWMA pin, Motor0 PWMB pin, Motor1 PWMA pin, Motor1 PWMB pin...
 */
#define ALTERNATE_MOTOR_PINS                         \
	{                                                  \
	}

#define DIR_MOTOR_PINS                               \
	{                                                  \
	}

#define SERVO_PINS \
	{                \
	}

/*
 * Pin order: Encoder0 COUNT pin, Encoder0 DIR pin, Encoder1 COUNT pin, Encoder1 DIR pin...
 * Pin order: AB doesn't matter
 */
#define Encoder_PINS                                     \
	{                                                          \
	}

#define FLASH_SIZE 32 * 1024 * 1024u // 128MB remember to modify linker script also
#define FLASH_PAGE_SIZE 256u
#define FLASH_SECTOR_SIZE 4u * 1024u
#define FLASH_BLOCK_SIZE 64u * 1024u
#define FLASH_FLEXSPI_SCK System::Pinout::Name::kGPIO_SD_B1_07
#define FLASH_FLEXSPI_CS System::Pinout::Name::kGPIO_SD_B1_06
#define FLASH_FLEXSPI_SIO0 System::Pinout::Name::kGPIO_SD_B1_08
#define FLASH_FLEXSPI_SIO1 System::Pinout::Name::kGPIO_SD_B1_09
#define FLASH_FLEXSPI_SIO2 System::Pinout::Name::kGPIO_SD_B1_10
#define FLASH_FLEXSPI_SIO3 System::Pinout::Name::kGPIO_SD_B1_11
#define FLASH_FLEXSPI_SIO4 System::Pinout::Name::kDisable
#define FLASH_FLEXSPI_SIO5 System::Pinout::Name::kDisable
#define FLASH_FLEXSPI_SIO6 System::Pinout::Name::kDisable
#define FLASH_FLEXSPI_SIO7 System::Pinout::Name::kDisable
#define FLASH_FLEXSPI_DQS System::Pinout::Name::kDisable
#define FLEXSPI_LUT_SEQ(cmd0, pad0, op0, cmd1, pad1, op1)                                                            \
	(FLEXSPI_LUT_OPERAND0(op0) | FLEXSPI_LUT_NUM_PADS0(pad0) | FLEXSPI_LUT_OPCODE0(cmd0) | FLEXSPI_LUT_OPERAND1(op1) | \
	 FLEXSPI_LUT_NUM_PADS1(pad1) | FLEXSPI_LUT_OPCODE1(cmd1))

#define FLASH_LUT_READ_NORMAL 0
#define FLASH_LUT_READ_FAST 1
#define FLASH_LUT_READ_FAST_QUAD 2
#define FLASH_LUT_READSTATUS 3
#define FLASH_LUT_WRITEENABLE 4
#define FLASH_LUT_ERASESECTOR 5
#define FLASH_LUT_PAGEPROGRAM_SINGLE 6
#define FLASH_LUT_PAGEPROGRAM_QUAD 7
#define FLASH_LUT_READID 8
#define FLASH_LUT_READJEDECID 9
#define FLASH_LUT_WRITESTATUSREG 10
#define FLASH_LUT_READSTATUSREG 11
#define FLASH_LUT_ERASECHIP 12
#define FLASH_LUT_AHB_PAGEPROGRAM_QUAD_1 13
#define FLASH_LUT_AHB_PAGEPROGRAM_QUAD_2 14

/* FLASH�������� */
#define W25Q_WriteEnable 0x06
#define W25Q_WriteDisable 0x04
#define W25Q_ReadStatusReg 0x05
#define W25Q_WriteStatusReg 0x01
#define W25Q_ReadData 0x03
#define W25Q_ReadData_4Addr 0x13
#define W25Q_FastReadData 0x0B
#define W25Q_FastReadData_4Addr 0x0C
#define W25Q_FastReadDual 0x3B
#define W25Q_FastReadDual_4Addr 0x3C
#define W25Q_FastReadQuad 0x6B
#define W25Q_FastReadQuad_4Addr 0x6C
#define W25Q_PageProgram 0x02
#define W25Q_PageProgram_4Addr 0x12
#define W25Q_PageProgramQuad 0x32
#define W25Q_PageProgramQuad_4Addr 0x34
#define W25Q_BlockErase 0xD8
#define W25Q_BlockErase_4Addr 0xDC
#define W25Q_SectorErase 0x20
#define W25Q_SectorErase_4Addr 0x21
#define W25Q_ChipErase 0xC7
#define W25Q_PowerDown 0xB9
#define W25Q_ReleasePowerDown 0xAB
#define W25Q_DeviceID 0xAB
#define W25Q_ManufactDeviceID 0x90
#define W25Q_JedecDeviceID 0x9F
/*����*/
#define FLASH_ID 0X18
#define FLASH_JEDECDEVICE_ID 0XEF4018
#define FLASH_LUT_SIZE 60
#define FLASH_LUT_CONTENT                                                                                                                                                                                                                                                                               \
	{ /* ��ͨ��ָ�Normal read mode -SDR */                                                                                                                                                                                                                                                   \
		FLEXSPI_LUT_SEQ(Driver::FlexSpi::LUTCommand::kSDR, Driver::FlexSpi::Pad::k1PAD, W25Q_ReadData_4Addr, Driver::FlexSpi::LUTCommand::kRADDR_SDR, Driver::FlexSpi::Pad::k1PAD, 32),                                                                                                                     \
				FLEXSPI_LUT_SEQ(Driver::FlexSpi::LUTCommand::kREAD_SDR, Driver::FlexSpi::Pad::k1PAD, 0x04, Driver::FlexSpi::LUTCommand::kSTOP, Driver::FlexSpi::Pad::k1PAD, 0), 0, 0, /* ���ٶ�ָ�Fast read mode - SDR */                                                                          \
				FLEXSPI_LUT_SEQ(Driver::FlexSpi::LUTCommand::kSDR, Driver::FlexSpi::Pad::k1PAD, W25Q_FastReadData_4Addr, Driver::FlexSpi::LUTCommand::kRADDR_SDR, Driver::FlexSpi::Pad::k1PAD, 32),                                                                                                             \
				FLEXSPI_LUT_SEQ(Driver::FlexSpi::LUTCommand::kDUMMY_SDR, Driver::FlexSpi::Pad::k1PAD, 0x08, Driver::FlexSpi::LUTCommand::kREAD_SDR, Driver::FlexSpi::Pad::k1PAD, 0x04), 0, 0, /* QUADģʽ���ٶ�ָ�Fast read quad mode - SDR */                                                     \
				FLEXSPI_LUT_SEQ(Driver::FlexSpi::LUTCommand::kSDR, Driver::FlexSpi::Pad::k1PAD, 0xEB, Driver::FlexSpi::LUTCommand::kRADDR_SDR, Driver::FlexSpi::Pad::k4PAD, 0x18),                                                                                                                              \
				FLEXSPI_LUT_SEQ(Driver::FlexSpi::LUTCommand::kDUMMY_SDR, Driver::FlexSpi::Pad::k4PAD, 0x06, Driver::FlexSpi::LUTCommand::kREAD_SDR, Driver::FlexSpi::Pad::k4PAD, 0x04), 0, 0,						 /* ��ȡ��չ������Read extend parameters */                                         \
				FLEXSPI_LUT_SEQ(Driver::FlexSpi::LUTCommand::kSDR, Driver::FlexSpi::Pad::k1PAD, W25Q_ReadStatusReg, Driver::FlexSpi::LUTCommand::kREAD_SDR, Driver::FlexSpi::Pad::k1PAD, 0x04), 0, 0, 0, /* дʹ�ܣ�Write Enable */                                                                         \
				FLEXSPI_LUT_SEQ(Driver::FlexSpi::LUTCommand::kSDR, Driver::FlexSpi::Pad::k1PAD, W25Q_WriteEnable, Driver::FlexSpi::LUTCommand::kSTOP, Driver::FlexSpi::Pad::k1PAD, 0), 0, 0, 0,					 /* ����������Erase Sector  */                                                      \
				FLEXSPI_LUT_SEQ(Driver::FlexSpi::LUTCommand::kSDR, Driver::FlexSpi::Pad::k1PAD, 0x20, Driver::FlexSpi::LUTCommand::kRADDR_SDR, Driver::FlexSpi::Pad::k1PAD, 24), 0, 0, 0,								 /* SINGLEģʽҳд�룬Page Program - single mode */                                                   \
				FLEXSPI_LUT_SEQ(Driver::FlexSpi::LUTCommand::kSDR, Driver::FlexSpi::Pad::k1PAD, W25Q_PageProgram_4Addr, Driver::FlexSpi::LUTCommand::kRADDR_SDR, Driver::FlexSpi::Pad::k1PAD, 32),                                                                                                              \
				FLEXSPI_LUT_SEQ(Driver::FlexSpi::LUTCommand::kWRITE_SDR, Driver::FlexSpi::Pad::k1PAD, 0x04, Driver::FlexSpi::LUTCommand::kSTOP, Driver::FlexSpi::Pad::k1PAD, 0), 0, 0, /* QUADģʽҳд�룬Page Program - quad mode */                                                                         \
				FLEXSPI_LUT_SEQ(Driver::FlexSpi::LUTCommand::kSDR, Driver::FlexSpi::Pad::k1PAD, 0x32, Driver::FlexSpi::LUTCommand::kRADDR_SDR, Driver::FlexSpi::Pad::k1PAD, 24),                                                                                                                                \
				FLEXSPI_LUT_SEQ(Driver::FlexSpi::LUTCommand::kWRITE_SDR, Driver::FlexSpi::Pad::k4PAD, 0x04, Driver::FlexSpi::LUTCommand::kSTOP, Driver::FlexSpi::Pad::k1PAD, 0), 0, 0, /* ��ID��Read ID ��ID7-ID0*/                                                                                 \
				FLEXSPI_LUT_SEQ(Driver::FlexSpi::LUTCommand::kSDR, Driver::FlexSpi::Pad::k1PAD, W25Q_DeviceID, Driver::FlexSpi::LUTCommand::kDUMMY_SDR, Driver::FlexSpi::Pad::k1PAD, 32),                                                                                                                       \
				FLEXSPI_LUT_SEQ(Driver::FlexSpi::LUTCommand::kREAD_SDR, Driver::FlexSpi::Pad::k1PAD, 0x04, Driver::FlexSpi::LUTCommand::kSTOP, Driver::FlexSpi::Pad::k1PAD, 0), 0, 0,																																	 /* ��JedecDeviceID,MF7-MF0+ID15-ID0 */               \
				FLEXSPI_LUT_SEQ(Driver::FlexSpi::LUTCommand::kSDR, Driver::FlexSpi::Pad::k1PAD, W25Q_JedecDeviceID, Driver::FlexSpi::LUTCommand::kREAD_SDR, Driver::FlexSpi::Pad::k1PAD, 0x04), 0, 0, 0,																							 /* д״̬�Ĵ�����Enable Quad mode */         \
				FLEXSPI_LUT_SEQ(Driver::FlexSpi::LUTCommand::kSDR, Driver::FlexSpi::Pad::k1PAD, W25Q_WriteStatusReg, Driver::FlexSpi::LUTCommand::kWRITE_SDR, Driver::FlexSpi::Pad::k1PAD, 0x04), 0, 0, 0,																						 /* ��״̬�Ĵ�����Read status register */ \
				FLEXSPI_LUT_SEQ(Driver::FlexSpi::LUTCommand::kSDR, Driver::FlexSpi::Pad::k1PAD, W25Q_ReadStatusReg, Driver::FlexSpi::LUTCommand::kREAD_SDR, Driver::FlexSpi::Pad::k1PAD, 0x04), 0, 0, 0,																							 /* ������ƬFLASH��Erase Chip */          \
				FLEXSPI_LUT_SEQ(Driver::FlexSpi::LUTCommand::kSDR, Driver::FlexSpi::Pad::k1PAD, W25Q_ChipErase, Driver::FlexSpi::LUTCommand::kSTOP, Driver::FlexSpi::Pad::k1PAD, 0), 0, 0, 0, /* ��AHB������ʵ� QUADģʽҳд�� ���У�����дʹ�ܺ�ҳд���������� */ /* дʹ�ܣ�Write Enable */                           \
				FLEXSPI_LUT_SEQ(Driver::FlexSpi::LUTCommand::kSDR, Driver::FlexSpi::Pad::k1PAD, W25Q_WriteEnable, Driver::FlexSpi::LUTCommand::kSTOP, Driver::FlexSpi::Pad::k1PAD, 0), 0, 0, 0,																												 /* QUADģʽҳд�룬Page Program - quad mode */         \
				FLEXSPI_LUT_SEQ(Driver::FlexSpi::LUTCommand::kSDR, Driver::FlexSpi::Pad::k1PAD, W25Q_PageProgramQuad_4Addr, Driver::FlexSpi::LUTCommand::kRADDR_SDR, Driver::FlexSpi::Pad::k1PAD, 32),                                                                                                          \
				FLEXSPI_LUT_SEQ(Driver::FlexSpi::LUTCommand::kWRITE_SDR, Driver::FlexSpi::Pad::k4PAD, 0x04, Driver::FlexSpi::LUTCommand::kSTOP, Driver::FlexSpi::Pad::k1PAD, 0)                                                                                                                                 \
	}

#define FIREGE5INCHTOUCHSCREEN_SDA(id) id == 0 ? System::Pinout::Name::kGPIO_SD_B1_05 : System::Pinout::Name::kDisable
#define FIREGE5INCHTOUCHSCREEN_SCL(id) id == 0 ? System::Pinout::Name::kGPIO_SD_B1_04 : System::Pinout::Name::kDisable
#define FIREGE5INCHTOUCHSCREEN_INT(id) id == 0 ? System::Pinout::Name::kGPIO_AD_B0_11 : System::Pinout::Name::kDisable
#define FIREGE5INCHTOUCHSCREEN_RST(id) id == 0 ? System::Pinout::Name::kGPIO_AD_B0_12 : System::Pinout::Name::kDisable
#define FIREGE5INCHTOUCHSCREEN_BK(id) id == 0 ? System::Pinout::Name::kGPIO_AD_B0_13 : System::Pinout::Name::kDisable
#define FIREGE5INCHTOUCHSCREEN_DISP(id) id == 0 ? System::Pinout::Name::kGPIO_AD_B0_14 : System::Pinout::Name::kDisable
#define FIREGE5INCHTOUCHSCREEN_D0(id) id == 0 ? System::Pinout::Name::kGPIO_B0_04 : System::Pinout::Name::kDisable
#define FIREGE5INCHTOUCHSCREEN_D1(id) id == 0 ? System::Pinout::Name::kGPIO_B0_05 : System::Pinout::Name::kDisable
#define FIREGE5INCHTOUCHSCREEN_D2(id) id == 0 ? System::Pinout::Name::kGPIO_B0_06 : System::Pinout::Name::kDisable
#define FIREGE5INCHTOUCHSCREEN_D3(id) id == 0 ? System::Pinout::Name::kGPIO_B0_07 : System::Pinout::Name::kDisable
#define FIREGE5INCHTOUCHSCREEN_D4(id) id == 0 ? System::Pinout::Name::kGPIO_B0_08 : System::Pinout::Name::kDisable
#define FIREGE5INCHTOUCHSCREEN_D5(id) id == 0 ? System::Pinout::Name::kGPIO_B0_09 : System::Pinout::Name::kDisable
#define FIREGE5INCHTOUCHSCREEN_D6(id) id == 0 ? System::Pinout::Name::kGPIO_B0_10 : System::Pinout::Name::kDisable
#define FIREGE5INCHTOUCHSCREEN_D7(id) id == 0 ? System::Pinout::Name::kGPIO_B0_11 : System::Pinout::Name::kDisable
#define FIREGE5INCHTOUCHSCREEN_D8(id) id == 0 ? System::Pinout::Name::kGPIO_B0_12 : System::Pinout::Name::kDisable
#define FIREGE5INCHTOUCHSCREEN_D9(id) id == 0 ? System::Pinout::Name::kGPIO_B0_13 : System::Pinout::Name::kDisable
#define FIREGE5INCHTOUCHSCREEN_D10(id) id == 0 ? System::Pinout::Name::kGPIO_B0_14 : System::Pinout::Name::kDisable
#define FIREGE5INCHTOUCHSCREEN_D11(id) id == 0 ? System::Pinout::Name::kGPIO_B0_15 : System::Pinout::Name::kDisable
#define FIREGE5INCHTOUCHSCREEN_D12(id) id == 0 ? System::Pinout::Name::kGPIO_B1_00 : System::Pinout::Name::kDisable
#define FIREGE5INCHTOUCHSCREEN_D13(id) id == 0 ? System::Pinout::Name::kGPIO_B1_01 : System::Pinout::Name::kDisable
#define FIREGE5INCHTOUCHSCREEN_D14(id) id == 0 ? System::Pinout::Name::kGPIO_B1_02 : System::Pinout::Name::kDisable
#define FIREGE5INCHTOUCHSCREEN_D15(id) id == 0 ? System::Pinout::Name::kGPIO_B1_03 : System::Pinout::Name::kDisable
#define FIREGE5INCHTOUCHSCREEN_D16(id) id == 0 ? System::Pinout::Name::kGPIO_B1_04 : System::Pinout::Name::kDisable
#define FIREGE5INCHTOUCHSCREEN_D17(id) id == 0 ? System::Pinout::Name::kGPIO_B1_05 : System::Pinout::Name::kDisable
#define FIREGE5INCHTOUCHSCREEN_D18(id) id == 0 ? System::Pinout::Name::kGPIO_B1_06 : System::Pinout::Name::kDisable
#define FIREGE5INCHTOUCHSCREEN_D19(id) id == 0 ? System::Pinout::Name::kGPIO_B1_07 : System::Pinout::Name::kDisable
#define FIREGE5INCHTOUCHSCREEN_D20(id) id == 0 ? System::Pinout::Name::kGPIO_B1_08 : System::Pinout::Name::kDisable
#define FIREGE5INCHTOUCHSCREEN_D21(id) id == 0 ? System::Pinout::Name::kGPIO_B1_09 : System::Pinout::Name::kDisable
#define FIREGE5INCHTOUCHSCREEN_D22(id) id == 0 ? System::Pinout::Name::kGPIO_B1_10 : System::Pinout::Name::kDisable
#define FIREGE5INCHTOUCHSCREEN_D23(id) id == 0 ? System::Pinout::Name::kGPIO_B1_11 : System::Pinout::Name::kDisable
#define FIREGE5INCHTOUCHSCREEN_PCLK(id) id == 0 ? System::Pinout::Name::kGPIO_B0_00 : System::Pinout::Name::kDisable
#define FIREGE5INCHTOUCHSCREEN_ENABLE(id) id == 0 ? System::Pinout::Name::kGPIO_B0_01 : System::Pinout::Name::kDisable
#define FIREGE5INCHTOUCHSCREEN_HSYNC(id) id == 0 ? System::Pinout::Name::kGPIO_B0_02 : System::Pinout::Name::kDisable
#define FIREGE5INCHTOUCHSCREEN_VSYNC(id) id == 0 ? System::Pinout::Name::kGPIO_B0_03 : System::Pinout::Name::kDisable

#endif /* CONFIG_2019_CREATIVE_1_H_ */
