#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include <stdlib.h>
#include <deque>

using namespace std;

struct vpair {
	int inst;
	int track;
};

class Scheduler
{
    protected:
	deque<vpair> deq; //holds waiting IO requests
	int curr;
	int trig;

    public:
	Scheduler();
	virtual ~Scheduler() {}
	virtual bool isEmpty() { return deq.empty(); }
	int trigger() { return trig; }
	virtual void add(int val, int track)
	{
		deq.push_back(vpair());
		deq[deq.size()-1].inst = val;
		deq[deq.size()-1].track = track;
	} // end ADD()

	virtual int getJob(int head, int dir) // FIFO method
	{
		int j = deq[0].inst;
		deq.pop_front();
		return j;
	} // end GETJOB()

}; // end Scheduler class

Scheduler::Scheduler()
{
	curr = 0;
	trig = 0;
} // end Scheduler::Scheduler() constructor

class Scheduler_f : public Scheduler
{
    protected:
    public:
}; // end Scheduler_f class

class Scheduler_s : public Scheduler
{
    protected:
    public:
	virtual int getJob(int head, int dir) //shortest seek time
	{
	    int best = 0;
	    for(int u=1; u<(int)deq.size(); u++)
	    {
		if(abs(deq[u].track-head)<abs(deq[best].track-head))
			{ best = u; }
	    }
	    int ret = deq[best].inst;
	    deq.erase(deq.begin()+best);
	    return ret;
	}
}; // end Scheduler_s class

class Scheduler_S : public Scheduler
{
    protected:
    public:
	virtual int getJob(int head, int dir) //closest in direction of travel
	{
	    int best = -1;
	    for(int p=0; p<(int)deq.size(); p++)
	    {
		if(dir==1 && deq[p].track>=head)
		{ best = p; break; }
		else if(dir==-1 && deq[p].track<=head)
		{ best = p; break; }
	    }
	    if(best!=-1)
	    {
		for(int u=(best+1); u<(int)deq.size(); u++)
	    	{
		    if(dir==1 && deq[u].track>=head && 
				(deq[u].track-head)<(deq[best].track-head))
			{ best = u; }
		    else if(dir==-1 && deq[u].track<=head &&
				(deq[u].track-head)>(deq[best].track-head))
			{ best = u; }
	    	}
	    }
	    else
	    {
		best = 0;
		for(int u=1; u<(int)deq.size(); u++)
		{
		    if(abs(deq[u].track-head)<abs(deq[best].track-head))
		    { best = u; }
		}
	    }
	    int ret = deq[best].inst;
	    deq.erase(deq.begin()+best);
	    return ret;
	}
}; // end Scheduler_S class

class Scheduler_C : public Scheduler
{
    protected:
    public:
	virtual int getJob(int head, int dir) //wrap around
	{
	    int best = -1;
	    for(int p=0; p<(int)deq.size(); p++)
	    {
		if(deq[p].track>=head)
		{ best = p; break; }
	    }
	    if(best!=-1)
	    {
		for(int u=(best+1); u<(int)deq.size(); u++)
	    	{
		    if(deq[u].track>=head && (deq[u].track-head)<(deq[best].track-head))
			{ best = u; }
	    	}
	    }
	    else
	    {
		best = 0;
		for(int u=1; u<(int)deq.size(); u++)
		{
		    if(deq[u].track<deq[best].track)
		    { best = u; }
		}
	    }
	    int ret = deq[best].inst;
	    deq.erase(deq.begin()+best);
	    return ret;
	}
}; // end Scheduler_C class

class Scheduler_F : public Scheduler
{
    protected:
	deque<vpair> deq2;
    public:
	virtual void add(int val, int track)
	{
		if(curr==0) // the current queue being added to
		{
			deq.push_back(vpair());
			deq[deq.size()-1].inst = val;
			deq[deq.size()-1].track = track;
			//printf("add to 0 ");
		}
		else
		{
			deq2.push_back(vpair());
			deq2[deq2.size()-1].inst = val;
			deq2[deq2.size()-1].track = track;
			//printf("add to 2 ");
		}
	} // end ADD()

	virtual bool isEmpty()
	{
		return (deq.empty() && deq2.empty());
	} // end ISEMPTY()

	virtual int getJob(int head, int dir) //closest in direction of travel
	{
	    int best = -1;
	    int ret;
	    trig = 0;
	    if(curr==0 && deq2.empty()) { curr = 2; dir = 1; trig = 1; }
	    else if(curr==2 && deq.empty()) { curr = 0; dir = 1; trig = 1; }

	    if(curr==2)
	    {
		    for(int p=0; p<(int)deq.size(); p++)
		    {
 			if(dir==1 && deq[p].track>=head)
			{ best = p; break; }
			else if(dir==-1 && deq[p].track<=head)
			{ best = p; break; }
		    }
		    if(best!=-1)
		    {
			for(int u=(best+1); u<(int)deq.size(); u++)
		    	{
			    if(dir==1 && deq[u].track>=head && 
					(deq[u].track-head)<(deq[best].track-head))
				{ best = u; }
			    else if(dir==-1 && deq[u].track<=head &&
					(deq[u].track-head)>(deq[best].track-head))
				{ best = u; }
		    	}
		    }
		    else
		    {
			best = 0;
			for(int u=1; u<(int)deq.size(); u++)
			{
			    if(abs(deq[u].track-head)<abs(deq[best].track-head))
			    { best = u; }
			}
		    }
		    ret = deq[best].inst;
		    deq.erase(deq.begin()+best);
	    } // end CURR==2
	    else if(curr==0)
	    {
		    for(int p=0; p<(int)deq2.size(); p++)
		    {
 			if(dir==1 && deq2[p].track>=head)
			{ best = p; break; }
			else if(dir==-1 && deq2[p].track<=head)
			{ best = p; break; }
		    }
		    if(best!=-1)
		    {
			for(int u=(best+1); u<(int)deq2.size(); u++)
		    	{
			    if(dir==1 && deq2[u].track>=head && 
					(deq2[u].track-head)<(deq2[best].track-head))
				{ best = u; }
			    else if(dir==-1 && deq2[u].track<=head &&
					(deq2[u].track-head)>(deq2[best].track-head))
				{ best = u; }
		    	}
		    }
		    else
		    {
			best = 0;
			for(int u=1; u<(int)deq2.size(); u++)
			{
			    if(abs(deq2[u].track-head)<abs(deq2[best].track-head))
			    { best = u; }
			}
		    }
		ret = deq2[best].inst;
		deq2.erase(deq2.begin()+best);
	    } // end CURR==2

	    return ret;
	}
}; // end Scheduler_F class

#endif
