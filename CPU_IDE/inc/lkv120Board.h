/*! \file lkv120Board.h
    \brief A Documented file.
    
    Details.
*/

/*! \def MAX(a,b)
    \brief A macro that returns the maximum of \a a and \a b.
   
    Details.
*/

/*! \var typedef unsigned int UINT32
    \brief A type definition for a .
    
    Details.
*/

/*! \var int errno
    \brief Contains the last error code.

    \warning Not thread safe!
*/

/*! \fn int open(const char *pathname,int flags)
    \brief Opens a file descriptor.

    \param pathname The name of the descriptor.
    \param flags Opening flags.
*/

/*! \fn int close(int fd)
    \brief Closes the file descriptor \a fd.
    \param fd The descriptor to close.
*/

/*! \fn size_t write(int fd,const char *buf, size_t count)
    \brief Writes \a count bytes from \a buf to the filedescriptor \a fd.
    \param fd The descriptor to write to.
    \param buf The data buffer to write.
    \param count The number of bytes to write.
*/

/*! \fn int read(int fd,char *buf,size_t count)
    \brief Read bytes from a file descriptor.
    \param fd The descriptor to read from.
    \param buf The buffer to read into.
    \param count The number of bytes to read.
*/

#ifndef BOARD_H
#define BOARD_H

#if (BOARD_TYPE == MVME2100_BOARD)




    #define BRD_NVRAM_START_ADDR    ((char *)(0xFFE00000 + 0x80000))
    #define BRD_NVRAM_MAX_SIZE      0x1F00 
    #define BRD_NVRAM_END_ADDR      ((char *)(BRD_NVRAM_START_ADDR+BRD_NVRAM_MAX_SIZE))


#define BRD_BOOT_LINE_ADDR      0x4200


#define	BRD_MAC_ADR_OFFSET	0x1f2c


#define	BRD_TIME_ADR_OFFSET	0x1ff8


#define	BRD_ETH_PARA_OFFSET	(256)
#define	BRD_ETH_PARA_SIZE	(255)


#define	USR_PARA_OFFSET		((256*2)+BRD_ETH_PARA_SIZE)


#define	BRD_ABORT_SWITCH_ADDR	(0xFFE00000)
#define	BRD_ABORT_SWITCH_BIT	(4)

 
#define LSI_CTL_EN             (1 << 31)       
#define LSI_CTL_WP             (1 << 30)       
#define LSI_CTL_D8             (0 << 22)       
#define LSI_CTL_D16            (1 << 22)       
#define LSI_CTL_D32            (2 << 22)       
#define LSI_CTL_D64            (3 << 22)       
#define LSI_CTL_A16            (0 << 16)       
#define LSI_CTL_A24            (1 << 16)       
#define LSI_CTL_A32            (2 << 16)       
#define LSI_CTL_CSR            (5 << 16)       
#define LSI_CTL_USER1          (6 << 16)       
#define LSI_CTL_USER2          (7 << 16)       
#define LSI_CTL_PGM            (1 << 14)       
#define LSI_CTL_DATA           (0 << 14)       
#define LSI_CTL_SUP            (1 << 12)       
#define LSI_CTL_USR            (0 << 12)       
#define LSI_CTL_BLK            (1 << 8)        
#define LSI_CTL_SINGLE         (0 << 8)        
#define LSI_CTL_PCI_MEM        (0 << 0)        
#define LSI_CTL_PCI_IO         (1 << 0)        
#define LSI_CTL_PCI_CONFIG     (2 << 0)        


#define VAL_LSI0_CTL    BOARD_PARAMETER_NOT_USE
#define	LSI0_CTL_OFFSET	0x100

#define	VAL_LSI0_BS		BOARD_PARAMETER_NOT_USE	
#define	LSI0_BS_OFFSET	0x104

#define	VAL_LSI0_BD		BOARD_PARAMETER_NOT_USE	
#define	LSI0_BD_OFFSET	0x108

#define	VAL_LSI0_TO		BOARD_PARAMETER_NOT_USE	
#define	LSI0_TO_OFFSET	0x10C



#define VAL_LSI1_CTL    ( LSI_CTL_EN     | LSI_CTL_WP  |\
                          LSI_CTL_D32    | LSI_CTL_A32 |\
                          LSI_CTL_DATA   | LSI_CTL_USR |\
                          LSI_CTL_SINGLE | LSI_CTL_PCI_MEM )
#define	LSI1_CTL_OFFSET	0x114

#define	VAL_LSI1_BS		0x81000000	
#define	LSI1_BS_OFFSET	0x118

#define	VAL_LSI1_BD		0xA0000000	
#define	LSI1_BD_OFFSET	0x11C

#define	VAL_LSI1_TO		0x80000000	
#define	LSI1_TO_OFFSET	0x120


#define VAL_LSI2_CTL    ( LSI_CTL_EN     | LSI_CTL_WP  |\
                          LSI_CTL_D32    | LSI_CTL_A24 |\
                          LSI_CTL_DATA   | LSI_CTL_USR |\
                          LSI_CTL_SINGLE | LSI_CTL_PCI_MEM )
#define	LSI2_CTL_OFFSET	0x128

#define	VAL_LSI2_BS		0xA0000000	
#define	LSI2_BS_OFFSET	0x12C

#define	VAL_LSI2_BD		0xA2000000	
#define	LSI2_BD_OFFSET	0x130

#define	VAL_LSI2_TO		0x50000000	
#define	LSI2_TO_OFFSET	0x134


#define VAL_LSI3_CTL    ( LSI_CTL_EN     | LSI_CTL_WP  |\
                          LSI_CTL_D16    | LSI_CTL_A16 |\
                          LSI_CTL_DATA   | LSI_CTL_USR |\
                          LSI_CTL_SINGLE | LSI_CTL_PCI_MEM )
#define	LSI3_CTL_OFFSET	0x13C

