#include "system.h"
#include "altera_up_avalon_accelerometer_spi.h"
#include "altera_avalon_timer_regs.h"
#include "altera_avalon_timer.h"
#include "altera_avalon_pio_regs.h"
#include "sys/alt_irq.h"
#include <stdlib.h>

#define OFFSET -32
#define PWM_PERIOD 16

alt_8 pwm = 0;
alt_u8 led;
int level;
int TAPS = 128;

void led_write(alt_u8 led_pattern) {
    IOWR(LED_BASE, 0, led_pattern);
}

void convert_read(alt_32 acc_read, int * level, alt_u8 * led) {
    acc_read += OFFSET;
    alt_u8 val = (acc_read >> 6) & 0x07;
    * led = (8 >> val) | (8 << (8 - val));
    * level = (acc_read >> 1) & 0x1f;
}

void sys_timer_isr() {
    IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_BASE, 0);

    if (pwm < abs(level)) {

        if (level < 0) {
            led_write(led << 1);
        } else {
            led_write(led >> 1);
        }

    } else {
        led_write(led);
    }

    if (pwm > PWM_PERIOD) {
        pwm = 0;
    } else {
        pwm++;
    }

}

void timer_init(void * isr) {

    IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_BASE, 0x0003);
    IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_BASE, 0);
    IOWR_ALTERA_AVALON_TIMER_PERIODL(TIMER_BASE, 0x0900);
    IOWR_ALTERA_AVALON_TIMER_PERIODH(TIMER_BASE, 0x0000);
    alt_irq_register(TIMER_IRQ, 0, isr);
    IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_BASE, 0x0007);

}

alt_32 LPF(alt_32 acc_read [TAPS], alt_32 coeffs [TAPS]){
	alt_32 avg = 0;
	for (int i = 0; i < TAPS; i++){
		avg += acc_read[i]*coeffs[i];
	}
	//alt_printf("average data: %f\n", sum);
	return avg >> 7;
}

int main() {
	int i = 0;
    alt_32 x_read [TAPS];
    alt_32 coeffs [TAPS];

    for (int i = 0; i<TAPS; i++){
    	coeffs[i] = 1;
    	x_read[i] = 0;
    }

    alt_up_accelerometer_spi_dev * acc_dev;
    acc_dev = alt_up_accelerometer_spi_open_dev("/dev/accelerometer_spi");
    if (acc_dev == NULL) { // if return 1, check if the spi ip name is "accelerometer_spi"
        return 1;
    }

    timer_init(sys_timer_isr);
    while (1) {
    	if (i == TAPS){
    		i = 0;
    	}
        alt_up_accelerometer_spi_read_x_axis(acc_dev, & x_read[i]);
        alt_32 avg = LPF(x_read, coeffs);

         //alt_printf("raw data: %x\naverage data: %d\n", x_read, temp);
        convert_read(avg, & level, & led);
        i++;
    }

    return 0;
}





//////////////////////////////////////
#include "system.h"
#include "altera_up_avalon_accelerometer_spi.h"
#include "altera_avalon_timer_regs.h"
#include "altera_avalon_timer.h"
#include "altera_avalon_pio_regs.h"
#include "sys/alt_irq.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sys/alt_stdio.h"  // Include Altera HAL for UART functions

#define OFFSET -32
#define PWM_PERIOD 16

#define CHARLIM 256    // Maximum character length of what the user places in memory.  Increase to allow longer sequences
#define QUITLETTER '~' // Letter to kill all processing

alt_8 pwm = 0;
alt_u8 led;
int level;
int TAPS = 128;

void led_write(alt_u8 led_pattern) {
    IOWR(LED_BASE, 0, led_pattern);
}

void convert_read(alt_32 acc_read, int * level, alt_u8 * led) {
    acc_read += OFFSET;
    alt_u8 val = (acc_read >> 6) & 0x07;
    * led = (8 >> val) | (8 << (8 - val));
    * level = (acc_read >> 1) & 0x1f;
}

void sys_timer_isr() {
    IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_BASE, 0);

    if (pwm < abs(level)) {

        if (level < 0) {
            led_write(led << 1);
        } else {
            led_write(led >> 1);
        }

    } else {
        led_write(led);
    }

    if (pwm > PWM_PERIOD) {
        pwm = 0;
    } else {
        pwm++;
    }

}

void timer_init(void * isr) {

    IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_BASE, 0x0003);
    IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_BASE, 0);
    IOWR_ALTERA_AVALON_TIMER_PERIODL(TIMER_BASE, 0x0900);
    IOWR_ALTERA_AVALON_TIMER_PERIODH(TIMER_BASE, 0x0000);
    alt_irq_register(TIMER_IRQ, 0, isr);
    IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_BASE, 0x0007);

}

alt_32 applyFilter(alt_32 new_reading, alt_32 filter[TAPS], alt_32 coeff[TAPS]){
	alt_32 filtered_reading = 0;

	for(int i = TAPS - 1; i > 0; i--){
		filter[i] =  filter[i-1];
	}

	filter[0] = new_reading;

	for(int i = 0; i < TAPS; i++){
		filtered_reading += filter[i] * coeff[i];
	}
	return filtered_reading >> 7;
}

int main() {
	printf("hello world \n");
	int i = 0;
    alt_32 x_read [TAPS];
    alt_32 y_read [TAPS];
    alt_32 coeff[TAPS];
    alt_32 val;

    for(int i = 0; i < TAPS; i++){
    	x_read[i] = 0;
    	y_read[i] = 0;
    	coeff[i] = 1;
    }


    alt_up_accelerometer_spi_dev * acc_dev;
    acc_dev = alt_up_accelerometer_spi_open_dev("/dev/accelerometer_spi");
    if (acc_dev == NULL) { // if return 1, check if the spi ip name is "accelerometer_spi"
        return 1;
    }

    timer_init(sys_timer_isr);
    while (1) {

    	if (i == TAPS){
    		i = 0;
    	}
        alt_up_accelerometer_spi_read_x_axis(acc_dev, & val);
        alt_32 avg_x = applyFilter(val, x_read, coeff);
        alt_up_accelerometer_spi_read_y_axis(acc_dev, & val);
        alt_32 avg_y = applyFilter(val, y_read, coeff);

        alt_printf("x-axis reading: %x ", avg_x);
        alt_printf("y-axis reading: %x\n", avg_y);
        convert_read(avg_x, & level, & led);
        i++;
    }

    return 0;
}
