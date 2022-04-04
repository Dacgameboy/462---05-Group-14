#pragma once

namespace Domain::Account
{

  class AccountHandler
  {
    public:



      virtual ~AccountHandler() noexcept = 0;

    protected:

      AccountHandler & operator=( const AccountHandler &  rhs ) = default;  // copy assignment
      AccountHandler & operator=(       AccountHandler && rhs ) = default;  // move assignment

  };


  inline AccountHandler::~AccountHandler() noexcept = default;
}
