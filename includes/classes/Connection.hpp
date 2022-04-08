#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "../main.hpp"

class Request;
class ConnectionManager;

class Connection
{
private:
	ConnectionManager* _connectionManager;
	int _fd;
	Request* _task;
	std::time_t _lastActivity;

	Connection();
	Connection(Connection const& src);
	Connection& operator=(Connection const& rhs);
public:
	Connection(ConnectionManager* cm, int fd);
	~Connection(void);
	void setLastActivity(void);
	std::time_t getLastActivity(void) const;

	void setTask(Request* task);
	Request* getTask(void) const;
	void killTask(void);
	ConnectionManager* getConnectionManager(void) const;
	int readData(t_server const& server_config);
	int sendData(void);

};

#endif
