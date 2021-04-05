
/**
 * VME.h
 * VME Device Driver Module Definition
 */

#ifndef _VME_H_
#define _VME_H_






#if 0
#define	VMEIR_ADD						0x80000004
#define	VME_INT_ADDR					VMEIR_ADD

#define	STD_MREAD_16BIT					1
#define	STD_MREAD_32BIT					2

#define	DEFAULT_STD_MRTY				STD_MREAD_16BIT

#endif

/** @name Symbolic Constants
 * Symbolic Constants
 */
///@{
#define HWREG_VMEM_BASE					0xA0000000						///> Master Space Base Address
#define HWREG_VMEM_SIZE					0x1000000						///> Master Space
#define HWREG_VMEM_EXT_BASE				HWREG_VMEM_BASE					///> Master Extended Space Base Address
#define	HWREG_VMEM_STD_BASE				(HWREG_VMEM_BASE + 0xE000000)	///> Master Standard Space Base Address

#ifdef FEATURE_PERIPHERAL_TEST
#define	HWREG_VMES_BASE					0x30000000						///> Slave Space Base Address
#define	HWREG_VMES_SIZE					0x2000							///> Slave Space
#else
#define	HWREG_VMES_BASE					0xB0000000						///> Slave Space Base Address
#define	HWREG_VMES_HEADER_SIZE			0x10							///> Slave Space
#define	HWREG_VMES_SIZE					0x2000							///> Slave Space


#define	HWREG_VMES_RUSER_OFFSET			(0x40/4)						///> Slave Space Base Address
#define	HWREG_VMES_RUSER_SIZE			(0x400/4)						///> Slave Space Base Address
#define	HWREG_VMES_WUSER_OFFSET			(0x440/4)						///> Slave Space Base Address
#define	HWREG_VMES_WUSER_SIZE			(0x400/4)						///> Slave Space Base Address
#define	HWREG_VMES_DIAG_OFFSET			(0x840/4)						///> Slave Space Base Address

#define	HWREG_VMES_RUSERDATA_BASE		(HWREG_VMES_BASE+HWREG_VMES_RUSER_OFFSET)		///> Slave Space Read Base Address
#define	HWREG_VMES_WUSERDATA_BASE		(HWREG_VMES_BASE+HWREG_VMES_WUSER_OFFSET)		///> Slave Space Write Base Address
#define	HWREG_VMES_DIAG_BASE		(HWREG_VMES_BASE+HWREG_VMES_DIAG_OFFSET)		///> Slave Space Base Address


#endif
///@}


/** @name Symbolic Constants
 * VME System Control Register
 */
///@{
#define HWREG_VME_SCON					0x80000001						///> Register Address
#define HWREG_VME_SCON_MSK_MASK			0x2								///> MSK Mask
#define HWREG_VME_SCON_MSK_DIP4ON		0x2								///> Sys Control by Dip Sw 4
#define HWREG_VME_SCON_MSK_DIP4OFF		0x0								///> Sys Control by Register

#define HWREG_VME_SCON_EN_MASK			0x1								///> Sys Control On
#define HWREG_VME_SCON_EN_SYSCON		0x1								///> Sys Control On
#define HWREG_VME_SCON_EN_NONE			0x0								///> Sys Control Off

#define HWREG_VME_SCON_MREAD_MASK		0x8								///> Mask
#define HWREG_VME_SCON_MREAD_16BIT		0x8								///> 16 Bits
#define HWREG_VME_SCON_MREAD_32BIT		0x0								///> 32 Bits


///@}


/** @name Symbolic Constants
 * VME Master Configuration Register
 */
///@{
#define HWREG_VME_MCFG					0x80000002						///> Register Address
#define HWREG_VME_MCFG_STDMAM_MASK		0x3F							///> Master Standard Access Mode Mask
#define HWREG_VME_MCFG_STDMAM_NPRIV		0x39							///> Master Standard Access Mode Non-Priviliged
#define HWREG_VME_MCFG_STDMAM_SUPRV		0x3D							///> Master Standard Access Mode Supervisor

#define HWREG_VME_MCFG_EXTMAM_MASK		0x3F00							///> Master Standard Access Mode Mask
#define HWREG_VME_MCFG_EXTMAM_NPRIV		0x0900							///> Master Standard Access Mode Non-Priviliged
#define HWREG_VME_MCFG_EXTMAM_SUPRV		0x0D00							///> Master Standard Access Mode Supervisor

#define HWREG_VME_MCFG_STDMADD_MASK		0xF0000							///> Master Standard Address
#define HWREG_VME_MCFG_STDMADD_DEF		0x0								///> Master Standard Address Default

#define HWREG_VME_MCFG_EXTMADD_MASK		0xFF000000						///> Master Extended Address 
#define HWREG_VME_MCFG_EXTMADD_DEF		0x90000000						///> Master Extended Address Default
///@}

/** @name Symbolic Constants
 * VME Slave Configuration Register
 */
///@{
#define HWREG_VME_SCFG					0x80000003						///> Register Address
#define HWREG_VME_SCFG_STDADD_MASK		0xF00000						///> Slave Standard Address Mask
#define HWREG_VME_SCFG_STDADD_DEF		0xB00000						///> Slave Standard Address Default
#define HWREG_VME_SCFG_EXTADD_MASK		0xFF000000						///> Slave Extended Address Default
#define HWREG_VME_SCFG_EXTADD_DEF		0x98000000						///> Slave Extended Address Default

#define HWREG_VME_SCFG_AUTOSLOT_MASK	0x1						///> Mask
#define HWREG_VME_SCFG_AUTOSLOT_MODE	0x1						///> AutoSlot Mode
#define HWREG_VME_SCFG_AUTOSLOT_NONE	0x0						///> None AutoSlot mode


#define HWREG_VME_SCTRL					0x80000013						///> Register Address
#define HWREG_VME_SCTRL_SLOTID_MASK		0x0F000000						///> Register Address



///@}




#endif

