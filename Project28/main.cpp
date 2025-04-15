#include <stdexcept>
#include <string>
#include <vector>

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "device_driver.h"
#include "application.cpp"

using std::string;
using std::vector;

class MockFlashMemory : public FlashMemoryDevice {
public:
	MOCK_METHOD(unsigned char, read, (long address), (override));
	MOCK_METHOD(void, write, (long address, unsigned char data), (override));
};

class DriverTestFixture : public testing::Test {
public:
	testing::NiceMock<MockFlashMemory> mock;
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

TEST_F(DriverTestFixture, AppReadTest) {
	EXPECT_CALL(mock, read(0x00)).WillRepeatedly(testing::Return(0x10));
	EXPECT_CALL(mock, read(0x01)).WillRepeatedly(testing::Return(0x20));
	EXPECT_CALL(mock, read(0x02)).WillRepeatedly(testing::Return(0x30));
	EXPECT_CALL(mock, read(0x03)).WillRepeatedly(testing::Return(0x40));
	EXPECT_CALL(mock, read(0x04)).WillRepeatedly(testing::Return(0x50));
	
	DeviceDriver driver{ &mock };
	Application app{ &driver };
	vector <int> actual = app.readAndPrint(0x00, 0x04);
	vector <int> expected = { 0x10, 0x20, 0x30, 0x40, 0x50 };
	
	EXPECT_EQ(expected, actual);
}

TEST_F(DriverTestFixture, AppWriteTest) {
	DeviceDriver driver{ &mock };
	EXPECT_CALL(mock, read(0x00)).WillRepeatedly(testing::Return(0xFF));
	EXPECT_CALL(mock, read(0x01)).WillRepeatedly(testing::Return(0xFF));
	EXPECT_CALL(mock, read(0x02)).WillRepeatedly(testing::Return(0xFF));
	EXPECT_CALL(mock, read(0x03)).WillRepeatedly(testing::Return(0xFF));
	EXPECT_CALL(mock, read(0x04)).WillRepeatedly(testing::Return(0xFF));

	Application app { &driver };
	app.writeAll(0x00);
}

int main() {


	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}