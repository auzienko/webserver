#ifndef POSTTask_HPP
#define POSTTask_HPP

#include "../Connections/AConnection.hpp"
#include "ATask.hpp"
class AConnection;

class PostTask : public ATask {
 private:
  std::stringstream _Header;
  std::stringstream _Body;
  std::stringstream _response;
  AConnection* _connection;
  t_uriInfo _parsedURI;

  PostTask(void);
  PostTask(PostTask const& src);
  PostTask& operator=(PostTask const& rhs);

 public:
  PostTask(AConnection* connection, int const& fd, t_uriInfo parsedURI);
  virtual ~PostTask();
  int collectData(void);
  int executeTask(void);
  int sendResult(void);

 private:
  int _MakeBody(void);
  int _MakeHeader(int status);
  int _AssembleResponse(void);
};

#endif
