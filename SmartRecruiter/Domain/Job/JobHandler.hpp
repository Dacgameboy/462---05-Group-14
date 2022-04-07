//Job Handler Interface - 462 Group 14
//Job handler is responsible for managing jobOpenings as well as any structures related to Jobs (joblists, applied resumes, etc.)
#pragma once


namespace Domain::Job
{

  using TechnicalServices::Persistence::JobCredentials;

  class JobHandler
  {
    public:

      virtual std::vector<JobCredentials> listJobs(std::string company) = 0;//lists available jobs of a company
      virtual std::string getJobListingReport(std::string jobID) = 0;//gets a jopOpening report based on jobID
      virtual std::string removeJobListing(std::string jobID) = 0;//removes a jobOpening based on jobID


      virtual ~JobHandler() noexcept = 0;

    protected:

      JobHandler & operator=( const JobHandler &  rhs ) = default;  // copy assignment
      JobHandler & operator=(       JobHandler && rhs ) = default;  // move assignment

  };


  inline JobHandler::~JobHandler() noexcept = default;
}
