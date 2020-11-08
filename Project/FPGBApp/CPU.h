#pragma once

#include "types.h"

enum class CPUMODES
{
	USER,
	FIQ,
	IRQ,
	SUPERVISOR,
	ABORT,
	UNDEFINED,
	SYSTEM
};

#if DEBUG
class cpustate
{
public:
	UInt32 debugregs[18];

	UInt32 opcode;
	UInt32 flags;
	bool flag_Negative;
	bool flag_Carry;
	bool flag_Zero;
	bool flag_V_Overflow;
	Int32 newticks;
	uint busprefetch;
	byte thumbmode;
	byte armmode;
	byte irpdisable;
	UInt16 IF_intern;
	int irp_wait;

	UInt32 timer0;
	UInt32 timer1;
	UInt32 timer2;
	UInt32 timer3;

	UInt32 memory01;
	UInt32 memory02;
	UInt32 memory03;
	UInt32 debug_dmatranfers;

	UInt32 R16;
	UInt32 R17;
	UInt32 R13_USR;
	UInt32 R14_USR;
	UInt32 R13_IRQ;
	UInt32 R14_IRQ;
	UInt32 R13_SVC;
	UInt32 R14_SVC;
	UInt32 SPSR_IRQ;
	UInt32 SPSR_SVC;

	void update(byte mode);
	void update_ticks(Int32 newticks);
};
#endif

class Cpu
{
public:
	bool thumbmode;
	bool old_thumbmode;

	bool output_debug = true;

	UInt32 regs[18];

	UInt32 PC;
	UInt32 PC_old;

	bool Flag_Zero;
	bool Flag_Carry;
	bool Flag_Negative;
	bool Flag_V_Overflow;

	bool IRQ_disable;
	bool FIQ_disable;
	bool irpnext;
	int irpdelay;
	bool irpdelay_next;

	CPUMODES cpu_mode;

	bool halt;
	bool stop;
	bool flushPipeline;
	UInt32 instr1;
	UInt32 instr2;

	UInt32 regbanks[6][18];

	Int32 newticks;
	Int32 totalticks;

	int additional_steps;
	UInt64 commands;
	UInt64 cyclenr;
	bool tracenext;

#if DEBUG
	const int Tracelist_Length = 1000000;
	cpustate Tracelist[1000000];
#endif
	int traclist_ptr;
	int runmoretrace;

	int op_since_dma = 0; // dma bus value - e.g. hello kitty

	void reset();
	void nextInstr();

	CPUMODES get_mode_from_value(uint value);
	UInt32 get_CPSR();
	void set_CPSR(UInt32 value);
	UInt32 RotateRight(UInt32 x, int n);

private:
	UInt32 debug_outdivcnt;

#if DEBUG
	void trace_file_last();
	void vcd_file_last();
#endif

	void interrupt();
	void thumb_command();
	void long_branch_with_link(bool high, UInt16 SOffset11);
	void unconditional_branch(UInt16 SOffset11);
	void conditional_branch(byte cond, byte SOffset8);
	void multiple_load_store(bool load, byte Rb, byte Rlist);
	void push_pop_register(bool load, bool link_pc, byte rlist);
	void add_offset_to_stack_pointer(bool sign, byte word7);
	void load_address(bool sp, byte Rd, byte word8);
	void sp_relative_load_store(bool load, byte Rd, byte word8);
	void load_store_halfword(bool load, byte Offset5, byte Rb, byte Rd);
	void load_store_with_immidiate_offset(bool load, bool byteflag, byte Offset5, byte Rb, byte Rd);
	void load_store_sign_extended_byte_halfword(byte opcode_hs, byte Ro, byte Rb, byte Rd);
	void load_store_with_register_offset(bool load, bool byteflag, byte Ro, byte Rb, byte Rd);
	void pc_relative_load(byte Rd, byte word8);
	void thumbbranch_by_R15_write();
	void hi_register_operations_branch_exchange(byte opcode_h1h2, byte RsHs, byte RdHd);
	void alu_operations(byte opcode, byte Rs, byte Rd);
	void move_compare_add_subtract_immediate(byte op, byte Rd, byte offset8);
	void add_subtract(bool imm, bool op_sub, byte Rn_offset3, byte Rs, byte Rd);
	void move_shifted_register(byte op, byte offset5, byte Rs, byte Rd);
	void arm_command();
	void software_interrupt();
	void branch(bool link, UInt32 offset);
	void block_data_transfer(byte opcode, bool load_store, byte Rn_op1, UInt16 reglist);
	void single_data_transfer(bool use_imm, byte opcode, byte opcode_low, bool load_store, byte Rn_op1, byte Rdest, UInt16 op2);
	void halfword_data_transfer(byte opcode, byte opcode_low, bool load_store, byte Rn_op1, byte Rdest, UInt32 value);
	void branch_and_exchange(byte register);
	void single_data_swap(bool byteswap, byte Rn_op1, byte Rdest, UInt16 Op2);
	void multiply(byte add, bool s_updateflags, byte Rn_op1, byte Rdest, byte reg1, byte reg2);
	void multiply_long(byte opcode, bool s_updateflags, byte RdestHi, byte RdestLo, UInt16 Op2);
	void data_processing_PSR(UInt32 asmcmd);
	UInt32 shift_logicalleft(bool& shiftercarry, UInt32 shiftervalue, int shiftamount);
	UInt32 shift_logicalright(bool& shiftercarry, UInt32 shiftervalue, int shiftamount);
	UInt32 shift_arithright(bool& shiftercarry, UInt32 shiftervalue, int shiftamount);
	UInt32 shift_rotateright(bool& shiftercarry, UInt32 shiftervalue, int shiftamount);
	bool shifter(bool& shiftercarry, UInt32& shiftervalue, UInt16 shift_opcode);
	void update_flags_logical(UInt32 result, bool shiftercarry);
	void alu_and(byte Rdest, UInt32 op1_val, UInt32 op2_val, bool updateFlags, bool writeback, bool shiftercarry);
	void alu_xor(byte Rdest, UInt32 op1_val, UInt32 op2_val, bool updateFlags, bool writeback, bool shiftercarry);
	void alu_or(byte Rdest, UInt32 op1_val, UInt32 op2_val, bool updateFlags, bool shiftercarry);
	void alu_and_not(byte Rdest, UInt32 op1_val, UInt32 op2_val, bool updateFlags, bool shiftercarry);
	void alu_mov(byte Rdest, UInt32 op2_val, bool updateFlags, bool shiftercarry);
	void alu_mov_not(byte Rdest, UInt32 op2_val, bool updateFlags, bool shiftercarry);
	void alu_add(byte Rdest, UInt32 op1_val, UInt32 op2_val, bool updateFlags, bool writeback);
	void alu_add_withcarry(byte Rdest, UInt32 op1_val, UInt32 op2_val, bool updateFlags, bool carry);
	void alu_sub(byte Rdest, UInt32 op1_val, UInt32 op2_val, bool updateFlags, bool writeback);
	void alu_sub_withcarry(byte Rdest, UInt32 op1_val, UInt32 op2_val, bool updateFlags, bool carry);
	void data_processing(bool imm, byte opcode, bool s_updateflags, byte Rn_op1, byte Rdest, UInt16 Op2, UInt32 asmcmd);
	void CPUSwitchMode(CPUMODES mode, bool saveState);
};
extern Cpu CPU;