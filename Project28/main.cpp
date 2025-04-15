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

class DriverTestFixture : public testing::Test {
public:
	MockFlashMemory mock;
};

TEST_F(DriverTestFixture, ReadFromHW) {
	EXPECT_CALL(mock, read(0xBB)).Times(5);
	DeviceDriver driver{ &mock };

	try {
		driver.read(0xBB);
	}
	catch (std::runtime_error& e) {
		EXPECT_EQ(string{ e.what() }, string{ "Read data error" });
	}
}

TEST_F(DriverTestFixture, WritetoHW) {
	DeviceDriver driver{ &mock };
	EXPECT_CALL(mock, read(0xBB)).WillRepeatedly(testing::Return(0xFF));
	EXPECT_CALL(mock, write(0xBB, 0x00)).Times(1);

	try {
		driver.write(0xBB, 0x00);
	}
	catch (std::runtime_error& e) {
		EXPECT_EQ(string{ e.what() }, string{ "data not clear" });
	}
}

int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}