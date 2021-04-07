#include "mbed.h"
#include "uLCD_4DGL.h"

int j = 0, i = 0, l;

using namespace std::chrono;

uLCD_4DGL uLCD(D1, D0, D2);
AnalogOut  aout(PA_4);
AnalogIn Ain(A0);

Thread t;
Thread thread;
Timer debounce;      

InterruptIn button1(D9); 
InterruptIn button2(D6);
InterruptIn button3(D5);

InterruptIn sw2(USER_BUTTON);
EventQueue queue(32 * EVENTS_EVENT_SIZE);

int sample = 128;
float ADCdata[128];

void toggle1()
{
   if (duration_cast<milliseconds>(debounce.elapsed_time()).count() > 200) {
       if (j == 0) {
            if (i == 4) {
                i = i;
            } else {
                i = i + 1;
            }
       } else {
           i = i;
       }
        debounce.reset(); 
   }
}

void toggle2()
{
   if (duration_cast<milliseconds>(debounce.elapsed_time()).count() > 200) {
       if (j == 0) {
            if (i == 1) {
                i = i;
            } else {
                i = i - 1;
            }
       } else {
           i = i;
       }
        debounce.reset(); 
   }
}

void toggle3()
{
   if (duration_cast<milliseconds>(debounce.elapsed_time()).count() > 200) {
        if (j == 0) {
            j = 1;
        } else {
            j = 0;
        }
        debounce.reset(); 
   }
}

void uLCD_thread() {
    uLCD.printf("menu\n"); //Default Green on black text
     uLCD.printf("current selection\n");
    while (1) {    
        uLCD.locate(0,3);
        if (i == 1) {
            uLCD.printf("1/8");
        } else if (i == 2) {
            uLCD.printf("1/4");
        } else if (i == 3) {
            uLCD.printf("1/2");
        } else if (i == 4) {
            uLCD.printf("1  ");
        } else {
            uLCD.printf("no");
        }
        if (j == 1) {
            uLCD.locate(0,5);
            uLCD.printf("confirm selection:\n");
            if (i == 1) {
                uLCD.printf("1/8");
            } else if (i == 2) {
                uLCD.printf("1/4");
            } else if (i == 3) {
                uLCD.printf("1/2");
            } else if (i == 4) {
                uLCD.printf("1  ");
            } else {
                uLCD.printf("no");
            }
        }
    }
}

void wave_info() {
   for (int l = 0; l < sample; l++){
            aout = Ain;
            ADCdata[l] = Ain;
            ThisThread::sleep_for(1000ms/sample);
        }
    for ( l = 0; l < sample; l++) {
        printf("%f\r\n", ADCdata[l]);
        ThisThread::sleep_for(100ms);
    }
}

void Trig_wave()  {
   queue.call(wave_info);
}

int main() {
    thread.start(uLCD_thread);

    t.start(callback(&queue, &EventQueue::dispatch_forever));
    sw2.rise(Trig_wave);

    debounce.start();
    button1.rise(&toggle1); // attach the address of the toggle
    button2.rise(&toggle2);
    button3.rise(&toggle3);

    while (1) {
        while (j == 1) {
            if (i == 4) {
                for (float k = 0.0; k < 10.0/11.0; k += 10.0/11/50) {
                    aout = k;
                //ThisThread::sleep_for(1ms);
                    wait_us(1200);
                }
                ThisThread::sleep_for(80ms);
                for (float h = 10.0/11; h >0; h -= 10.0/11/50) {
                    aout = h;
                //ThisThread::sleep_for(1ms);
                    wait_us(1200);
                }
            } else if (i == 3) {
                for (float k = 0.0; k < 10.0/11.0; k += 10.0/11/50) {
                    aout = k;
                //ThisThread::sleep_for(1ms);
                    wait_us(550);
                }
                ThisThread::sleep_for(160ms);
                for (float h = 10.0/11; h >0; h -= 10.0/11/50) {
                    aout = h;
                //ThisThread::sleep_for(1ms);
                    wait_us(550);
                }
            } else if (i == 2) {
                for (float k = 0.0; k < 10.0/11.0; k += 10.0/11/50) {
                    aout = k;
                //ThisThread::sleep_for(1ms);
                    wait_us(300);
                }
                ThisThread::sleep_for(200ms);
                for (float h = 10.0/11; h >0; h -= 10.0/11/50) {
                    aout = h;
                //ThisThread::sleep_for(1ms);
                    wait_us(300);
                }
            } else if (i = 1) {
                for (float k = 0.0; k < 10.0/11.0; k += 10.0/11/50) {
                    aout = k;
                //ThisThread::sleep_for(1ms);
                    wait_us(150);
                }
                ThisThread::sleep_for(220ms);
                for (float h = 10.0/11; h >0; h -= 10.0/11/50) {
                    aout = h;
                //ThisThread::sleep_for(1ms);
                    wait_us(150);
                }
            } else {
                aout = 0;
            }
        }
        ThisThread::sleep_for(100ms);
    }
}