#pragma once

#include "Domain/Job/JobHandler.hpp"

namespace Domain::Job
{
  class JobOpening : public Domain::Job::JobHandler
  {
    public:
      using JobHandler::JobHandler;

      ~JobOpening() noexcept override;
  };

  inline Books::~Books() noexcept
  {}

}
