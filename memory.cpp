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
#include "memory.h"
#include "hex.h"

/**
* Constructor for memory class that allocates siz bytes in mem vec and
* initializes every byte to 0xa5
*
* @param siz Unsigned int that indicates size of memory to allocate
****************************************************************************/
memory::memory(uint32_t siz)
{
        siz = (siz+15) & 0xfffffff0; //round the length up, mod-16
        mem.resize(siz, 0xa5);
}

/**
* Destructor for memory class that cleans up anything necessary
***************************************************************************/
memory::~memory()
{
        mem.clear();
}

/**
* Checks if the given address is present in the mem vector
*
* @param i Unsigned int containing the address to be checked
*
* @return true if address is out of range, else returns false
*************************************************************************/
bool memory::check_illegal(uint32_t i) const
{
        if(i >= mem.size())
        {
                std::cout << "WARNING: Address out of range: " << hex::to_hex0x32(i) << std::endl;
                return true;
        }
        else
        {
                return false;
        }
}

/**
* Gets the rounded up number of bytes in memory
*
* @returns The size of mem vector in unsigned int
*************************************************************************/
uint32_t memory::get_size() const
{
        return mem.size();

}

/**
* Check if addr is in the vector
*
* @param addr Address that will be checked
*
* @return addr if it is in vector, else 0
***********************************************************************/
uint8_t memory::get8(uint32_t addr) const
{
        if(check_illegal(addr) == false)
        {
                return mem[addr];
        }
        else
        {
                return 0;
        }
}

/**
* Gets two bytes and combines them using get8()
*
* @param addr Address of vector element
*
* @return Unsigned 16 bit little endian order of addr
***********************************************************************/
uint16_t memory::get16(uint32_t addr) const
{
        uint8_t moreSignificant = get8(addr);
        uint8_t lessSignificant = get8(addr + 1);

        uint16_t order = (lessSignificant << 8) | moreSignificant;
        return order;
}

/**
* Gets two halfwords and combines using get16()
*
* @param addr Address of vector element
*
* @return Unsigned 32 bit little endian order of addr
***********************************************************************/
uint32_t memory::get32(uint32_t addr) const
{
        uint16_t moreSignificant = get16(addr);
        uint16_t lessSignificant = get16(addr + 2);

        uint32_t order = (lessSignificant << 16) | moreSignificant;
        return order;
}

/**
* Calls get8 and returns the sign extended value as a 32 bit int
*
* @param addr Address of vector element
*
* @return Signed 32 bit int address sign extended
**********************************************************************/
int32_t memory::get8_sx(uint32_t addr) const
{
        int32_t val = memory::get8(static_cast<int32_t>(addr));
	int32_t s = 0x00000080;

	if(val & s)
	{
		val += 0xffffff00;
		return val;
	}
	else
	{
		return val;
	}
}

/**
* Calls get16 and returns the sign extended value as a 32 bit int
*
* @param addr Address of vector element
*
* @return Signed 32 bit int address
*********************************************************************/
int32_t memory::get16_sx(uint32_t addr) const
{
        int32_t val = memory::get16(addr);
	uint32_t s = 0x00008000;

	if(val & s)
	{
		uint32_t se = val | 0xffff0000;
		return se;
	}
	else
	{
		return val;
	}
}

/**
* Calls get32 and returns the sign extended value as a 32 bit int
*
* @param addr Address of vector element
*
* @return Signed 32 bit int of address
*********************************************************************/
int32_t memory::get32_sx(uint32_t addr) const
{
        return (int32_t)memory::get32(addr);
}

/**
* Checks if addr is valid and sets the byte in memory to val
*
* @param addr Address from vector
* @param val Value stored in vector addr
*********************************************************************/
void memory::set8(uint32_t addr, uint8_t val)
{
        if (check_illegal(addr) == false)
        {
                mem[addr] = val;
        }
}

/**
* Stores val in little endian order in memory
*
* @param addr Address from vector
* @param val Value stored in vector addr
********************************************************************/
void memory::set16(uint32_t addr, uint16_t val)
{
        uint8_t moreSignificant = (val & 0xff00) >> 8;
        uint8_t lessSignificant = (val & 0x00ff);

        set8(addr + 1, moreSignificant);
        set8(addr, lessSignificant);
}

/**
* Stores val in little-endian order by using get16 twice
*
* @param addr Address from vector
* @param val Value stored in vector addr
*******************************************************************/
void memory::set32(uint32_t addr, uint32_t val)
{
        uint16_t moreSignificant = (val & 0xffff0000) >> 16;
        uint16_t lessSignificant = (val & 0x0000ffff);

        set16(addr + 2, moreSignificant);
        set16(addr, lessSignificant);
}

/**
* Dumps the memory contents in hex with ASCII
*****************************************************************/
void memory::dump() const
{

        char arr[17];     //array for ASCII char
        arr[16] = '\0';

        //create dump for every byte in mem
        for (unsigned i = 0; i < mem.size(); i++)
        {
                uint8_t ch = get8(i);
                ch = isprint(ch) ? ch : '.';

                arr[i % 16] = ch;

                if ((i % 16) == 0)
                {
                        std::cout << hex::to_hex32(i) << ": ";
                }

                std::cout << hex::to_hex8(get8(i)) << " ";

                if ((i % 16) == 7)
                {
                        std::cout << " ";
                }

                if ((i % 16) == 15)
                {
                        std::cout << "*" << arr << "*" << std::endl;
                }
        }
}

/**
* Opens file in binary mode and writes content to memory
*
* @param &fname Name of file
*
* @return true if successful open, false if unsuccessful
********************************************************************/
bool memory::load_file(const std::string &fname)
{
        std::ifstream infile(fname, std::ios::in|std::ios::binary);

        uint8_t i;
        infile >> std::noskipws;

        if (!infile.is_open())
        {
                std::cout << std::endl;
                std::cerr << "Can't open file '" << fname << "' for reading." << std::endl;
                return false;
        }
        else
        {
                for (uint32_t addr = 0; infile >> i; addr++)
                {
                        if (check_illegal(addr))
                        {
                                std::cerr << "Program too big." << std::endl;
                                infile.close();
                                return false;
                        }
                        else
                        {
                                mem[addr] = i;
                        }
                }
        }

        infile.close();
        return true;
}
