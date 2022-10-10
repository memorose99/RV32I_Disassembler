//***************************************************************************
//
//  Guillermo Rizo-Mut
//  Z1939912
//  CSCI 463 - PE1
//
//  I certify that this is my own work and where appropriate an extension 
//  of the starter code provided for the assignment.
//
//***************************************************************************
#include <iostream>
#include "hex.h"
#include "memory.h"
#include "rv32i_decode.h"
#include <unistd.h>
#include <sstream>

using namespace std;

/**
 * Prints Usage error message to stderr and ends program
 ********************************************************************/
static void usage()
{
	cerr << "Usage: rv32i [-m hex-mem-size] infile" << endl;
	cerr << "    -m specify memory size (default = 0x100)" << endl;
	exit(1);
}

/**
 * Decodes and prints each instruction in a loop
 * ****************************************************************/
static void disassemble(const memory &mem)
{
	uint32_t memAddr = 0;
	uint32_t output = 0;
	std::string formatOutput;

	for(unsigned i=0; i < mem.get_size()/4; i++)
	{
		output = mem.get32(memAddr);
		formatOutput = rv32i_decode::decode(memAddr, output);
		
		std::cout << hex::to_hex32(memAddr) << ": " 
		<< hex::to_hex32(output) << " " << formatOutput << std::endl;
		
		memAddr += 4;
	}
}

/**
 * Main function in program to simulate RV32I disassembler
 *
 * @param argc Integer argument
 * @param argv Char pointer point to a pointer  argument
 *
 * @return 0
 ***************************************************************************/
int main(int argc, char **argv)
{
	uint32_t memory_limit = 0x100; // default memory size = 256 bytes
	
	int opt;
	
	while ((opt = getopt(argc, argv, "m:")) != -1)
	{
		switch (opt)
		{
			case 'm':
				{
					std::istringstream iss(optarg);
					iss >> std::hex >> memory_limit;
				}
				break;
			default: /* ’?’ */
				usage();
		}
	}
	
	if (optind >= argc)
		usage(); // missing filename
	
	memory mem(memory_limit);
	
	if (!mem.load_file(argv[optind]))
		usage();
	
	disassemble(mem);
	mem.dump();
	
	return 0;
}