#define	VAL_LSI3_BS		0xAFFF0000	
#define	LSI3_BS_OFFSET	0x140

#define	VAL_LSI3_BD		0xB0000000	
#define	LSI3_BD_OFFSET	0x144

#define	VAL_LSI3_TO		0x50000000	
#define	LSI3_TO_OFFSET	0x148




#define VRAI_CTL_EN             (1 << 31)       
#define VRAI_CTL_AM_DATA        (1 << 22)       
#define VRAI_CTL_AM_PGM         (2 << 22)       
#define VRAI_CTL_AM_SUPER       (2 << 20)       
#define VRAI_CTL_AM_USER        (1 << 20)       
#define VRAI_CTL_VAS_A16        (0 << 16)       
#define VRAI_CTL_VAS_A24        (1 << 16)       
#define VRAI_CTL_VAS_A32        (2 << 16)       
#define VRAI_CTL_VAS_USER1      (6 << 16)       
#define VRAI_CTL_VAS_USER2      (7 << 16)       



#define VRAI_REG_CTL    ( VRAI_CTL_EN     	 | VRAI_CTL_AM_DATA  |\
                          VRAI_CTL_AM_PGM    | VRAI_CTL_AM_SUPER |\
                          VRAI_CTL_AM_USER   | VRAI_CTL_VAS_A32 )
#define	VRAI_CTL_OFFSET	0xf70

#define	VARI_LSI1_BS		0x10000000
#define	VRAI_BS_OFFSET	0xf74


#define	VME_SLAVE7_REG_CTRL_DATA	BOARD_PARAMETER_NOT_USE	
#define	VME_SLAVE7_CTRL_OFFSET	(0xfcc)
#define	VME_SLAVE7_HIGH_OFFSET	(0xfd0)
#define	VME_SLAVE7_LOW_OFFSET	(0xfd4)
	

#define	BOARD_MCH_EXP_OFFSET	0x00200		
#define	BOARD_DATA_EXP_OFFSET	0x00300			
#define	BOARD_ALN_EXP_OFFSET	0x00600		


#define	BOARD_LOCAL_BUS_ERR		0x00000200 
#define	BOARD_VME_BUS_ERR		0x00000400 
#define	BOARD_ACC_FAIL_ERR		0x00008000 

#elif ((BOARD_TYPE == MVME3100_BOARD) || (BOARD_TYPE == MVME4100_BOARD))




#if (BOARD_TYPE == MVME3100_BOARD)

	#define	BRD_NVRAM_BASE_ADDR		0xa8
    #define BRD_NVRAM_START_ADDR    ((char *)(BRD_NVRAM_BASE_ADDR + 0x200))
    #define BRD_NVRAM_MAX_SIZE      0xD00 
    #define BRD_NVRAM_END_ADDR      ((char *)(BRD_NVRAM_START_ADDR+BRD_NVRAM_MAX_SIZE))
    
#elif (BOARD_TYPE == MVME4100_BOARD)

	#define	BRD_NVRAM_BASE_ADDR		0xa4	
    #define BRD_NVRAM_START_ADDR    ((char *)(0x200))
    #define BRD_NVRAM_MAX_SIZE      0xE00 
    #define BRD_NVRAM_END_ADDR      ((char *)(BRD_NVRAM_START_ADDR+BRD_NVRAM_MAX_SIZE))

#endif


#define BRD_BOOT_LINE_ADDR      0x4200


#define	BRD_MAC_ADR_OFFSET	BOARD_PARAMETER_NOT_USE 


#define	BRD_TIME_ADR_OFFSET	BOARD_PARAMETER_NOT_USE 


#define	BRD_ETH_PARA_OFFSET	(512)
#define	BRD_ETH_PARA_SIZE	(256)


#define	USR_PARA_OFFSET		(BRD_ETH_PARA_OFFSET+BRD_ETH_PARA_SIZE)


#define	BRD_ABORT_SWITCH_ADDR	BOARD_PARAMETER_NOT_USE 
#define	BRD_ABORT_SWITCH_BIT	BOARD_PARAMETER_NOT_USE	

 
#define LSI_CTL_EN			BOARD_PARAMETER_NOT_USE       
#define LSI_CTL_WP      	BOARD_PARAMETER_NOT_USE       
#define LSI_CTL_D8      	BOARD_PARAMETER_NOT_USE       
#define LSI_CTL_D16     	BOARD_PARAMETER_NOT_USE		  
#define LSI_CTL_D32     	BOARD_PARAMETER_NOT_USE       
#define LSI_CTL_D64     	BOARD_PARAMETER_NOT_USE       
#define LSI_CTL_A16     	BOARD_PARAMETER_NOT_USE       
#define LSI_CTL_A24     	BOARD_PARAMETER_NOT_USE       
#define LSI_CTL_A32     	BOARD_PARAMETER_NOT_USE       
#define LSI_CTL_CSR     	BOARD_PARAMETER_NOT_USE       
#define LSI_CTL_USER1   	BOARD_PARAMETER_NOT_USE       
#define LSI_CTL_USER2   	BOARD_PARAMETER_NOT_USE       
#define LSI_CTL_PGM     	BOARD_PARAMETER_NOT_USE       
#define LSI_CTL_DATA   		BOARD_PARAMETER_NOT_USE       
#define LSI_CTL_SUP     	BOARD_PARAMETER_NOT_USE       
#define LSI_CTL_USR     	BOARD_PARAMETER_NOT_USE       
#define LSI_CTL_BLK     	BOARD_PARAMETER_NOT_USE       
#define LSI_CTL_SINGLE  	BOARD_PARAMETER_NOT_USE       
#define LSI_CTL_PCI_MEM 	BOARD_PARAMETER_NOT_USE       
#define LSI_CTL_PCI_IO  	BOARD_PARAMETER_NOT_USE       
#define LSI_CTL_PCI_CONFIG 	BOARD_PARAMETER_NOT_USE       


