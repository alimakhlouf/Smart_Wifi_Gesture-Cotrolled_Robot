/*
 * mcp2515.c
 *
 * Created: 3/18/2019 08:34:55 ص
 *  Author: hppp
 */ 

#include "mcp2515.h"
#include "uart_api.h"

inline void mcp_set_eid(unint32_t id);

inline void  mcp_set_sid(unint16_t id);

void mcp_init()
{
	spi_init();
	
	mcp_reset();
	
	//RX0BF - RX1BF STATES
	mcp_bit_modify(BFPCTRL, 0b00001111, RX0BF_STATE | RX1BF_STATE);
	
	//REQUENST TRANSMISSION MESSAGE viA PINS
	mcp_bit_modify(TXRTSCTRL, 0b00000111, RTS_PINS);
	
	//remove all masks
	mcp_bit_modify(RXB0CTRL, 0b01100000, 0b00000000);
	mcp_bit_modify(RXB1CTRL, 0b01100000, 0b00000000);
	
	mcp_bit_modify(CANTINTE, 0b11111111, MCP_INT_BITS);

	mcp_bit_modify(CNF1, 0b11111111, ((SJW - 1) << 6) | BRP);
	
	mcp_bit_modify(CNF2, 0b11111111, (1 << 7) | (SAMP_TIMES << 6) | ((PS1_SEG - 1) << 3) | (PROP_SEG - 1));

	mcp_bit_modify(CNF3, 0b01000111, (PS2_SEG - 1));
	
	
	
	

	
	#if (MASK0_ID != CLEARED)
	mcp_set_mask_filter(MCP_MASK(0), MASK0_TYPE, MASK0_ID);
	#endif
	
	#if (MASK1_ID != CLEARED)
	mcp_set_mask_filter(MCP_MASK(1), MASK1_TYPE, MASK1_ID);
	#endif
	
	#if (FILTER0_ID != CLEARED)
	mcp_set_mask_filter(MCP_FILTER(0), FILTER0_TYPE, FILTER0_ID);
	#endif
	
	#if (FILTER1_ID != CLEARED)
	mcp_set_mask_filter(MCP_FILTER(1), FILTER1_TYPE, FILTER1_ID);
	#endif
	
	#if (FILTER2_ID != CLEARED)
	mcp_set_mask_filter(MCP_FILTER(2), FILTER2_TYPE, FILTER2_ID);
	#endif
	
	#if (FILTER3_ID != CLEARED)
	mcp_set_mask_filter(MCP_FILTER(3), FILTER3_TYPE, FILTER3_ID);
	#endif
	
	#if (FILTER4_ID != CLEARED)
	mcp_set_mask_filter(MCP_FILTER(4), FILTER4_TYPE, FILTER4_ID);
	#endif
	
	#if (FILTER5_ID != CLEARED)
	mcp_set_mask_filter(MCP_FILTER(5), FILTER5_TYPE, FILTER5_ID);
	#endif
	
	//determine the operation mode.. one shot mode
	//CANCTRL = 0b01001000;  //loopback mode ... one shot mode
	mcp_bit_modify(CANCTRL, 0b11111111, (MCP_OP_MODE << 5) | (ONE_SHOT << 3) | (MCP_CLOCKOUT << 2) | (MCP_CKOUT_PRE));
	
	
}


void mcp_reset()
{
	DIO_SET_VAL(SS, DIO_LOW);
	
	spi_send(MCP_RESET);
	DIO_SET_VAL(SS, DIO_HIGH);
}



void mcp_read(unint8_t add, unint8_t * data, unint8_t num_bytes)
{
	DIO_SET_VAL(SS, DIO_LOW);
	spi_send(0b00000011);
	spi_send(add);
	
	if (num_bytes == 1)
	{
		data[0] = spi_read();
	}
	else
	{
		spi_read_str_size(data, num_bytes);
	}

	DIO_SET_VAL(SS, DIO_HIGH);
}

void mcp_read_rx(unint8_t pos, unint8_t * data,unint8_t num_bytes)
{
	DIO_SET_VAL(SS, DIO_LOW);
	spi_send(0b10010000 | pos);
	
	spi_read_str_size(data, num_bytes);

	DIO_SET_VAL(SS, DIO_HIGH);
}

void mcp_write(unint8_t add, unint8_t * data, unint8_t num_bytes)
{
	DIO_SET_VAL(SS, DIO_LOW);
	spi_send(0b00000010);
	spi_send(add);
	
	spi_send_str_size(data, num_bytes);
	
	DIO_SET_VAL(SS, DIO_HIGH);
}

void mcp_load_tx(unint8_t pos, unint8_t * data, unint8_t num_bytes)
{
	DIO_SET_VAL(SS, DIO_LOW);
	spi_send(0b01000000 | pos);
	spi_send_str_size(data, num_bytes);
	DIO_SET_VAL(SS, DIO_HIGH);
}


