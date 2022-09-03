#include "mbed.h"
#include "PS3.h"
#include "QEI.h"

#define ADDRESS 0x30

I2C i2c(D14,D15);
PS3 ps3 (A0,A1);
DigitalOut sig(D13);

QEI encoder( D5, D4, NC, 2048, QEI::X2_ENCODING);
Ticker flip;
//QEI 任意の名前( A相のピン, B相のピン, Z相のピン, 分解能, 逓倍);

int pulse;
double rpm;
double angle;
char date;

void get_rpm();
void get_angle();
void send(char add,char data);

int main(void){

    date = 0x80;

    encoder.reset();
    flip.attach(&get_rpm,50ms);

    while (true) {

        printf("%5.2lf[rpm] pulse:%6.0d MotorDate:%d\n",rpm,pulse,date);

        if(ps3.getSELECTState()){
            date = 0x80;
        }

        if(ps3.getButtonState(PS3::ue) && date>=0 && date<255){
            date += 1;
            wait_us(100000);
        }
        if(ps3.getButtonState(PS3::sita) && date>0 && date<=255){
            date -= 1;
            wait_us(100000);
        }


        if (ps3.getButtonState(PS3::maru)){
            send( ADDRESS, date);
        }else{
            send( ADDRESS, 0x80);
        }

    }
}

void get_rpm(){
    pulse = encoder.getPulses();
    encoder.reset();
    rpm = (60*20*(double)pulse) / (2048*2) ;
    
}

void get_angle(){
    pulse = encoder.getPulses();
    angle = (360*(double)pulse)/(2048*2);
}

void send(char address, char data){
    wait_us(15000);
    i2c.start();
    i2c.write(address);
    i2c.write(data);
    i2c.stop();
}

