#include "Domain/Account/Account.hpp"
namespace Domain::Account
{
  std::map<std::string, Account::accountCreateMethod> Domain::Account::Account::a_methods;
  //bool Domain::Account::StudentAccount::a_registered = Account::registerType(StudentAccount::getName(), Domain::Account::StudentAccount::createMethod);
}
