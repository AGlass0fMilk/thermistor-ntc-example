/*
 * main.cpp
 *
 *  Created on: Feb 16, 2020
 *      Author: gdbeckstein
 */

#include "ThermistorNTC.h"
#include "mbed_wait_api.h"
#include "ge1923.h"
#include "ValueMapping.h"
#include "Si7021.h"

#define NTC_ADC_PIN A0

ep::LinearlyInterpolatedValueMapping ge1923_map(
        mbed::make_const_Span(ge1923::calibration_table));

ep::ThermistorNTC ntc(NTC_ADC_PIN, 10000.0f, &ge1923_map);

mbed::I2C sensor_i2c(I2C_SDA, I2C_SCL);
Si7021 si_temp_sensor(sensor_i2c);

int main(void) {
    sensor_i2c.frequency(100000);
    bool success = si_temp_sensor.check();
    printf("si7021: found? %s\r\n", success? "yes" : "no");
    while(true) {
        success = si_temp_sensor.measure();
        if(!success) {
            printf("failed to read si sensor\r\n");
        }
        printf("ntc: %.2fC | si: %.2fC\r\n",
                ntc.get_temperature(),
                (si_temp_sensor.get_temperature()/1000.0f));
        wait_us(1000000);
    }
}


