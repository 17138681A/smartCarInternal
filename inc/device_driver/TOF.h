#pragma once

#ifndef H_TOF
#define H_TOP

#include "system/pinout/pinout.h"
#include "system/cmsis/access_layer/access_layer.h"
#include "system/systick.h"

#define SOFTWAREWIRE_NO_ERROR 0
#define SOFTWAREWIRE_BUFFER_FULL 1
#define SOFTWAREWIRE_ADDRESS_NACK 2
#define SOFTWAREWIRE_DATA_NACK 3
#define SOFTWAREWIRE_OTHER 4

#define SOFTWAREWIRE_BUFSIZE 32 // same as buffer size of Arduino Wire library

namespace DeviceDriver
{
class TOF
{
public:
    struct Config
    {
        System::Pinout::Name sda = System::Pinout::Name::kDisable;
        System::Pinout::Name scl = System::Pinout::Name::kDisable;
    };

    TOF(const Config con) : scl(getGPIOConfig(con.scl)), sda(getGPIOConfig(con.sda))
    {
    }

    // inline bool prepareReading(uint8_t addr, uint8_t reg = 0x00 /* 0x00 */)
    // {
    //     // bool isWorking = true;
    //     // isWorking &= i2cMaster->SendByteWithStartCMD(addr & 0b11111110);
    //     // isWorking &= i2cMaster->SendByte(reg);
    //     // isWorking &= i2cMaster->SendStopCMD();

    //     // return isWorking; // indicate if the device is found

    //     return true;
    // }

    // inline int getReading(uint8_t addr)
    // {
        // uint8_t d0, d1; // 0x0 and 0x1
        // bool isWorking = true;
        // isWorking &= i2cMaster->SendByteWithStartCMD(addr| 0x01);
        // // isWorking &= i2cMaster->GetByte(&d0, false);
        // while (!(((Driver::SoftI2CMaster*) i2cMaster)->isClockLow()))
        //     ;
        // isWorking &= i2cMaster->GetByte(&d1, true);
        // isWorking &= i2cMaster->SendStopCMD();
        // return (d0 << 8 | d1);
    // }

    inline int getRealtimeDistance(uint8_t addr, uint8_t reg = 0x01)
    {
        beginTransmission(addr);
        write(reg);
        endTransmission();
        
        System::Systick::DelayUS(40);

        requestFrom(addr, 1);

        volatile int d0 = 0, d1 = 0;

        // char t[40];
        // sprintf(t, "%3d, ", available());
        // printf(t);

        // while (2 <= available()) { // if two bytes were received
        //     d0 = read();  // receive high byte (overwrites previous reading)    

        //     // sprintf(t, "%3d, ", d0);
        //     // printf(t);
        // }

        d1 = read(); // receive low byte as lower 8 bits

        // sprintf(t, "%3d, ", d1);
        // printf(t);

        int returnValue = d0;
        returnValue <<= 8;
        returnValue |= d1;

        // if (returnValue > 110 && dirsend_flag == false) {
            // System::Systick::DelayUS(30);
            // dirsend_flag = true;

            // beginTransmission(addr);
            // write(0x09);
            // System::Systick::DelayUS(35);
            // write((uint8_t*) (&dirsend_flag), 1);
            // endTransmission();

        //     // SensorWrite(0x09, &dirsend_flag, 1);
        // } 
        // else if(returnValue < 90 && dirsend_flag == true)
        // {
        //     System::Systick::DelayUS(35);
        //     dirsend_flag = false;

        //     beginTransmission(addr);
        //     write(0x09);
        //     System::Systick::DelayUS(35);
        //     write((uint8_t*) (&dirsend_flag), 1);
        //     endTransmission();

        //     // SensorWrite(0x09, &dirsend_flag, 1);
        // }

        return returnValue;
    }

    // inline bool setReg(uint8_t addr, uint8_t reg, uint8_t data)
    // {
        // volatile bool isWorking = true;
        // isWorking &= i2cMaster->SendByteWithStartCMD(addr & 0b11111110);
        // isWorking &= i2cMaster->SendByte(reg);
        // System::Systick::DelayUS(50); // specified by datasheet
        // isWorking &= i2cMaster->SendByte(data);
        // isWorking &= i2cMaster->SendStopCMD();

        // System::Systick::DelayMS(500);
        // return isWorking; // indicate if the device is found
    // }

