//***************************************************************************
//
//  Guillermo Rizo-Mut
//  Z1939912
//  CSCI463 - PE1
//
//  I certify that this is my own work and where appropriate an extension 
//  of the starter code provided for the assignment.
//
//***************************************************************************
#include "memory.h"
#include "hex.h"
#include "rv32i_decode.h"

/**
 * Decodes an insatruction by calling the appropriate render function
 *
 * @param addr Memory address of the instruction to decode
 * @param insn Instruction that will be decoded
 *
 * @return An std::string containing the disassembled instruction text
 ***************************************************************************/
std::string rv32i_decode::decode(uint32_t addr, uint32_t insn)
{
	//declares variable to store funct3 and funct7 
	uint32_t funct3 = get_funct3(insn);
	uint32_t funct7 = get_funct7(insn);

	if(insn == insn_ecall)
	{
		return render_ecall(insn);
	}
  
	else if(insn == insn_ebreak)
	{
		return render_ebreak(insn);
	}

	//switch statement to print pc and fetched intruction-word values
	switch (get_opcode(insn))
	{
		default:		return render_illegal_insn(insn);
		case opcode_lui:	return render_lui(insn);
		case opcode_auipc:	return render_auipc(insn);
		case opcode_jal:	return render_jal(addr, insn);
		case opcode_jalr:	return render_jalr(insn);
		//print pc and fetched instruction-word values that share render code
		case opcode_btype:
			switch (funct3)
			{
				default:		return render_illegal_insn(insn);
				case funct3_beq:	return render_btype(addr, insn, "beq");
				case funct3_bne:	return render_btype(addr, insn, "bne");
				case funct3_blt:	return render_btype(addr, insn, "blt");
				case funct3_bge:	return render_btype(addr, insn, "bge");
				case funct3_bltu:	return render_btype(addr, insn, "bltu");
				case funct3_bgeu:	return render_btype(addr, insn, "bgeu");
			}
			assert (0 && "unrecognized funct3"); //impossible
		//go through values that share render code load_imm
		case opcode_load_imm:
			switch (funct3)
			{
				default:		return render_illegal_insn(insn);
				case funct3_lb:		return render_itype_load(insn, "lb");
				case funct3_lh:		return render_itype_load(insn, "lh");
				case funct3_lw:		return render_itype_load(insn, "lw");
				case funct3_lbu:	return render_itype_load(insn, "lbu");
				case funct3_lhu:	return render_itype_load(insn, "lhu");
			}
			assert (0 && "unrecognized funct3"); //impossible
		//go through values that share render code stype
		case opcode_stype:
			switch (funct3)
			{
				default:		return render_illegal_insn(insn);
				case funct3_sb:		return render_stype(insn, "sb");
				case funct3_sh:		return render_stype(insn, "sh");
				case funct3_sw:		return render_stype(insn, "sw");
			}
			assert (0 && "unrecognized funct3"); //impossible
		//go through  values that share render code alu_imm
		case opcode_alu_imm:
			switch (funct3)
			{
				default:		return render_illegal_insn(insn);
				case funct3_add:	return render_itype_alu(insn, "addi", get_imm_i(insn));
				case funct3_slt:	return render_itype_alu(insn, "slti", get_imm_i(insn));
				case funct3_sltu:	return render_itype_alu(insn, "sltiu", get_imm_i(insn));
				case funct3_xor:	return render_itype_alu(insn, "xori", get_imm_i(insn));
				case funct3_or:		return render_itype_alu(insn, "ori", get_imm_i(insn));
				case funct3_and:	return render_itype_alu(insn, "andi", get_imm_i(insn));
				case funct3_sll:	return render_itype_alu(insn, "slli", get_imm_i(insn)%XLEN);
				//go through values that share render code srx
				case funct3_srx:
					switch (funct7)
					{
						default:		return render_illegal_insn(insn);
						case funct7_srl:	return render_itype_alu(insn, "srli", get_imm_i(insn)%XLEN);
						case funct7_sra:	return render_itype_alu(insn, "srai", get_imm_i(insn)%XLEN);
					}
					assert(0 && "unrecognized funct7"); //impossible
			}
			assert (0 && "unrecognized funct3"); //impossible
		//go through values that share render code rtype
		case opcode_rtype:
			switch (funct3)
			{
				default:	return render_illegal_insn(insn);
				case funct3_add:
					switch (funct7)
					{
						default:		return render_illegal_insn(insn);
						case funct7_add:	return render_rtype(insn, "add");
						case funct7_sub:	return render_rtype(insn, "sub");
					}
					assert (0 && "unrecognized funct7"); //impossible 
				case funct3_sll:	return render_rtype(insn, "sll");
				case funct3_slt:	return render_rtype(insn, "slt");
				case funct3_sltu:	return render_rtype(insn, "sltu");
				case funct3_xor:	return render_rtype(insn, "xor");
				//go through values that share render code type
				case funct3_srx:
					switch (funct7)
					{
						default:		return render_illegal_insn(insn);
						case funct7_srl:	return render_rtype(insn, "srl");
						case funct7_sra:	return render_rtype(insn, "sra");
					}
					assert (0 && "unrecognized funct7"); //impossible
				case funct3_or:		return render_rtype(insn, "or");
				case funct3_and:	return render_rtype(insn, "and");
			}
			assert (0 && "unrecognized funct3"); //impossible
		//go through values that share render code type
		case opcode_system:
			switch (funct3)
			{
				default:		
				case funct3_csrrw:	return render_csrrx(insn, "csrrw");
				case funct3_csrrs:	return render_csrrx(insn, "csrrs");
				case funct3_csrrc:	return render_csrrx(insn, "csrrc");
				case funct3_csrrwi:	return render_csrrxi(insn, "csrrwi");
				case funct3_csrrsi:	return render_csrrxi(insn, "csrrsi");
				case funct3_csrrci:	return render_csrrxi(insn, "csrrci");
			}
			assert (0 && "unrecognized funct3"); //impossible
	}
	assert (0 && "unrecognized opcode"); //impossible
}

