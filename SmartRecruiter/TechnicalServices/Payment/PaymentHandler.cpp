#include <iostream>			// A C++ Standard Library header.		
#include <algorithm>
#include "TechnicalServices/Payment/PaymentHandler.hpp"

namespace TechnicalServices::Payment
{
	static std::string credidCardTypeArray[] = { "visa", "mastercard", "paypal", "debit" };
	static std::vector<std::string> creditCardVector = { "1234567890",
						 "2345678901",
						 "1234567890" };
        int const NO_CREDIT_CARD_TYPE = 4;

	bool PaymentHandler::CheckCreditCardType(std::string ccType)
	{
		transform(ccType.begin(), ccType.end(), ccType.begin(), ::tolower);

		for (int i = 0; i < NO_CREDIT_CARD_TYPE; i++)
		{
			if (ccType.compare(credidCardTypeArray[i]) == 0) {
				return true;
			}
		}
		return false;
	}

	bool PaymentHandler::CheckCreditCardNumber(std::string ccNumber)
	{
		for (std::vector<std::string>::iterator it = creditCardVector.begin(); it != creditCardVector.end(); ++it)
		{
			if (ccNumber == *it) {
				return true;
			}
		}
		return false;
	}
}
