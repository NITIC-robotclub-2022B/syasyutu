#include "mbed.h"
#include "PS3.h"
#include "mbed_wait_api.h"

#define ADDRESS 0x30

I2C i2c(D14,D15);
PS3 ps3 (A0,A1);
DigitalOut sig(D13);

void send(char add,char data);

char date = 0x90;

int main(){
    while (true) {
        if(ps3.getButtonState(PS3::maru)){
            send(ADDRESS, 0xff);
        }else if(ps3.getButtonState(PS3::sankaku)){
            send(ADDRESS, 0xcc);
        }
    }
}

void send(char address, char data){
    wait_us(15000);
    i2c.start();
    i2c.write(address);
    i2c.write(data);
    i2c.stop();
}