/**
 * Renderer for invalid instructions
 *
 * @param insn Unsigned 32 bit int
 *
 * @return Error message
 ***********************************************************/
std::string rv32i_decode::render_illegal_insn(uint32_t insn)
{
	return "ERROR: UNIMPLEMENTED INSTRUCTION";
}

/**
 * Renders lui 
 *
 * @param insn Unsigned 32 bit int
 *
 * @return Mnemonic, rd, and the imm_u value
******************************************************************/
std::string rv32i_decode::render_lui(uint32_t insn)
{
	uint32_t rd = get_rd(insn);
	int32_t imm_u = get_imm_u(insn);

	//string of the instruction and return it
	std::ostringstream os;
	os << render_mnemonic("lui") << render_reg(rd) << "," 
	  << hex::to_hex0x20((imm_u >> 12) & 0x0fffff);
	
	return os.str();
}

/**
 * Renders auipc
 *
 * @param insn Unsigned 32 bit int
 *
 * return Mnemonic, rd, and the imm_u value
 ***************************************************************/
std::string rv32i_decode::render_auipc(uint32_t insn)
{
	uint32_t rd = get_rd(insn);
	int32_t imm_u = get_imm_u(insn);

	//string of the instruction and return it
	std::ostringstream os;
	os << render_mnemonic("auipc") << render_reg(rd) << "," 
		<< hex::to_hex0x20((imm_u >> 12) & 0x0fffff);
	
	return os.str();
}

/**
 * Renders jal 
 *
 * @param addr Unsigned 32 bit int
 * @param insn Unsigned 32 bit int
 *
 * return Mnemonic, rd, and the pcrel_21 value
 ************************************************************************/
