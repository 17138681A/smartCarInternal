/*
 * eflexpwm.h
 *
 *  Created on: Aug 12, 2018
 *      Author: LeeChunHei
 */

#ifndef INC_DRIVER_EFLEXPWM_H_
#define INC_DRIVER_EFLEXPWM_H_

#include "driver/timer.h"
#include "system/cmsis/access_layer/access_layer.h"
#include "system/pinout/MIMXRT1052.h"

namespace Driver
{

/*!
 * @addtogroup eflexpwm Enhanced Flex Pulse Width Modulator
 * @ingroup timer
 * This PWM module can generate various switching patterns, including highly sophisticated waveforms. It can be used to control all known motor types and is ideal for controlling different Switched Mode Power Supplies (SMPS) topologies as well
 * @{
 */
/**
 * @brief Enhanced Flex Pulse Width Modulator
 */
class eFlexPWM : public Timer
{
  public:
	struct Config
	{
		enum struct Alignment
		{
			kSignedCenterAligned,
			kCenterAligned,
			kSignedEdgeAligned,
			kEdgeAligned
		};
		enum struct ReloadLogic
		{
			kReloadImmidate,
			kReloadHalfCycle,
			kReloadFullCycle,
			kReloadHalfAndFullCycle
		};
		enum struct ClockSource
		{
			kBusClock,
			kExternalClock,
			kSubmodule0Clock
		};
		enum struct InitializationControl
		{
			kInitialize_LocalSync,
			kInitialize_MasterReload,
			kInitialize_MasterSync,
			kInitialize_ExtSync
		};
		enum struct ForceTrigger
		{
			kForce_Local = 0U,   /*!< The local force signal, CTRL2[FORCE], from the submodule is used to force updates */
			kForce_Master,		 /*!< The master force signal from submodule 0 is used to force updates */
			kForce_LocalReload,  /*!< The local reload signal from this submodule is used to force updates without regard to
			 the state of LDOK */
			kForce_MasterReload, /*!< The master reload signal from submodule 0 is used to force updates if LDOK is set */
			kForce_LocalSync,	/*!< The local sync signal from this submodule is used to force updates */
			kForce_MasterSync,   /*!< The master sync signal from submodule0 is used to force updates */
			kForce_External,	 /*!< The external force signal, EXT_FORCE, from outside the PWM module causes updates */
			kForce_ExternalSync  /*!< The external sync signal, EXT_SYNC, from outside the PWM module causes updates */
		};
		enum struct ReloadFrequence
		{
			kLoadEveryOportunity = 0U, /*!< Every PWM opportunity */
			kLoadEvery2Oportunity,	 /*!< Every 2 PWM opportunities */
			kLoadEvery3Oportunity,	 /*!< Every 3 PWM opportunities */
			kLoadEvery4Oportunity,	 /*!< Every 4 PWM opportunities */
			kLoadEvery5Oportunity,	 /*!< Every 5 PWM opportunities */
			kLoadEvery6Oportunity,	 /*!< Every 6 PWM opportunities */
			kLoadEvery7Oportunity,	 /*!< Every 7 PWM opportunities */
			kLoadEvery8Oportunity,	 /*!< Every 8 PWM opportunities */
			kLoadEvery9Oportunity,	 /*!< Every 9 PWM opportunities */
			kLoadEvery10Oportunity,	/*!< Every 10 PWM opportunities */
			kLoadEvery11Oportunity,	/*!< Every 11 PWM opportunities */
			kLoadEvery12Oportunity,	/*!< Every 12 PWM opportunities */
			kLoadEvery13Oportunity,	/*!< Every 13 PWM opportunities */
			kLoadEvery14Oportunity,	/*!< Every 14 PWM opportunities */
			kLoadEvery15Oportunity,	/*!< Every 15 PWM opportunities */
			kLoadEvery16Oportunity	 /*!< Every 16 PWM opportunities */
		};
		System::Pinout::Name pwm_pin;
		System::Pinout::Name comp_pwm_pin = System::Pinout::Name::kDisable;
		bool polarity_inverted = false;
		uint16_t duty_cycle; //0-1000, represent 0%-100.0% duty cycle
		uint16_t dead_time;  //Deadtime value, only useful when channel pair is in complementary mode
		uint32_t frequency;  //In Hz
		Alignment alignment = Alignment::kSignedCenterAligned;
		bool enable_debug_mode = false;
		bool enable_wait = false;
		ReloadLogic reload_logic = ReloadLogic::kReloadHalfAndFullCycle;
		bool is_master_reload = false;
		ClockSource clock_source = ClockSource::kBusClock;
		InitializationControl initialization_control = InitializationControl::kInitialize_LocalSync;
		ForceTrigger force_trigger = ForceTrigger::kForce_Local;
		ReloadFrequence reload_frequence = ReloadFrequence::kLoadEveryOportunity;
	};
	explicit eFlexPWM(const Config &config);
	void SetDutyCycle(uint16_t duty_cycle) override; //value base on period count

	static bool CheckPin(System::Pinout::Name pin);

  private:
	enum struct PWMModule
	{
		kPWM1,
		kPWM2,
		kPWM3,
		kPWM4
	};
	enum struct SubModule
	{
		kSubModule0,
		kSubModule1,
		kSubModule2,
		kSubModule3
	};
	enum struct Channel
	{
		kPWMA,
		kPWMB,
		kPWMX
	};

	static bool GetModuleDetail(System::Pinout::Name pin, PWMModule &pwm_module, SubModule &sub_module, Channel &channel);
	void InitChannel(Channel channel, System::Pinout::Name pin_name, bool polarity_inverted = false, uint16_t duty_cycle = 0, uint16_t dead_time = 0);

	PWM_Type *pwm_base;
	SubModule sub_module;
	Channel channel;
	uint16_t period;
	const Config::Alignment alignment;
	static uint16_t pwm_modules_inited;
};
/** @}*/

} // namespace Driver

#endif /* INC_DRIVER_EFLEXPWM_H_ */
