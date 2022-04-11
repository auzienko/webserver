#ifndef AUTOINDEXTASK_HPP
#define AUTOINDEXTASK_HPP

#include "AConnection.hpp"
#include "ATask.hpp"
class AConnection;

class AutoindexTask : public ATask {
 private:
  AConnection* _connection;
  std::stringstream _Header;
  std::stringstream _Body;
  std::stringstream _response;
  std::string _realPath;
  std::string _publicPath;

  AutoindexTask(void);
  AutoindexTask(AutoindexTask const& src);
  AutoindexTask& operator=(AutoindexTask const& rhs);

 public:
  AutoindexTask(AConnection* connection, int const& fd, std::string realPath,
                std::string publicPath);
  virtual ~AutoindexTask();
  int collectData(void);
  int executeTask(void);
  int sendResult(void);

 private:
  int _MakeBody(void);
  int _MakeHeader(void);
  int _AssembleResponse(void);
};

#endif
