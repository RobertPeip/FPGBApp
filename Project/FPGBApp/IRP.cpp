#include "IRP.h"
#include "GBRegs.h"

Irp IRP;

void Irp::set_irp_bit(UInt16 mask)
{
	IRP_Flags |= mask;
	GBRegs.Sect_system.IF.write(IRP_Flags);
}

void Irp::update_IE()
{
	IE = (UInt16)GBRegs.Sect_system.IE.read();
}

void Irp::clear_irp_bits()
{
	UInt16 clearvector = (UInt16)GBRegs.Sect_system.IF.read();
	IRP_Flags = (UInt16)(IRP_Flags & (~clearvector));
	GBRegs.Sect_system.IF.write(IRP_Flags);
}

void Irp::update_IME(UInt16 value)
{
	Master_enable = (value & 1) == 1;
}

UInt16 Irp::get_IF_with_mask()
{
	return (UInt16)(IRP_Flags & IE);
}