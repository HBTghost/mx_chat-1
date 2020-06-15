#include "pch.h"
#include "CppUnitTest.h"
#include "../ServerCLI/DataHelper.h"

#include "../ServerCLI/SDataPacket.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ServerTestCLI
{
	TEST_CLASS(TEST_DataHelper)
	{
	public:
		TEST_METHOD(TEST_CONVERT_UINT32_BIT)
		{
			unsigned char temp[4]{ 0 };
			uint32_t goc = 9876;
			DataHelper::ConvertUint32Bit(temp, goc);
			uint32_t revert = DataHelper::ConvertBitUint32(temp);
			Assert::AreEqual(goc, revert);
		}
		TEST_METHOD(TEST_CONVERT_INT32_BIT)
		{
			char temp[4]{ 0 };
			int32_t goc = -9876;
			DataHelper::ConvertInt32Bit(temp, goc);
			int32_t revert = DataHelper::ConvertBitInt32(temp, 0);
			Assert::AreEqual(goc, revert);
		}
		TEST_METHOD(TEST_CONVERT_STRING_BIT)
		{
			char origin[13]{ 0x6c, 0xd9, 0xff, 0xff, 0x63, 0x68, 0x61, 0x6f, 0x20 , 0x62, 0x61, 0x6e , 0x6e };

			string text = DataHelper::ConvertBitString(origin, 13, 4, '\0');

			string expected = "chao bann";
			Assert::AreEqual(expected, text);
		}



	};

	TEST_CLASS(TEST_SDataPacket)
	{
	public:
		TEST_METHOD(TEST_BUILD_REVERT_PACKET)
		{
			SDataPackage* sdata = (new SDataPackage())
				->SetHeaderCommand(EMessageCommand::CLIENT_SIGN_IN)
				->SetHeaderDesSrc("src", "des")
				->SetHeaderNumPackage(2)
				->SetHeaderTotalSize(4096);
			sdata->_data_items.push_back("abc");
			sdata->_data_items.push_back("123");
			sdata->_data_items.push_back("xyz");

			char* data = sdata->BuildMessage();


			SDataPackage revert(data);
			char* revert_data = revert.BuildMessage(); 
			bool validation = true;
			for (int i = 0; i < 4096; i++) {
				if (data[i] != revert_data[i]) {
					char d_d = data[i];
					char rd_d = revert_data[i];
					validation = false;
					break;
				}
			}
			
			Assert::IsTrue(validation);
		}
	};
}
