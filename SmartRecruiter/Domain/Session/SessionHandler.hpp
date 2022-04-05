#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <any>
#include <memory>
#include <algorithm>

#include "TechnicalServices/Persistence/PersistenceHandler.hpp"

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
      virtual std::vector<std::string> getActions() = 0;
      virtual std::any executeAction(const std::string & action, const std::vector<std::string> & args) = 0;

      virtual ~SessionHandler() noexcept = 0;

    protected:
      SessionHandler & operator=( const SessionHandler &  rhs ) = default;
      SessionHandler & operator=(       SessionHandler && rhs ) = default;
  };
}
