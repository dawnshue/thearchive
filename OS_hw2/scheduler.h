/*
 * scheduler.h
 */

#ifndef __SCHED_H__
#define __SCHED_H__

using namespace std;

#include <deque>
#include "process.h"

//#include <cstring>
//#include <cstdio>
//#include "scheduler.h"

class Scheduler
{
    protected:
	deque<unsigned int> queued;
	vector<Process> *processV;
//	deque<unsigned int>::iterator it;

    public:
	virtual ~Scheduler() { delete processV; }
	void setProcesses(vector<Process> *v)
	{
	    this->processV = v;
	}

	int isEmpty() const { return queued.empty(); }
	int size() const { return queued.size(); }

	virtual void addProcess(unsigned int p) { queued.push_back(p); }
	virtual unsigned int getProcess()
	{
	    unsigned int p(queued.front());
	    queued.pop_front();
	    return p;
	}

	void print()
	{
	    deque<unsigned int>::iterator it = queued.begin();
	    while (it!=queued.end()) { std::cout << *it; ++it; }
	    std::cout << '\n';
	}

	//void print() { (*processV)[0].print(); }


};

class FCFS : public Scheduler
{
    public:
};

class LCFS : public Scheduler
{
    public:
	virtual ~LCFS() { delete processV; }
	virtual unsigned int getProcess()
	{
	    unsigned int p(queued.back());
	    queued.pop_back();
	    return p;
	}
};

class SJF : public Scheduler
{
    public:
	virtual ~SJF() { delete processV; }
	virtual void addProcess(unsigned int p)
	{
	    if(queued.empty()) { queued.push_back(p); }
	    else
	    {	
		std::deque<unsigned int>::iterator it=queued.begin();
		while((*processV)[p].getREM() < (*processV)[*it].getREM()
				 && it!=queued.end()) { ++it; }
		queued.insert(it, p);
	    }
	}

	virtual unsigned int getProcess()
	{
	    unsigned int p(queued.back());
	    queued.pop_back();
	    return p;
	}
};

class RR : public Scheduler
{
    public:
        virtual ~RR() { delete processV; }

};
#endif
