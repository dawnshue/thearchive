#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Frame.h"
#include "VSpace.h"
#include "Replacer.h"

using namespace std;

/*int *rands;
int rofs=0;
int rands_max=0;*/

void die(char *errorMessage);
//int random(int burst);
//void generate_rands(char* rfile);

int main(int argc, char **argv)
{

//-------------------READING COMMAND LINE---------------------//
    int aflag = 0;
    int oflag = 0;
    int fflag = 0;
    string algo = "l";
    string options = "";
    string fvalue = "32";
    char *ifile = NULL;
    char *rfile = NULL;
    int index, c;

    opterr = 0;
    while((c=getopt(argc, argv, "a:o:f:"))!=-1)
    {
        switch (c)
	{
	    case 'a': aflag = 1; algo = optarg; break;
	    case 'o': oflag = 1; options = optarg; break;
	    case 'f': fflag = 1; fvalue = optarg; break;
	    case '?':
		if (optopt == 's')
		{ fprintf(stderr, "Option -%c requires an argument.\n", optopt); }
		else if(isprint(optopt))
		{ fprintf(stderr, "Unknown option '-%c'.\n", optopt); }
		else
		{ fprintf(stderr, "Unknown option character '\\x%x'.\n", optopt); }
		return 1;
	    default:
		die("usage: ./mmu [-a<algo>] [-o<options>] [-f<num_frames>] inputfile randomfile\n");
	}
    }
    if(optind != (argc-2))
	{ die("usage: ./mmu [-a<algo>] [-o<options>] [-f<num_frames>] inputfile randomfile\n"); }
    index=optind;
    ifile = argv[optind++];
    rfile = argv[optind];
    /*{
	cout << "algo = " << algo << ", options = " << options;
	cout << ", frames = " << fvalue << "\n";
	printf("ifile = %s, rfile = %s\n", ifile, rfile);
    }*/
//------------------PARSING COMMAND LINE ARGS----------------------//
    //--- READING RANDOM # FILE
    //generate_rands(rfile);

    //--- INITIALIZING VIRTUAL SPACE OBJECT
    VSpace vtable;

    //--- INITIALIZING PHYSICAL FRAME OBJECT
    int frames = atoi(fvalue.c_str());
    Frame physical(frames);

    //--- INITIALIZING THE PAGE REPLACER OBJECT
    Replacer *replacer;
    if(algo.compare("l")==0)
    { replacer = new Replacer_l(); }
    else if(algo.compare("N")==0)
    { replacer = new Replacer_N(); }
    else if(algo.compare("r")==0)
    { replacer = new Replacer_r(); }
    else if(algo.compare("f")==0)
    { replacer = new Replacer_f(); }
    else if(algo.compare("s")==0)
    { replacer = new Replacer_s(); }
    else if(algo.compare("c")==0)
    { replacer = new Replacer_c(); }
    else if(algo.compare("C")==0)
    { replacer = new Replacer_C(); }
    else if(algo.compare("a")==0)
    { replacer = new Replacer_a(); }
    else if(algo.compare("A")==0)
    { replacer = new Replacer_A(); }
    else
    { die("invalid algorithm option: only [ NlrfscCaA ] accepted.\n"); }
    replacer->setVSpace(&vtable);
    replacer->setFrame(&physical,frames);
    replacer->setRands(rfile);
    if(options.find("O")!=string::npos) { replacer->setO(); }
//------------------READ INPUT FILE---------------------//
    std::ifstream in2(ifile);
    int read, instruct;
    string line;

    int inst_count = 0;
    while (getline(in2, line))
    {
	istringstream in2(line);
	if(in2 >> read >> instruct)
	{
	    //cout << read << " " << instruct << "\n";
	    //if(inst_count==3) { physical.set(1,1); }
	    if(options.find("O")!=string::npos)
		{ printf("==> inst: %d %d\n", read, instruct); }
	    replacer->next(read, instruct);
	    if(options.find("p")!=string::npos) { vtable.print(); }
	    if(options.find("f")!=string::npos)
	    {
		physical.print();
		replacer->print();
	    }
	    inst_count++;
	}
    }
    in2.close();

//------------------PRINTING END OPTIONS--------------------//
    if(options.find("P")!=string::npos) { vtable.print(); }
    if(options.find("F")!=string::npos) { physical.print(); printf("\n"); }

    unsigned long long int totalcost = inst_count+(replacer->getUnmaps()*400);
	totalcost = totalcost+(replacer->getMaps())*400;
	totalcost = totalcost+(replacer->getIns()+replacer->getOuts())*3000;
	totalcost = totalcost+replacer->getZeros()*150;
    if(options.find("S")!=string::npos)
    {
	printf("SUM %d U=%d M=%d I=%d O=%d Z=%d ===> %llu\n",
		inst_count, replacer->getUnmaps(), replacer->getMaps(), 
		replacer->getIns(), replacer->getOuts(), replacer->getZeros(), 
		totalcost);
    }

//-------------------CLEAN UP / END OF PROG----------------//
//    delete [] rands;
    return 0;
}

/*void generate_rands(char* rfile)
{
    std::ifstream in(rfile);
    in >> rands_max;
    rands = new int[rands_max];
    int rcount = 0;
    while(rcount < rands_max)
    { in >> rands[rcount++]; }
    in.close();
    rofs = 0;
//	{ printf("random: %d\n", random(10)); }
}

int random(int burst)
{
    if(rofs==rands_max)
	{ rofs = 0; }
    return rands[rofs++];
}*/

void die(char *errorMessage)
{
    perror(errorMessage);
    exit(1);
}