#define VAL_LSI0_CTL    BOARD_PARAMETER_NOT_USE
#define	LSI0_CTL_OFFSET	BOARD_PARAMETER_NOT_USE

#define	VAL_LSI0_BS		BOARD_PARAMETER_NOT_USE	
#define	LSI0_BS_OFFSET	BOARD_PARAMETER_NOT_USE

#define	VAL_LSI0_BD		BOARD_PARAMETER_NOT_USE	
#define	LSI0_BD_OFFSET	BOARD_PARAMETER_NOT_USE

#define	VAL_LSI0_TO		BOARD_PARAMETER_NOT_USE	
#define	LSI0_TO_OFFSET	BOARD_PARAMETER_NOT_USE



#define VAL_LSI1_CTL    ( LSI_CTL_EN     | LSI_CTL_WP  |\
                          LSI_CTL_D32    | LSI_CTL_A32 |\
                          LSI_CTL_DATA   | LSI_CTL_USR |\
                          LSI_CTL_SINGLE | LSI_CTL_PCI_MEM )
#define	LSI1_CTL_OFFSET	BOARD_PARAMETER_NOT_USE

#define	VAL_LSI1_BS		BOARD_PARAMETER_NOT_USE	
#define	LSI1_BS_OFFSET	BOARD_PARAMETER_NOT_USE

#define	VAL_LSI1_BD		BOARD_PARAMETER_NOT_USE	
#define	LSI1_BD_OFFSET	BOARD_PARAMETER_NOT_USE

#define	VAL_LSI1_TO		BOARD_PARAMETER_NOT_USE	
#define	LSI1_TO_OFFSET	BOARD_PARAMETER_NOT_USE


#define VAL_LSI2_CTL    ( LSI_CTL_EN     | LSI_CTL_WP  |\
                          LSI_CTL_D32    | LSI_CTL_A24 |\
                          LSI_CTL_DATA   | LSI_CTL_USR |\
                          LSI_CTL_SINGLE | LSI_CTL_PCI_MEM )
#define	LSI2_CTL_OFFSET	BOARD_PARAMETER_NOT_USE

#define	VAL_LSI2_BS		BOARD_PARAMETER_NOT_USE	
#define	LSI2_BS_OFFSET	BOARD_PARAMETER_NOT_USE

#define	VAL_LSI2_BD		BOARD_PARAMETER_NOT_USE	
#define	LSI2_BD_OFFSET	BOARD_PARAMETER_NOT_USE

#define	VAL_LSI2_TO		BOARD_PARAMETER_NOT_USE	
#define	LSI2_TO_OFFSET	BOARD_PARAMETER_NOT_USE


#define VAL_LSI3_CTL    ( LSI_CTL_EN     | LSI_CTL_WP  |\
                          LSI_CTL_D16    | LSI_CTL_A16 |\
                          LSI_CTL_DATA   | LSI_CTL_USR |\
                          LSI_CTL_SINGLE | LSI_CTL_PCI_MEM )
#define	LSI3_CTL_OFFSET	BOARD_PARAMETER_NOT_USE

#define	VAL_LSI3_BS		BOARD_PARAMETER_NOT_USE	
#define	LSI3_BS_OFFSET	BOARD_PARAMETER_NOT_USE

#define	VAL_LSI3_BD		BOARD_PARAMETER_NOT_USE	
#define	LSI3_BD_OFFSET	BOARD_PARAMETER_NOT_USE

#define	VAL_LSI3_TO		BOARD_PARAMETER_NOT_USE	
#define	LSI3_TO_OFFSET	BOARD_PARAMETER_NOT_USE




#define VRAI_CTL_EN    		BOARD_PARAMETER_NOT_USE 	
#define VRAI_CTL_AM_DATA    BOARD_PARAMETER_NOT_USE    	
#define VRAI_CTL_AM_PGM     BOARD_PARAMETER_NOT_USE  	
#define VRAI_CTL_AM_SUPER   BOARD_PARAMETER_NOT_USE   	
#define VRAI_CTL_AM_USER    BOARD_PARAMETER_NOT_USE    	
#define VRAI_CTL_VAS_A16    BOARD_PARAMETER_NOT_USE    	
#define VRAI_CTL_VAS_A24    BOARD_PARAMETER_NOT_USE    	
#define VRAI_CTL_VAS_A32    BOARD_PARAMETER_NOT_USE    	
#define VRAI_CTL_VAS_USER1  BOARD_PARAMETER_NOT_USE    	
#define VRAI_CTL_VAS_USER2  BOARD_PARAMETER_NOT_USE    	



#define VRAI_REG_CTL    ( VRAI_CTL_EN     	 | VRAI_CTL_AM_DATA  |\
                          VRAI_CTL_AM_PGM    | VRAI_CTL_AM_SUPER |\
                          VRAI_CTL_AM_USER   | VRAI_CTL_VAS_A32 )
#define	VRAI_CTL_OFFSET	BOARD_PARAMETER_NOT_USE

#define	VARI_LSI1_BS	BOARD_PARAMETER_NOT_USE
#define	VRAI_BS_OFFSET	BOARD_PARAMETER_NOT_USE


#define	VME_SLAVE7_REG_CTRL_DATA	BOARD_PARAMETER_NOT_USE	
#define	VME_SLAVE7_CTRL_OFFSET		BOARD_PARAMETER_NOT_USE
#define	VME_SLAVE7_HIGH_OFFSET		BOARD_PARAMETER_NOT_USE
#define	VME_SLAVE7_LOW_OFFSET		BOARD_PARAMETER_NOT_USE	
	

#define	BOARD_MCH_EXP_OFFSET	0x00200		
#define	BOARD_DATA_EXP_OFFSET	0x00300			
#define	BOARD_ALN_EXP_OFFSET	0x00600		


#define	BOARD_LOCAL_BUS_ERR		0x6c 
#define	BOARD_VME_BUS_ERR		0x6b 
#define	BOARD_ACC_FAIL_ERR		0x67 


