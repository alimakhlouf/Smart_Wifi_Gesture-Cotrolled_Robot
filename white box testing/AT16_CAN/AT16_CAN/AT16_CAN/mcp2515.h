/*
 * mcp2515.h
 *
 * Created: 3/18/2019 08:35:17 ص
 *  Author: hppp
 */ 


#ifndef MCP2515_H_
#define MCP2515_H_

#include "micro_config.h"
#include "spi_api.h"


#define UNUSED -1
#define CLEARED 0 

#define DATA_FRAME 0
#define REMOTE_FRAME 1 

#define SID 0
#define EID 1

#define MCP_RESET  0b11000000
#define BIT_MODIFY 0b00000101
#define BFPCTRL    0b00001100
#define TXRTSCTRL  0b00001101
#define RXB0CTRL   0b01100000
#define RXB1CTRL   0b01110000
#define TXB0CTRL   0b00110000
#define TXB1CTRL   0b01000000
#define TXB2CTRL   0b01010000
#define TXBnCTRL(n)(0x30 + ((n) << 4))
#define EFLG       0b00101101

#define CANTINTE   0b00101011
#define CNF1       0b00101010
#define CNF2       0b00101001
#define CNF3       0b00101000
#define TXB0SIDH   0b00110001
#define TXB0SIDL   0b00110010
#define TXBNSIDH(n)(TXB0SIDH + ((n) << 4))
#define TXB0DLC    0b00110101
#define TXB0D0     0b00110110
#define CANSTATE   0b00101110
#define CANTINF    0b00101100
#define CANCTRL    0b00101111
#define RXB0DLC    0b01100101
#define RXB0D0     0b01100110
#define RXB0SIDH   0b01100001
#define RXB1SIDH   0b01110001
#define RXBNSIDH(n)(RXB0SIDH + ((n) << 4))


//those bits are obtained by the read_status spi interuction
#define RX0IF_BIT 0
#define RX1IF_BIT 1
#define TXREQ_BIT(n) (2 + ((n) << 1) // #define TXREQ_BIT(2) 6 #define TXREQ_BIT(1) 4 #define TXREQ_BIT(0) 2
#define TX0IF_BIT 3
#define TX1IF_BIT 5
#define TX2IF_BIT 7

//bits from CANTINF
#define ERRIF_BIT 13 //8 + 5
#define WAKIF_BIT 14 //8 + 6
#define MERRF_BIT 15 //8 + 7


//TXBnCTRL  ABTF MLOA TXERR
#define TXERR_BIT(n) (16 + ((n) << 3)) //16/24/32 + 4
#define MLOA_BIT(n)  (24 + ((n) << 3)) //16 + 5
#define ABTF_BIT(n)  (24 + ((n) << 3)) //16 + 6

//EFLG bits RX1OVR RX0OVR TXBO TXEP RXEP TXWAR RXWAR EWARN
#define RX1OVR_BIT 47
#define RX0OVR_BIT 46
#define TXBO_BIT   45
#define TXEP_BIT   44
#define RXEP_BIT   43
#define TXWAR_BIT  42
#define RXWAR_BIT  41
#define EWARN_BIT  40



#define RX_HI      0b00000000
#define RX_IN      0b00000100
#define RX_INT     0b00000101

#define RTS0 1
#define RTS1 2
#define RTS2 3

#define NORMAL_MODE   0
#define SLEEP_MODE    1
#define LOOPBACK_MODE 2
#define LISTEN_MODE   3
#define CONFIG_MODE   4

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
#define T0D   0x01
#define T1SID 0x02
#define T1D   0x03
#define T2SID 0x04
#define T2D   0x05

//RTS
#define MCP_TX(n) (1 << (n))

#define MCP_RX(n)  (n)


#define MCP_FILTER(n) (0 + ((n) * 4)+(((n) > 2)? 4 : 0))

#define MCP_MASK(n) (0x20 + ((n) << 2))

#define RX_STATUS_FRAME_BITS 0b00011000
#define RX_STATUS_SREMOTE    0b00001000
#define RX_STATUS_EREMOTE    0b00011000

#define RX_STATUS_RX_BITS    0b00011000
#define RX_STATUS_RX0        0b01000000
#define RX_STATUS_RX1        0b10000000
#define RX_STATUS_BOTH_RX    0b11000000
#define RX_STATUS_NO_RX      0b00000000

#define RX_STATUS_RXF_BITS   0b00000111
#define RX_STATUS_RXF(n)     (n)

