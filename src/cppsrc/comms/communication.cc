#include "communication.h"

HardwareConnection::HardwareConnection() {
    for (int i = 0; i < 8; ++i) {
        pins[i] = pinopen(gpio_ports[i], OUTPUT);
    }
}

HardwareConnection::~HardwareConnection() {}

std::mutex& HardwareConnection::get_mutex() {
    return lock;
}

void HardwareConnection::send_message(uint8_t msg) {
    for (int i = 0; i < 8; ++i) {
        pinwrite(pins[i], (((msg >> i) & 2) == 1) ? HIGH : LOW);
    }
}