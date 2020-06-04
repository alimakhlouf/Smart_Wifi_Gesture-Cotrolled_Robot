/*
 * mcp2515.c
 *
 * Created: 3/18/2019 08:34:55 ص
 *  Author: hppp
 */ 

#include "mcp2515.h"

inline void mcp_set_eid(unint32_t id);

inline void  mcp_set_sid(unint16_t id);

void mcp_reset()
{
	SPI_PORT &= ~(1 << SS);
	SPI_sendChr(MCP_RESET);
	SPI_PORT|= (1 << SS);
}

void mcp_read(unint8_t add, unint8_t * data, unint8_t num_bytes)
{
	SPI_PORT &= ~(1 << SS);
	SPI_sendChr(0b00000011);
	SPI_sendChr(add);
	
	if (num_bytes == 1)
	{
		data[0] = SPI_receiveChr();
	}
	else
	{
		SPI_receiveStrBySize(data, num_bytes);
	}

	SPI_PORT |= (1 << SS);
}

void mcp_read_rx(unint8_t pos, unint8_t * data,unint8_t num_bytes)
{
	SPI_PORT &= ~(1 << SS);
	SPI_sendChr(10010000 | pos);
	
	SPI_receiveStrBySize(data, num_bytes);

	SPI_PORT |= (1 << SS);
}

void mcp_write(unint8_t add, unint8_t * data, unint8_t num_bytes)
{
	SPI_PORT &= ~(1 << SS);
	SPI_sendChr(0b00000010);
	SPI_sendChr(add);
	
	SPI_sendStrBySize(data, num_bytes);
	
	SPI_PORT |= (1 << SS);
}

void mcp_load_tx(unint8_t pos, unint8_t * data, unint8_t num_bytes)
{
	SPI_PORT &= ~(1 << SS);
	SPI_sendChr(0b01000000 | pos);
	SPI_sendStrBySize(data, num_bytes);
	SPI_PORT |= (1 << SS);
}


void mcp_set_mask_filter(unint8_t m_f_no, unint8_t id_type, unint32_t id)
{
	SPI_PORT &= ~(1 << SS);
	SPI_sendChr(0b00000010);
	SPI_sendChr(m_f_no);

	if (id_type == 1) //if extended ID
	{
		mcp_set_eid(id);
	}
	else // if standard ID
	{
		mcp_set_sid(id);
	}
	
	SPI_PORT |= (1 << SS);
}


void mcp_tx_id(unint8_t tx_no, unint8_t id_type, unint32_t id)
{
	//unint8_t var;
	
	SPI_PORT &= ~(1 << SS);

	if (id_type == 1) //if extended ID
	{
		SPI_sendChr(0b00000010);
		SPI_sendChr((TXB0SIDH + ((tx_no >> 1) << 4)));
		mcp_set_eid(id);		
	}
	else // if standard ID
	{
		
		if (tx_no == MCP_TX(0))
		{
			tx_no >>= 1;
		}

		SPI_sendChr(0b01000000 | tx_no);
		mcp_set_sid(id);

	}
	
	SPI_PORT |= (1 << SS);
}


inline void mcp_set_eid(unint32_t id)
{

	SPI_sendChr((id >> 21));
	SPI_sendChr((((id >> 13) & 0b11100000) | (1 << 3) | ((id >> 16) & 0b00000011)));
	SPI_sendChr((id >> 8));
	SPI_sendChr(id);
}

inline void mcp_set_sid(unint16_t id)
{
	SPI_sendChr((id >> 3));
	SPI_sendChr(((id << 5)));
}

//void mcp_filter(unint8_t filter_no

void mcp_rx_data(unint8_t rx_no, unint8_t * rx_buff)
{
	unint8_t data_size = 0;
	
	SPI_PORT &= ~(1 << SS);
	SPI_sendChr(0b00000011);
	SPI_sendChr(RXB0DLC + ((rx_no >> 1) << 4));
	data_size = SPI_receiveChr();
	SPI_receiveStrBySize(rx_buff, data_size);
	SPI_PORT |= (1 << SS);
}


void mcp_tx_data(unint8_t tx_no, unint8_t * data, unint8_t d_size)
{
	SPI_PORT &= ~(1 << SS);
	SPI_sendChr(0b00000010);
	SPI_sendChr(TXB0DLC + ((tx_no >> 1) << 4));
	SPI_sendChr(d_size);		
	SPI_sendStrBySize(data, d_size);
	SPI_PORT |= (1 << SS);
}

void mcp_tx_trigger(unint8_t tx_no)
{
	SPI_PORT &= ~(1 << SS);	
	SPI_sendChr(0b10000000 | tx_no);		
	SPI_PORT|= (1 << SS);
	_delay_ms(5);

}

unint8_t mcp_read_status()
{
	unint8_t status = 0;
	
	SPI_PORT &= ~(1 << SS);
	SPI_sendChr(0b10100000);
	status = SPI_receiveChr();
	SPI_PORT|= (1 << SS);

	return status;
}

unint8_t mcp_rx_status()
{
	unint8_t rx_status = 0;
	
	SPI_PORT &= ~(1 << SS);
	SPI_sendChr(0b10110000);
	rx_status = SPI_receiveChr();
	SPI_PORT|= (1 << SS);
	
	return rx_status;
}

void mcp_bit_modify(unint8_t add, unint8_t mask, unint8_t data)
{
	SPI_PORT &= ~(1 << SS);
	SPI_sendChr(BIT_MODIFY);
	SPI_sendChr(add);
	SPI_sendChr(mask);
	SPI_sendChr(data);
	SPI_PORT|= (1 << SS);
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

void mcp_init()
{
	mcp_reset();
	
	//RX0BF - RX1BF STATES  
	mcp_bit_modify(BFPCTRL, 0b00001111, RX0BF_STATE | RX1BF_STATE);
	
	//REQUENST TRANSMISSION MESSAGE via PINS
	mcp_bit_modify(TXRTSCTRL, 0b00000111, RTS_PINS);
	
	//remove all masks
	mcp_bit_modify(RXB0CTRL, 0b01100000, 0b00000000);	
	
	mcp_bit_modify(CANTINTE, 0b11111111, MCP_INT_BITS);

	mcp_bit_modify(CNF1, 0b11111111, ((SJW - 1) << 6) | BRP);
	
	mcp_bit_modify(CNF2, 0b11111111, (1 << 7) | (SAMP_TIMES << 6) | ((PS1_SEG - 1) << 3) | (PROP_SEG - 1));

	mcp_bit_modify(CNF3, 0b01000111, (PS2_SEG - 1));
	
	mcp_set_mask_filter(MCP_MASK(0), MASK0_TYPE, MASK0_ID);
	
	mcp_set_mask_filter(MCP_FILTER(0), FILTER0_TYPE, FILTER0_ID);

	
	#if (MASK0_ID != CLEARED)
	//mcp_set_mask_filter(MCP_MASK(0), MASK0_TYPE, MASK0_ID);
	#endif
	
	#if (MASK1_ID != CLEARED)
	mcp_set_mask_filter(MCP_MASK(1), MASK1_TYPE, MASK1_ID);
	#endif
	
	#if (FILTER0_ID != CLEARED)
	//mcp_set_mask_filter(MCP_FILTER(0), FILTER0_TYPE, FILTER0_ID);
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
