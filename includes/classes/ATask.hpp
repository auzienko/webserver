#ifndef ATASK_HPP
#define ATASK_HPP

enum Status { NEW, READING, READY_TO_HANDLE, READY_TO_SEND, SENDING, DONE };
enum Type { NETWORK, CGI };

#include "../main.hpp"

class ATask
{
private:
	int _status;
	int _type;
	ATask(ATask const& src);
	ATask& operator=(ATask const& rhs);
	ATask(void);
public:
	ATask(int type);
	virtual ~ATask();
	int getStatus(void) const;
	void setStatus(int status);
	int getType(void) const;
	virtual int collectData(void) = 0;
	virtual int executeTask(void) = 0;
  	virtual int sendResult(void) = 0;
};
#endif
