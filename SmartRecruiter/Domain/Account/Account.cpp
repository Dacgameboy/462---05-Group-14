#include "Domain/Account/Account.hpp"
namespace Domain::Account
{
  std::map<std::string, Account::accountCreateMethod> Domain::Account::Account::a_methods;

  struct StudentAccount : public Account
  {
    //StudentAccount();
    static std::unique_ptr<Account> createMethod()
    {
      return std::make_unique<StudentAccount>();
    }

    static std::string getName()
    {
      return "Student";
    }

    inline static bool a_registered = Account::registerType(StudentAccount::getName(), StudentAccount::createMethod);
  };

  struct EmployeeAccount      : Account
  {
    static std::unique_ptr<Account> createMethod()
    {
      return std::make_unique<EmployeeAccount>();
    }

    static std::string getName()
    {
      return "Employee";
    }

    inline static bool a_registered = Account::registerType(EmployeeAccount::getName(), EmployeeAccount::createMethod);
  };
  
  struct ExpertAccount     : Account
  {
    static std::unique_ptr<Account> createMethod()
    {
      return std::make_unique<ExpertAccount>();
    }

    static std::string getName()
    {
      return "Expert";
    }

    inline static bool a_registered = Account::registerType(ExpertAccount::getName(), ExpertAccount::createMethod);
  };

  struct ManagementAccount    : Account
  {
    static std::unique_ptr<Account> createMethod()
    {
      return std::make_unique<ManagementAccount>();
    }

    static std::string getName()
    {
      return "Management";
    }

    inline static bool a_registered = Account::registerType(ManagementAccount::getName(), ManagementAccount::createMethod);
  };
}