std::string rv32i_decode::render_jal(uint32_t addr, uint32_t insn)
{
	uint32_t rd = get_rd(insn);
	int32_t imm_j = get_imm_j(insn);
	int32_t pcrel_21 = addr + imm_j;

	//string of the instruction and return it
	std::ostringstream os;
	os << render_mnemonic("jal") << render_reg(rd) << "," 
		<< hex::to_hex0x32(pcrel_21);
  
	return os.str();
}

/**
 * Renders jalr
 *
 * @param insn Unsigned 32 bit int
 *
 * @return Mnemonic, rd, the imm_j value, and the rs2
 *********************************************************************/
std::string rv32i_decode::render_jalr(uint32_t insn)
{
	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	int32_t imm_i = get_imm_i(insn);

	//string of the instruction and return it
	std::ostringstream os;
	os << render_mnemonic("jalr") << render_reg(rd) << "," 
		<< render_base_disp(rs1, imm_i);
  
	return os.str();
}

/**
 * Renders btype instructions
 *
 * @param addr Unsigned 32 bit int
 * @param insn Unsigned 32 bit int
 * @param mnemonic Constant character
 *
 * @return Mnemonic, rs1, rs2, and the pcrel_13 value
 **************************************************************************/
std::string rv32i_decode::render_btype(uint32_t addr, uint32_t insn, const char *mnemonic)
{
	uint32_t rs1 = get_rs1(insn);
	uint32_t rs2 = get_rs2(insn);
	int32_t imm_b = get_imm_b(insn);
	int32_t pcrel_13 = addr + imm_b;

	//string of the instruction and return it
	std::ostringstream os;
	os << render_mnemonic(mnemonic) << render_reg(rs1) << "," 
		<< render_reg(rs2) << "," << hex::to_hex0x32(pcrel_13);
  
	return os.str();
}

/**
 * Renders itype_load instructions
 *
 * @param insn Unsigned 32 bit int
 * @param mnemonic Constant character
 *
 * @return Mnemonic, rd, imm_i value, and rs1
 ***********************************************************************/
 std::string rv32i_decode::render_itype_load(uint32_t insn, const char *mnemonic)
{
	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	int32_t imm_i = get_imm_i(insn);

	//string of the instruction and return it
	std::ostringstream os;
	os << render_mnemonic(mnemonic) << render_reg(rd) << "," 
		<< render_base_disp(rs1, imm_i);
  
	return os.str();
}

/**
 * Renders itype_alu instructions
 *
 * @param insn Unsigned 32 bit int
 * @param mnemonic Constant character
 * @param imm_i Signed 32 bit int
 *
 * @return Mnemonic, rd, rs1, and the imm_i value
 ************************************************************************/
std::string rv32i_decode::render_itype_alu(uint32_t insn, const char *mnemonic, int32_t imm_i)
{
	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
  
	//string of the instruction and return it
	std::ostringstream os;
	os << render_mnemonic(mnemonic) << render_reg(rd) << "," 
		<< render_reg(rs1) << "," << imm_i;
  
	return os.str();
}

/**
 * Renders stype instructions
 *
 * @param insn Unsigned 32 bit int
 * @param mnemonic Constant character
 *
 * @return Mnemonic, rs2, imm_s value, and rs2
 **************************************************************************/
std::string rv32i_decode::render_stype(uint32_t insn, const char *mnemonic)
{
	uint32_t rs1 = get_rs1(insn);
	uint32_t rs2 = get_rs2(insn);
	int32_t imm_s = get_imm_s(insn);

	//string of the instruction and return it
	std::ostringstream os;
	os << render_mnemonic(mnemonic) << render_reg(rs2) << "," 
		<< render_base_disp(rs1, imm_s);
  
	return os.str();
}

/**
 * Renders rtype instructions
 *
 * @param insn Unsigned 32 bit int
 * @param mnemonic Constant character
 *
 * @return Mnemonic, rd, rs1, and rs2
 ***************************************************************************/
