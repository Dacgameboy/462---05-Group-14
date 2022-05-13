#pragma once

#include <memory>
#include <iostream>
#include "Domain/Account/AccountHandler.hpp"

namespace Domain::Account
{
  class Account : public Domain::Account::AccountHandler
  {
    public:
      using AccountHandler::AccountHandler;

      using accountCreateMethod = std::unique_ptr<Domain::Account::Account>(*)();

      static bool registerType(const std::string& name,  accountCreateMethod accountMethod){
        auto it = a_methods.find(name);
        if(it == a_methods.end())
        {
          a_methods.insert(std::make_pair(name, accountMethod));
          //a_methods[name] = accountMethod;
          for(std::map<std::string, accountCreateMethod>::iterator itr = a_methods.begin(); itr != a_methods.end(); ++itr)
          {
            //std::cout << itr->first;
          }

          std::cout << "Account type " << name << " registered\n";
          return true;
        }

        return false;
      }

      static std::map<std::string, accountCreateMethod>& getRegistry(){
        //static std::map<std::string, accountCreateMethod> a_methods;
        return a_methods;
      }

      static std::unique_ptr<Account> Create(std::string name)
      {
        if(auto it = a_methods.find(name); it != a_methods.end())
        {
          return it->second();
        }
        return nullptr;
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

  //struct StudentAccount : Account{};
  //struct EmployeeAccount      : Account{};
  //struct ExpertAccount     : Account{};
  //struct ManagementAccount       : Account{};
}
