#pragma once

#include <memory>
#include "Domain/Account/AccountHandler.hpp"

namespace Domain::Account
{
  class Account : public Domain::Account::AccountHandler
  {
    public:
      using AccountHandler::AccountHandler;

      using accountCreateMethod = std::unique_ptr<Domain::Account::Account>(*)();

      static bool registerType(const std::string& name,  accountCreateMethod accountMethod){
        if(auto it = a_methods.find(name); it == a_methods.end())
        {
          //a_methods.insert(std::make_pair(name, accountMethod));
          a_methods[name] = accountMethod;
          return true;
        }

        return false;
      }

      static std::map<std::string, accountCreateMethod>& getRegistry(){
        //static std::map<std::string, accountCreateMethod> a_methods;
        return a_methods;
      }

      bool removeAccount(std::string accountID) override
      {
        auto &          persistentData    = TechnicalServices::Persistence::PersistenceHandler::instance();
        return persistentData.removeAccount(accountID);
      }

      std::vector<AccountCredentials> listReportedUsers(std::string date) override
      {
        auto &          persistentData    = TechnicalServices::Persistence::PersistenceHandler::instance();
        return persistentData.findReportedUsers(date);
      }

      AccountCredentials getAccountInfo(std::string accountID) override
      {
        auto &          persistentData    = TechnicalServices::Persistence::PersistenceHandler::instance();
        return persistentData.findCredentialsByID(accountID);
      }

      std::string getUserReport(std::string accountID) override
      {
        auto &          persistentData    = TechnicalServices::Persistence::PersistenceHandler::instance();
        return persistentData.findUserReport(accountID);
      }

      std::string setUsername(std::string username) override
      {
        auto &          persistentData    = TechnicalServices::Persistence::PersistenceHandler::instance();
        return persistentData.setUsername("000000002", username);

      }

      std::string setPassword(std::string password) override
      {
        auto &          persistentData    = TechnicalServices::Persistence::PersistenceHandler::instance();
        return persistentData.setPassword("000000002", password);

      }

      ~Account() noexcept override;

    private:
      static std::map<std::string, accountCreateMethod> a_methods;
  };

  inline Account::~Account() noexcept
  {}

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
    //inline bool a_registered = Account::registerType(StudentAccount::getName(), StudentAccount::createMethod);
    struct EmployeeAccount      : Account
    {
      static std::unique_ptr<Account> createMethod()
      {
        return std::make_unique<EmployeeAccount>();
      }

      static std::string getName()
      {
        return "EmployeeAccount";
      }

      inline static bool a_registered = Account::registerType(EmployeeAccount::getName(), EmployeeAccount::createMethod);
    };
    struct ExpertAccount     : Account{ ExpertAccount    (); };
    struct ManagementAccount    : Account{ ManagementAccount   (); };
}
