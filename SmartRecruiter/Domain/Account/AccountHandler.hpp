//Account Handler Interface - 462 Group 14
//Account Handler is responsible for account management as well as generation of lists and processes involving accounts
#pragma once

#include <string>
#include <vector>
#include <map>
#include "TechnicalServices/Persistence/PersistenceHandler.hpp"

using TechnicalServices::Persistence::AccountCredentials;

namespace Domain::Account
{
  class AccountHandler
  {
    public:


      virtual bool removeAccount(std::string accountID) = 0;//removes an account based on accountID
      virtual std::vector<AccountCredentials> listReportedUsers(std::string date) = 0;//lists reported users taking in a certain day. (date sort not implemented yet)
      virtual AccountCredentials getAccountInfo(std::string accountID) = 0;//gets basic account info for the logged in user, using their accountID
      virtual std::string getUserReport(std::string accountID) = 0;//gets log information on any account using accountID (admin only)
      virtual std::string setUsername(std::string username) = 0;//changes username of logged in account
      virtual std::string setPassword(std::string password) = 0;//changes password of logged in account


      virtual ~AccountHandler() noexcept = 0;

    protected:

      AccountHandler & operator=( const AccountHandler &  rhs ) = default;  // copy assignment
      AccountHandler & operator=(       AccountHandler && rhs ) = default;  // move assignment
      static AccountHandler & instance();

    private:

  };


  inline AccountHandler::~AccountHandler() noexcept = default;
}
