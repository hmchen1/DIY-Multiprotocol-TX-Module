/*
 This project is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Multiprotocol is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Multiprotocol.  If not, see <http://www.gnu.org/licenses/>.
 */


//---------------------------
// AVR nrf chip bitbang SPI functions
//---------------------------
#include "iface_nrf24l01.h"

void nrf_spi_write(uint8_t command)
{
	uint8_t n=8; 

	SCK_off;//SCK start low
	SDI_off;
	while(n--) {
		if(command&0x80)
			SDI_on;
		else 
			SDI_off;
		SCK_on;
		NOP();
		SCK_off;
		command = command << 1;
	}
	SDI_on;
}  

//VARIANT 2
uint8_t nrf_spi_read(void)
{
	uint8_t result;
	uint8_t i;
	result=0;
	for(i=0;i<8;i++) {                    
		result<<=1;
		if(SDO_1)  ///
			result|=0x01;
		SCK_on;
		NOP();
		SCK_off;
		NOP();
	}
	return result;
}   
//--------------------------------------------



//---------------------------
// NRF24L01+ SPI Specific Functions
//---------------------------

uint8_t rf_setup;

void NRF24L01_Initialize()
{
    rf_setup = 0x09;
}  

void NRF24L01_WriteReg(uint8_t reg, uint8_t data)
{
	NRF_CSN_off;
	nrf_spi_write(W_REGISTER | (REGISTER_MASK & reg));
	nrf_spi_write(data);
	NRF_CSN_on;
}

void NRF24L01_WriteRegisterMulti(uint8_t reg, uint8_t * data, uint8_t length)
{
	NRF_CSN_off;

	nrf_spi_write(W_REGISTER | ( REGISTER_MASK & reg));
	for (uint8_t i = 0; i < length; i++)
		nrf_spi_write(data[i]);
	NRF_CSN_on;
}

void NRF24L01_WritePayload(uint8_t * data, uint8_t length)
{
	NRF_CSN_off;
	nrf_spi_write(W_TX_PAYLOAD);
	for (uint8_t i = 0; i < length; i++)
		nrf_spi_write(data[i]);
	NRF_CSN_on;
}

uint8_t NRF24L01_ReadReg(uint8_t reg)
{
	NRF_CSN_off;
	nrf_spi_write(R_REGISTER | (REGISTER_MASK & reg));
	uint8_t data = nrf_spi_read();
	NRF_CSN_on;
	return data;
}

void NRF24L01_ReadRegisterMulti(uint8_t reg, uint8_t * data, uint8_t length)
{
	NRF_CSN_off;
	nrf_spi_write(R_REGISTER | (REGISTER_MASK & reg));
	for(uint8_t i = 0; i < length; i++)
		data[i] = nrf_spi_read();
	NRF_CSN_on;
}

void NRF24L01_ReadPayload(uint8_t * data, uint8_t length)
{
	NRF_CSN_off;
	nrf_spi_write(R_RX_PAYLOAD);
	for(uint8_t i = 0; i < length; i++)
		data[i] = nrf_spi_read();
	NRF_CSN_on; 
}

void  NRF24L01_Strobe(uint8_t state)
{
	NRF_CSN_off;
	nrf_spi_write(state);
	NRF_CSN_on;
}

void NRF24L01_FlushTx()
{
	NRF24L01_Strobe(FLUSH_TX);
}

void NRF24L01_FlushRx()
{
	NRF24L01_Strobe(FLUSH_RX);
}

void NRF24L01_Activate(uint8_t code)
{
	NRF_CSN_off;
	nrf_spi_write(ACTIVATE);
	nrf_spi_write(code);
	NRF_CSN_on;
}

