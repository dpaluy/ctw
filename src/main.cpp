#include <iostream>
#include <fstream>
#include <stdio.h>
#include "CTW.h"
#include "Context.h"

using namespace std;
using namespace ctw;

bool load_data_from_files(const char* valueFileName, const char* lFileName, CTW & tree)
{
	ifstream file(valueFileName);

	if(!file)
	{
	    cout << "Cannot open file: " << valueFileName <<"\n";
	    return false;
	}

	ifstream lfile(lFileName);
	if(!lfile)
	{
	    cout << "Cannot open file: " << lFileName <<"\n";
	    return false;
	}

	cout << "Load CTW from " <<  valueFileName << endl;
    ifstream::pos_type start_of_data = file.tellg();
    file.seekg(0, ios::end);
    ifstream::pos_type end_of_data = file.tellg();
    file.seekg(start_of_data);
    unsigned int signal;
    unsigned int action = 0;
    while(file >> signal){
        // L
        lfile >> action;
        tree.add(signal, action);
        int percent_done = static_cast<unsigned long >(file.tellg()) * 100 / end_of_data;
        if(percent_done > 100 || (percent_done < 0))
            percent_done = 100;

        cout << "\rProgress: " << percent_done << "%" << flush;
    }
    cout << "\r\n\n";
    file.close();
    lfile.close();
    return true;
}

bool load_set(CTW& tree)
{
    const char *files[3][2] = {
    		{"Input/input1M0N3.183K.v", "Input/input1M0N3.183K.l"},
    		{"Input/input2M0N4.775K.v", "Input/input2M0N4.775K.l"},
    		{"Input/input3M0N63.662K.v", "Input/input3M0N63.662K.l"}
    };

    srand((unsigned )(time(0)));
    for(int count = 0;count < 3;count++){
        cout << count << endl;
        for(int f = 0;f < 1;f++){
            if(false == load_data_from_files(files[f][0], files[f][1], tree))
            	return false;
        }
    }
    return true;
}

bool load_args(int argc, const char* argv[], CTW& tree)
{
	if (argc < 3)
	{
		cout << "Missing input file\nUsage: " << argv[0] << " signal.file L\n";
		cin.get();
		return false;
	}
	if(false == load_data_from_files(argv[1], argv[2], tree))
		return false;
	return true;
}

int main(int argc, const char* argv[]) {

	CTW tree;

	load_set(tree);

	tree.collectP(); // turn on collector

	if(false == load_data_from_files("Input/2input2M0N4.775K.v", "Input/2input2M0N4.775K.l", tree))
		return 1;


	tree.dumpCollector();

    tree.toXML("tree");
	std::cout << "Done\n";
	cin.get();
	return 0;
}