#define MCP_STAT_NO_RX        0x00
#define MCP_STAT_BOTH_RX      0x03
#define MCP_STAT_RX_BITS      0x03
#define MCP_STAT_RX0		  0x01
#define MCP_STAT_RX1		  0x02
#define MCP_STAT_NO_TX        0x00
#define MCP_STAT_BOTH_TX	  0xa8
#define MCP_STAT_TX_BITS      0xa8
#define MCP_STAT_TX0          0x08
#define MCP_STAT_TX1          0x20
#define MCP_STAT_TX2          0x80


/**************************CONFIGURATIONS**********************************************************/
 
// I tried to make A macro that takes A the bit rate as A parameter and then configures the BPR and the number of
//TQ for each segment .. but I found from searching that the TQ for each segment has specific calculations based on
//some measurements implemented so .. the user of the system is the one who should apply these parameters as well
//rather than the code that should assign these value 

#define BRP 1   
#define PROP_SEG 2
#define PS1_SEG 7
#define PS2_SEG 6
#define SJW 2

/* 
1: three times
0 : one time
*/ 
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

#define MASK0_ID   0x0fff
#define MASK0_TYPE SID

#define MASK1_ID   0x0fff
#define MASK1_TYPE SID

#define FILTER0_ID   0x058f
#define FILTER0_TYPE SID

#define FILTER1_ID   CLEARED
#define FILTER1_TYPE SID

#define FILTER2_ID   0x057f
#define FILTER2_TYPE SID

#define FILTER3_ID   0x056f
#define FILTER3_TYPE SID

#define FILTER4_ID   CLEARED
#define FILTER4_TYPE SID

#define FILTER5_ID   CLEARED
#define FILTER5_TYPE SID



/*
RTS pins(this setting can be adjusted later by the function RTS_pins
0b00000nnn  : TX2RTS - TX1RTS - TX0RTS respectively
NOTE:disable RTS pins will be digital inputs
*/
#define RTS_PINS 0b00000000

/******INT interrupts status*******/
//1 enable - 0 disable 
#define MERRE 1
 
#define WAKIE 1
 
#define ERRIE 1

#define TX2IE 1  

#define TX1IE 1

#define TX0IE 1

#define RX1IE 1

#define RX0IE 1

#define MCP_INT_BITS ((MERRE << 7) |(WAKIE << 6) | (ERRIE << 5) | (TX2IE << 4) | (TX1IE << 3) | \
									(TX0IE << 2) | (RX1IE << 1) | (RX0IE << 0))


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
#define MCP_OP_MODE NORMAL_MODE

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
void RTS_pins (unint8_t rts_pins);

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

void mcp_read_rx (unint8_t rx_no, unint8_t * data, unint8_t num_bytes);

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

/*
in the second parameter you either enter:
DATA_FRAME 
REMOTE_FRAME ( in this case the third segment is unused .. so you can pass NULL to it)
*/
void mcp_tx_data(unint8_t tx_no, uint8_t d_r_frame, unint8_t * data, unint8_t d_size);

void mcp_rx_data(unint8_t rx_no, unint8_t * rx_buff);




//MCP_MASK(n)
//MCP_FILTER(n)
void mcp_set_mask_filter (unint8_t m_f_add, unint8_t id_type, unint32_t id);

//CHANGED
uint8_t mcp_rx_status ();
uint8_t mcp_status ();

/*supported flags for the function
RX0IF
RX1IF
TX0IF
TX1IF
TX2IF
WAKIF
MERRF
TXREQ(n)
ABTF(n)
TXERR(n)
MLOA(n)
RX1OVR
RX0OVR
TXBO
TXEP
RXEP
TXWAR
RXWAR
EWARN
*/
uint8_t mcp_check_flag (uint8_t flag);

void mcp_clear_flag (uint8_t flag);
void mcp_send_dataframe (uint8_t tx_no, uint8_t * data_buff, uint8_t size); // could be inline
void mcp_send_remoteframe (uint8_t tx_no, uint8_t size);
uint8_t mcp_remote_handler (uint8_t rx_no, uint8_t id_type);

uint8_t mcp_rx_status_frame (uint8_t rx_status_reg, uint8_t);
uint8_t mcp_rx_status_rx (uint8_t rx_status_reg, uint8_t);
uint8_t mcp_rx_status_filter (uint8_t rx_status_reg, uint8_t);

uint8_t mcp_status_tx (uint8_t stat_reg, uint8_t );
uint8_t mcp_status_rx (uint8_t stat_reg, uint8_t );


#endif /* MCP2515_H_ */