#define TSI148_INT_ENABLE(i)	intEnable (i)
#define TSI148_INT_DISABLE(i)	intDisable (i)


#elif (BOARD_TYPE == MVME2500_BOARD)




	#define	BRD_NVRAM_BASE_ADDR		0xa8
    #define BRD_NVRAM_START_ADDR    ((char *)(BRD_NVRAM_BASE_ADDR + 0x200))
    #define BRD_NVRAM_MAX_SIZE      0xD00 
    #define BRD_NVRAM_END_ADDR      ((char *)(BRD_NVRAM_START_ADDR+BRD_NVRAM_MAX_SIZE))


#define BRD_BOOT_LINE_ADDR      0x4200


#define	BRD_MAC_ADR_OFFSET	BOARD_PARAMETER_NOT_USE 


#define	BRD_TIME_ADR_OFFSET	BOARD_PARAMETER_NOT_USE 


#define	BRD_ETH_PARA_OFFSET	(512)
#define	BRD_ETH_PARA_SIZE	(256)


#define	USR_PARA_OFFSET		(BRD_ETH_PARA_OFFSET+BRD_ETH_PARA_SIZE)


#define	BRD_ABORT_SWITCH_ADDR	BOARD_PARAMETER_NOT_USE 
#define	BRD_ABORT_SWITCH_BIT	BOARD_PARAMETER_NOT_USE	

 
#define LSI_CTL_EN			BOARD_PARAMETER_NOT_USE       
#define LSI_CTL_WP      	BOARD_PARAMETER_NOT_USE       
#define LSI_CTL_D8      	BOARD_PARAMETER_NOT_USE       
#define LSI_CTL_D16     	BOARD_PARAMETER_NOT_USE		  
#define LSI_CTL_D32     	BOARD_PARAMETER_NOT_USE       
#define LSI_CTL_D64     	BOARD_PARAMETER_NOT_USE       
#define LSI_CTL_A16     	BOARD_PARAMETER_NOT_USE       
#define LSI_CTL_A24     	BOARD_PARAMETER_NOT_USE       
#define LSI_CTL_A32     	BOARD_PARAMETER_NOT_USE       
#define LSI_CTL_CSR     	BOARD_PARAMETER_NOT_USE       
#define LSI_CTL_USER1   	BOARD_PARAMETER_NOT_USE       
#define LSI_CTL_USER2   	BOARD_PARAMETER_NOT_USE       
#define LSI_CTL_PGM     	BOARD_PARAMETER_NOT_USE       
#define LSI_CTL_DATA   		BOARD_PARAMETER_NOT_USE       
#define LSI_CTL_SUP     	BOARD_PARAMETER_NOT_USE       
#define LSI_CTL_USR     	BOARD_PARAMETER_NOT_USE       
#define LSI_CTL_BLK     	BOARD_PARAMETER_NOT_USE       
#define LSI_CTL_SINGLE  	BOARD_PARAMETER_NOT_USE       
#define LSI_CTL_PCI_MEM 	BOARD_PARAMETER_NOT_USE       
#define LSI_CTL_PCI_IO  	BOARD_PARAMETER_NOT_USE       
#define LSI_CTL_PCI_CONFIG 	BOARD_PARAMETER_NOT_USE       


#define VAL_LSI0_CTL    BOARD_PARAMETER_NOT_USE
#define	LSI0_CTL_OFFSET	BOARD_PARAMETER_NOT_USE

#define	VAL_LSI0_BS		BOARD_PARAMETER_NOT_USE	
#define	LSI0_BS_OFFSET	BOARD_PARAMETER_NOT_USE

#define	VAL_LSI0_BD		BOARD_PARAMETER_NOT_USE	
#define	LSI0_BD_OFFSET	BOARD_PARAMETER_NOT_USE

#define	VAL_LSI0_TO		BOARD_PARAMETER_NOT_USE	
#define	LSI0_TO_OFFSET	BOARD_PARAMETER_NOT_USE



#define VAL_LSI1_CTL    ( LSI_CTL_EN     | LSI_CTL_WP  |\
                          LSI_CTL_D32    | LSI_CTL_A32 |\
                          LSI_CTL_DATA   | LSI_CTL_USR |\
                          LSI_CTL_SINGLE | LSI_CTL_PCI_MEM )
#define	LSI1_CTL_OFFSET	BOARD_PARAMETER_NOT_USE

#define	VAL_LSI1_BS		BOARD_PARAMETER_NOT_USE	
#define	LSI1_BS_OFFSET	BOARD_PARAMETER_NOT_USE

#define	VAL_LSI1_BD		BOARD_PARAMETER_NOT_USE	
#define	LSI1_BD_OFFSET	BOARD_PARAMETER_NOT_USE

#define	VAL_LSI1_TO		BOARD_PARAMETER_NOT_USE	
#define	LSI1_TO_OFFSET	BOARD_PARAMETER_NOT_USE


#define VAL_LSI2_CTL    ( LSI_CTL_EN     | LSI_CTL_WP  |\
                          LSI_CTL_D32    | LSI_CTL_A24 |\
                          LSI_CTL_DATA   | LSI_CTL_USR |\
                          LSI_CTL_SINGLE | LSI_CTL_PCI_MEM )
#define	LSI2_CTL_OFFSET	BOARD_PARAMETER_NOT_USE

#define	VAL_LSI2_BS		BOARD_PARAMETER_NOT_USE	
#define	LSI2_BS_OFFSET	BOARD_PARAMETER_NOT_USE

#define	VAL_LSI2_BD		BOARD_PARAMETER_NOT_USE	
#define	LSI2_BD_OFFSET	BOARD_PARAMETER_NOT_USE

#define	VAL_LSI2_TO		BOARD_PARAMETER_NOT_USE	
#define	LSI2_TO_OFFSET	BOARD_PARAMETER_NOT_USE


