#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <deque>

#include "process.h"
#include "scheduler.h"

//using namespace std;

int *g_randvals;
int g_ofs=0;
int g_max=0;

void die(char *errorMessage)
{
    perror(errorMessage);
    exit(1);
}

int myrandom(int burst)
{
    if(g_ofs==g_max)
        { g_ofs = 0; }
    return 1+(g_randvals[g_ofs++]%burst);
}

int main(int argc, char **argv)
{
    //Re-used variables
    vector<Process>::iterator it;
    deque<int>::iterator dit;
    int x=0;

//-----------------------READING COMMAND LINE-----------------------//
    int vflag = 0;
    int sflag = 0;
    char *schar = NULL;
    char *ifile = NULL;
    char *rfile = NULL;
    int index;
    int c;

    opterr = 0;
    while((c=getopt(argc, argv, "vs:"))!=-1) 
    {
        switch (c)
        {
            case 'v':
                vflag = 1;
                break;
            case 's':
                sflag = 1;
                schar = optarg;
                break;
            case '?':
                if (optopt == 's')
                {
                    fprintf(stderr, 
			"Option -%c requires an argument.\n", optopt);
                } else if(isprint(optopt)) {
                    fprintf(stderr, "Unknown option '-%c'.\n", optopt);
                } else {
                    fprintf(stderr, 
			"Unknown option character '\\x%x'.\n", optopt);
                }
                return 1;
            default:
                 die("usage: ./OSlab2 [-v] [-s<schedspec>] inputfile rfile\n");
        }
    }
    //printf("vflag = %d\n", vflag);
    if(optind != (argc-2))
        { die("usage: ./OSlab2 [-v] [-s<schedspec>] inputfile rfile\n"); }
    index=optind;
    ifile = argv[optind++];
    rfile = argv[optind];
    //printf("schar = %s, ifile = %s, rfile = %s\n", schar, ifile, rfile);
    /*for(index=optind; index<argc; index++) 
    {
        printf("Non-option argument %s\n", argv[index]);
    }*/

//--------------------PARSING COMMAND LINE ARGS----------------------//
    //-----reading RANDFILE
    std::ifstream in(rfile);
    int rcount=0;
    in >> g_max;
    g_randvals = new int[g_max];
    while(rcount<g_max)
    {
        in >> g_randvals[rcount++];
    }
    in.close();
    g_ofs = 0;
    //printf("size %d|top %d|end %d\n",g_max,g_randvals[0],g_randvals[g_max-1]);
    //printf("random: %d\n", myrandom(10));

    //-----reading INPUTFILE of PROCESS objects
    std::ifstream in2(ifile);
    int at, tc, cpu, io;
    vector<Process> processV;
    std::vector<int> ATarray;
    while (in2 >> at)
    {
	ATarray.push_back(at);
        in2 >> tc;
        in2 >> cpu;
        in2 >> io;
        processV.push_back(Process(at, tc, cpu, io));
        //processV[processV.size()-1].print();
    }
    in2.close();

    //-----setting SCHEDULER based on SVALUE
    Scheduler *sched;
    int quantum = 0;
    if(sflag==0)
        { schar = ""; }
    std::string svalue(schar);
    if(sflag==0 || svalue.compare("F")==0) // FCFS: First Come First Serve
    {
        printf("FCFS\n");
        sched = new FCFS();
    } else if(svalue.compare("L")==0) // LCFS: Last Come First Serve
    {
        printf("LCFS\n");
        sched = new LCFS();
    } else if(svalue.compare("S")==0) // SJF: Shortest Job First
    {
        printf("SJF\n");
        sched = new SJF();
    } else if(svalue.compare(0,1,"R")==0) // RR: Round Robin
    {
        std::string intR = svalue.substr(1);
        std::cout << "RR " << intR << '\n';
        quantum = atoi(intR.c_str());
        sched = new RR();
    } else
    {   sched = new Scheduler();
	die("invalid scheduler: only [ FLS | R<num> ] accepted.\n");
    }

//--------------------START PROGRAM LOOP----------------------------//
    int time = 0; // GLOBAL TIME
    unsigned int a = 0; // holding current pos in ATarray
    unsigned int finished = 0; // number of processes completed

    int cb=-1; // CPU burst; time when CPU burst will finish
    int iob=-1; // IO burst
    int ib=-1; // time when IO burst will finish
    std::deque<int> active; // active processes

    int CPUcount = 0; // CPU counter
    int onCPU = -1; // -1: no processes currently using CPU

    int IOcount = 0; // IO counter
    int onIO = 0; // 0: no processes currently using IO

    sched->setProcesses(&processV);
    while (true)
    {
	// ADD PROCESSES WITH THIS ARRIVAL TIME
	while(a<ATarray.size())
	{
	    if(ATarray[a]==time) { sched->addProcess(a++); }
	    if(ATarray[a]>time) { break; }
	}

	// EVALUATE ALL PROCESSES at TS before SCHEDULING
	if(time==cb || time==ib)
	{
	    dit=active.begin();
	    while(dit!=active.end())
	    {
		if(processV[*dit].getReady()==time)
		{
		    if(*dit!=onCPU) //IO done, back to queue
		    {
			onIO--;
			sched->addProcess(*dit);
			dit = active.erase(dit);
		    } else { //CB done, move to IO burst
			if( processV[*dit].getREM()<1 )
			{
			    processV[*dit].setFT(time);
			    finished++;
			    dit = active.erase(dit);
			} else if( processV[*dit].getRR()>0 )
			{
			    sched->addProcess(*dit);
			    dit = active.erase(dit);
			} else
			{
			    onIO++;
			    iob = myrandom(processV[onCPU].getIO());
			    processV[onCPU].setIT(iob);
			    iob = iob+time;
			    processV[onCPU].setReady(iob);
			    ++dit;
			}
			onCPU = -1;
		    }
		} else { ++dit; }
	    }
	    if(!active.empty())
	    {
		dit = active.begin();
		ib = processV[*dit].getReady();
		++dit;
		while(dit!=active.end())
		{
		    if(processV[*dit].getReady()<ib)
			{ ib = processV[*dit].getReady(); }
		    ++dit;
		}
	    } else { ib=-1; }
	}

	// WAITING: GET A PROCESS
	if (onCPU==-1 && !(sched->isEmpty()))
	{
	    onCPU = sched->getProcess();
//std::cout << "onCPU: "<<onCPU<<'\n';
	    if(quantum>0)
	    {
		if(processV[onCPU].getRR()>0) { cb = processV[onCPU].getRR(); }
		else { cb = myrandom(processV[onCPU].getCB()); }
		if(quantum<cb) // Round Robin case
	    	{
	            processV[onCPU].setRR(cb-quantum);
		    cb = quantum;
	    	} else { processV[onCPU].setRR(0); }
	    } else { cb = myrandom(processV[onCPU].getCB()); }
	    if(processV[onCPU].getREM()<cb) { cb = processV[onCPU].getREM(); }
	    processV[onCPU].setREM(cb);
	    processV[onCPU].setCW(time);
	    active.push_back(onCPU);
//std::cout<<time<<" process:"<<onCPU<<" cb:"<<cb<<" REM:"
//<<processV[onCPU].getREM()<<'\n';
	    cb = cb+time;
	    processV[onCPU].setReady(cb);
	}

        if( finished == processV.size() ) // all processes finished
	    { break; }

	if( onCPU>-1 ) { CPUcount++; }
	if( onIO>0 ) { IOcount++; }
	time++;
    }

//---------------------PRINT RESULTS----------------------------------//
    int avgTT = 0;
    int avgCW = 0;
    x = 0;
    for(it = processV.begin(); it!=processV.end(); ++it)
    {
        printf("%04d: ", x++);
        it->print();
        avgTT = avgTT + it->getTT();
	avgCW = avgCW + it->getCW();
    }
    printf("SUM: %d %.2lf %.2lf %.2lf %.2lf %.3lf\n", time, 
	(double)CPUcount*100/time, (double)IOcount*100/time, (double)avgTT/x, 
		(double)avgCW/x, (double)finished/((double)time/100));

//----------------------CLEAN UP / END OF PROG-------------------------//
    delete [] g_randvals;
    return 0;
}
