#include "UI/SimpleUI.hpp"

#include <any>         // any_cast()
#include <iomanip>     // setw()
#include <iostream>    // streamsize
#include <limits>      // numeric_limits
#include <memory>      // unique_ptr, make_unique<>()
#include <string>      // string, getline()
#include <vector>

#include "Domain/Account/Account.hpp"    // Include for now - will replace next increment
#include "Domain/Session/SessionHandler.hpp"

#include "TechnicalServices/Logging/LoggerHandler.hpp"
#include "TechnicalServices/Persistence/PersistenceHandler.hpp"

  using TechnicalServices::Persistence::AccountCredentials;
  using TechnicalServices::Persistence::JobCredentials;

namespace UI
{
  // Default constructor
  SimpleUI::SimpleUI()
  : _accountHandler   ( std::make_unique<Domain::Account::Account>()                     ),   // will replace these with factory calls in the next increment
    _loggerPtr     ( TechnicalServices::Logging::LoggerHandler::create()            ),
    _persistentData( TechnicalServices::Persistence::PersistenceHandler::instance() )
  {
    _logger << "Simple UI being used and has been successfully initialized";
  }




  // Destructor
  SimpleUI::~SimpleUI() noexcept
  {
    _logger << "Simple UI shutdown successfully";
  }