#define VAL_LSI3_CTL    ( LSI_CTL_EN     | LSI_CTL_WP  |\
                          LSI_CTL_D16    | LSI_CTL_A16 |\
                          LSI_CTL_DATA   | LSI_CTL_USR |\
                          LSI_CTL_SINGLE | LSI_CTL_PCI_MEM )
#define	LSI3_CTL_OFFSET	BOARD_PARAMETER_NOT_USE

#define	VAL_LSI3_BS		BOARD_PARAMETER_NOT_USE	
#define	LSI3_BS_OFFSET	BOARD_PARAMETER_NOT_USE

#define	VAL_LSI3_BD		BOARD_PARAMETER_NOT_USE	
#define	LSI3_BD_OFFSET	BOARD_PARAMETER_NOT_USE

#define	VAL_LSI3_TO		BOARD_PARAMETER_NOT_USE	
#define	LSI3_TO_OFFSET	BOARD_PARAMETER_NOT_USE




#define VRAI_CTL_EN    		BOARD_PARAMETER_NOT_USE 	
#define VRAI_CTL_AM_DATA    BOARD_PARAMETER_NOT_USE    	
#define VRAI_CTL_AM_PGM     BOARD_PARAMETER_NOT_USE  	
#define VRAI_CTL_AM_SUPER   BOARD_PARAMETER_NOT_USE   	
#define VRAI_CTL_AM_USER    BOARD_PARAMETER_NOT_USE    	
#define VRAI_CTL_VAS_A16    BOARD_PARAMETER_NOT_USE    	
#define VRAI_CTL_VAS_A24    BOARD_PARAMETER_NOT_USE    	
#define VRAI_CTL_VAS_A32    BOARD_PARAMETER_NOT_USE    	
#define VRAI_CTL_VAS_USER1  BOARD_PARAMETER_NOT_USE    	
#define VRAI_CTL_VAS_USER2  BOARD_PARAMETER_NOT_USE    	



#define VRAI_REG_CTL    ( VRAI_CTL_EN     	 | VRAI_CTL_AM_DATA  |\
                          VRAI_CTL_AM_PGM    | VRAI_CTL_AM_SUPER |\
                          VRAI_CTL_AM_USER   | VRAI_CTL_VAS_A32 )
#define	VRAI_CTL_OFFSET	BOARD_PARAMETER_NOT_USE

#define	VARI_LSI1_BS	BOARD_PARAMETER_NOT_USE
#define	VRAI_BS_OFFSET	BOARD_PARAMETER_NOT_USE


#define	VME_SLAVE7_REG_CTRL_DATA	BOARD_PARAMETER_NOT_USE	
#define	VME_SLAVE7_CTRL_OFFSET		BOARD_PARAMETER_NOT_USE
#define	VME_SLAVE7_HIGH_OFFSET		BOARD_PARAMETER_NOT_USE
#define	VME_SLAVE7_LOW_OFFSET		BOARD_PARAMETER_NOT_USE	
	

#define	BOARD_MCH_EXP_OFFSET	0x00200		
#define	BOARD_DATA_EXP_OFFSET	0x00300			
#define	BOARD_ALN_EXP_OFFSET	0x00600		


#define	BOARD_LOCAL_BUS_ERR		0x6c 
#define	BOARD_VME_BUS_ERR		0x6b 
#define	BOARD_ACC_FAIL_ERR		0x67 


#define TSI148_INT_ENABLE(i)	intEnable (i)
#define TSI148_INT_DISABLE(i)	intDisable (i)

