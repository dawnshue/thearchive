#ifndef __REPLACER_H__
#define __REPLACER_H__

#include <deque>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>

using namespace std;

#include "Frame.h"
#include "VSpace.h"

class Replacer
{
    protected:
	VSpace *vspace;
	Frame *frame;
	const static int vsize = 64;
	int fsize;
	int unmaps;
	int maps;
	int ins;
	int outs;
	int zeros;
	int inst_count;
	int *rands;
	int rofs;
	int rands_max;
	int clock;
	int hand;
	int printout;
    public:
	Replacer();
	virtual ~Replacer() {}
	virtual void initialize() {}
	void setVSpace(VSpace *v) { this->vspace = v; }
	void setFrame(Frame *f, int fs) { this->frame = f; fsize = fs; }
	void setO() { printout = 1; }
	int getUnmaps() const { return unmaps; }
	int getMaps() const { return maps; }
	int getIns() const { return ins; }
	int getOuts() const { return outs; }
	int getZeros() const { return zeros; }
	void zero(int f);
	void map(int page, int f);
	virtual void touched(int a) {}
	virtual void add(int a) {}
	virtual void print() { printf("\n"); }
	virtual int replace() { return 0; }
	void unmap(int page, int f);
	virtual void setRands(char* rfile) {}
	int random();
	void swapIn(int page, int f);
	void generateRands(char* rfile);
	virtual void next(int read, int page)
	{
	    int f;
	    int p;
	    if(read==0)
	    {
		if(vspace->isPresent(page)==1) {
		    vspace->setReferenced(page);
		    touched(vspace->getFrame(page));
		}
		else if(vspace->isSwapped(page)==1) {
		    f = frame->getEmpty();
		    if(f!=-1) { swapIn(page, f); }
		    else {
			f = replace();
			p = frame->getPage(f);
			unmap(p,f); swapIn(page,f);
		    }
		    map(page, f);
		}
		else if(vspace->isPresent(page)==0) {
		    f = frame->getEmpty();
		    if(f!=-1) { zero(f); }
		    else {
			f = replace();
			p = frame->getPage(f);
			unmap(p, f); zero(f);
		    }
		    map(page, f);
		}
	    }
	    else //write
	    {
		if(vspace->isPresent(page)==1) {
		    vspace->setModified(page);
		    vspace->setReferenced(page);
		    touched(vspace->getFrame(page));
		}
		else if(vspace->isSwapped(page)==1) {
		    f = frame->getEmpty();
		    if(f!=-1) { swapIn(page, f); }
		    else
		    {
			f = replace();
			p = frame->getPage(f);
			unmap(p, f);
			swapIn(page, f);
		    }
		    map(page, f);
		    vspace->setModified(page);
		}
		else if(vspace->isPresent(page)==0) {
		    f = frame->getEmpty();
		    if(f!=-1) { zero(f); }
		    else
		    {
			f = replace();
			p = frame->getPage(f);
			unmap(p, f); zero(f);
		    }
		    map(page, f);
		    vspace->setModified(page);
		}
	    }
	    inst_count++;
	}
};
Replacer::Replacer()
{
	inst_count = 0;
	unmaps = 0;
	maps = 0;
	ins = 0;
	outs = 0;
	zeros = 0;
	fsize = 0;
	clock = 0;
	hand = 0;
	printout = 0;
	initialize();
}
void Replacer::zero(int f)
{
    zeros++;
    frame->zero(f);
    if(printout==1) { printf("%d: ZERO      %3d\n",inst_count,f); }
}
void Replacer::swapIn(int page, int f)
{
    ins++;
    if(printout==1) { printf("%d: IN    %3d %3d\n",inst_count,page,f); }
}
void Replacer::map(int page, int f)
{
    maps++;
    vspace->map(page, f);
    frame->map(page, f);
    add(f);
    if(printout==1) { printf("%d: MAP   %3d %3d\n",inst_count,page,f); }
}
void Replacer::unmap(int page, int f)
{
    unmaps++;
    if(printout==1) { printf("%d: UNMAP %3d %3d\n", inst_count, page, f); }
    if(vspace->isModified(page)==1)
    {
	if(printout==1) { printf("%d: OUT   %3d %3d\n",inst_count,page,f); }
	outs++;
	vspace->setSwapped(page);
	vspace->unModify(page);
	vspace->unmap(page);
    }
    else { vspace->unmap(page); /*zero(f);*/ }
}
class Replacer_N : public Replacer
{
    protected:
	vector<int> class1;
	vector<int> class2;
	vector<int> class3;
	vector<int> class4;
    public:
	virtual void setRands(char* rfile){ generateRands(rfile); }
	virtual ~Replacer_N() { delete [] rands; }
	virtual int replace()
	{
	    clock++;
	    if(clock==10) { vspace->resetRef(); clock = 0; }
	    class1.clear();
	    class2.clear();
	    class3.clear();
	    class4.clear();
	    for(int i=0; i<vsize; i++)
	    {
		if(vspace->isPresent(i)==1)
		{
		    if(vspace->isReferenced(i)==0)
		    {
		        if(vspace->isModified(i)==0) { class1.push_back(i); }
		        else { class2.push_back(i); /*printf("pushed\n");*/ }
		    } else
		    {
		        if(vspace->isModified(i)==0) { class3.push_back(i); }
		        else { class4.push_back(i); }
		    }
		}
	    }
	    //int r = random();
	    //printf("%d\n",r);
	    int p = 0;
	    if((int)class1.size()>0) { p = class1[random()%(int)class1.size()]; }
	    else if((int)class2.size()>0) { p = class2[random()%(int)class2.size()]; }
	    else if((int)class3.size()>0) { p = class3[random()%(int)class3.size()]; }
	    else if((int)class4.size()>0) { p = class4[random()%(int)class4.size()]; }
	    //printf("random page: %d\n",p);
	    return (int)vspace->getFrame(p);
	}
};