void NRF24L01_SetBitrate(uint8_t bitrate)
{
    // Note that bitrate 250kbps (and bit RF_DR_LOW) is valid only
    // for nRF24L01+. There is no way to programmatically tell it from
    // older version, nRF24L01, but the older is practically phased out
    // by Nordic, so we assume that we deal with with modern version.

    // Bit 0 goes to RF_DR_HIGH, bit 1 - to RF_DR_LOW
    rf_setup = (rf_setup & 0xD7) | ((bitrate & 0x02) << 4) | ((bitrate & 0x01) << 3);
    NRF24L01_WriteReg(NRF24L01_06_RF_SETUP, rf_setup);
}

void NRF24L01_SetPower_Value(uint8_t power)
{
    uint8_t nrf_power = 0;
    switch(power) {
        case TXPOWER_100uW: nrf_power = 0; break;
        case TXPOWER_300uW: nrf_power = 0; break;
        case TXPOWER_1mW:   nrf_power = 0; break;
        case TXPOWER_3mW:   nrf_power = 1; break;
        case TXPOWER_10mW:  nrf_power = 1; break;
        case TXPOWER_30mW:  nrf_power = 2; break;
        case TXPOWER_100mW: nrf_power = 3; break;
        case TXPOWER_150mW: nrf_power = 3; break;
        default:            nrf_power = 0; break;
    };
    // Power is in range 0..3 for nRF24L01
    rf_setup = (rf_setup & 0xF9) | ((nrf_power & 0x03) << 1);
    NRF24L01_WriteReg(NRF24L01_06_RF_SETUP, rf_setup);
}

void NRF24L01_SetPower()
{
	uint8_t power=NRF_BIND_POWER;
	if(IS_BIND_DONE_on)
		power=IS_POWER_FLAG_on?NRF_HIGH_POWER:NRF_LOW_POWER;
    else
		if(IS_RANGE_FLAG_on)
			power=NRF_POWER_0;
	rf_setup = (rf_setup & 0xF9) | (power << 1);
    NRF24L01_WriteReg(NRF24L01_06_RF_SETUP, rf_setup);
}

void NRF24L01_SetTxRxMode(enum TXRX_State mode)
{
	if(mode == TX_EN) {
		NRF_CSN_off;
		NRF24L01_WriteReg(NRF24L01_07_STATUS, (1 << NRF24L01_07_RX_DR)    //reset the flag(s)
											| (1 << NRF24L01_07_TX_DS)
											| (1 << NRF24L01_07_MAX_RT));
		NRF24L01_WriteReg(NRF24L01_00_CONFIG, (1 << NRF24L01_00_EN_CRC)   // switch to TX mode
											| (1 << NRF24L01_00_CRCO)
											| (1 << NRF24L01_00_PWR_UP));
		_delay_us(130);
		NRF_CSN_on;
	}
	else
		if (mode == RX_EN) {
			NRF_CSN_off;
			NRF24L01_WriteReg(NRF24L01_07_STATUS, 0x70);        // reset the flag(s)
			NRF24L01_WriteReg(NRF24L01_00_CONFIG, 0x0F);        // switch to RX mode
			NRF24L01_WriteReg(NRF24L01_07_STATUS, (1 << NRF24L01_07_RX_DR)    //reset the flag(s)
												| (1 << NRF24L01_07_TX_DS)
												| (1 << NRF24L01_07_MAX_RT));
			NRF24L01_WriteReg(NRF24L01_00_CONFIG, (1 << NRF24L01_00_EN_CRC)   // switch to RX mode
												| (1 << NRF24L01_00_CRCO)
												| (1 << NRF24L01_00_PWR_UP)
												| (1 << NRF24L01_00_PRIM_RX));
			_delay_us(130);
			NRF_CSN_on;
		}
		else
		{
			NRF24L01_WriteReg(NRF24L01_00_CONFIG, (1 << NRF24L01_00_EN_CRC)); //PowerDown
			NRF_CSN_off;
		}
}

