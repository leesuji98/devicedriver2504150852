#include "device_driver.h"

DeviceDriver::DeviceDriver(FlashMemoryDevice* hardware) : m_hardware(hardware)
{
}

int DeviceDriver::read(long address)
{
    m_hardware->read(address);
    m_hardware->read(address);
    m_hardware->read(address);
    m_hardware->read(address);
    return m_hardware->read(address);
}

void DeviceDriver::write(long address, int data)
{
    // TODO: implement this method
    m_hardware->write(address, (unsigned char)data);
}