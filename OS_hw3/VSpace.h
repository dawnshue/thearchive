#ifndef __VSPACE_H__
#define __VSPACE_H__

#include <vector>

class Replacer;

using namespace std;

struct pte {
	unsigned int p:1; // PRESENT
	unsigned int m:1; // MODIFIED
	unsigned int r:1; // REFERENCED
	unsigned int s:1; // PAGED OUT
	unsigned int frame:6; // stores physical frame address
	unsigned int left:22; // remaining bits for 32 total bits
};

class VSpace
{
    protected:
	vector<pte> table;
	const static int size = 64;
    public:
	VSpace() // default constructor
	{
	    for(int a=0; a<64; a++)
	    {
		table.push_back(pte());
		// set all bits to 0;
		table[a].p = 0; // not present
		table[a].m = 0; // not modified
		table[a].r = 0; // not referenced
		table[a].s = 0;
		table[a].frame = 0; // no physical frame address
		table[a].left = 0;
	    }
	}
	~VSpace() {}
	int isPresent(int p) { test(p); return table[p].p; }
	void test(int page);
	int isReferenced(int p) { test(p); return table[p].r; }
	void unReference(int p) { test(p); table[p].r = 0; }
	int isModified(int p) { test(p); return table[p].m; }
	int isSwapped(int p) { test(p); return table[p].s; }
	void setModified(int p) { test(p); table[p].m = 1; }
	void setReferenced(int p) { test(p); table[p].r = 1; }
	void unModify(int p) { test(p); table[p].m = 0; }
	void setSwapped(int p) { test(p); table[p].s = 1; }
	void unmap(int p);
	void print();
	void map(int page, int f);
	void resetRef();
	int getFrame(int p) { test(p); return table[p].frame; }
};
void VSpace::test(int page)
{
    if(page<0 || page>=size) { printf("ERROR: VSPACE p=%d??\n",page); exit(1); }
}
void VSpace::resetRef()
{
    for(int i=0; i<size; i++) { if(table[i].p==1) { table[i].r=0; } }
}
void VSpace::unmap(int page)
{
    test(page);
    table[page].frame = 0;
    table[page].p = 0;
    table[page].r = 0;
}
void VSpace::map(int page, int f)
{
    test(page);
    table[page].frame = f;
    table[page].r = 1;
    table[page].p = 1;
}
void VSpace::print()
{
    for(int i=0; i<size; i++)
    {
	if(table[i].p==1)
	{
	    printf("%d:",i);
	    if(table[i].r==1) { printf("R"); } else { printf("-"); }
	    if(table[i].m==1) { printf("M"); } else { printf("-"); }
	    if(table[i].s==1) { printf("S"); } else { printf("-"); }
	    printf(" ");
	}
	else if(table[i].s==1) { printf("# "); }
	else { printf("* "); }
    }
    printf("\n");
}

#endif