void mcp_set_mask_filter(unint8_t m_f_no, unint8_t id_type, unint32_t id)
{
	DIO_SET_VAL(SS, DIO_LOW);
	spi_send(0b00000010);
	spi_send(m_f_no);

	if (id_type == 1) //if extended ID
	{
		mcp_set_eid(id);
	}
	else // if standard ID
	{
		mcp_set_sid(id);
	}
	
	DIO_SET_VAL(SS, DIO_HIGH);
}


void mcp_tx_id(unint8_t tx_no, unint8_t id_type, unint32_t id)
{
	//unint8_t var;
	
	DIO_SET_VAL(SS, DIO_LOW);

	if (id_type == 1) //if extended ID
	{
		spi_send(0b00000010);
		spi_send((TXB0SIDH + ((tx_no >> 1) << 4)));
		mcp_set_eid(id);		
	}
	else // if standard ID
	{
		
		if (tx_no == MCP_TX(0))
		{
			tx_no >>= 1;
		}

		spi_send(0b01000000 | tx_no);
		mcp_set_sid(id);

	}
	
	DIO_SET_VAL(SS, DIO_HIGH);
}


inline void mcp_set_eid(unint32_t id)
{

	spi_send((id >> 21));
	spi_send((((id >> 13) & 0b11100000) | (1 << 3) | ((id >> 16) & 0b00000011)));
	spi_send((id >> 8));
	spi_send(id);
}

inline void mcp_set_sid(unint16_t id)
{
	spi_send((id >> 3));
	spi_send(((id << 5)));
}

//void mcp_filter(unint8_t filter_no

void mcp_rx_data(unint8_t rx_no, unint8_t * rx_buff)
{
	unint8_t data_size = 0;
	
	DIO_SET_VAL(SS, DIO_LOW);
	spi_send(0b00000011);
	spi_send(RXB0DLC + (rx_no << 4));
	data_size = spi_read();
	//data_size = 8;
	spi_read_str_size(rx_buff, (data_size & 0x0f));
	DIO_SET_VAL(SS, DIO_HIGH);
}


void mcp_tx_data(unint8_t tx_no, uint8_t d_r_frame, unint8_t * data, unint8_t d_size)
{
	DIO_SET_VAL(SS, DIO_LOW);
	//DIO_SET_VAL(SS, DIO_LOW);
	spi_send(0b00000010);
	spi_send(TXB0DLC + ((tx_no >> 1) << 4));
	spi_send(d_size | (d_r_frame << 6));	
	
	if (d_r_frame == 0) //if the frame is A datA frame .. the store the bytes into the datA segment
	{
		spi_send_str_size(data, d_size);
	}	
	
	DIO_SET_VAL(SS, DIO_HIGH);
	//DIO_SET_VAL(SS, DIO_HIGH);
}

void mcp_tx_trigger(unint8_t tx_no)
{
	DIO_SET_VAL(SS, DIO_LOW);
	//DIO_SET_VAL(SS, DIO_LOW);	
	spi_send(0b10000000 | tx_no);		
	DIO_SET_VAL(SS, DIO_HIGH);
	//SPI_PORT|= (1 << SS);
	_delay_ms(5);
}



void mcp_bit_modify(unint8_t add, unint8_t mask, unint8_t data)
{
	DIO_SET_VAL(SS, DIO_LOW);
	//DIO_SET_VAL(SS, DIO_LOW);
	spi_send(BIT_MODIFY);
	spi_send(add);
	spi_send(mask);
	spi_send(data);
	DIO_SET_VAL(SS, DIO_HIGH);
	//SPI_PORT|= (1 << SS);
}

void mcp_rx_pins(unint8_t rx0bf , unint8_t rx1bf)
{
	mcp_bit_modify(BFPCTRL, 0b00001111, (rx0bf | (rx1bf << 1)));
}

void rts_pins(unint8_t pins, unint8_t state)
{
	mcp_bit_modify(TXRTSCTRL, 0b00000111, pins);
}

void mcp_set_priority(unint8_t tx_no, unint8_t priority)
{
	mcp_bit_modify(TXB0CTRL + ((tx_no >> 1) << 4), 0b00000011, priority);
}


void mcp_set_opmode(unint8_t opmode) 
{
	mcp_bit_modify(CANCTRL, 0b11100000, (opmode << 5));	
}

void mcp_en_ckout(unint8_t prescaler)
{
	mcp_bit_modify(CANCTRL, 0b00000111, (1 << 2) | prescaler);
}



uint8_t mcp_status()
{
	uint8_t data;
	
	DIO_SET_VAL(SS, DIO_LOW);
	spi_send(0b10100000);
	data = spi_read();
	DIO_SET_VAL(SS, DIO_HIGH);

	return data;
}