#elif (BOARD_TYPE == LKV120_BOARD)	
		#define	BRD_NVRAM_BASE_ADDR		0xa8
	    #define BRD_NVRAM_START_ADDR    ((char *)(BRD_NVRAM_BASE_ADDR + 0x200))
	    #define BRD_NVRAM_MAX_SIZE      0xD00 
	    #define BRD_NVRAM_END_ADDR      ((char *)(BRD_NVRAM_START_ADDR+BRD_NVRAM_MAX_SIZE))
	
	#define BRD_BOOT_LINE_ADDR      0x4200
	
	
	#define	BRD_MAC_ADR_OFFSET	BOARD_PARAMETER_NOT_USE 
	
	
	#define	BRD_TIME_ADR_OFFSET	BOARD_PARAMETER_NOT_USE 
	
	
	#define	BRD_ETH_PARA_OFFSET	(512)
	#define	BRD_ETH_PARA_SIZE	(256)
	
	#define	USR_PARA_OFFSET		(BRD_ETH_PARA_OFFSET+BRD_ETH_PARA_SIZE)
	
	
	#define	BRD_ABORT_SWITCH_ADDR	BOARD_PARAMETER_NOT_USE 
	#define	BRD_ABORT_SWITCH_BIT	BOARD_PARAMETER_NOT_USE	
	
	
	#define LSI_CTL_EN			BOARD_PARAMETER_NOT_USE       
	#define LSI_CTL_WP      	BOARD_PARAMETER_NOT_USE       
	#define LSI_CTL_D8      	BOARD_PARAMETER_NOT_USE       
	#define LSI_CTL_D16     	BOARD_PARAMETER_NOT_USE		  
	#define LSI_CTL_D32     	BOARD_PARAMETER_NOT_USE       
	#define LSI_CTL_D64     	BOARD_PARAMETER_NOT_USE       
	#define LSI_CTL_A16     	BOARD_PARAMETER_NOT_USE       
	#define LSI_CTL_A24     	BOARD_PARAMETER_NOT_USE       
	#define LSI_CTL_A32     	BOARD_PARAMETER_NOT_USE       
	#define LSI_CTL_CSR     	BOARD_PARAMETER_NOT_USE       
	#define LSI_CTL_USER1   	BOARD_PARAMETER_NOT_USE       
	#define LSI_CTL_USER2   	BOARD_PARAMETER_NOT_USE       
	#define LSI_CTL_PGM     	BOARD_PARAMETER_NOT_USE       
	#define LSI_CTL_DATA   		BOARD_PARAMETER_NOT_USE       
	#define LSI_CTL_SUP     	BOARD_PARAMETER_NOT_USE       
	#define LSI_CTL_USR     	BOARD_PARAMETER_NOT_USE       
	#define LSI_CTL_BLK     	BOARD_PARAMETER_NOT_USE       
	#define LSI_CTL_SINGLE  	BOARD_PARAMETER_NOT_USE       
	#define LSI_CTL_PCI_MEM 	BOARD_PARAMETER_NOT_USE       
	#define LSI_CTL_PCI_IO  	BOARD_PARAMETER_NOT_USE       
	#define LSI_CTL_PCI_CONFIG 	BOARD_PARAMETER_NOT_USE       
	
	#define VAL_LSI0_CTL    BOARD_PARAMETER_NOT_USE
	#define	LSI0_CTL_OFFSET	BOARD_PARAMETER_NOT_USE
	
	#define	VAL_LSI0_BS		BOARD_PARAMETER_NOT_USE	
	#define	LSI0_BS_OFFSET	BOARD_PARAMETER_NOT_USE
	
	#define	VAL_LSI0_BD		BOARD_PARAMETER_NOT_USE	
	#define	LSI0_BD_OFFSET	BOARD_PARAMETER_NOT_USE
	
	#define	VAL_LSI0_TO		BOARD_PARAMETER_NOT_USE	
	#define	LSI0_TO_OFFSET	BOARD_PARAMETER_NOT_USE
	
	#define VAL_LSI1_CTL    ( LSI_CTL_EN     | LSI_CTL_WP  |\
	                          LSI_CTL_D32    | LSI_CTL_A32 |\
	                          LSI_CTL_DATA   | LSI_CTL_USR |\
	                          LSI_CTL_SINGLE | LSI_CTL_PCI_MEM )
	#define	LSI1_CTL_OFFSET	BOARD_PARAMETER_NOT_USE
	
	#define	VAL_LSI1_BS		BOARD_PARAMETER_NOT_USE	
	#define	LSI1_BS_OFFSET	BOARD_PARAMETER_NOT_USE
	
	#define	VAL_LSI1_BD		BOARD_PARAMETER_NOT_USE	
	#define	LSI1_BD_OFFSET	BOARD_PARAMETER_NOT_USE
	
	#define	VAL_LSI1_TO		BOARD_PARAMETER_NOT_USE	
	#define	LSI1_TO_OFFSET	BOARD_PARAMETER_NOT_USE
	
	#define VAL_LSI2_CTL    ( LSI_CTL_EN     | LSI_CTL_WP  |\
	                          LSI_CTL_D32    | LSI_CTL_A24 |\
	                          LSI_CTL_DATA   | LSI_CTL_USR |\
	                          LSI_CTL_SINGLE | LSI_CTL_PCI_MEM )
	#define	LSI2_CTL_OFFSET	BOARD_PARAMETER_NOT_USE
	
	#define	VAL_LSI2_BS		BOARD_PARAMETER_NOT_USE	
	#define	LSI2_BS_OFFSET	BOARD_PARAMETER_NOT_USE
	
	#define	VAL_LSI2_BD		BOARD_PARAMETER_NOT_USE	
	#define	LSI2_BD_OFFSET	BOARD_PARAMETER_NOT_USE
	
	#define	VAL_LSI2_TO		BOARD_PARAMETER_NOT_USE	
	#define	LSI2_TO_OFFSET	BOARD_PARAMETER_NOT_USE
	
	
	#define VAL_LSI3_CTL    ( LSI_CTL_EN     | LSI_CTL_WP  |\
	                          LSI_CTL_D16    | LSI_CTL_A16 |\
	                          LSI_CTL_DATA   | LSI_CTL_USR |\
	                          LSI_CTL_SINGLE | LSI_CTL_PCI_MEM )
	#define	LSI3_CTL_OFFSET	BOARD_PARAMETER_NOT_USE
	
	#define	VAL_LSI3_BS		BOARD_PARAMETER_NOT_USE	
	#define	LSI3_BS_OFFSET	BOARD_PARAMETER_NOT_USE
	
	#define	VAL_LSI3_BD		BOARD_PARAMETER_NOT_USE	
	#define	LSI3_BD_OFFSET	BOARD_PARAMETER_NOT_USE
	
	#define	VAL_LSI3_TO		BOARD_PARAMETER_NOT_USE	
	#define	LSI3_TO_OFFSET	BOARD_PARAMETER_NOT_USE
	
	
	#define VRAI_CTL_EN    		BOARD_PARAMETER_NOT_USE 	
	#define VRAI_CTL_AM_DATA    BOARD_PARAMETER_NOT_USE    	
	#define VRAI_CTL_AM_PGM     BOARD_PARAMETER_NOT_USE  	
	#define VRAI_CTL_AM_SUPER   BOARD_PARAMETER_NOT_USE   	
	#define VRAI_CTL_AM_USER    BOARD_PARAMETER_NOT_USE    	
	#define VRAI_CTL_VAS_A16    BOARD_PARAMETER_NOT_USE    	
	#define VRAI_CTL_VAS_A24    BOARD_PARAMETER_NOT_USE    	
	#define VRAI_CTL_VAS_A32    BOARD_PARAMETER_NOT_USE    	
	#define VRAI_CTL_VAS_USER1  BOARD_PARAMETER_NOT_USE    	
	#define VRAI_CTL_VAS_USER2  BOARD_PARAMETER_NOT_USE    	
	
	
	#define VRAI_REG_CTL    ( VRAI_CTL_EN     	 | VRAI_CTL_AM_DATA  |\
	                          VRAI_CTL_AM_PGM    | VRAI_CTL_AM_SUPER |\
	                          VRAI_CTL_AM_USER   | VRAI_CTL_VAS_A32 )
	#define	VRAI_CTL_OFFSET	BOARD_PARAMETER_NOT_USE
	
	#define	VARI_LSI1_BS	BOARD_PARAMETER_NOT_USE
	#define	VRAI_BS_OFFSET	BOARD_PARAMETER_NOT_USE
	
	#define	VME_SLAVE7_REG_CTRL_DATA	BOARD_PARAMETER_NOT_USE	
	#define	VME_SLAVE7_CTRL_OFFSET		BOARD_PARAMETER_NOT_USE
	#define	VME_SLAVE7_HIGH_OFFSET		BOARD_PARAMETER_NOT_USE
	#define	VME_SLAVE7_LOW_OFFSET		BOARD_PARAMETER_NOT_USE	
	
	#define	BOARD_MCH_EXP_OFFSET	0x00200		
	#define	BOARD_DATA_EXP_OFFSET	0x00300			
	#define	BOARD_ALN_EXP_OFFSET	0x00600		
	
	#define	BOARD_LOCAL_BUS_ERR		0x6c 
	#define	BOARD_VME_BUS_ERR		0x6b 
	#define	BOARD_ACC_FAIL_ERR		0x67 
	
	#define TSI148_INT_ENABLE(i)	intEnable (i)
	#define TSI148_INT_DISABLE(i)	intDisable (i)
	
