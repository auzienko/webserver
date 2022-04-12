#ifndef CGITASK_HPP
#define CGITASK_HPP

#include "../Connections/AConnection.hpp"
#include "ATask.hpp"
class AConnection;

class CGITask : public ATask {
 private:
  AConnection* _connection;
  t_server const& _server_config;
  std::stringstream _Header;
  std::stringstream _Body;
  std::stringstream _response;

  CGITask(void);
  CGITask(CGITask const& src);
  CGITask& operator=(CGITask const& rhs);

 public:
  virtual ~CGITask();
  int collectData(void);
  int executeTask(void);
  int sendResult(void);

  static void Build(int fd, t_server const& _server_config, t_uriInfo &cur);
};

#endif