/*supported flags for the function
RX0IF_BIT
RX1IF_BIT
TX0IF_BIT
TX1IF_BIT
TX2IF_BIT


WAKIF_BIT
MERRF_BIT
ERRIF_BIT

TXREQ_BIT(n)
ABTF_BIT(n)
TXERR_BIT(n)
MLOA_BIT(n)
RX1OVR
RX0OVR
TXBO
TXEP
RXEP
TXWAR
RXWAR
EWARN
*/
uint8_t mcp_check_flag(uint8_t flag)
{
	uint8_t state;
	uint8_t data;
	
	if (flag < 8)
	{
		state = BIT_IS_SET(mcp_status(), flag);
	}
	else if (flag < 16)//from CANTINF
	{
		mcp_read(CANTINF, &data, 1);
		state = BIT_IS_SET(data, (flag - 8));
	}
	else if (flag < 24) // TXB0CTRL
	{
		mcp_read(TXB0CTRL, &data, 1);
		state = BIT_IS_SET(data, (flag - 16));
	}
	else if (flag < 32)// TXB1CTRL
	{
		mcp_read(TXB1CTRL, &data, 1);
		state = BIT_IS_SET(data, (flag - 24));
	}
	else if (flag < 40)// TXB2CTRL
	{
		mcp_read(TXB2CTRL, &data, 1);
		state = BIT_IS_SET(data, (flag - 32));
	}
	else //EFLG flags
	{
		mcp_read(EFLG, &data, 1);
		state = BIT_IS_SET(data, (flag - 40));
	}
	
	return state;
}

void mcp_clear_flag (uint8_t flag)
{
	if (flag < 8)
	{
		if (flag == 3)
		{
			flag -= 1;
		}
		
		if (flag == 5)
		{
			flag -= 2;
		}
		
		if (flag == 7)
		{
			flag -= 3;
		} 
		
		mcp_bit_modify(CANTINF, (1 << flag), 0);
	}
	else if (flag < 16)//from CANTINF
	{
		mcp_bit_modify(CANTINF, (1 << (flag - 8)), 0);
	}
	else
	{
		mcp_bit_modify(CANTINF, (1 << (flag - 40)), 0);
	}
}

//changed 

void mcp_send_dataframe(uint8_t tx_no, uint8_t * data_buff, uint8_t size) // could be inline
{
	mcp_tx_data(tx_no, DATA_FRAME, data_buff, size);
	mcp_tx_trigger(tx_no);
}

void mcp_send_remoteframe(uint8_t tx_no, uint8_t size)
{
	mcp_tx_data(tx_no, REMOTE_FRAME, NULL, size);
	mcp_tx_trigger(tx_no);
}
 
 
uint8_t mcp_remote_handler(uint8_t rx_no, uint8_t id_type)
{
	uint8_t rx_id[5];
	uint8_t tx_id[5];
	int8_t  tx_count = 0;
	int8_t  id_count = 0;
	
 	mcp_read(RXBNSIDH(rx_no), rx_id, (2 << id_type));
	 
	 rx_id[1] &= ~(1 << 4);
		
	for (tx_count = 0; (tx_count < 3); tx_count++)
	{
		mcp_read(TXBNSIDH(tx_count), tx_id, (2 << id_type));
		
		for (id_count = 0; id_count < (2 << id_type); id_count++)
		{
			
			if ((tx_id[id_count] != rx_id[id_count]))
			{
				break;
			}
			
		}
		
		if (id_count == (2 << id_type)) //if an id match is found
		{
			mcp_tx_trigger(MCP_TX(tx_count)); // send the corresponding tx_buff
			return 1;
		}
		
	}
	
	return 0;
}

uint8_t mcp_rx_status()
{
	uint8_t data;
	DIO_SET_VAL(SS, DIO_LOW);
	spi_send(0b10110000);
	data = spi_read();
	DIO_SET_VAL(SS, DIO_HIGH);
	
	return data;
}

uint8_t mcp_rx_status_frame(uint8_t rx_status_reg, uint8_t specific_bits) //volatile
{	
	return ((rx_status_reg & RX_STATUS_FRAME_BITS) == specific_bits);
}

uint8_t mcp_rx_status_rx(uint8_t rx_status_reg, uint8_t specific_bits) //volatile
{
	uint8_t temp = rx_status_reg & ((specific_bits == RX_STATUS_NO_RX)? RX_STATUS_BOTH_RX : specific_bits);
	return temp == specific_bits;
}

uint8_t mcp_rx_status_filter(uint8_t rx_status_reg, uint8_t specific_bits) // volatile
{
	return ((rx_status_reg & RX_STATUS_RXF_BITS) == specific_bits);
}

uint8_t mcp_status_rx(uint8_t stat_reg, uint8_t specific_bits) //volatile
{
	uint8_t temp = stat_reg & ((specific_bits == MCP_STAT_NO_RX)? MCP_STAT_BOTH_RX : specific_bits);
	return temp == specific_bits;
}

uint8_t mcp_status_tx(uint8_t stat_reg, uint8_t specific_bits) // volatile
{
	uint8_t temp = stat_reg & ((specific_bits == MCP_STAT_NO_TX)? MCP_STAT_BOTH_TX : specific_bits);
	return temp == specific_bits;	
}
