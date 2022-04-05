#include "Domain/Session/Session.hpp"

#include <string>
#include <any>
#include <vector>
#include <iostream>

namespace
{
  #define STUB(functionName)  std::any functionName(Domain::Session::SessionBasic & , const std::vector<std::string> & ) \
                              {return {};}
  STUB(getUserReport)
  STUB(removeAccount)

  std::any listReportedUsers ( Domain::Session::SessionBasic & session, const std::vector<std::string> & args )
  {
    std::string results = "Date \"" + args[0] + "\" searched by \"" + session._credentials.userName + '"';
    session._logger << "search reported users from: " + results;
    return {results};
  }
}

namespace Domain::Session
{
  SessionBasic::SessionBasic(const std::string & description, const AccountCredentials & credentials) : _credentials( credentials), _name(description)
  {
    _logger << "Session \"" + _name + "\" being used";
  }




  SessionBasic::~SessionBasic() noexcept
  {
    _logger << "Session \"" + _name + "\" has shut down";
  }

  std::vector<std::string> SessionBasic::getActions()
  {
    std::vector<std::string> availableActions;
    availableActions.reserve( _actionDispatch.size() );

    for( const auto & [action, function] : _actionDispatch ) availableActions.emplace_back( action );

    return availableActions;
  }

  std::any SessionBasic::executeAction( const std::string & action, const std::vector<std::string> & args )
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
      throw BadCommand( message );
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
