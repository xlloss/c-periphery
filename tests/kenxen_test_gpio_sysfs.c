/*
 * c-periphery
 * https://github.com/vsergeev/c-periphery
 * License: MIT
 */

#include "test.h"

#include <stdlib.h>
#include <errno.h>

#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#include "../src/gpio.h"

#define GPIO(n) n
#define GPIO_TEST_N 23 - 2
#define TEST_LOOP 2

unsigned int pin_input;
unsigned int pin_output;

unsigned int pin_class1[GPIO_TEST_N] = {
    GPIO(1),
    GPIO(3),
    GPIO(5),
    GPIO(12),
    GPIO(15),
    GPIO(17),
    GPIO(33),
    GPIO(50),
    GPIO(52),
    GPIO(54),
    GPIO(56),
    GPIO(58),
    GPIO(61),
    /* GPIO(71),//CAMERA_EN */
    GPIO(73),
    GPIO(75),
    GPIO(77),
    GPIO(79),
    GPIO(81),
    /* GPIO(83),//IDC1_I2C_SCL_EEPROM */
    GPIO(87),
    GPIO(89),
    GPIO(91)
};

unsigned int pin_class2[GPIO_TEST_N] = {
    GPIO(1) + 1,
    GPIO(3) + 1,
    GPIO(5) + 1,
    GPIO(12) + 1,
    GPIO(15) + 1,
    GPIO(17) + 1,
    GPIO(33) + 1,
    GPIO(50) + 1,
    GPIO(52) + 1,
    GPIO(54) + 1,
    GPIO(56) + 1,
    GPIO(58) + 1,
    GPIO(61) + 1,
    /* GPIO(71) + 1,//CAMERA_RST */
    GPIO(73) + 1,
    GPIO(75) + 1,
    GPIO(77) + 1,
    GPIO(79) + 1,
    GPIO(81) + 1,
    /* GPIO(83) + 1,//IDC1_I2C_SDA_EEPROM */
    GPIO(87) + 1,
    GPIO(89) + 1,
    GPIO(60)
};

void test_loopback(void) {
    gpio_t *gpio_in, *gpio_out;
    bool value;

    /* Allocate GPIO */
    gpio_in = gpio_new();
    gpio_passert("Allocate GPIO IN:", gpio_in != NULL);
    gpio_out = gpio_new();
    gpio_passert("Allocate GPIO OUT:", gpio_out != NULL);

    /* Open in and out pins */
    gpio_passert("Open GPIO IN:",
        gpio_open_sysfs(gpio_in, pin_input, GPIO_DIR_IN) == 0);
    gpio_passert("Open GPIO OUT:",
        gpio_open_sysfs(gpio_out, pin_output, GPIO_DIR_OUT) == 0);

    /* Drive out low, check in low */
    gpio_write(gpio_out, false);
    gpio_read(gpio_in, &value);
    gpio_passert("Drive out low check:", value == false);

    /* Drive out high, check in high */
    gpio_write(gpio_out, true);
    gpio_read(gpio_in, &value);
    gpio_passert("Drive out high check:", value == true);

    /* Free GPIO */
    gpio_free(gpio_in);
    gpio_free(gpio_out);
}

int main(int argc, char *argv[]) {

    int i, j;
    unsigned int *input_pin_class;
    unsigned int *output_pin_class;

    for (j = 0; j < TEST_LOOP; j++) {
        for (i = 0; i < GPIO_TEST_N; i++) {
            if (!j) {
                pin_input = pin_class1[i];
                pin_output = pin_class2[i];
                input_pin_class = pin_class1;
                output_pin_class = pin_class2;
            } else {
                pin_input = pin_class2[i];
                pin_output = pin_class1[i];
                input_pin_class = pin_class2;
                output_pin_class = pin_class1;
            }

            printf("=======================================\n");
            printf("Test: Pin Input  : %d\n", input_pin_class[i]);
            printf("Test: Pin Output : %d\n", output_pin_class[i]);
            printf("=======================================\n");
            test_loopback();
            printf("\n\n");
        }

        if (j == 0) {
            printf("=======================================\n");
            printf("GPIO Role Switch TEST\n");
            printf("=======================================\n");
            printf("\n");
        } else {
            printf("All tests passed!" STR_OK);
        }
    }

    return 0;
}

