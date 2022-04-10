#ifndef CGITASK_HPP
#define CGITASK_HPP

#include "ATask.hpp"

class CGITask : public ATask
{
private:
    /* data */
public:
    CGITask(/* args */);
    virtual ~CGITask();
    int collectData(void);
	int executeTask(void);
  	int sendResult(void);

};

#endif
