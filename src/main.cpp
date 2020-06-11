#include "system/system.h"

#include "system/systick.h"
#include "device_driver/led.h"
#include "device_driver/alternate_motor.h"
#include "device_driver/MT9V034.h"
#include "device_driver/st7735r.h"
#include "device_driver/servo.h"


int main() {


    
    
    System::InitSystem();

    DeviceDriver::AlternateMotor::Config motorA_config;
    motorA_config.id = 0;
    DeviceDriver::AlternateMotor motorA(motorA_config);

    motorA.SetSpeed(280);

    // DeviceDriver::AlternateMotor::Config motorB_config;   
    // motorB_config.id = 1;
    // DeviceDriver::AlternateMotor motorB(motorB_config);

    // motorB.SetSpeed(0);

    DeviceDriver::Led::Config led_config;
    led_config.id = 0;
    led_config.is_enable = true;
    led_config.is_active_low = true;
    DeviceDriver::Led led(led_config);
    
    int degree = 1050;
    DeviceDriver::Servo::Config servo_config;
    servo_config.id = 0;
    servo_config.frequency = 50;
	servo_config.min_pos_width = 1000;
	servo_config.max_pos_width = 2000;
    DeviceDriver::Servo servo(servo_config);
    servo.SetDegree(degree);

    // servo.SetDegree(700); // center: 1100 // negative: turn right



    DeviceDriver::MT9V034::Config camera_config;
    camera_config.id = 0;
    camera_config.width = 160;
    camera_config.height = 120;
    DeviceDriver::MT9V034 camera(camera_config);
    

    DeviceDriver::St7735r::Config lcd_config;
    lcd_config.id = 0;
    lcd_config.orientation = 3;
    DeviceDriver::St7735r lcd(lcd_config);

    uint32_t previousTime = System::Systick::GetTimeInMS();
    uint32_t initTime = System::Systick::GetTimeInMS();
    
    
    const int height = 120;
    const int width = 160;
    int leftBorder = 0;
    int rightBorder = width-1;
    int pivot = height*width-(width/2);
    bool checked = false;
    bool inited = false;
    const uint8_t* buffer = nullptr;
    // uint8_t filter[]
    // bool dir = false;

    // motorA.SetDirection(dir);
    // motorB.SetDirection(dir);
    camera.Start();
    while(1){
        // System::Systick::DelayMS(100);
        if (System::Systick::GetTimeInMS() - previousTime > 200){
            previousTime = System::Systick::GetTimeInMS();
            led.Toggle();
            
            // if(degree > 700 && degree <= 1600)
            //     degree -= 50;
            // servo.SetDegree(degree);
            // dir = !dir;
            // motorA.SetDirection(dir);
            // motorB.SetDirection(dir);
            
        }

        if(System::Systick::GetTimeInMS() > 2000)
            inited = true;


        

        if(camera.IsNewFrame()){
			buffer = camera.LockBuffer();
            // lcd.SetRegion(DeviceDriver::Lcd::Rect(0, 0, 160, 120));
			// lcd.FillGrayscalePixel(buffer,camera_config.width*camera_config.height);

            // leftBorder = height*width-width;

            
            //Last row
            for(int i = height*width-width; i < height*width; ++i){

                    
                if(buffer[i] <= 120){
                    leftBorder = i%width;
                    break;
                }

            }

            //First row
            // for(int i = 0; i < width; ++i){

                    
            //     if(buffer[i] <= 50){
            //         leftBorder = i%width;
            //         break;
            //     }

            // }


            // if(leftBorder == 0)
            //     servo.SetDegree(1300);
            //     else
            //     servo.SetDegree(1030);

            // leftBorder = 119;

            for(int i = height*width-1; i >= height*width-width; --i){

                    
                if(buffer[i] <= 120){
                    rightBorder = i%width;
                    break;
                }
            }

            for(int i = height*width-width/2; i > height*width/2; i-=width){

                    
                if(buffer[i] <= 120 && inited){
                    checked = true;
                    motorA.SetSpeed(200);
                    break;
                }
            }



            

            // if(rightBorder == 119)
            //     servo.SetDegree(1300);
            //     else
            //     {
            //         servo.SetDegree(1050);
            //     }
                
            // rightBorder = 0;
            // }
            // if(leftBorder < width/2)
            //     servo.SetDegree(1300);
            //     else
            //     {
            //         servo.SetDegree(1030);
            //     }
                

            // if(leftBorder < width/2 && rightBorder >= width/2){
            
            //     servo.SetDegree(1050);

            // }
            // if(leftBorder == 0 && rightBorder == width-1)
            //     motorA.SetSpeed(0);
            // if(checked)
            //     motorA.SetSpeed(200);
            //     else
            //     motorA.SetSpeed(200);

            // if(leftBorder==0 && rightBorder == width - 1)
            //     checked = true;
            // if(buffer[width*height/3-1+(width/2)] < 120 && inited){
            //     checked = true;
            //     motorA.SetSpeed(200);
            // }
                

            // if(leftBorder == width-1-rightBorder){
            //     servo.SetDegree(1050);
            //     // motorA.SetSpeed(300);
            //     // checked = false;
            // }
                if(leftBorder < width/2 && rightBorder >= width/2)
                        servo.SetDegree(1050);

                    else if(leftBorder < width/2 && rightBorder < width/2){
                            
                            if(checked)
                                servo.SetDegree(800);
                                else
                                servo.SetDegree(975);
                      
                            // motorA.SetSpeed(200);
                        }
                        else if(leftBorder >= width/2 && rightBorder >= width/2){
                            
                            if(checked)
                                servo.SetDegree(1300);
                                else
                                servo.SetDegree(1150);
                            
                                // motorA.SetSpeed(200);
                            }

            leftBorder = 0;
            rightBorder = width-1;
            // checked = false;

                // if(buffer[width/2] <= 50){
                //     servo.SetDegree(800);
                //     // break;
                // }
                //     else
                //     {
                //         servo.SetDegree(1300);
                //         // break;
                //     }
                    

            


		 }

            
    }
}