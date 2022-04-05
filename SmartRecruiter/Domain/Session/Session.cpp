#include "Domain/Session/Session.hpp"

#include <string>
#include <any>
#include <vector>
#include <iostream>

namespace
{

  using TechnicalServices::Persistence::AccountCredentials;

  #define STUB(functionName)  std::any functionName(Domain::Session::SessionBasic & , const std::vector<std::string> & ) \
                              {return {};}
  STUB(getUserReport)
  STUB(removeAccount)
  STUB(getAccountInfo)
  STUB(setUsername)
  STUB(setPassword)
  STUB(listJobAdvertisements)
  STUB(getJobAdvertisements)
  STUB(setQualifications)



  std::any listReportedUsers ( Domain::Session::SessionBasic & session, const std::vector<std::string> & args )
  {
    auto &          persistentData    = TechnicalServices::Persistence::PersistenceHandler::instance();
    auto results = persistentData.findReportedUsers(args[0]);
    session._logger << "search reported users from: " + args[0];
    return {results};
  }


  //getuserreport
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
      //_logger << "Responding with: \"" + std::any_cast<const std::string &>( results ) + '"';
    }

    return results;
  }


  AdministratorSession::AdministratorSession(const AccountCredentials & credentials) : SessionBasic("Administrator", credentials)
  {
    _actionDispatch = {{"List Reported Users", listReportedUsers},
                       {"Get User Report", getUserReport },
                       {"Remove Account", removeAccount},
                       {"List Job Advertisements",listJobAdvertisements },
                       {"Get Job Advertisements", getJobAdvertisements},
                       {"Set Qualifications", setQualifications}};
  }

  EmployerSession::EmployerSession(const AccountCredentials & credentials) : SessionBasic("Employer", credentials)
  {

  }

  JobSeekerSession::JobSeekerSession(const AccountCredentials & credentials) : SessionBasic("JobSeeker", credentials)
  {
    _actionDispatch = {{"Get Account Info", getAccountInfo},
                       {"Set Username", setUsername},
                       {"Set Password", setPassword}};
  }

  GeneralSession::GeneralSession(const AccountCredentials & credentials) : SessionBasic("General", credentials)
  {

  }
}
