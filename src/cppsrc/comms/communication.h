#include "send.cc"
#include <mutex>
#include <array>

constexpr int gpio_ports[8] = {4, 17, 27, 22, 18, 23, 24, 25}; 

namespace comms {

class HardwareConnection {
public:
  HardwareConnection(); // Open the pins for communication
  ~HardwareConnection(); 
  std::mutex& get_mutex();
  void send_message(uint8_t msg);
private:
  std::mutex lock;
  std::array<pin_t, 8> out_pins;
};

}
