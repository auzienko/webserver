#ifndef CGIINPUTTASK_HPP
#define CGIINPUTTASK_HPP

#include "classes/Connections/AConnection.hpp"
#include "ATask.hpp"
class AConnection;

class CgiInputTask : public ATask {
 private:
  int const _parentFd;

  CgiInputTask(void);
  CgiInputTask(CgiInputTask const& src);
  CgiInputTask& operator=(CgiInputTask const& rhs);

 public:
  CgiInputTask(AConnection* connection, int const& fd, int const& parentFd);
  virtual ~CgiInputTask();
  int collectData(void);
  int executeTask(void);
  int sendResult(void);
  int setLastActivity(void);
};

#endif