    // inline bool changeFirstFoundDeviceToNewAddress(uint8_t newAddr)
    // {
        // uint8_t addr = 0;
        // for (addr = 0; addr < 127; ++addr)
        // {
        //     volatile bool t = i2cMaster->SendByteWithStartCMD(addr << 1);
        //     i2cMaster->SendStopCMD();

        //     if (t) {
        //         break;
        //     }
        // }

        // if (addr == 127)
        //     return false;

        // return setReg(addr << 1, 0x0f, newAddr); // 0x0f is the device address register
    // }

    inline void scanI2cBus() // std::vector<uint8_t> addrs
    {
        // addrs.clear();
        // for (int addr = 0; addr < 127; ++addr)
        // {
        //     bool t = i2cMaster->SendByteWithStartCMD(addr << 1);
        //     i2cMaster->SendStopCMD();

        //     if (t) {
        //         addrs.push_back(addr);
        //     }
        // }

        uint8_t error, address;
        int nDevices;

        while (true) {

            printf("Scanning...");

            nDevices = 0;
            for(address = 1; address < 127; address++ ) 
            {
                // The i2c_scanner uses the return value of
                // the Write.endTransmisstion to see if
                // a device did acknowledge to the address.
                beginTransmission(address);
                error = endTransmission();

                if (error == 0)
                {
                    printf("I2C device found at address int ");
                    char c[40];
                    sprintf(c, "%3d\n", address);
                    printf(c);

                    nDevices++;
                }
                else if (error==4) 
                {
                    printf("Unknown error at address 0x");
                    // if (address<16) 
                    //     printf("0");
                    // printf(address);
                }    
            }
            if (nDevices == 0)
                printf("No I2C devices found\n");
            else
                printf("done\n");

            System::Systick::DelayMS(100);
        }
    }

private:
    bool dirsend_flag;

    Driver::GPIO scl, sda;
    const Driver::GPIO::Config getGPIOConfig(const System::Pinout::Name pin)
    {
        Driver::GPIO::Config pConfig;
        pConfig.pin = pin;
        pConfig.default_high = true;
        pConfig.gpio_dir = Driver::GPIO::Direction::kDigitalOutput;
        pConfig.pin_config.open_drain_enable = true;
        pConfig.force_input = true;
        return pConfig;
    }

    inline void i2c_sda_lo()
    {
        sda.Set(false);
    }

    inline void i2c_sda_hi()
    {
        sda.Set(true);
    }

    inline void i2c_scl_lo()
    {
        scl.Set(false);
    }

    inline void i2c_scl_hi()
    {
        scl.Set(true);
    }

    inline uint8_t i2c_sda_read()
    {
        return sda.Get();
    }

    inline uint8_t i2c_scl_read()
    {
        return scl.Get();
    }

    inline void delayMicroseconds(int t)
    {
        System::Systick::DelayCount(t);
    }

    inline uint32_t millis() {
        return System::Systick::GetTickCount();
    }

    // below soft i2c implementation comes from
    // https://raw.githubusercontent.com/Testato/SoftwareWire/master/SoftwareWire.cpp

    uint8_t _transmission;  // transmission status, returned by endTransmission(). 0 is no error.
    int _i2cdelay = 20000;     // delay in micro seconds for sda and scl bits.
    bool _stretch = false;       // should code handle clock stretching by the slave or not.
    unsigned long _timeout = _i2cdelay * 2; // timeout in ms. When waiting for a clock pulse stretch. 2017, Fix issue #6

    uint8_t rxBuf[SOFTWAREWIRE_BUFSIZE]; // buffer inside this class, a buffer per SoftwareWire.
    uint8_t rxBufPut;                    // index to rxBuf, just after the last valid byte.
    uint8_t rxBufGet;                    // index to rxBuf, the first new to be read byte.

    void beginTransmission(uint8_t address)
    {
        // // Reset error returned by endTransmission.
        _transmission = SOFTWAREWIRE_NO_ERROR;

        // check return value of the start condition.
        // It indicates if the i2c bus is okay.
        if (i2c_start())
        {
            uint8_t rc = i2c_write((address << 1) | 0); // The r/w bit is zero for write

            if (rc == 0) // a sda zero from Slave for the 9th bit is ack
            {
                _transmission = SOFTWAREWIRE_NO_ERROR;
            }
            else
            {
                _transmission = SOFTWAREWIRE_ADDRESS_NACK;
            }
        }
        else
        {
            // If the bus was not okay, the scl or sda didn't work.
            _transmission = SOFTWAREWIRE_OTHER;
        }
    }

    uint8_t endTransmission(bool sendStop = true)
    {
        if (sendStop)
            i2c_stop();
        else
            i2c_repstart();

        return (_transmission); // return the transmission status that was set during writing address and data
    }

