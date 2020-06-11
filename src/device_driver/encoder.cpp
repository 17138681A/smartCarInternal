/*
 * encoder.cpp
 *
 *  Created on: 3 Apr 2019
 *      Author: lch19
 */

#include "device_driver/encoder.h"
#include "driver/xbar.h"
#include <assert.h>

#ifdef ENCODER_USED

namespace DeviceDriver
{

const System::Pinout::Name encoder_pins[ENCODER_USED<<1] = Encoder_PINS;

const Driver::QuadDecoder::Config Encoder::GetDecoderConfig(const Config &config)
{
	Driver::QuadDecoder::Config decoder_config;
	decoder_config.module = (Driver::QuadDecoder::Config::Module)config.id;
    if(config.type==Config::Type::kDirEncoder){
        decoder_config.decoder_work_mode = Driver::QuadDecoder::Config::WorkMode::kSignalPhaseCountMode;
	}
	decoder_config.enable_reverse_direction = config.reverse_counting;
	return decoder_config;
}

Encoder::Encoder(const Config &config) : quad_decoder(GetDecoderConfig(config))
{
	Driver::XBARA::XBAR1InputSignal input_signal_a, input_signal_b;
	Driver::XBARA::XBAR1OutputSignal output_singal;
	assert(Driver::XBARA::OpenPin(encoder_pins[(config.id) << 1], input_signal_a, output_singal));
	assert(Driver::XBARA::OpenPin(encoder_pins[((config.id) << 1) + 1], input_signal_b, output_singal));
	switch (config.id)
	{
	case 0:
		Driver::XBARA::Connect(input_signal_a, Driver::XBARA::XBAR1OutputSignal::kEnc1PhaseAInput);
		Driver::XBARA::Connect(input_signal_b, Driver::XBARA::XBAR1OutputSignal::kEnc1PhaseBInput);
		break;
	case 1:
		Driver::XBARA::Connect(input_signal_a, Driver::XBARA::XBAR1OutputSignal::kEnc2PhaseAInput);
		Driver::XBARA::Connect(input_signal_b, Driver::XBARA::XBAR1OutputSignal::kEnc2PhaseBInput);
		break;
	case 2:
		Driver::XBARA::Connect(input_signal_a, Driver::XBARA::XBAR1OutputSignal::kEnc3PhaseAInput);
		Driver::XBARA::Connect(input_signal_b, Driver::XBARA::XBAR1OutputSignal::kEnc3PhaseBInput);
		break;
	case 3:
		Driver::XBARA::Connect(input_signal_a, Driver::XBARA::XBAR1OutputSignal::kEnc4PhaseAInput);
		Driver::XBARA::Connect(input_signal_b, Driver::XBARA::XBAR1OutputSignal::kEnc4PhaseBInput);
		break;
	}
	quad_decoder.LoadInitialPositionValue();
}

} // namespace DeviceDriver

#endif
