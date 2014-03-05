#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <string> // string
#include <iostream> // cout
#include <fstream> // file stream
#include <sstream> // string stream
#include <vector> // vector
#include <utility> // pair

#include "Scheduler.h"

using namespace std;

void die(char *errorMessage);

struct record {
	int add;
	int track;
	int issue;
	int finish;
};

int main(int argc, char **argv)
{

//-------------------READING COMMAND LINE---------------------//
    int sflag = 0; // scheduler algorithm
    string algo = "f"; // default algoritm
    char *ifile = NULL;
    int index, c;

    opterr = 0;
    while((c=getopt(argc, argv, "s:"))!=-1)
    {
        switch (c)
	{
	    case 's': sflag = 1; algo = optarg; break;
	    case '?':
		if (optopt == 's')
		{ fprintf(stderr,"Option -%c requires an argument.\n",optopt); }
		else if(isprint(optopt))
		{ fprintf(stderr, "Unknown option '-%c'.\n", optopt); }
		else
		{ fprintf(stderr,"Unknown option character '\\x%x'.\n",optopt);}
		return 1;
	    default:
		die("usage: ./iosched -s[schedalgo] [inputfile]\n");
	}
    }
    if(optind != (argc-1)) //2 to 1
	{ die("usage: ./iosched -s[schedalgo] [inputfile]"); }
    index=optind;
    ifile = argv[optind];

//------------------PARSING COMMAND LINE ARGS----------------------//

    //--- INITIALIZING THE PAGE REPLACER OBJECT
    Scheduler *sched;
    if(algo.compare("f")==0)
    { sched = new Scheduler_f(); } // f: FIFO
    else if(algo.compare("s")==0)
    { sched = new Scheduler_s(); } // s: SSTF
    else if(algo.compare("S")==0)
    { sched = new Scheduler_S(); } // S: SCAN
    else if(algo.compare("C")==0)
    { sched = new Scheduler_C(); } // C: CSCAN
    else if(algo.compare("F")==0)
    { sched = new Scheduler_F(); } // F: FSCAN
    else
    { die("invalid algorithm option: only [ fsSCF ] accepted.\n"); }

//------------------READ INPUT FILE---------------------//
    std::ifstream in2(ifile);
    string line;
    int st, tr;
    vector<record> info;
    while (getline(in2, line))
    {
	istringstream in2(line);
	if(in2 >> st >> tr)
	{
	    info.push_back(record());
		info[info.size()-1].add = st;
		info[info.size()-1].track = tr;
	}
    }
    in2.close();

//------------------THE PROGRAM---------------------//
    int total_time, tot_movement, max_waittime;
    int head = 0;
    int time = 0;
    int job = -1;
    int move = 0;
    int dir = 1; // direction; +1 increasing, -1 decreasing
    int next = 0;
    while(time>-1)
    {
	time++;
	if(job!=-1 && head!=info[job].track) // move head closer to IO track
	{
	    if(head<info[job].track) { head++; dir=1; }
	    else { head--; dir=-1; }
	    move++;
	}

	while(next<(int)info.size() && time==info[next].add) // adding job to scheduler
	{
	    sched->add(next,info[next].track);
	    //printf("%d:     %d add %d\n", time, next, info[next].track);
	    next++;
	}

	if(job!=-1 && head==info[job].track) // finish job/reached track
	{
	    info[job].finish = time;
	    //printf("%d:     %d finish %d\n",time,job,time-info[job].add);
	    job = -1;
	}

	while(job==-1 && !sched->isEmpty()) // get next IO request
	{
	    job = sched->getJob(head,dir);
	    info[job].issue = time;
	    //printf("%d:     %d issue %d %d\n",time,job,info[job].track,head);
	    if(info[job].track==head)
	    {
    		if(sched->trigger()==1) { dir = 1; }
		info[job].finish = time;
		//printf("%d:     %d finish %d\n",time,job,time-info[job].add);
		job = -1;
	    }
	}

	if(next>=(int)info.size() && job==-1 && sched->isEmpty()) //end while looping
	{
		total_time = time;
		time = -1;
	}
    } // END WHILE

//------------------PRINTING END OPTIONS--------------------//
    double avg_turnaround = 0;
    double avg_waittime = 0;
    max_waittime = 0;

    tot_movement = move;
    for(int b=0; b<(int)info.size(); b++)
    {
	avg_turnaround = avg_turnaround+(info[b].finish-info[b].add);
	avg_waittime = avg_waittime+(info[b].issue - info[b].add);
	if((info[b].issue-info[b].add)>max_waittime)
		{ max_waittime = info[b].issue-info[b].add; }
    }
    avg_turnaround = (double)avg_turnaround/info.size();
    avg_waittime = (double)avg_waittime/info.size();

    printf("SUM: %d %d %.2lf %.2lf %d\n", total_time, tot_movement, 
		avg_turnaround, avg_waittime, max_waittime);

//-------------------CLEAN UP / END OF PROG----------------//
    return 0;
}

void die(char *errorMessage)
{
    perror(errorMessage);
    exit(1);
}
