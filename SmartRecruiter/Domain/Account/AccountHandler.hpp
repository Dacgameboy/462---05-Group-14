#pragma once

#include <string>
#include <vector>
#include "TechnicalServices/Persistence/PersistenceHandler.hpp"

using TechnicalServices::Persistence::AccountCredentials;

namespace Domain::Account
{
  class AccountHandler
  {
    public:

      virtual bool removeAccount(std::string accountID) = 0;
      virtual std::vector<AccountCredentials> listReportedUsers(std::string date) = 0;

      virtual ~AccountHandler() noexcept = 0;

    protected:

      AccountHandler & operator=( const AccountHandler &  rhs ) = default;  // copy assignment
      AccountHandler & operator=(       AccountHandler && rhs ) = default;  // move assignment
      static AccountHandler & instance();

  };


  inline AccountHandler::~AccountHandler() noexcept = default;
}
