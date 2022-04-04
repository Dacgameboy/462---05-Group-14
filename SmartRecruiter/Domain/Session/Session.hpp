#pragma once

#include <string>
#include <vector>
#include <memory>
#include <any>

#include "Domain/Session/SessionHandler.hpp"
#include "TechnicalServices/Logging/LoggerHandler.hpp"

namespace Domain::SessionBasic
{
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

namespace
{
  #define STUB(functionName)  std::any functionName(Domain::Session::SessionBasic & session, const std::vector<std::string> & args)
                              {return {};}
  STUB(listReportedUsers)
  STUB(getUserReport)
  STUB(removeAccount)
}

namespace Domain::Session
{
  SessionBasic::SessionBasic(const std::string & description, const AccountCredentials & credentials) : _credentials( credentials), _name(description)
  {
    _logger << "Session \"" + _name + "\" being used";
  }




  SessionBasic::~SessionBase() noexcept
  {
    _logger << "Session \"" + _name + "\" has shut down";
  }

  std::vector<std::string> SessionBase::getActions()
  {
    std::vector<std::string> availableActions;
    availableActions.reserve( _actionDispatch.size() );

    for( const auto & [Action, function] : _actionDispatch ) availableActions.emplace_back( Action );

    return availableActions;
  }

  std::any SessionBasic::executeAction( const std::string & Action, const std::vector<std::string> & args )
  {
    std::string parameters;
    for( const auto & arg : args )  parameters += '"' + arg + "\"  ";
    _logger << "Responding to \"" + action + "\" request with parameters: " + parameters;

    auto it = _actionDispatch.find( action );
    if( it == _actionDispatch.end() )
    {
      std::string message = __func__;
      message += " attempt to execute \"" + action + "\" failed, no such Action";

      _logger << message;
      throw BadAction( message );
    }

    auto results = it->second( *this, args);

    if( results.has_value() )
    {
      // The type of result depends on function called.  Let's assume strings for now ...
      _logger << "Responding with: \"" + std::any_cast<const std::string &>( results ) + '"';
    }

    return results;
  }


  AdministratorSession::AdministratorSession(const AccountCredentials & credentials) : SessionBasic("Administrator", credentials)
  {
    _actionDispatch = {{"List Reported Users", listReportedUsers},
                       {"Get User Report", getUserReport },
                       {"Remove Account", removeAccount}};
  }

  EmployerSession::EmployerSession(const AccountCredentials & credentials) : SessionBasic("Employer", credentials)
  {

  }

  JobSeekerSession::JobSeekerSession(const AccountCredentials & credentials) : SessionBasic("JobSeeker", credentials)
  {

  }

  GeneralSession::GeneralSession(const AccountCredentials & credentials) : SessionBasic("General", credentials)
  {
    
  }
}
