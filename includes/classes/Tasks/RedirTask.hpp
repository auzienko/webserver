#ifndef REDIRTASK_HPP
#define REDIRTASK_HPP

#include "../Connections/AConnection.hpp"
#include "ATask.hpp"
class AConnection;

class RedirTask : public ATask {
 private:
  AConnection* _connection;
  std::stringstream _response;
  t_uriInfo _parsedURI;

  RedirTask(void);
  RedirTask(RedirTask const& src);
  RedirTask& operator=(RedirTask const& rhs);

 public:
  RedirTask(AConnection* connection, int const& fd, t_uriInfo parsedURI);
  virtual ~RedirTask();
  int collectData(void);
  int executeTask(void);
  int sendResult(void);

 private:
  int _MakeHeader(int status);
  int _AssembleResponse(void);
};

#endif
