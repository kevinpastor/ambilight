#include <CppUnitTest.h>

#include "Pixel.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
	TEST_CLASS(PixelTests)
	{
	public:
		TEST_METHOD(ShouldMixBlackAndWhite)
		{
			Pixel black({ 0, 0, 0 });
			Pixel white({ 255, 255, 255 });

			Pixel mix = Pixel::mix(black, white, 0.5);

			Assert::AreEqual(static_cast<unsigned char>(127), mix.red);
			Assert::AreEqual(static_cast<unsigned char>(127), mix.green);
			Assert::AreEqual(static_cast<unsigned char>(127), mix.blue);
		}

	};
}
