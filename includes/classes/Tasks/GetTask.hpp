#ifndef GETTASK_HPP
#define GETTASK_HPP

#include "../Connections/AConnection.hpp"
#include "ATask.hpp"
class AConnection;

class GetTask : public ATask {
 private:
  AConnection* _connection;
  std::stringstream _Header;
  std::stringstream _Body;
  std::stringstream _response;
  t_uriInfo _parsedURI;

  GetTask(void);
  GetTask(GetTask const& src);
  GetTask& operator=(GetTask const& rhs);

 public:
  GetTask(AConnection* connection, int const& fd, t_uriInfo parsedURI);
  virtual ~GetTask();
  int collectData(void);
  int executeTask(void);
  int sendResult(void);

 private:
  int _MakeBody(void);
  int _MakeHeader(int status);
  int _AssembleResponse(void);
};

#endif
