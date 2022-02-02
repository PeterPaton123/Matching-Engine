// Compile with $ gcc water.c -o water
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Pin modes:
#define INPUT (0)
#define OUTPUT (1)
#define LOW (0)
#define HIGH (1)

typedef struct {
        int     pin;
        char*   fn;
} pin_t;

static pin_t pinopen(int pin, int mode);
static void pinclose(pin_t pin);
static void pinwrite(pin_t pin, int value);
static int pinread(pin_t pin);

int main(int argc, char** argv)
{
        int time = 10;

        if (argc > 1) {
          time = atoi(argv[1]);
        }

	/*
	int gpios[] = {5, 6, 12, 13, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27};

	
        pin_t gpio04 = pinopen(4, OUTPUT); 
        pin_t gpio17 = pinopen(17, OUTPUT);
        pin_t gpio27 = pinopen(27, OUTPUT);
        pin_t gpio22 = pinopen(22, OUTPUT);
        pin_t gpio18 = pinopen(18, OUTPUT);
        pin_t gpio23 = pinopen(23, OUTPUT);
        pin_t gpio24 = pinopen(24, OUTPUT);
        pin_t gpio25 = pinopen(25, OUTPUT);
        
        pin_t pins[8] = {gpio04, gpio17, gpio27, gpio22, gpio18, gpio23, gpio24, gpio25};
 
        for (int i = 0; i < 8; ++i) {
            pinwrite(pins[i], HIGH);
        }

        sleep(time);

        for (int i = 0; i < 8; ++i) {
            pinwrite(pins[i], LOW);
        }

	for(int i = 0; i < 16; ++i) {
		pin_t gpio = pinopen(gpios[i], OUTPUT);
		printf("write to %d\n", gpios[i]);
		pinwrite(gpio, LOW);
	}

	sleep(10);
	printf("going low\n");

	for(int i = 0; i < 16; ++i) {
		pin_t gpio = pinopen(gpios[i], OUTPUT);
		pinwrite(gpio, HIGH);
	}


	sleep(10);
	*/

	pin_t pin1  = pinopen(17, OUTPUT);
	pin_t pin2  = pinopen(4, OUTPUT);
	pin_t pin3  = pinopen(27, OUTPUT);
	pin_t pin4  = pinopen(24, OUTPUT);
	pin_t pin7  = pinopen(18, OUTPUT);
	pin_t pin8  = pinopen(25, OUTPUT);
	pin_t pin9  = pinopen(22, OUTPUT);
	pin_t pin10 = pinopen(23, OUTPUT);

	pin_t pins[8] = {pin1, pin2, pin3, pin4, pin7, pin8, pin9, pin10};

	for (int i = 0; i < 256; ++i) {
		for (int j = 0; j < 8; ++j) {
			pinwrite(pins[j], (((i >> j) % 2) == 1) ? HIGH : LOW);
		}
		sleep(1);
	}
	return 0;
}

pin_t pinopen(int pin, int mode)
{
        char*   pinfn = malloc(1024);
        char    dirfn[1024];
        FILE*   dir = NULL;
        FILE*   fp = fopen("/sys/class/gpio/export", "w");
        fprintf(fp, "%d", pin);
        fclose(fp);
        snprintf(dirfn, 1024, "/sys/class/gpio/gpio%d/direction", pin);
        snprintf(pinfn, 1024, "/sys/class/gpio/gpio%d/value", pin);
        while (dir == NULL) {
                dir = fopen(dirfn, "w");
        }
        if (mode == INPUT) {
                fprintf(dir, "in");
        } else {
                fprintf(dir, "out");
        }
        fclose(dir);
        return (pin_t) { pin, pinfn };
}

void pinclose(pin_t pin)
{
        FILE*   fp = fopen("/sys/class/gpio/unexport", "w");
        fprintf(fp, "%d", pin.pin);
        fclose(fp);
        free(pin.fn);
}

void pinwrite(pin_t pin, int value)
{
        FILE*   fp = fopen(pin.fn, "w");
        if (value == LOW) {
                fprintf(fp, "0");
        } else {
                fprintf(fp, "1");
        }
        fclose(fp);
}

int pinread(pin_t pin)
{
        char    buf[2];
        FILE*   fp = fopen(pin.fn, "r");
        size_t  read = fread(buf, 1, 2, fp);
        fclose(fp);
        if (read != 2) {
                return -1;
        } else {
                return (buf[0] == '1') ? HIGH : LOW;
        }
}
