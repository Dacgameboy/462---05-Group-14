#pragma once

#include <string>
#include <vector>
#include <memory>
#include <any>

#include "Domain/Session/SessionHandler.hpp"
#include "TechnicalServices/Logging/LoggerHandler.hpp"



namespace Domain::Session
{
  using TechnicalServices::Persistence::AccountCredentials;

  class SessionBasic : public SessionHandler
  {
    public:
      SessionBasic(const std::string & description,  const AccountCredentials & credentials);

      std::vector<std::string> getActions() override;
      std::any executeAction( const std::string & action, const std::vector<std::string> & args ) override;


      ~SessionBasic() noexcept override = 0;


    protected:
    public:  // Dispatched functions need access to these attributes, so for now make these public instead of protected
      // Types
      using DispatchTable = std::map<std::string, std::any (*)( Domain::Session::SessionBasic &, const std::vector<std::string> & )>;
      friend class Policy;

      // Instance Attributes
      std::unique_ptr<TechnicalServices::Logging::LoggerHandler> _loggerPtr = TechnicalServices::Logging::LoggerHandler::create();
      TechnicalServices::Logging::LoggerHandler &                _logger    = *_loggerPtr;

      AccountCredentials const                                      _credentials;
      std::string     const                                      _name      = "Undefined";
      DispatchTable                                              _actionDispatch;

  };

  struct AdministratorSession : SessionBasic{ AdministratorSession( const AccountCredentials & credentials ); };
  struct EmployerSession      : SessionBasic{ EmployerSession     ( const AccountCredentials & credentials ); };
  struct JobSeekerSession     : SessionBasic{ JobSeekerSession    ( const AccountCredentials & credentials ); };
  struct GeneralSession    : SessionBasic{ GeneralSession   ( const AccountCredentials & credentials ); };

}
