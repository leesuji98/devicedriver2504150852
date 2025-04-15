#include <stdexcept>
#include "device_driver.h"

DeviceDriver::DeviceDriver(FlashMemoryDevice* hardware) : m_hardware(hardware)
{
}

int DeviceDriver::read(long address)
{
    int savedData = m_hardware->read(address);
    for (int i = 0; i < 4; i++) {
        int temp = m_hardware->read(address);
        if (savedData != temp)
            throw std::runtime_error("Read data error");
        savedData = temp;
    }
    return savedData;
}

void DeviceDriver::write(long address, int data)
{
    // TODO: implement this method
    m_hardware->write(address, (unsigned char)data);
}