    uint8_t requestFrom(uint8_t address, uint8_t size, bool sendStop = true)
    {
        uint8_t n = 0; // number of valid received bytes. Start with 0 bytes.

        // The transmission status is set, although it is not returned.
        // Start with the status : no error
        _transmission = SOFTWAREWIRE_NO_ERROR;

        // Clear the RX buffer
        rxBufPut = 0;
        rxBufGet = 0;

        bool bus_okay = i2c_start();

        if (bus_okay)
        {
            uint8_t rc = i2c_write((address << 1) | 1); // The r/w bit is '1' to read

            if (rc == 0) // a sda zero from Slave for the 9th bit is ack
            {
                _transmission = SOFTWAREWIRE_NO_ERROR;

                // TODO: check if the Slave returns less bytes than requested.

                for (; n < size; n++)
                {
                    if (n < (size - 1))
                        rxBuf[n] = i2c_read(true); // read with ack
                    else
                        rxBuf[n] = i2c_read(false); // last byte, read with nack
                }
                rxBufPut = n;
            }
            else
            {
                _transmission = SOFTWAREWIRE_ADDRESS_NACK;
            }
        }
        else
        {
            // There was a bus error.
            _transmission = SOFTWAREWIRE_OTHER;
        }

        if (sendStop || _transmission != SOFTWAREWIRE_NO_ERROR)
            i2c_stop();
        else
            i2c_repstart();

        return (n);
    }

    size_t write(uint8_t data)
    {
        // When there was an error during the transmission, no more bytes are transmitted.
        if (_transmission == SOFTWAREWIRE_NO_ERROR)
        {
            if (i2c_write(data) == 0) // a sda zero from Slave for the 9th bit is ack
            {
                _transmission = SOFTWAREWIRE_NO_ERROR;
            }
            else
            {
                _transmission = SOFTWAREWIRE_ADDRESS_NACK;
            }
        }

        return (1); // ignore any errors, return the number of bytes that are written.
    }

    size_t write(const uint8_t *data, size_t quantity)
    {
        for (size_t i = 0; i < quantity; i++)
        {
            write(data[i]);
        }

        return (quantity); // ignore any errors, return the number of bytes that are written.
    }

    int available(void)
    {
        return (rxBufPut - rxBufGet);
    }

    int peek(void)
    {
        int data;

        if (rxBufPut > rxBufGet)
        {
            data = rxBuf[rxBufGet];
        }
        else
        {
            data = -1;
        }

        return (data);
    }

    int read(void)
    {
        int data;

        if (rxBufPut > rxBufGet)
        {
            data = rxBuf[rxBufGet];
            rxBufGet++;
        }
        else
        {
            data = -1;
        }

        return (data);
    }

    int readBytes(uint8_t *buf, uint8_t size)
    {
        int data;
        int n;

        for (n = 0; n < size; n++)
        {
            data = read();
            if (data == -1)
                break;
            else
                buf[n] = (uint8_t)data;
        }

        return (n);
    }

    int readBytes(char *buf, uint8_t size)
    {
        return readBytes((uint8_t *)buf, size);
    }

    int readBytes(char *buf, int size)
    {
        return readBytes((uint8_t *)buf, (uint8_t)size);
    }

    void i2c_writebit(uint8_t c)
    {
        if (c == 0)
        {
            i2c_sda_lo();
        }
        else
        {
            i2c_sda_hi();
        }

        if (_i2cdelay != 0)               // This delay is not needed, but it makes it safer
            delayMicroseconds(_i2cdelay); // This delay is not needed, but it makes it safer

        i2c_scl_hi(); // clock high: the Slave will read the sda signal

        // Check if clock stretching by the Slave should be detected.
        if (_stretch)
        {
            // If the Slave was stretching the clock pulse, the clock would not go high immediately.
            // For example if the Slave is an Arduino, that has other interrupts running (for example Serial data).
            unsigned long prevMillis = millis();
            while (i2c_scl_read() == 0)
            {
                if (millis() - prevMillis >= _timeout)
                    break;
            };
        }

        // After the clock stretching, the clock must be high for the normal duration.
        // That is why this delay has still to be done.
        if (_i2cdelay != 0)
            delayMicroseconds(_i2cdelay);

        i2c_scl_lo();

        if (_i2cdelay != 0)
            delayMicroseconds(_i2cdelay);
    }