class Replacer_l : public Replacer
{
    protected:
	deque<int> used;
    public:
	/*virtual void print()
	{
	    printf(" || ");
	    for(int i=0; i<(int)used.size(); i++) { printf("%d ",used[i]); }
	    printf("\n");
	}*/
	virtual void add(int a) { touched(a); }
	virtual void touched(int a)
	{
	    for(int i=0; i<(int)used.size(); i++)
	    { if(used[i]==a) { used.erase(used.begin()+i); } }
	    used.push_back(a);
	}
	virtual int replace()
	{
	    int f = used.front();
	    used.pop_front();
	    return f;
	}
};

class Replacer_r : public Replacer
{
    public:	
	virtual void setRands(char* rfile){ generateRands(rfile); }
	virtual ~Replacer_r() { delete [] rands; }
	virtual int replace()
	{
		return random()%fsize;
	}
};

class Replacer_f : public Replacer
{
    protected:
	deque<int> fifo;
    public:
	virtual void print()
	{
	    printf(" || ");
	    for(int i=0; i<(int)fifo.size(); i++) { printf("%d ",fifo[i]); }
	    printf("\n");
	}
	virtual void add(int a)
	{
	    if((int)fifo.size()<fsize) { fifo.push_back(a); }
	}
	virtual int replace()
	{
	    int f = fifo.front();
	    fifo.pop_front();
	    return f;
	}
};

class Replacer_s : public Replacer
{
    protected:
	deque<int> fifo;
    public:
	virtual void print()
	{
	    printf(" || ");
	    for(int i=0; i<(int)fifo.size(); i++) { printf("%d ",fifo[i]); }
	    printf("\n");
	}
	virtual void add(int a)
	{
	    if((int)fifo.size()<fsize) { fifo.push_back(a); }
	}
	virtual int replace()
	{
	    int i = 0;
	    int f = 0;
	    int p = 0;
	    while(i<(int)fifo.size()) {
		f = fifo.front();
		p = frame->getPage(f);
		if(vspace->isReferenced(p)==1) {
		    vspace->unReference(p);
		    fifo.pop_front();
		    fifo.push_back(f);
		}
		else { fifo.pop_front(); return f; }
	    }
	    return f;
	}
};

class Replacer_c : public Replacer
{
    protected:
	//deque<int> llist;
    public:
	virtual void print()
	{
	    printf(" || hand = %d\n", hand);
	}
	/*virtual void add(int a)
	{
	    if(hand==0) { llist.push_back(a); }
	    else { llist.insert(llist.begin()+(hand-1), a); }
	}*/
	virtual int replace()
	{
	    int f = -1; int p = 0;
	    while(hand<fsize) {
		f = hand;
		//f = llist[hand];
		p = frame->getPage(f);
		if(vspace->isReferenced(p)==1){
		    vspace->unReference(p);
		    hand++; if(hand==fsize) { hand = 0; }
		} else {
		    hand++; if(hand==fsize) { hand = 0; }
		    //if(hand>0) { llist.erase(llist.begin()+(hand-1)); }
		    //else { llist.pop_back(); }
		    return f;
		}
	    }
	    return f;
	}
};

