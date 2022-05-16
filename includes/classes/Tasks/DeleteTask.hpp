#ifndef DELETETASK_HPP
#define DELETETASK_HPP

#include "../Connections/AConnection.hpp"
#include "ATask.hpp"
class AConnection;

class DeleteTask : public ATask {
 private:
  std::stringstream _response;
  t_uriInfo _parsedURI;

  DeleteTask(void);
  DeleteTask(DeleteTask const& src);
  DeleteTask& operator=(DeleteTask const& rhs);

 public:
  DeleteTask(AConnection* connection, int const& fd, t_uriInfo const& parsedURI);
  virtual ~DeleteTask();
  int collectData(void);
  int executeTask(void);
  int sendResult(void);

 private:
  int _MakeHeader(void);
  int _AssembleResponse(void);
};

#endif