void NRF24L01_Reset()
{
	//** not in deviation but needed to hot switch between models
	NRF24L01_Activate(0x73);                          // Activate feature register
	NRF24L01_WriteReg(NRF24L01_1C_DYNPD, 0x00);       // Disable dynamic payload length on all pipes
	NRF24L01_WriteReg(NRF24L01_1D_FEATURE, 0x00);     // Set feature bits off
	NRF24L01_Activate(0x73);
	//**

	NRF24L01_FlushTx();
    NRF24L01_FlushRx();
    NRF24L01_Strobe(0xff);			// NOP
    NRF24L01_ReadReg(0x07);
    NRF24L01_SetTxRxMode(TXRX_OFF);
	_delay_us(100);
}

uint8_t NRF24L01_packet_ack()
{
    switch (NRF24L01_ReadReg(NRF24L01_07_STATUS) & (BV(NRF24L01_07_TX_DS) | BV(NRF24L01_07_MAX_RT))) {
		case BV(NRF24L01_07_TX_DS):
			return PKT_ACKED;
		case BV(NRF24L01_07_MAX_RT):
			return PKT_TIMEOUT;
    }
	return PKT_PENDING;
}



//---------------------------
/*
void NRF24L01_spi_test(void)
{
unsigned long errors = 0;
unsigned long test = 0;
unsigned long time;
uint8_t test_data_r[5];
uint8_t test_data_w[5] = {0x01,0x02,0x03,0x04,0x05};

time = micros();
Serial.println("Testing SPI");
  for(test=0; test < 2775600 ; test++) // should run for X mins.
  {
  NRF24L01_WriteRegisterMulti(NRF24L01_0B_RX_ADDR_P1, test_data_w, 5);
  NRF24L01_ReadRegisterMulti(NRF24L01_0B_RX_ADDR_P1, test_data_r, 5);
    if(0 != memcmp(test_data_r, test_data_w, sizeof(test_data_r))) errors++;   
  test_data_w[0] ++;
  test_data_w[1] ++;
  test_data_w[2] ++;
  test_data_w[3] ++;
  test_data_w[4] ++;
  }
Serial.print("test "); Serial.print(test, HEX); Serial.print("\n");
Serial.print("errors "); Serial.print(errors, HEX); Serial.print("\n");
Serial.print("time "); Serial.print(micros()- time, DEC); Serial.print("\n");

// 124211960
//  90899216
}
*/
//---------------------------


///////////////
// XN297 emulation layer
uint8_t xn297_addr_len;
uint8_t xn297_tx_addr[5];
uint8_t xn297_rx_addr[5];
uint8_t xn297_crc = 0;

static const uint8_t xn297_scramble[] = {
	0xe3, 0xb1, 0x4b, 0xea, 0x85, 0xbc, 0xe5, 0x66,
	0x0d, 0xae, 0x8c, 0x88, 0x12, 0x69, 0xee, 0x1f,
	0xc7, 0x62, 0x97, 0xd5, 0x0b, 0x79, 0xca, 0xcc,
	0x1b, 0x5d, 0x19, 0x10, 0x24, 0xd3, 0xdc, 0x3f,
	0x8e, 0xc5, 0x2f};

static const uint16_t xn297_crc_xorout[] = {
	0x0000, 0x3448, 0x9BA7, 0x8BBB, 0x85E1, 0x3E8C, // 1st entry is missing, probably never needed
	0x451E, 0x18E6, 0x6B24, 0xE7AB, 0x3828, 0x8148, // it's used for 3-byte address w/ 0 byte payload only
	0xD461, 0xF494, 0x2503, 0x691D, 0xFE8B, 0x9BA7,
	0x8B17, 0x2920, 0x8B5F, 0x61B1, 0xD391, 0x7401,
	0x2138, 0x129F, 0xB3A0, 0x2988};

uint8_t bit_reverse(uint8_t b_in)
{
    uint8_t b_out = 0;
    for (uint8_t i = 0; i < 8; ++i)
	{
        b_out = (b_out << 1) | (b_in & 1);
        b_in >>= 1;
    }
    return b_out;
}

uint16_t crc16_update(uint16_t crc, uint8_t a)
{
	static const uint16_t polynomial = 0x1021;

	crc ^= a << 8;
    for (uint8_t i = 0; i < 8; ++i)
        if (crc & 0x8000)
            crc = (crc << 1) ^ polynomial;
		else
            crc = crc << 1;
    return crc;
}

