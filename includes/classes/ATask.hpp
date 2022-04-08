#ifndef ATASK_HPP
#define ATASK_HPP

enum Status { NEW, READING, READY_TO_HANDLE, READY_TO_SEND, SENDING, DONE };

class ATask
{
private:
	int _status;
	ATask(ATask const& src);
	ATask& operator=(ATask const& rhs);
public:
	ATask(void);
	~ATask();
	int getStatus(void) const;
	void setStatus(int status);
};
#endif
