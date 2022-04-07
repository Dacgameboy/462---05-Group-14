#pragma once

#include "Domain/Job/JobHandler.hpp"



namespace Domain::Job
{
  class JobOpening : public Domain::Job::JobHandler
  {
    public:
      using JobHandler::JobHandler;

      std::vector<JobCredentials> listJobs(std::string company) override
      {
        auto &          persistentData    = TechnicalServices::Persistence::PersistenceHandler::instance();
        return persistentData.listJobs(company);
      }

      std::string getJobListingReport(std::string jobID) override
      {
        auto &          persistentData    = TechnicalServices::Persistence::PersistenceHandler::instance();
        return persistentData.getJobListingReport(jobID);
      }

      std::string removeJobListing(std::string jobID) override
      {
        auto &          persistentData    = TechnicalServices::Persistence::PersistenceHandler::instance();
        return persistentData.removeJobListing(jobID);
      }

      ~JobOpening() noexcept override;
  };

  inline JobOpening::~JobOpening() noexcept
  {}

}
