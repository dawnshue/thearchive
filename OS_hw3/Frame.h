#ifndef __FRAME_H__
#define __FRAME_H__

class Replacer;

class Frame
{
    protected:
	const int size;
	int* table;

    public:
	Frame(int x): size(x), table(new int[x])
	{
	    for(int i=0; i<x; i++)
	    { table[i] = -1; }
	}
	~Frame() { delete[] table; }

	//void set(int pos, int val) { table[pos] = val; }
	int getEmpty();
	void map(int page, int f);
	void print();
	void zero(int f);
	int getPage(int f) { test(f); return table[f]; }
	void test(int f);

};
int Frame::getEmpty()
{
    int pos = 0;
    while(table[pos]!=-1)
    {
	pos++;
	if (!(pos<size)) { return -1; }
    }
    return pos;
}
void Frame::map(int page, int f)
{
    test(f);
    table[f] = page;
}
void Frame::zero(int f)
{
    test(f);
    table[f] = -1;
}
void Frame::print()
{
    for(int i=0; i<size; i++)
    {
	if(table[i]==-1) { printf("* "); }
	    else { printf("%d ", table[i]); }
    }
    //printf("\n");
}
void Frame::test(int f)
{
    if(f<0 || f>=size){ printf("ERROR: FRAME f=%d\n",f); exit(1); }
}
#endif
