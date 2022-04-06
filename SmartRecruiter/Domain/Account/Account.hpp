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

      ~Account() noexcept override;
  };

  inline Account::~Account() noexcept
  {}

}