void XN297_SetTXAddr(const uint8_t* addr, uint8_t len)
{
	if (len > 5) len = 5;
	if (len < 3) len = 3;
	uint8_t buf[] = { 0x55, 0x0F, 0x71, 0x0C, 0x00 }; // bytes for XN297 preamble 0xC710F55 (28 bit)
	xn297_addr_len = len;
	if (xn297_addr_len < 4)
		for (uint8_t i = 0; i < 4; ++i)
			buf[i] = buf[i+1];
	NRF24L01_WriteReg(NRF24L01_03_SETUP_AW, len-2);
	NRF24L01_WriteRegisterMulti(NRF24L01_10_TX_ADDR, buf, 5);
	// Receive address is complicated. We need to use scrambled actual address as a receive address
	// but the TX code now assumes fixed 4-byte transmit address for preamble. We need to adjust it
	// first. Also, if the scrambled address begins with 1 nRF24 will look for preamble byte 0xAA
	// instead of 0x55 to ensure enough 0-1 transitions to tune the receiver. Still need to experiment
	// with receiving signals.
	memcpy(xn297_tx_addr, addr, len);
}

void XN297_SetRXAddr(const uint8_t* addr, uint8_t len)
{
	if (len > 5) len = 5;
	if (len < 3) len = 3;
	uint8_t buf[] = { 0, 0, 0, 0, 0 };
	memcpy(buf, addr, len);
	memcpy(xn297_rx_addr, addr, len);
	for (uint8_t i = 0; i < xn297_addr_len; ++i)
		buf[i] = xn297_rx_addr[i] ^ xn297_scramble[xn297_addr_len-i-1];
	NRF24L01_WriteReg(NRF24L01_03_SETUP_AW, len-2);
	NRF24L01_WriteRegisterMulti(NRF24L01_0A_RX_ADDR_P0, buf, 5);
}

void XN297_Configure(uint8_t flags)
{
	xn297_crc = !!(flags & BV(NRF24L01_00_EN_CRC));
	flags &= ~(BV(NRF24L01_00_EN_CRC) | BV(NRF24L01_00_CRCO));
	NRF24L01_WriteReg(NRF24L01_00_CONFIG, flags);
}

void XN297_WritePayload(uint8_t* msg, uint8_t len)
{
	uint8_t buf[32];
	uint8_t last = 0;

	if (xn297_addr_len < 4)
	{
		// If address length (which is defined by receive address length)
		// is less than 4 the TX address can't fit the preamble, so the last
		// byte goes here
		buf[last++] = 0x55;
	}
	for (uint8_t i = 0; i < xn297_addr_len; ++i)
		buf[last++] = xn297_tx_addr[xn297_addr_len-i-1] ^ xn297_scramble[i];

	for (uint8_t i = 0; i < len; ++i) {
		// bit-reverse bytes in packet
		uint8_t b_out = bit_reverse(msg[i]);
		buf[last++] = b_out ^ xn297_scramble[xn297_addr_len+i];
	}
	if (xn297_crc)
	{
		uint8_t offset = xn297_addr_len < 4 ? 1 : 0;
		uint16_t crc = 0xb5d2;
		for (uint8_t i = offset; i < last; ++i)
			crc = crc16_update(crc, buf[i]);
		crc ^= xn297_crc_xorout[xn297_addr_len - 3 + len];
		buf[last++] = crc >> 8;
		buf[last++] = crc & 0xff;
	}
	NRF24L01_WritePayload(buf, last);
}

void XN297_ReadPayload(uint8_t* msg, uint8_t len)
{
	NRF24L01_ReadPayload(msg, len);
	for(uint8_t i=0; i<len; i++)
		msg[i] = bit_reverse(msg[i]) ^ bit_reverse(xn297_scramble[i+xn297_addr_len]);
}

// End of XN297 emulation