std::string rv32i_decode::render_rtype(uint32_t insn, const char *mnemonic)
{
	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	uint32_t rs2 = get_rs2(insn);

	//string of the instruction and return it
	std::ostringstream os;
	os << render_mnemonic(mnemonic) << render_reg(rd) << "," 
		<< render_reg(rs1) << "," << render_reg(rs2);
  
	return os.str();
}

/**
 * Renders ecall 
 *
 * @param insn Unsigned 32 bit int
 *
 * @return String "ecall"
 ********************************************************************/
std::string rv32i_decode::render_ecall(uint32_t insn)
{
	//set the string to ecall
	std::ostringstream os;
	os << "ecall";
	return os.str();
}

/**
 * Renders ebreak 
 *
 * @param insn Unsigned 32 bit int
 *
 * @return String "ebreak"
 **********************************************************************/
std::string rv32i_decode::render_ebreak(uint32_t insn)
{
	//set the string to ebreak
	std::ostringstream os;
	os << "ebreak";
	return os.str();
}

/**
 * Renders csrrx instructions
 *
 * @param insn Unsigned 32 bit int
 * @param mnemonic Constant character
 *
 * @return Mnemonic, rd, csr value, and rs1
 **************************************************************************/
std::string rv32i_decode::render_csrrx(uint32_t insn, const char *mnemonic)
{
	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	int32_t csr = get_imm_i(insn);

	//string of the instruction 
	std::ostringstream os;
	os << render_mnemonic(mnemonic) << render_reg(rd) << "," 
		<< hex::to_hex0x12(csr) << "," << render_reg(rs1);
  
	return os.str();
}

/**
 * Renders csrrxi instructions
 *
 * @param insn Unsigned 32 bit int
 * @param mnemonic Constant character
 *
 * @return Mnemonic, rd, csr value, and zimm value
 ****************************************************************************/
 std::string rv32i_decode::render_csrrxi(uint32_t insn, const char *mnemonic)
{
	uint32_t rd = get_rd(insn);
	int32_t csr = get_imm_i(insn);
	uint32_t zimm = get_rs1(insn);

	//string of the instruction
	std::ostringstream os;
	os << render_mnemonic(mnemonic) << render_reg(rd) << "," 
		<< hex::to_hex0x12(csr) << "," << zimm;
  
	return os.str();
}

/**
 * Renders name of the register
 *
 * @param r Integer
 *
 * @return Register as string
 *************************************************************************/
std::string rv32i_decode::render_reg(int r)
{
	//set string to x
	std::ostringstream os;
	os << "x" << r;
	return os.str();
}

/**
 * Renders imm(register)
 *
 * @param r Unsigned 32 bit int
 * @param imm Unsigned 32 bit int
 *
 * @return String of imm(register)
 *************************************************************************/
std::string rv32i_decode::render_base_disp(uint32_t r, int32_t imm)
{
	//set string to the register 
	std::ostringstream os;
	os << imm << "(" << render_reg(r) << ")";
	return os.str();
}

/**
 * Renders menmonic with space
 *
 * @param mnemonic Constant string
 *
 * @return String of mnemonic with space
 ************************************************************************/
std::string rv32i_decode::render_mnemonic(const std::string &mnemonic)
{
	//set string with spaces
	std::ostringstream os;
	os << std::setw(mnemonic_width) << std::setfill(' ') 
		<< std::left << mnemonic;
  
	return os.str();
}

/**
 * Get opcode
 *
 * @param insn Unsigned 32 bit int
 *
 * @return Opcode 
 ******************************************************************/
uint32_t rv32i_decode::get_opcode(uint32_t insn)
{
	return (insn & 0x0000007f);
}

/**
 * Get rd 
 *
 * @param insn Unsigned 32 bit int
 *
 * @return rd 
 ***********************************************************************/
uint32_t rv32i_decode::get_rd(uint32_t insn)
{
	return (insn & 0x00000f80) >> 7;
}

