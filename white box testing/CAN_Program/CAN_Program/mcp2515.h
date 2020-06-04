/*
 * mcp2515.h
 *
 * Created: 3/18/2019 08:35:17 ص
 *  Author: hppp
 */ 


#ifndef MCP2515_H_
#define MCP2515_H_

#include "micro_config.h"
#include "spi.h"
#define UNUSED -1
#define CLEARED 0 

#define SID 0
#define EID 1

#define MCP_RESET  0b11000000
#define BIT_MODIFY 0b00000101
#define BFPCTRL    0b00001100
#define TXRTSCTRL  0b00001101
#define RXB0CTRL   0b01100000
#define TXB0CTRL   0b00110000
#define CANTINTE   0b00101011
#define CNF1       0b00101010
#define CNF2       0b00101001
#define CNF3       0b00101000
#define TXB0SIDH   0b00110001
#define TXB0SIDL   0b00110010
#define TXB0DLC    0b00110101
#define TXB0D0     0b00110110
#define CANSTATE   0b00101110
#define CANCTRL    0b00101111
#define RXB0DLC    0b01100101
#define RXB0D0     0b01100110



#define RX_HI      0b00000000
#define RX_IN      0b00000100
#define RX_INT     0b00000101

#define RTS0 1
#define RTS1 2
#define RTS2 3

#define NORMAL_MODE 0
#define SLEEP_MODE 1
#define LOOPBACK_MODE 2
#define LISTEN_MODE 3
#define CONFIG_MODE 4

#define MCP_PRES_1 0
#define MCP_PRES_2 1
#define MCP_PRES_4 2
#define MCP_PRES_8 3

//read buff
#define R0SID      0x00
#define R0D        0x02
#define R1SID      0x04
#define R1D        0x06


//load tx
#define T0SID 0x00
#define T0D 0x01
#define T1SID 0x02
#define T1D 0x03
#define T2SID 0x04
#define T2D 0x05

//RTS
#define MCP_TX(n) (1 << (n))

#define MCP_RX(n)  (1 << (n))


#define MCP_FILTER(n) (0 + ((n) * 4)+(((n) > 2)? 4 : 0))

#define MCP_MASK(n) (0x20 + ((n) << 2))
//#define MCP_MASK(n) (0x20 + n)


/**************************CONFIGURATIONS******************************/
 
// I tried to make a macro that takes a the bit rate as a parameter and then configures the BPR and the number of
//TQ for each segment .. but I found from searching that the TQ for each segment has specific calculations based on
//some measurements implemented so .. the user of the system is the one who should apply these parameters as well
//rather than the code that should assign these value 

#define BRP 1   
#define PROP_SEG 2
#define PS1_SEG 7
#define PS2_SEG 6
#define SJW 2

// 1: three times  -- 0 : once 
#define SAMP_TIMES 0

/****RX0 receive interrupt pin(RX0BF) mode (this setting can be adjusted later by the function mcp_rx_pins****/
/*
0  : RX0BF high impedance (disabled)
5  : RX0BF pin interrupt
4  : RX0BF pin digital output 
*/
#define RX0BF_STATE 0


/****RX1 receive interrupt pin(RX0BF) mode (this setting can be adjusted later by the function mcp_rx_pins****/
/*
0  : RX1BF high impedance (disabled)
10 : RX1BF pin interrupt
8  : RX1BF pin digital output
*/
#define RX1BF_STATE 0

#define MASK0_ID 0x0fff
#define MASK0_TYPE SID

#define MASK1_ID CLEARED
#define MASK1_TYPE 0

#define FILTER0_ID 0x058f
#define FILTER0_TYPE SID

#define FILTER1_ID CLEARED
#define FILTER1_TYPE 0

#define FILTER2_ID CLEARED
#define FILTER2_TYPE 0

#define FILTER3_ID CLEARED
#define FILTER3_TYPE 0

#define FILTER4_ID CLEARED
#define FILTER4_TYPE 0

#define FILTER5_ID CLEARED
#define FILTER5_TYPE 0



/*
RTS pins(this setting can be adjusted later by the function RTS_pins
0b00000nnn  : TX2RTS - TX1RTS - TX0RTS respectively
NOTE:disable RTS pins will be digital inputs
*/
#define RTS_PINS 0b00000000

/******INT interrupts status*******/
//1 enable - 0 disable 
#define MERRE 0
 
