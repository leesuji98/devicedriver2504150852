#include <iostream>
#include <vector>
#include "device_driver.h"

using std::vector;

class Application {
public:
	Application(DeviceDriver* driver) : driver(driver) {}
	vector<int> readAndPrint(long startAddr, long endAddr) {
		vector <int> ret;
		for (long i = startAddr; i <= endAddr; i++) {
			int temp = driver->read(i);
			std::cout << temp << std::endl;
			ret.push_back(temp);
		}
		return ret;
	}

	void writeAll(int value) {
		for (int i = 0; i < 5; i++)
			driver->write((long)i, value);
	}

private:
	DeviceDriver * driver;
};