#pragma once

#include "Domain/Account/AccountHandler.hpp"

namespace Domain::Account
{
  class Account : public Domain::Account::AccountHandler
  {
    public:
      using AccountHandler::AccountHandler;

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
  };

  inline Account::~Account() noexcept
  {}

}
