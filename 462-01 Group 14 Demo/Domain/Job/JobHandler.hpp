#pragma once


namespace Domain::Job
{

  using TechnicalServices::Persistence::JobCredentials;

  class JobHandler
  {
    public:

      virtual std::vector<JobCredentials> listJobs(std::string company) = 0;
      virtual std::string getJobListingReport(std::string jobID) = 0;
      virtual std::string removeJobListing(std::string jobID) = 0;


      virtual ~JobHandler() noexcept = 0;

    protected:

      JobHandler & operator=( const JobHandler &  rhs ) = default;  // copy assignment
      JobHandler & operator=(       JobHandler && rhs ) = default;  // move assignment

  };


  inline JobHandler::~JobHandler() noexcept = default;
}