  // Operations
  void SimpleUI::launch()
  {
    // 1) Fetch Role legal value list
    std::vector<std::string> roleLegalValues = _persistentData.findRoles();


    // 2) Present login screen to user and get username, password, and valid role
    Domain::Session::AccountCredentials credentials  = {"", "", {""}, ""};           // ensures roles[0] exists
    auto &                           selectedRole = credentials.roles[0];     // convenience alias

    std::unique_ptr<Domain::Session::SessionHandler> sessionControl;

    do
    {
      std::cin.ignore(  std::numeric_limits<std::streamsize>::max(), '\n' );

      std::cout << "  name: ";
      std::getline( std::cin, credentials.userName );

      std::cout << "  pass phrase: ";
      std::getline( std::cin, credentials.passPhrase );

      unsigned menuSelection;
      do
      {
        for( unsigned i = 0; i != roleLegalValues.size(); ++i )   std::cout << std::setw( 2 ) << i << " - " << roleLegalValues[i] << '\n';
        std::cout << "  role (0-" << roleLegalValues.size()-1 << "): ";
        std::cin  >> menuSelection;
      } while( menuSelection >= roleLegalValues.size() );

      selectedRole = roleLegalValues[menuSelection];


      // 3) Validate user is authorized for this role, and if so create session
      sessionControl = Domain::Session::SessionHandler::createSession( credentials );
      if( sessionControl != nullptr )
      {
        _logger << "Login Successful for \"" + credentials.userName + "\" as role \"" + selectedRole + "\"";
        break;
      }

      std::cout << "** Login failed\n";
      _logger << "Login failure for \"" + credentials.userName + "\" as role \"" + selectedRole + "\"";

    } while( true );


    // 4) Fetch functionality options for this role
    do
    {
      auto        actions = sessionControl->getActions();
      std::string selectedCommand;
      unsigned    menuSelection;

      do
      {
        for( unsigned i = 0; i != actions.size(); ++i ) std::cout << std::setw( 2 ) << i << " - " << actions[i] << '\n';
        std::cout << std::setw( 2 ) << actions.size() << " - " << "Quit\n";

        std::cout << "  action (0-" << actions.size() << "): ";
        std::cin >> menuSelection;
      } while( menuSelection > actions.size() );

      if( menuSelection == actions.size() ) break;

      selectedCommand = actions[menuSelection];
      _logger << "Command selected \"" + selectedCommand + '"';


      /******************************************************************************************************************************
      **  5) The user interface will collect relevant information to execute the chosen command.  This section requires the UI to
      **     know what information to collect, and hence what the available actions are.  Our goal is loose (minimal) coupling, not
      **     no coupling. This can be achieved in a variety of ways, but one common way is to pass strings instead of strong typed
      **     parameters.
      ******************************************************************************************************************************/
      if( selectedCommand == "List Reported Users" )
      {
        std::vector<std::string> parameters( 1 );

        std::cout << " Enter date **/**/****:  ";  std::cin >> std::ws;  std::getline( std::cin, parameters[0] );

        auto results = sessionControl->executeAction( selectedCommand, parameters );
        if( results.has_value() )
        {
          _logger << "Received List of Accounts";
          std::cout << "\nUsername        Account ID  \n";
          for(AccountCredentials credential :std::any_cast<std::vector<AccountCredentials>>( results ))
          {

            std::cout << credential.userName + "          " + credential.accountID + "\n";
          }
          std::cout << std::endl;
        }
      }
      else if( selectedCommand == "Remove User Account")
      {
        std::vector<std::string> parameters(2);

        std::cout << "Enter Account ID \"*********\":"; std::cin >> std::ws; std::getline(std::cin, parameters[0]);
        std::cout << "Enter Reason for removal:"; std::cin >> std::ws; std::getline(std::cin, parameters[1]);

        auto results = sessionControl->executeAction( selectedCommand, parameters );

        if( results.has_value() )
        {
          _logger << "Success";
        }
      }
      else if( selectedCommand == "Get Account Info" )
      {
        std::vector<std::string> parameters(1);
        parameters[0] = "000000002";
        auto results = sessionControl->executeAction( selectedCommand, parameters );
        AccountCredentials credential = std::any_cast<AccountCredentials>(results);
        std::cout <<"Username: " << credential.userName << "\nPass phrase: " << credential.passPhrase << "\nAccount ID: " << credential.accountID << "\n\n";
      }
      else if( selectedCommand == "Get User Report" )
      {
        std::vector<std::string> parameters(1);
        std::cout << "Enter Account ID \"*********\":"; std::cin >> std::ws; std::getline(std::cin, parameters[0]);
        auto results = sessionControl->executeAction( selectedCommand, parameters );
        std::cout << std::any_cast<std::string>(results) << "\n";
      }
      else if(selectedCommand == "Set Username")
      {
        std::vector<std::string> parameters(1);
        std::cout << "Enter New Username:"; std::cin >> std::ws; std::getline(std::cin, parameters[0]);
        auto results = sessionControl->executeAction( selectedCommand, parameters );
        std::cout << std::any_cast<std::string>(results);
      }
      else if(selectedCommand == "Set Password")
      {
        std::vector<std::string> parameters(1);
        std::cout << "Enter New Password:"; std::cin >> std::ws; std::getline(std::cin, parameters[0]);
        auto results = sessionControl->executeAction( selectedCommand, parameters );
        std::cout << std::any_cast<std::string>(results);
      }
      else if(selectedCommand == "List Jobs")
      {
        std::vector<std::string> parameters(1);
        parameters[0] = "";
        auto results = sessionControl->executeAction( selectedCommand, parameters );
        std::cout << "Employer                  Position                      Status                     Job ID\n";
        for(JobCredentials credential :std::any_cast<std::vector<JobCredentials>>( results ))
        {

          std::cout << credential.employer + "                  " + credential.position + "                      " + credential.status + "                     " + credential.jobID + "\n";
        }
        std::cout << std::endl;
      }
      else if(selectedCommand == "Get Job Listing report")
      {
        std::vector<std::string> parameters(1);
        std::cout << "Enter Job ID:"; std::cin >> std::ws; std::getline(std::cin, parameters[0]);
        auto results = sessionControl->executeAction( selectedCommand, parameters );
        std::cout << std::any_cast<std::string>(results);
      }
      else if(selectedCommand == "Remove Job Listing")
      {
        std::vector<std::string> parameters(2);
        std::cout << "Enter Job ID:"; std::cin >> std::ws; std::getline(std::cin, parameters[0]);
        std::cout << "Enter Reason:"; std::cin >> std::ws; std::getline(std::cin, parameters[1]);
        auto results = sessionControl->executeAction( selectedCommand, parameters );
        std::cout << std::any_cast<std::string>(results) << "\n";
      }

      else sessionControl->executeAction( selectedCommand, {} );
    } while( true );

    _logger << "Ending session and terminating";
  }
}
