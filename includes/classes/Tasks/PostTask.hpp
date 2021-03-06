#ifndef POSTTASK_HPP
#define POSTTASK_HPP

#include "classes/Connections/AConnection.hpp"
#include "ATask.hpp"
class AConnection;

class PostTask : public ATask {
 private:
  std::stringstream _response;
  t_uriInfo _parsedURI;
  std::string _inputBody;

  PostTask(void);
  PostTask(PostTask const& src);
  PostTask& operator=(PostTask const& rhs);

 public:
  PostTask(AConnection* connection, int const& fd, t_uriInfo const& parsedURI, std::string &inputBody);
  virtual ~PostTask();
  int collectData(void);
  int executeTask(void);
  int sendResult(void);

 private:
  int _MakeHeader(void);
  int _AssembleResponse(void);
};

#endif
