///*
// * gpt.h
// *
// *  Created on: 23 Mar 2019
// *      Author: LeeChunHei
// */
//
//#ifndef DRIVER_GPT_H_
//#define DRIVER_GPT_H_
//
//#include "driver/timer.h"
//#include "system/cmsis/access_layer/access_layer.h"
//
//namespace Driver {
//
//class GPT: public Timer {
//public:
//	struct Config {
//		enum struct ClockSource {
//			kOff, /*!< GPT Clock Source Off.*/
//			kPeriph, /*!< GPT Clock Source from Peripheral Clock.*/
//			kHighFreq, /*!< GPT Clock Source from High Frequency Reference Clock.*/
//			kExt, /*!< GPT Clock Source from external pin.*/
//			kLowFreq, /*!< GPT Clock Source from Low Frequency Reference Clock.*/
//			kOsc /*!< GPT Clock Source from Crystal oscillator.*/
//		};
//		ClockSource clock_source = ClockSource::kPeriph; /*!< clock source for GPT module. */
//		uint32_t divider = 1; /*!< clock divider (prescaler+1) from clock source to counter. */
//		bool free_run = false; /*!< true: FreeRun mode, false: Restart mode. */
//		bool run_in_wait = true; /*!< GPT enabled in wait mode. */
//		bool run_in_stop = true; /*!< GPT enabled in stop mode. */
//		bool run_in_doze = false; /*!< GPT enabled in doze mode. */
//		bool run_in_debug = false; /*!< GPT enabled in debug mode. */
//		bool reset_mode = true; /*!< true:  counter reset to 0 when enabled;
//		 false: counter retain its value when enabled. */
//	};
//	GPT(const Config& config);
//	void SetDutyCycle(uint16_t duty_cycle) override;
//
//private:
//	GPT_Type* gpt_base;
//};
//
//}
//
//
//#endif /* DRIVER_GPT_H_ */
