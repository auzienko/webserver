#ifndef HEADTASK_HPP
#define HEADTASK_HPP

#include "../Connections/AConnection.hpp"
#include "GetTask.hpp"
class AConnection;

class HeadTask : public GetTask {

 public:
  HeadTask(AConnection* connection, int const& fd, t_uriInfo parsedURI);
  virtual ~HeadTask();

 private:
  int _AssembleResponse(void);
};

#endif
