#include "TechnicalServices/Persistence/SimpleDB.hpp"

#include <fstream>    // streamsize
#include <iomanip>    // quoted()
#include <limits>     // numeric_limits
#include <memory>     // make_unique()
#include <string>
#include <vector>

#include "TechnicalServices/Logging/SimpleLogger.hpp"
#include "TechnicalServices/Persistence/PersistenceHandler.hpp"





namespace
{
  // User defined manipulator with arguments that allows std::istream::ignore to be called "in-line" (chained)
  // Usage example:
  //    stream >> first >> ignore(',') second >> ignore('\n') ;
  struct ignore
  {
    char _seperator;
    ignore( char delimiter = '\n' ) : _seperator( delimiter ) {}
  };

  std::istream & operator>>( std::istream & s, ignore && delimiter )
  {
    s.ignore( std::numeric_limits<std::streamsize>::max(), delimiter._seperator );
    return s;
  }
}    // namespace




namespace TechnicalServices::Persistence
{
  // Design decision/Programming note:
  //  - The persistence database contains adaptation data, and one of the adaptable items is which Logger component to use
  //  - The factory function TechnicalServices::Logging::create(std::ostream &) depends of the persistence database to obtain
  //    through adaptation data which kind of Logging component to create
  //  - However, the Persistence database implementations, like this one, should (must?) be able to log messages
  //  - Therefore, to maintain the design decision to allow Logging to depend on Persistence, but not Persistence to depend on
  //    Logging, we mustn't create this logger through the LoggingHandler interface, but rather select and create a specific Logger
  SimpleDB::SimpleDB() : _loggerPtr( std::make_unique<TechnicalServices::Logging::SimpleLogger>() )
  {
    _logger << "Simple DB being used and has been successfully initialized";



    // Let's look for an adaptation data file, and if found load the contents.  Otherwise create some default values.
    std::ifstream adaptationDataFile( "Library_System_AdaptableData.dat", std::ios::binary );

    if( adaptationDataFile.is_open() )
    {
      // Expected format:  key = value
      // Notes:
      //   1) if key or value contain whitespace, they must be enclosed in double quotes
      //   2) if the same Key appears more than once, last one wins
      //   3) everything after the value is ignored, allowing that space to be used as comments
      //   4) A Key of "//" is ignored, allowing the file to contain comment lines of the form // = ...
      std::string key, value;
      while( adaptationDataFile >> std::quoted( key ) >> ignore( '=' ) >> std::quoted( value ) >> ignore( '\n' ) )   _adaptablePairs[key] = value;
      _adaptablePairs.erase( "//" );
    }

    else
    {
      _adaptablePairs = { /* KEY */               /* Value*/
                          {"Component.Logger",    "Simple Logger"},
                          {"Component.UI",        "Simple UI"}
//                        {"Component.UI",        "Contracted UI"}
                        };
    }
  }

  struct UserBase{

      std::vector<AccountCredentials> storedUsers =
      {
      // Username    Pass Phrase         Authorized roles                Account_ID
        {"Andres",     "CalState462",  {"JobSeeker", "Employer"},       "000000001"},
        {"Chris",       "manidk462",   {"JobSeeker"                  }, "000000002"},
        {"admin462",  "462FUN!",       {"Administrator"},               "000000000"}
      };

      std::vector<AccountCredentials> reportedUsers =
      {
      // Username    Pass Phrase         Authorized roles                Account_ID
        {"Andres",     "CalState462",  {"JobSeeker", "Employer"},       "000000001"},
        {"Chris",       "manidk462",   {"JobSeeker"            },       "000000002"},
      };

      std::vector<JobCredentials> jobListings =
      {
      // employer        position         jobID       status

        {"Amazon", "Software Engineer",  "1234", "Unavailable"},
        {"Google", "Software Engineer",  "123452", "Available"},
        {"Facebook", "Software Engineer",  "123453", "Available"},
        {"Twitter", "Software Engineer",  "123454", "Available"},
        {"Amazon", "Warehouse manager",  "123455", "Available"},
        {"Amazon", "HR Manager",  "123456", "Available"},
        {"Amazon", "Call Center Rep",  "123457", "Available"},
        {"Bank of America", "Financial consultant",  "123458", "Available"},
        {"Amazon", "QA Analyst",  "123459", "Unavailable"}
      };
  };