#define WAKIE 0
 
#define ERRIE 0

#define TX2IE 0  

#define TX1IE 0

#define TX0IE 0

#define RX1IE 1

#define RX0IE 1

#define MCP_INT_BITS (MERRE << 7) |(WAKIE << 6) | (ERRIE << 5) | (TX2IE << 4) | (TX1IE << 3) | \
									(TX0IE << 2) | (RX1IE << 1) | (RX0IE << 0)


/*****ONE SHOT MODE**********
1: ONE SHOT
0: MULTISHOT
*/
#define ONE_SHOT 1

/*******INITIAL CLOCK OUTSTATE (CAN BE CHANGED IN THE afterwards by enable_ckout function)****
1: enable 
0: disable
*/
#define MCP_CLOCKOUT 0

//if MCP_CLOCKOUT = 1
/******CLOCK PRESCALER*
0: system_clock/1
1: system_clock/2
2: system_clock/4
3: system_clock/8
*/
#define MCP_CKOUT_PRE 0


/*********INITIAL OPERATION MODE - the state that will be activated after the initialization are complete 
***********(CAN BE CHANGED AFTERWARDS using mcp_opmode function**********
use one of these options
NORMAL_MODE
SLEEP_MODE
LOOPBACK_MODE
LISTEN_MODE
CONFIG_MODE
*/
#define MCP_OP_MODE LOOPBACK_MODE

/*******************END OF CONFIGURATIONS*********************/

/************************************************************************/ 


/*
rxnbf: RX_HI = high impedance ... RX_IN = input ... RX_INT = interrupt 
*/ 
void mcp_rx_pins (unint8_t rx0bf , unint8_t rx1bf);

/*
	RTSn = rts pin n as RTS function
	NOTE: to set multiple pins in rts mode .. you can OR them (RTS0 | RTS1) 
*/
void RTS_pins(unint8_t rts_pins);

/*
  tx_no    : 0 , 1 , 2
  priority : 0 ,1 , 2, 3 (highest is 3) 
*/
void txn_priorities (unint8_t tx_no, unint8_t priority);

/*
use one of these options(defined above)
NORMAL_MODE
SLEEP_MODE 
LOOPBACK_MODE 
LISTEN_MODE 
CONFIG_MODE 
*/
void mcp_set_opmode (unint8_t opmode);

//use prescaler #defines that are defined above
/*
you have these pre scalers
PRESCALER 1 PRES_1
PRESCALER 2 PRES_2
PRESCALER 4 PRES_4
PRESCALER 8 PRES_8
*/
void mcp_en_ckout (unint8_t prescaler);

void mcp_reset ();

void mcp_read (unint8_t add, unint8_t * data, unint8_t num_bytes);

void mcp_read_rx (unint8_t pos, unint8_t * data, unint8_t num_bytes);

void mcp_write (unint8_t add, unint8_t * data, unint8_t num_bytes);

void mcp_load_tx (unint8_t pos, unint8_t * data, unint8_t num_bytes);

/*
MCP_TX0
MCP_TX1
MCP_TX2

you can point to multiple tx by performing OR between the terms ie MCP_TX0 | MCP_TX1   
*/
void mcp_tx_trigger (unint8_t tx_no);

unint8_t mcp_read_status ();

unint8_t mcp_rx_status ();

void mcp_bit_modify (unint8_t add, unint8_t mask, unint8_t data);

void mcp_init (); 

/*
MCP_TX0
MCP_TX1
MCP_TX2

you can't assign multiple tx together
*/
void mcp_set_priority (unint8_t tx_no, unint8_t priority);

/*
MCP_TX0
MCP_TX1
MCP_TX2

you can point to multiple tx by performing OR between the terms ie. MCP_TX0 | MCP_TX1

state:
0 input pin
1 rts pin
*/
void rts_pins (unint8_t pins, unint8_t state);

/*
MCP_TX0
MCP_TX1
MCP_TX2
*/
void mcp_tx_id (unint8_t tx_no, unint8_t id_type, unint32_t id);

void mcp_tx_data (unint8_t tx_no, unint8_t * data, unint8_t d_size);

void mcp_rx_data(unint8_t rx_no, unint8_t * rx_buff);




//MCP_MASK(n)
//MCP_FILTER(n)
void mcp_set_mask_filter(unint8_t m_f_add, unint8_t id_type, unint32_t id);




#endif /* MCP2515_H_ */