class Replacer_C : public Replacer
{
    protected:
	//deque<int> llist;
    public:
	virtual void print()
	{
	    printf(" || hand = %d\n", hand);
	}
	/*virtual void add(int a)
	{
	    if(hand==0) { llist.push_back(frame->getPage(a)); }
	    else { llist.insert(llist.begin()+(hand-1), frame->getPage(a)); }
	}*/
	virtual int replace()
	{
	    int f = -1; int p = -1;
	    while(hand<vsize) {
		p = hand;
		if(vspace->isPresent(p)==0){
		    hand++;
		    if(hand==vsize) { hand = 0; }
		} else if(vspace->isReferenced(p)==1) {
		    vspace->unReference(p);
		    hand++; if(hand==vsize) { hand = 0; }
		} else {
		    f = vspace->getFrame(p);
		    hand++; if(hand==vsize) { hand = 0; }
		    return f;
		}
	    }
	    return f;
	}
};

struct count32 {
	unsigned int p:32;
};
class Replacer_a : public Replacer
{
    protected:
	vector<unsigned int> counter;
    public:
	/*virtual void initialize()
	{ printf("init\n");
	    for(int a=0; a<fsize; a++)
	    {counter.push_back(0);}
	}*/
	void shift()
	{ //printf("shift\n");
	    for(int a=0; a<fsize; a++)
	    { counter[a] = counter[a]>>1;
		//counter[a] = counter[a] & (2^31-1);
		if(vspace->isReferenced(frame->getPage(a))==1)
			{ counter[a] = counter[a] | (1<<31); 
			vspace->unReference(frame->getPage(a)); }
	    }
	}
	/*virtual void touched(int f)
	{
	    if(f<(int)counter.size()) counter[f] = 0;
	}*/
	virtual void add(int f)
	{ //printf("add\n");
	    if((int)counter.size()<fsize)
		{ counter.insert(counter.begin()+f,0); }
	    counter[f] = 0;
	}
	virtual void print()
	{ //printf("print\n");
	    printf(" || ");
	    for(int a=0; a<(int)counter.size(); a++)
	    { if(frame->getPage(a)>=0)
		{ printf("%d:%u",a,counter[a]); //printBits(counter[a]);
			printf(" "); }
	    }
	    printf("\n");
	}
	void printBits(unsigned int x)
	{
	    int a = 0; int b = 0; int c = 0; int d = 0;
	    for(int i=0; i<8; i++)
	    {
		if((x&(1<<(32-i*4-1)))>0) a = 1;
		if((x&(1<<(32-i*4-2)))>0) b = 1;
		if((x&(1<<(32-i*4-3)))>0) c = 1;
		if((x&(1<<(32-i*4-4)))>0) d = 1;
		//if(a || b || c || d)
		printf("%d",a*8+b*4+c*2+d*1);
		a = 0; b = 0; c = 0; d = 0;
	    }
	}
	virtual int replace()
	{ //printf("replace\n");
	    shift();
	    int min = 0;
	    for(int i=0; i<(int)counter.size(); i++)
	    { //vspace->unReference(frame->getPage(i));
		if(counter[i]<counter[min]) { min = i; }
	    }
	    return min;
	}
};
struct AAA {
	int page;
	unsigned int x:32; // 32 bit unsigned int?
};
class Replacer_A : public Replacer
{
    protected:
	vector<AAA> v;
    public:
	void shift()
	{
	    for(int a=0; a<(int)v.size(); a++)
	    {	v[a].x = v[a].x >>1;
		if(vspace->isReferenced(v[a].page)==1)
			{ v[a].x = v[a].x | (1<<31);
			  vspace->unReference(v[a].page); }
	    }
	}
	virtual void add(int f)
	{
	    if((int)v.size()<fsize)
	    {	v.push_back(AAA());
		v[(int)v.size()-1].page = frame->getPage(f);
		v[(int)v.size()-1].x = 0;
	    }
	    v[f].page = frame->getPage(f);
	    v[f].x = 0;
	}
	virtual void print()
	{
	    printf(" || ");
	    for(int a=0; a<vsize; a++)
	    { if(vspace->isPresent(a)) 
		{ printf("%d:%u",a,v[vspace->getFrame(a)].x); printf(" "); }
		else { printf("* "); }
	    }
	    printf("\n");
	}
	virtual int replace()
	{
	    shift();
	    int min = -1;
	    int p = 0;
	    while(p<vsize)
	    {
		if(vspace->isPresent(p)==1)
		{
			if(min == -1) { min = p; }
			else if(v[vspace->getFrame(p)].x<v[vspace->getFrame(min)].x)
				{ min = p; }
		}
		p++;
	    }
	    return vspace->getFrame(min);
	}
};
void Replacer::generateRands(char* rfile)
{
	std::ifstream in(rfile);
	in >> rands_max;
	rands = new int[rands_max];
	int rcount = 0;
	while(rcount < rands_max)
	{ in >> rands[rcount++]; }
	in.close();
	rofs = 0;
}
int Replacer::random()
{
	if(rofs==rands_max) { rofs = 0; }
	return rands[rofs++];
}
#endif
