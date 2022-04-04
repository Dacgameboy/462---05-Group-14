#pragma once

#include <map>
#include <stdexcept>    // domain_error, runtime_error
#include <string>
#include <vector>


#include "TechnicalServices/Persistence/SimpleDB.hpp"

namespace TechnicalServices::Persistence
{
  // Function argument type definitions
  struct AccountCredentials
  {
    std::string               userName;
    std::string               passPhrase;
    std::vector<std::string>  roles;
  };




  // Persistence Package within the Technical Services Layer Abstract class
  // Singleton Class - only one instance of the DB exists for the entire system
  class PersistenceHandler
  {
    public:
      // Exceptions
      struct PersistenceException : std::runtime_error   {using runtime_error       ::runtime_error;};
      struct   NoSuchUser         : PersistenceException {using PersistenceException::PersistenceException;};
      struct   NoSuchProperty     : PersistenceException {using PersistenceException::PersistenceException;};

      // Creation (Singleton)
      PersistenceHandler            (                            ) = default;
      PersistenceHandler            ( const PersistenceHandler & ) = delete;
      PersistenceHandler & operator=( const PersistenceHandler & ) = delete;
      static  PersistenceHandler & instance();


      // Operations
      virtual std::vector<std::string> findRoles()                                       = 0;   // Returns list of all legal roles
      virtual AccountCredentials          findCredentialsByName( const std::string & name ) = 0;   // Returns credentials for specified user, throws NoSuchUser if user not found


      // Adaptation Data read only access.  Adaptation data is a Key/Value pair
      // Throws NoSuchProperty
      virtual const std::string & operator[]( const std::string & key ) const = 0;


      // Destructor
      // Pure virtual destructor helps force the class to be abstract, but must still be implemented
      virtual ~PersistenceHandler() noexcept = 0;
  };

  PersistenceHandler::~PersistenceHandler() noexcept = default;




  PersistenceHandler & PersistenceHandler::instance()
  {
    // Can't read the DB component preference from the database because the DB has not yet been created. So choosing the database
    // implementation is really a configuration item (set by the vendor before delivery), not an adaptable item (set by the end-user
    // after delivery)
    using SelectedDatabase = SimpleDB;

    static SelectedDatabase instance;    // Note the creation of a DB specialization (derived class), but returning a reference to
                                         // the generalization (base class). Since SimpleDB is-a PersistenceHandler, we can return a
                                         // reference to the base class that refers to a specific derived class.  SimpleDB is
                                         // accessed polymorphicly through the PersistenceHandler interface.  This source file knows
                                         // about the specific SimpleDB derived class, but that's okay.  This source file is not
                                         // delivered with the interface and remains That is, PersistenceHandler.hpp is given to the
                                         // upper architectural layers, but not PersistenceHandler.cpp.
    return instance;
  }
} // namespace TechnicalServices::Persistence
