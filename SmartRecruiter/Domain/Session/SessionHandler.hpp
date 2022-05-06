//Session Handler Interface - 462 Group 14
//session handler is responsible for taking in commands from the UI, and distributing them to the other domain handlers
//it also handles initial login and session creation to retrieve available actions based on session type
#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <any>
#include <memory>
#include <algorithm>

#include "TechnicalServices/Persistence/PersistenceHandler.hpp"
#include "Domain/Account/AccountHandler.hpp"

namespace Domain::Session
{
  using TechnicalServices::Persistence::AccountCredentials;

  class SessionHandler
  {
    public:

      struct SessionException : std::runtime_error {using runtime_error   ::runtime_error;   };
      struct   BadCommand     : SessionException   {using SessionException::SessionException;};

      static std::unique_ptr<SessionHandler> createSession (const AccountCredentials & credentials);


      //Operations
      virtual std::vector<std::string> getActions() = 0;//gets action options taking in session type
      virtual std::any executeAction(const std::string & action, const std::vector<std::string> & args) = 0;//executes a selected action
      virtual void logout() = 0; //logs out the user.

      virtual ~SessionHandler() noexcept = 0;

    protected:
      SessionHandler & operator=( const SessionHandler &  rhs ) = default;
      SessionHandler & operator=(       SessionHandler && rhs ) = default;
  };
}
