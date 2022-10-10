//***************************************************************************
//
//  Guillermo Rizo-Mut
//  Z1939912
//  CSCI 463 -PE1
//
//  I certify that this is my own work and where appropriate an extension
//  of the starter code provided for the assignment.
//
//***************************************************************************
#include "hex.h"

/**
* This function returns a std::string with exactly 2 hex digits
*
* @param i Unsigned 8 bit int
*
* @return std::string with exactly 2 hex digits
*****************************************************************************/
std::string hex::to_hex8(uint8_t i)
{
	std::ostringstream os;
        os << std::hex << std::setfill('0') << std::setw(2) << static_cast<uint16_t>(i);
        return os.str();
}

/**
* This function returns an std::string with exactly 8 hex digits
*
* @param i Unsigned 32 bit int
*
* @return std::string with exactly 8 hex digits
****************************************************************************/
std::string hex::to_hex32(uint32_t i)
{
        std::ostringstream os;
        os << std::hex << std::setfill('0') << std::setw(8) << i;
        return os.str();
}

/**
* This function returns an std::string with exactly 8 hex digits preceeded by "0x"
*
* @param i Unsigned 32 bit int
*
* @return std::string with exactly 8 hex digits and "0x" preceeding them
****************************************************************************/
std::string hex::to_hex0x32(uint32_t i)
{
        return std::string("0x")+to_hex32(i);
}

/**
 * This function returns an std::string beginning with 0x, followed by 5
 * hex digits representing 20 least sig digits of i
 *
 * @param i Unsigned 32 bit int
 *
 * @return std::string with 5 hex digits preceded by "0x"
 * ***********************************************************************/ 
std::string hex::to_hex0x20(uint32_t i)
{
	std::ostringstream os;
	os << std::hex << std::setfill('0') << std::setw(5) << i;
	return std::string("0x") + os.str();
}

/**
 * This function returns and std::string beginning with "0x" followed by 3 hex
 * digits
 *
 * @param i Unsigned 32 bit int
 *
 * @return std::string of 3 hex digits representing 12 least sig bits of i
 * ***********************************************************************/ 
std::string hex::to_hex0x12(uint32_t i)
{
	std::ostringstream os;
	os << std::hex << std::setfill('0') << std::setw(3) << (i & 0x00000fff);
	return std::string("0x") + os.str();
}
