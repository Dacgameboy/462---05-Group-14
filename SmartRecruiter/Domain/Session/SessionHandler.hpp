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

  SessionHandler::~SessionHandler() noexcept = default;



  std::unique_ptr<SessionHandler> SessionHandler:createSession(const AccountCredentials & credentials)
  {
   try
   {
     auto &          persistentData    = TechnicalServices::Persistence::PersistenceHandler::instance();
     AccountCredentials credentialsFromDB = persistentData.findCredentialsByName( credentials.userName );

     // 1)  Perform the authentication
     // std::set_intersection might be a better choice, but here I'm assuming there will be one and only one role in the passed-in
     // credentials I just need to verify the requested role is in the set of authorized roles.  Someday, if a user can sign in
     // with many roles combined, I may have to revisit this approach.  But for now, this is good enough.
     if(    credentials.userName   == credentialsFromDB.userName
         && credentials.passPhrase == credentialsFromDB.passPhrase
         && std::any_of( credentialsFromDB.roles.cbegin(), credentialsFromDB.roles.cend(),
                         [&]( const std::string & role ) { return credentials.roles.size() > 0 && credentials.roles[0] == role; }
                       )
       )
     {
       // 2) If authenticated user is authorized for the selected role, create a session specific for that role
       if( credentials.roles[0] == "JobSeeker"      ) return std::make_unique<Domain::Session::JobSeekerSession>     ( credentials );
       if( credentials.roles[0] == "Employer"     ) return std::make_unique<Domain::Session::EmployerSession>    ( credentials );
       if( credentials.roles[0] == "Administrator" ) return std::make_unique<Domain::Session::AdministratorSession>( credentials );
       if( credentials.roles[0] == "General"    ) return std::make_unique<Domain::Session::GeneralSession>   ( credentials );

       throw std::logic_error( "Invalid role requested in function " + std::string(__func__) ); // Oops, should never get here but ...  Throw something
     }
   }
   catch( const TechnicalServices::Persistence::PersistenceHandler::NoSuchUser & ) {}  // Catch and ignore this anticipated condition

   return nullptr;
  }
}
