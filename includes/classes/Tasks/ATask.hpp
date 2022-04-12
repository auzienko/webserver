#ifndef ATASK_HPP
#define ATASK_HPP

enum Status { NEW, READING, READY_TO_HANDLE, EXECUTION, READY_TO_SEND, SENDING, DONE };
enum Type { UNKNOWN_NETWORK, NETWORK_AUTOINDEX, NETWORK_GET, NETWORK_CGI_PARENT, LOCAL_CGI_INPUT, LOCAL_CGI_OUTPUT };

#include "../../main.hpp"

class ATask
{
private:
  	int _fd;
	int _status;
	int _type;
	ATask(ATask const& src);
	ATask& operator=(ATask const& rhs);
	ATask(void);
public:
	ATask(int type, int fd);
	virtual ~ATask();
	int getStatus(void) const;
	void setStatus(int status);
	int getType(void) const;
	int getFd(void) const;
	int doTask(void);
	virtual int collectData(void) = 0;
	virtual int executeTask(void) = 0;
  	virtual int sendResult(void) = 0;
};
#endif
