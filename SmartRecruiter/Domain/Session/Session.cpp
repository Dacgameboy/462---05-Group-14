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

  STUB(listJobAdvertisements)
  STUB(getJobAdvertisements)
  STUB(setQualifications)



  std::any listReportedUsers ( Domain::Session::SessionBasic & session, const std::vector<std::string> & args )
  {
    auto account = Domain::Account::Account::Create("Student");
    auto results = account->listReportedUsers(args[0]);
    session._logger << "search reported users from: " + args[0];
    return {results};
  }

  std::any removeAccount ( Domain::Session::SessionBasic & session, const std::vector<std::string> & args )
  {

    auto account = Domain::Account::Account::Create("Student");
    auto results = account->removeAccount(args[0]);
    if(results) session._logger << "Removed account with ID: " + args[0] + " with justification: " + args[1];
    return {results};
  }

  std::any getAccountInfo( Domain::Session::SessionBasic & session, const std::vector<std::string> & args )
  {
    auto account = Domain::Account::Account::Create("Student");
    auto results = account->getAccountInfo(args[0]);
    session._logger << "Retrieved Account info with ID: " + args[0];
    return {results};
  }

  std::any getUserReport( Domain::Session::SessionBasic & session, const std::vector<std::string> & args )
  {
    auto account = Domain::Account::Account::Create("Student");
    auto results = account->getUserReport(args[0]);
    session._logger << "Retrieved user report of ID: " + args[0];
    return {results};
  }

  std::any setUsername(Domain::Session::SessionBasic & session, const std::vector<std::string> & args)
  {
    auto account = Domain::Account::Account::Create("Student");
    auto results = account->setUsername(args[0]);
    session._logger << "Changed Username to: " + args[0];
    return {results};
  }

  std::any setPassword(Domain::Session::SessionBasic & session, const std::vector<std::string> & args)
  {
    auto account = Domain::Account::Account::Create("Student");
    auto results = account->setPassword(args[0]);
    session._logger << "Changed Password to: " + args[0];
    return {results};
  }

  std::any listJobs ( Domain::Session::SessionBasic & session, const std::vector<std::string> & args )
  {
    auto job = new Domain::Job::JobOpening;
    auto results = job->listJobs(args[0]);
    session._logger << "search reported jobs from: " + args[0];
    return {results};
  }

  std::any jobListingReport(Domain::Session::SessionBasic & session, const std::vector<std::string> & args )
  {
    auto job = new Domain::Job::JobOpening;
    auto results = job->getJobListingReport(args[0]);
    session._logger << "Got Job report from: " + args[0];
    return {results};
  }

  std::any removeJobListing(Domain::Session::SessionBasic & session, const std::vector<std::string> & args )
  {
    auto job = new Domain::Job::JobOpening;
    auto results = job->removeJobListing(args[0]);
    session._logger << "Removing job listing with ID: " + args[0] + "\nWith Reason: " + args[1];
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

  void SessionBasic::logout()
  {
    _logger << "Ending session and terminating";
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
                       {"Remove User Account", removeAccount},
                       {"List Job Advertisements",listJobAdvertisements },
                       {"Get Job Advertisements", getJobAdvertisements},
                       {"Set Qualifications", setQualifications}};
  }

  EmployerSession::EmployerSession(const AccountCredentials & credentials) : SessionBasic("Employer", credentials)
  {
    _actionDispatch = {{"List Jobs", listJobs },
                      {"Get Job Listing report", jobListingReport },
                      {"Remove Job Listing", removeJobListing}};

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
