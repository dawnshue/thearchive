/*
 * process.h
 */

#ifndef __PROCESS_H__
#define __PROCESS_H__

using namespace std;

//#include <iostream>

class Process
{
    private:
	int AT; // arrival time
	int TC; // total CPU time
	int CB; // CPU burst
	int IO; // IO burst

	int REM; // CPU remaining
	int ready; // time when [last] ready (time stamp)

	int FT; // finishing time
	int TT; // turnaround time
	int IT; // IO time
	int CW; // CPU waiting time

	int RR; // round robin?

    public:

	// default constructor
	Process(int at, int tc, int cb, int io);

	// getter methods
	int getAT() const { return AT; }
	int getTC() const { return TC; }
	int getCB() const { return CB; }
	int getIO() const { return IO; }
	int getREM() const { return REM; }
	int getReady() const { return ready; }
	int getCW() const { return CW; }
	int getIT() const { return IT; }
	int getFT() const { return FT; }
	int getTT() const { return TT; }
	int getRR() const { return RR; }
	// setting/modifying methods
	void setFT(int ft)
	{
	    FT = ft;
	    TT = FT - AT;
	}
	void setREM(int cb) { REM = REM - cb; }
	void setCW(int t) { CW = CW + (t-ready); }
	void setReady(int r) { ready = r; }
	void setIT(int i) { IT = IT+i; }
	void setRR(int r) { RR = r; }
	// printing methods
	void print();

};

Process::Process(int at, int tc, int cb, int io)
{
    AT = at;
    TC = tc;
    CB = cb;
    IO = io;

    REM = TC;
    ready = AT;
    RR = 0;

    FT = 0;
    TT = 0;
    IT = 0;
    CW = 0;
}

void Process::print()
{
    printf("%4i %4i %4i %4i | %4i %4i %4i %4i\n",
        AT, TC, CB, IO, FT, TT, IT, CW);
}

#endif
