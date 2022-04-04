#pragma once

namespace Domain::Job
{

  class JobHandler
  {
    public:



      virtual ~JobHandler() noexcept = 0;

    protected:

      JobHandler & operator=( const JobHandler &  rhs ) = default;  // copy assignment
      JobHandler & operator=(       JobHandler && rhs ) = default;  // move assignment

  };


  inline JobHandler::~JobHandler() noexcept = default;
}
