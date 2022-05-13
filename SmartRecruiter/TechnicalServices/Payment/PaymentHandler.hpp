#include <vector>
#include <string>

namespace TechnicalServices::Payment
{
	// Logging Package within the Technical Services Layer Abstract class
	class PaymentHandler
	{
		public:
			static bool CheckCreditCardType(std::string ccType);
			static bool CheckCreditCardNumber(std::string ccNumber);
	};
}
