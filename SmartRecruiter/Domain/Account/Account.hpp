#pragma once

#include "Domain/Account/AccountHandler.hpp"

namespace Domain::Account
{
  class Account : public Domain::Account::AccountHandler
  {
    public:
      using AccountHandler::AccountHandler;

      ~Account() noexcept override;
  };

  inline Account::~Account() noexcept
  {}

}