#elif (BOARD_TYPE == X86_PC_BOARD) 
 #define BRD_NVRAM_START_ADDR    ((char *)(0xFFE00000 + 0x80000))
 #define BRD_NVRAM_MAX_SIZE      0x1F00 
 #define BRD_NVRAM_END_ADDR      ((char *)(BRD_NVRAM_START_ADDR+BRD_NVRAM_MAX_SIZE))

 #define	BRD_ETH_PARA_OFFSET	(256)
 #define	BRD_ETH_PARA_SIZE	(255)
 #define	USR_PARA_OFFSET		((256*2)+BRD_ETH_PARA_SIZE)
#else 
#error "You Should Define CPU Board(MVME2100, MVME3100, MVME3100, MVME2500, LKV120_BOARD, X86_PC_BOARD) !!!!"
#endif 

#define	ABORT_SWITCH_ADDR		BRD_ABORT_SWITCH_ADDR
#define	ABORT_SWITCH_PUSH_VAL	(0x0)
#define	ABORT_SWITCH_BIT		BRD_ABORT_SWITCH_BIT
	
#define NVRAM_START_ADDR    BRD_NVRAM_START_ADDR
#define NVRAM_MAX_SIZE      BRD_NVRAM_MAX_SIZE
#define NVRAM_END_ADDR      BRD_NVRAM_END_ADDR

#define BOOT_LINE_ADDR      BRD_BOOT_LINE_ADDR


#define	VME_MASTER_LM_REG_START_ADDR	VAL_LSI0_BS
#define	VME_MASTER_LM_REG_END_ADDR		VAL_LSI0_BD
#define	VME_MASTER_LM_REG_BIAS_ADDR		VAL_LSI0_TO
#define	VME_MASTER_LM_REG_CTRL_DATA		VAL_LSI0_CTL


#define	VME_MASTER_A32_START_ADDR	VAL_LSI1_BS
#define	VME_MASTER_A32_END_ADDR		VAL_LSI1_BD
#define	VME_MASTER_A32_BIAS_ADDR	VAL_LSI1_TO
#define	VME_MASTER_A32_CTRL_DATA	VAL_LSI1_CTL


#define	VME_MASTER_A24_START_ADDR	VAL_LSI2_BS
#define	VME_MASTER_A24_END_ADDR		VAL_LSI2_BD
#define	VME_MASTER_A24_BIAS_ADDR	VAL_LSI2_TO
#define	VME_MASTER_A24_CTRL_DATA	VAL_LSI2_CTL


#define	VME_MASTER_A16_START_ADDR	VAL_LSI3_BS
#define	VME_MASTER_A16_END_ADDR		VAL_LSI3_BD
#define	VME_MASTER_A16_BIAS_ADDR	VAL_LSI3_TO
#define	VME_MASTER_A16_CTRL_DATA	VAL_LSI3_CTL


#define	VME_SLAVE_REG_SRART_ADDR	VARI_LSI1_BS
#define	VME_SLAVE_REG_CTRL_DATA		VRAI_REG_CTL


#ifdef VXWORKS
#define ENET_ADDR   ((char *)(NVRAM_START_ADDR+BRD_MAC_ADR_OFFSET))
#endif

#define TOD_ADDR    ((char *)(NVRAM_START_ADDR+BRD_TIME_ADR_OFFSET))


typedef struct {
    uint8    msec;
    uint8    sec;
    uint8    min;
    uint8    hour;
    uint8    day; 
    uint8    date; 
    uint8    month;
    uint8    year;
}strBoardTimeInfo;


#define	BOARD_SERIAL_PORT_NUM	(1)
#define	MSG_LOG_PORT_ID			(1)

#define	MYCONSOLE_BAUD_RATE		(9600)


#define MAX_SYS_LOG_MSG_NUM     50


#define BOOT_PARA_START_ADDR    	NVRAM_START_ADDR
#define SYS_ENET_BOOT_PARA_OFFSET   BRD_ETH_PARA_OFFSET 
#define SYS_ENET_BOOT_PARA_SIZE     BRD_ETH_PARA_SIZE 


#ifdef VXWORKS
#define SYS_ENET_BOOT_LINE_ADDR     BOOT_LINE_ADDR

#if (BOARD_TYPE == MVME2100_BOARD)
	#define USR_ENET_CONFIG_PARA_OFFSET ((USR_PARA_OFFSET + 1)) 
#elif ((BOARD_TYPE == MVME3100_BOARD)||(BOARD_TYPE == MVME4100_BOARD)||(BOARD_TYPE == MVME2500_BOARD)||(BOARD_TYPE == LKV120_BOARD))
	#define USR_ENET_CONFIG_PARA_OFFSET (USR_PARA_OFFSET) 