    uint8_t i2c_readbit(void)
    {
        i2c_sda_hi(); // 'hi' is the same as releasing the line
        i2c_scl_hi();

        // Check if clock stretching by the Slave should be detected.
        if (_stretch)
        {
            // Wait until the clock is high, the Slave could keep it low for clock stretching.
            unsigned long prevMillis = millis();
            while (i2c_scl_read() == 0)
            {
                if (millis() - prevMillis >= _timeout)
                    break;
            };
        }

        // After the clock stretching, this delay has still be done before reading sda.
        if (_i2cdelay != 0)
            delayMicroseconds(_i2cdelay);

        uint8_t c = i2c_sda_read();

        i2c_scl_lo();

        if (_i2cdelay != 0)
            delayMicroseconds(_i2cdelay);

        return (c);
    }

    bool i2c_start(void)
    {
        i2c_sda_hi(); // can perhaps be removed some day ? if the rest of the code is okay
        i2c_scl_hi(); // can perhaps be removed some day ? if the rest of the code is okay

        if (_i2cdelay != 0)
            delayMicroseconds(_i2cdelay);

        // Both the sda and scl should be high.
        // If not, there might be a hardware problem with the i2c bus signal lines.
        // This check was added to prevent that a shortcut of sda would be seen as a valid ACK
        // from a i2c Slave.
        uint8_t sda_status = i2c_sda_read();
        uint8_t scl_status = i2c_scl_read();
        if (sda_status == 0 || scl_status == 0)
        {
            return (false);
        }
        else
        {
            i2c_sda_lo();

            if (_i2cdelay != 0)
                delayMicroseconds(_i2cdelay);

            i2c_scl_lo();

            if (_i2cdelay != 0)
                delayMicroseconds(_i2cdelay);
        }
        return (true);
    }

    void i2c_repstart(void)
    {
        i2c_sda_hi();
        //  i2c_scl_hi();               // ??????

        i2c_scl_lo(); // force SCL low

        if (_i2cdelay != 0)
            delayMicroseconds(_i2cdelay);

        i2c_sda_hi(); // release SDA

        if (_i2cdelay != 0)
            delayMicroseconds(_i2cdelay);

        i2c_scl_hi(); // release SCL

        // Check if clock stretching by the Slave should be detected.
        if (_stretch)
        {
            // If the Slave was stretching the clock pulse, the clock would not go high immediately.
            // For example if the Slave is an Arduino, that has other interrupts running (for example Serial data).
            unsigned long prevMillis = millis();
            while (i2c_scl_read() == 0)
            {
                if (millis() - prevMillis >= _timeout)
                    break;
            };
        }

        if (_i2cdelay != 0)
            delayMicroseconds(_i2cdelay);
    }

    void i2c_stop(void)
    {
        i2c_scl_lo(); // ADDED1, it should already be low.
        i2c_sda_lo();

        // ADDED1, wait to be sure that the slave knows that both are low
        if (_i2cdelay != 0)               // ADDED1
            delayMicroseconds(_i2cdelay); // ADDED1

        // For a stop, make SCL high wile SDA is still low
        i2c_scl_hi();

        // Check if clock stretching by the Slave should be detected.
        if (_stretch)
        {
            // Wait until the clock is high, the Slave could keep it low for clock stretching.
            // Clock pulse stretching during a stop condition seems odd, but when
            // the Slave is an Arduino, it might happen.
            unsigned long prevMillis = millis();
            while (i2c_scl_read() == 0)
            {
                if (millis() - prevMillis >= _timeout)
                    break;
            };
        }

        if (_i2cdelay != 0)
            delayMicroseconds(_i2cdelay);

        // complete the STOP by setting SDA high
        i2c_sda_hi();

        // A delay after the STOP for safety.
        // It is not known how fast the next START will happen.
        if (_i2cdelay != 0)
            delayMicroseconds(_i2cdelay);
    }

    uint8_t i2c_write(uint8_t c)
    {
        for (uint8_t i = 0; i < 8; i++)
        {
            i2c_writebit(c & 0x80); // highest bit first
            c <<= 1;
        }

        return (i2c_readbit());
    }

    uint8_t i2c_read(bool ack)
    {
        uint8_t res = 0;

        for (uint8_t i = 0; i < 8; i++)
        {
            res <<= 1;
            res |= i2c_readbit();
        }

        if (ack)
        {
            i2c_writebit(0);
        }
        else
        {
            i2c_writebit(1);
        }

        if (_i2cdelay != 0)
            delayMicroseconds(_i2cdelay);

        return (res);
    }
};
}; // namespace DeviceDriver

#endif