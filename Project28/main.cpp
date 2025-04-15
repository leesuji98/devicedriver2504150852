#include <stdexcept>
#include <string>

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "device_driver.h"

using std::string;

class MockFlashMemory : public FlashMemoryDevice {
public:
	MOCK_METHOD(unsigned char, read, (long address), (override));
	MOCK_METHOD(void, write, (long address, unsigned char data), (override));
};

TEST(DeviceDriver, ReadFromHW) {
	MockFlashMemory mock;
	EXPECT_CALL(mock, read(0xBB)).Times(5);

	DeviceDriver driver{ &mock };

	try {
		driver.read(0xBB);
	}
	catch (std::runtime_error& e) {
		EXPECT_EQ(string{ e.what() }, string{ "Read data error" });
	}
}
/*
TEST(DeviceDriver, WritetoHW) {
	MockFlashMemory mock;
	DeviceDriver driver{ &mock };
	driver.write(0xFF, 0x00);

	EXPECT_EQ(0, driver.read(0xFF));
}*/

int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}