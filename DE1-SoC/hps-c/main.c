#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "hwlib.h"
#include "socal/socal.h"
#include "socal/hps.h"
//#include "socal/alt_gpio.h"
#include "hps_0.h"

#define HW_REGS_BASE ( ALT_STM_OFST ) //0xfc000000
#define HW_REGS_SPAN ( 0x04000000 )
#define HW_REGS_MASK ( HW_REGS_SPAN - 1 ) //0x3FFFFFF

int main() {

	void *virtual_base;
	int fd;
	void *h2p_lw_key_addr;
	void *h2p_lw_blockSeg1_addr;
	void *h2p_lw_blockSeg2_addr;
	void *h2p_lw_blockSeg3_addr;

	// map the address space for the LED registers into user space so we can interact with them.
	// we'll actually map in the entire CSR span of the HPS since we want to access various registers within that span

	if( ( fd = open( "/dev/mem", ( O_RDWR | O_SYNC ) ) ) == -1 ) {
		printf( "ERROR: could not open \"/dev/mem\"...\n" );
		return( 1 );
	}

	virtual_base = mmap( NULL, HW_REGS_SPAN, ( PROT_READ | PROT_WRITE ), MAP_SHARED, fd, HW_REGS_BASE );

	if( virtual_base == MAP_FAILED ) {
		printf( "ERROR: mmap() failed...\n" );
		close( fd );
		return( 1 );
	}
	// ALT_LWFPGALVS_OFST is the base address byte offset for the start of the ALT_LWFPGASLVS_OFST component. 
	h2p_lw_key_addr       = virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + PUSHBUTTONS_BASE ) & ( unsigned long)( HW_REGS_MASK ) );
	h2p_lw_blockSeg1_addr = virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + BLOCKSEG1_BASE ) & ( unsigned long)( HW_REGS_MASK ) );
	h2p_lw_blockSeg2_addr = virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + BLOCKSEG2_BASE ) & ( unsigned long)( HW_REGS_MASK ) );
	h2p_lw_blockSeg3_addr = virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + BLOCKSEG3_BASE ) & ( unsigned long)( HW_REGS_MASK ) );
	
	int exit = 1;
	while(exit){
		int pushButtons  = 0xf & *(uint32_t *) h2p_lw_key_addr;
		int displayState = 0;
		usleep(100*1000);
		if(pushButtons == 0b1100)
			exit = 0;
		else{
			switch(pushButtons){
				case 0b1110: displayState = 1; break;
				case 0b1101: displayState = 2; break;
				case 0b1011: displayState = 3; break;
				case 0b0111: displayState = 4; break;
			}
			// Controls 7-Segment Display
			switch(displayState){
				case 1:
					// FPGA P1
					*(uint32_t *) h2p_lw_blockSeg3_addr = 0b00011000001110; // HEX5 = F e HEX4 = P
					*(uint32_t *) h2p_lw_blockSeg2_addr = 0b00010000000010; // HEX2 = A e HEX3 = G
					*(uint32_t *) h2p_lw_blockSeg1_addr = 0b10000000001100;
					break;
				case 2:
					// FPGA P2
					*(uint32_t *) h2p_lw_blockSeg3_addr = 0b00011000001110; // HEX5 = F e HEX4 = P
					*(uint32_t *) h2p_lw_blockSeg2_addr = 0b00010000000010; // HEX2 = A e HEX3 = G
					*(uint32_t *) h2p_lw_blockSeg1_addr = 0b11110010001100;
					break;
				case 3:
					// FPGA P1
					*(uint32_t *) h2p_lw_blockSeg3_addr = 0b00011000001110; // HEX5 = F e HEX4 = P
					*(uint32_t *) h2p_lw_blockSeg2_addr = 0b00010000000010; // HEX2 = A e HEX3 = G
					*(uint32_t *) h2p_lw_blockSeg1_addr = 0b01001000001100;
					break;
				case 4:
					// FPGA P1
					*(uint32_t *) h2p_lw_blockSeg3_addr = 0b00011000001110; // HEX5 = F e HEX4 = P
					*(uint32_t *) h2p_lw_blockSeg2_addr = 0b00010000000010; // HEX2 = A e HEX3 = G
					*(uint32_t *) h2p_lw_blockSeg1_addr = 0b01100000001100;
					break;
				default:
					// INTEL
					*(uint32_t *) h2p_lw_blockSeg3_addr = 0b01010111111011;
					*(uint32_t *) h2p_lw_blockSeg2_addr = 0b00001100000111;
					*(uint32_t *) h2p_lw_blockSeg1_addr = 0b11111111000111;
			}
		}
	}
	// clean up our memory mapping and exit
	if( munmap( virtual_base, HW_REGS_SPAN ) != 0 ) {
		printf( "ERROR: munmap() failed...\n" );
		close( fd );
		return( 1 );
	}

	close( fd );

	return( 0 );
}