/**
 * Get funct3 
 *
 * @param insn Unsigned 32 bit int
 *
 * @return funct3 
 ************************************************************************/
uint32_t rv32i_decode::get_funct3(uint32_t insn)
{
	return (insn & 0x00007000) >> 12;
}

/**
 * Get rs1 
 *
 * @param insn Unsigned 32 bit int
 *
 * @return rs1
 ****************************************************************************/
uint32_t rv32i_decode::get_rs1(uint32_t insn)
{
	return (insn & 0x000f8000) >> 15;
}

/**
 * Get rs2 
 *
 * @param insn Unsigned 32 bit int
 *
 * @return rs2 
 ********************************************************************/
uint32_t rv32i_decode::get_rs2(uint32_t insn)
{
	return (insn & 0x01f00000) >> 20;
}

/**
 * Get funct7 
 *
 * @param insn Unsigned 32 bit int
 *
 * @return funct7 
 ************************************************************************/
uint32_t rv32i_decode::get_funct7(uint32_t insn)
{
	return (insn & 0xfe000000) >> 25;
}

/**
 * Get imm_i 
 *
 * @param insn Unsigned 32 bit int
 *
 * @return imm_i 
 ************************************************************************/
int32_t rv32i_decode::get_imm_i(uint32_t insn)
{
	//shift bits 
	int32_t imm_i = (insn & 0xfff00000) >> (20 - 0);

	//sign extend 
	if (insn & 0x80000000) 
	{ 
		imm_i |= 0xfffff000; 
	}
	return imm_i;
}

/**
 * Get imm_u 
 *
 * @param insn Unsigned 32 bit int
 *
 * @return imm_u 
 *************************************************************************/
int32_t rv32i_decode::get_imm_u(uint32_t insn)
{
	//shift bits
	int32_t imm_u = (insn & 0x00000fff) & 0;
	imm_u |= (insn & 0xfffff000);
	return imm_u;
}

/**
 * Get imm_b field from the given instruction
 *
 * @param insn Unsigned 32 bit int
 *
 * @return imm_b 
 ***********************************************************************/
int32_t rv32i_decode::get_imm_b(uint32_t insn)
{
	//shift bits 
	int32_t imm_b = (insn & 0x00000001) & 0;

	imm_b |= (insn & 0x00000f00) >> (8 - 1);

	imm_b |= (insn & 0x7e000000) >> (25 - 5);	

	imm_b |= (insn & 0x00000080) << (11 - 7);

	imm_b |= (insn & 0x80000000) >> (31 - 12);

	//sign extend
	if (insn & 0x80000000) 
	{ 
		imm_b |= 0xffffe000; 
	}

	return imm_b;
}

/**
 * Get imm_s 
 *
 * @param insn Unsigned 32 bit int
 *
 * @return imm_s 
 ********************************************************************/
int32_t rv32i_decode::get_imm_s(uint32_t insn)
{
	//shift bits
	int32_t imm_s = (insn & 0xfe000000) >> (25 - 5);	

	imm_s |= (insn & 0x00000f80) >> (7 - 0);		

	//sign extend
	if (insn & 0x80000000) 
	{ 
		imm_s |= 0xfffff000; 
	}
	return imm_s;
}

/**
 * Get imm_j 
 *
 * @param insn Unsigned 32 bit int
 *
 * @return imm_j 
 *********************************************************************/
int32_t rv32i_decode::get_imm_j(uint32_t insn)
{
	//shift bits
	int32_t imm_j = (insn & 0x00000001) & 0;

	imm_j |= (insn & 0x7fe00000) >> (21 - 1);		

	imm_j |= (insn & 0x00100000) >> (20 - 11);		

	imm_j |= (insn & 0x000ff000);				

	imm_j |= (insn & 0x80000000) >> (31 - 20);		

	//sign extend
	if (insn & 0x80000000) 
	{ 
		imm_j |= 0xffe00000; 
	}
	return imm_j;
}