  static std::vector<UserReport> userReports = { {"000000001", "activity report:\nCreated Account 4/5/2022"} ,
                                               {"000000002", "activity report"}  };


  static UserBase Users;

  SimpleDB::~SimpleDB() noexcept
  {
    _logger << "Simple DB shutdown successfully";
  }




  std::vector<std::string> SimpleDB::findRoles()
  {
    return { "JobSeeker", "Employer", "Administrator"};
  }

  bool SimpleDB::removeAccount(std::string accountID)
  {
    for(auto user = Users.reportedUsers.begin(); user != Users.reportedUsers.end(); ++user)
    {
      if( user->accountID == accountID )
      {
        Users.reportedUsers.erase(user);
        return true;
      }
    }
    // Name not found, log the error and throw something
    std::string message = __func__;
    message += " attempt to find and remove user with ID \"" + accountID + "\" failed";

    _logger << message;
    throw PersistenceHandler::NoSuchUser( message );
  }

  std::vector<AccountCredentials> SimpleDB::findReportedUsers(std::string date)
  {
    date = "";

    return Users.reportedUsers;
  }

  AccountCredentials SimpleDB::findCredentialsByName( const std::string & name )
  {

    for( const auto & user : Users.storedUsers ) if( user.userName == name ) return user;

    // Name not found, log the error and throw something
    std::string message = __func__;
    message += " attempt to find user \"" + name + "\" failed";

    _logger << message;
    throw PersistenceHandler::NoSuchUser( message );
  }

  AccountCredentials SimpleDB::findCredentialsByID(const std::string & ID)
  {
    for( const auto & user : Users.storedUsers ) if( user.accountID == ID ) return user;

    // Name not found, log the error and throw something
    std::string message = __func__;
    message += " attempt to find user \"" + ID + "\" failed";

    _logger << message;
    throw PersistenceHandler::NoSuchUser( message );
  }

  std::string SimpleDB::findUserReport(const std::string & accountID)
  {

     for (UserReport report : userReports)
     {
        if (report.account_ID == accountID)
             return report.report;

     }
     return "no report found";
  }

  std::string SimpleDB::setUsername(std::string accountID, std::string username)
  {
    for(auto user = Users.storedUsers.begin(); user != Users.storedUsers.end(); ++user)
    {
      if( user->accountID == accountID )
      {
        user->userName = username;
        return "Success\n";
      }
    }
    return "FAILED";
  }

  std::string SimpleDB::setPassword(std::string accountID, std::string password)
  {
    for(auto user = Users.storedUsers.begin(); user != Users.storedUsers.end(); ++user)
    {
      if( user->accountID == accountID )
      {
        user->passPhrase = password;
        return "Success\n";
      }
    }
    return "FAILED";
  }

  std::vector<JobCredentials> SimpleDB::listJobs(std::string company)
  {
      company = "";
      return Users.jobListings;
  }


  std::string SimpleDB::getJobListingReport(std::string jobID)
  {
    std::string job = jobID;
    return "\nthe employer is no longer considering any applicants for this position. \n\n";
  }

  std::string SimpleDB::removeJobListing(std::string jobID)
  {
    for(auto user = Users.jobListings.begin(); user != Users.jobListings.end(); ++user)
    {
      if( user->jobID == jobID)
      {
        Users.jobListings.erase(user);
        return "Success";
      }
    }
    return "FAILED";
  }

  const std::string & SimpleDB::operator[]( const std::string & key ) const
  {
    auto pair = _adaptablePairs.find( key );
    if( pair != _adaptablePairs.cend() ) return pair->second;

    // Key not found - error
    std::string message = __func__;
    message += " attempt to access adaptation data with Key = \"" + key + "\" failed, no such Key";

    _logger << message;
    throw NoSuchProperty( message );
  }
} // namespace TechnicalServices::Persistence