#endif

#define USR_ENET_CONFIG_PARA_SIZE   (sizeof(strSystemBootParaInfo)) 

#if ((BOARD_TYPE == MVME3100_BOARD)||(BOARD_TYPE == MVME4100_BOARD)||(BOARD_TYPE == MVME2500_BOARD)||(BOARD_TYPE == LKV120_BOARD))
#ifdef  _MBTS_PROGRAM	

	#define USR_MBTS_CONFIG_OFFSET (USR_PARA_OFFSET + USR_ENET_CONFIG_PARA_SIZE + (0x200) + (0x300))
	#define USR_MBTS_CONFIG_SIZE   (0x100)
#endif	
#endif

#if (BOARD_TYPE == MVME2100_BOARD)
	#define SYS_ENET_BOOT_PARA1 \
    	"dc(0,0)host:/ftp/vxWorks h=192.9.211.155"
#elif ((BOARD_TYPE == MVME3100_BOARD)||(BOARD_TYPE == MVME2500_BOARD)||(BOARD_TYPE == LKV120_BOARD))


	#if (ROM_RELEASE_TO_USER == 1)
		#define	SYS_ENET_BOOT_PARA1 	"tffs=0(0,0)host:"
		#define	SYS_DEFAULT_BOOTFILE	"/tffs0/tb3100.rom"
	#elif (ROM_RELEASE_TO_USER == 0)
		
		#define	SYS_ENET_BOOT_PARA1 	"mottsec(0,0)host:"  
		#define	SYS_DEFAULT_BOOTFILE	"tb3100.rom"
	#else
		#error "You Should Define ROM_RELEASE_TO_USER Flag !!!!"
	#endif
	
	#define	SYS_DEFAULT_HOSTNAME	" h=192.9.211.123"
#elif (BOARD_TYPE == MVME4100_BOARD)

	#if (ROM_RELEASE_TO_USER == 1)
		#define	SYS_ENET_BOOT_PARA1 	"tffs=0(0,0)host:"
		#define	SYS_DEFAULT_BOOTFILE	"/RFA0/tb4100.rom"
	#elif (ROM_RELEASE_TO_USER == 0)
		
		#define	SYS_ENET_BOOT_PARA1 	"motetsec(0,0)host:"  
		#define	SYS_DEFAULT_BOOTFILE	"tb4100_xx.rom"
	#else
		#error "You Should Define ROM_RELEASE_TO_USER Flag !!!!"
	#endif

	#define	SYS_DEFAULT_HOSTNAME	" h=192.9.211.123"
#endif

#if ((BOARD_TYPE == MVME4100_BOARD) || (BOARD_TYPE == LKV120_BOARD))
	#define SYS_ENET_BOOT_PARA2 \
	    " u=tornado pw=tornado o=motetsec0"
#else
	#define SYS_ENET_BOOT_PARA2 \
	    " u=tornado pw=tornado o=mottsec0"
#endif


#define	DATA_EXP_VECTOR_OFFSET	BOARD_DATA_EXP_OFFSET
#define	MCH_EXP_VECTOR_OFFSET	BOARD_MCH_EXP_OFFSET
#define	ALN_EXP_VECTOR_OFFSET	BOARD_ALN_EXP_OFFSET


#define	LOCAL_BUS_ERR	BOARD_LOCAL_BUS_ERR
#define	VME_BUS_ERR		BOARD_VME_BUS_ERR
#define	ACC_FAIL_ERR	BOARD_ACC_FAIL_ERR

#else  
extern char* SYS_ENET_BOOT_LINE_ADDR;
extern char* ENET_ADDR;

#define SYS_ENET_BOOT_PARA1 \
	"HKEY_LOCAL_MACHINE\\Comm\\E100CE1\\Parms h=192.9.211.155"
#define SYS_ENET_BOOT_PARA2 \
	" u=WINCE4.2"
#define USR_ENET_CONFIG_PARA_OFFSET ((USR_PARA_OFFSET + 1)) 
#define USR_ENET_CONFIG_PARA_SIZE   sizeof(strSystemBootParaInfo) 

#ifdef  _MBTS_PROGRAM	
#define USR_MBTS_CONFIG_OFFSET (USR_PARA_OFFSET + USR_ENET_CONFIG_PARA_SIZE + (0x200) + (0x300))
#define USR_MBTS_CONFIG_SIZE   (0x100)
#endif	

#endif 


#define	MPU_RUN_MODE	(0x1)
#define	MPU_STBY_MODE	(0x2)
#define	MPU_ERR_MODE	(0x3)

typedef enum {
	enum_NORMAL,
	enum_RIGHT_TO_LEFT,
	enum_LEFT_TO_RIGHT,
	enum_BLINK
}enumShowType;

#define	USR_LED_1		1

#define	USR_LED_ON		1
#define	USR_LED_OFF		0

#endif 

/********************************************************************
**
**			Time Data Buf
**
*********************************************************************/
#define	MAX_TIME_DATA_YEAR	(2100)

#define	MIN_TIME_DATA_YEAR	(2010)
#define	REF_TIME_DATA_YEAR	(1900)
#define	YEAR_CONV_OFFSET	(100)

#define	MAX_TIME_DATA_MONTH	(11)
#define	MIN_TIME_DATA_MONTH	(0)

#define	MAX_TIME_DATA_DAY	(31)
#define	MIN_TIME_DATA_DAY	(1)

#define	MAX_TIME_DATA_HOUR	(23)
#define	MIN_TIME_DATA_HOUR	(0)

#define	MAX_TIME_DATA_MIN	(59)
#define	MIN_TIME_DATA_MIN	(0)

#define	MAX_TIME_DATA_SEC	(59)
#define	MIN_TIME_DATA_SEC	(0)


