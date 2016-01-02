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

//******************
enum PROTOCOLS
{
	MODE_SERIAL = 0,		// Serial commands
	MODE_FLYSKY = 1,		// =>A7105		/ FLYSKY protocol
	MODE_HUBSAN = 2,		// =>A7105		/ HUBSAN protocol
	MODE_FRSKY = 3,			// =>CC2500		/ FRSKY protocol
	MODE_HISKY = 4,			// =>NRF24L01	/ HISKY protocol
	MODE_V2X2 = 5,			// =>NRF24L01	/ V2x2 protocol
	MODE_DSM2 = 6,			// =>CYRF6936	/ DSM2 protocol
	MODE_DEVO =7,			// =>CYRF6936	/ DEVO protocol
	MODE_YD717 = 8,			// =>NRF24L01	/ YD717 protocol	(CX10 red pcb)
	MODE_KN  = 9,			// =>NRF24L01	/ KN protocol
	MODE_SYMAX = 10,		// =>NRF24L01	/ SYMAX protocol	(SYMAX4 working)
	MODE_SLT = 11,			// =>NRF24L01	/ SLT protocol
	MODE_CX10 = 12,			// =>NRF24L01	/ CX-10 protocol
	MODE_CG023 = 13,		// =>NRF24L01	/ CG023 protocol
	MODE_BAYANG = 14,		// =>NRF24L01	/ BAYANG protocol
	MODE_FRSKYX = 15,		// =>CC2500		/ FRSKYX protocol
};
enum Flysky
{
	Flysky=0,
	V9X9=1,
	V6X6=2,
	V912=3
};
enum Hisky
{
	Hisky=0,
	HK310=1
};
enum DSM2{
	DSM2=0,
	DSMX=1
};
enum YD717
{       			
	YD717=0,
	SKYWLKR=1,
	SYMAX2=2,
	XINXUN=3,
	NIHUI=4
};
enum SYMAX
{
	SYMAX=0,
	SYMAX5C=1,
};

enum CX10 {
    CX10_GREEN = 0,
    CX10_BLUE,		// also compatible with CX10-A, CX12
    DM007
};

enum CG023 {
    CG023 = 0,
    YD829 = 1
};

#define PPM_MIN_COMMAND 1250
#define PPM_SWITCH	1550
#define PPM_MAX_COMMAND 1750

enum TXRX_State {
	TXRX_OFF,
	TX_EN,
	RX_EN
};

// Packet ack status values
enum {
	PKT_PENDING = 0,
	PKT_ACKED,
	PKT_TIMEOUT
};

//*******************
//***   Pinouts   ***
//*******************
//#define BIND_pin 13
#define LED_pin 13						//Promini original led on B5
//
#define PPM_pin 3						//PPM -D3
#define SDI_pin 5						//SDIO-D5
#define SCLK_pin 4						//SCK-D4
#define CS_pin 2						//CS-D2
#define SDO_pin 6						//D6
//
#define CTRL1 1							//C1 (A1)
#define CTRL2 2							//C2 (A2)
//
#define CTRL1_on  PORTC |= _BV(1)
#define CTRL1_off PORTC &= ~_BV(1)
//
#define CTRL2_on  PORTC |= _BV(2)
#define CTRL2_off PORTC &= ~_BV(2)
//
#define  CS_on PORTD |= _BV(2)			//D2
#define  CS_off PORTD &= ~_BV(2)		//D2
//
#define  SCK_on PORTD |= _BV(4)			//D4
#define  SCK_off PORTD &= ~_BV(4)		//D4
//
#define  SDI_on PORTD |= _BV(5)			//D5
#define  SDI_off PORTD &= ~_BV(5)		//D5

#define  SDI_1 (PIND & (1<<SDI_pin)) == (1<<SDI_pin)	//D5
#define  SDI_0 (PIND & (1<<SDI_pin)) == 0x00			//D5
//
#define SDI_SET_INPUT DDRD &= ~_BV(5)	//D5
#define SDI_SET_OUTPUT DDRD |= _BV(5)	//D5
//Hisky /CC2500/CYRF aditional pinout
#define CC25_CSN_pin 7
#define NRF_CSN_pin 8
#define CYRF_CSN_pin 9
//
#define CYRF_RST_pin A5					//reset pin
//
#define CC25_CSN_on PORTD |= _BV(7)		//D7
#define CC25_CSN_off PORTD &= ~_BV(7)	//D7
//
#define NRF_CSN_on PORTB |= _BV(0)		//D8
#define NRF_CSN_off PORTB &= ~_BV(0)	//D8
//
#define CYRF_CSN_on PORTB |= _BV(1)		//D9
#define CYRF_CSN_off PORTB &= ~_BV(1)	//D9
//  
#define  SDO_1 (PIND & (1<<SDO_pin)) == (1<<SDO_pin)	//D6
#define  SDO_0 (PIND & (1<<SDO_pin)) == 0x00			//D6
//
#define RS_HI PORTC|=_BV(5)				//reset pin cyrf 
#define RX_LO PORTB &= ~_BV(5)//
//
//

// LED
#define LED_ON  PORTB |= _BV(5)
#define LED_OFF  PORTB &= ~_BV(5)
#define LED_TOGGLE  PORTB ^= _BV(5)
#define LED_SET_OUTPUT DDRB |= _BV(5)

// Macros
#define NOP() __asm__ __volatile__("nop")
#define BV(bit) (1 << bit)

//Serial flags definition
#define RX_FLAG_on			protocol_flags |= _BV(0)
#define RX_FLAG_off			protocol_flags &= ~_BV(0)
#define IS_RX_FLAG_on		( ( protocol_flags & _BV(0) ) !=0 )
//
#define CHANGE_PROTOCOL_FLAG_on		protocol_flags |= _BV(1)
#define CHANGE_PROTOCOL_FLAG_off		protocol_flags &= ~_BV(1)
#define IS_CHANGE_PROTOCOL_FLAG_on	( ( protocol_flags & _BV(1) ) !=0 )
//
#define POWER_FLAG_on		protocol_flags |= _BV(2)
#define POWER_FLAG_off		protocol_flags &= ~_BV(2)
#define IS_POWER_FLAG_on	( ( protocol_flags & _BV(2) ) !=0 )
//
#define RANGE_FLAG_on		protocol_flags |= _BV(3)
#define RANGE_FLAG_off		protocol_flags &= ~_BV(3)
#define IS_RANGE_FLAG_on	( ( protocol_flags & _BV(3) ) !=0 )
//
#define AUTOBIND_FLAG_on	protocol_flags |= _BV(4)
#define AUTOBIND_FLAG_off	protocol_flags &= ~_BV(4)
#define IS_AUTOBIND_FLAG_on	( ( protocol_flags & _BV(4) ) !=0 )
//
#define BIND_BUTTON_FLAG_on		protocol_flags |= _BV(5)
#define BIND_BUTTON_FLAG_off	protocol_flags &= ~_BV(5)
#define IS_BIND_BUTTON_FLAG_on	( ( protocol_flags & _BV(5) ) !=0 )

//PPM RX OK
#define PPM_FLAG_off			protocol_flags &= ~_BV(6)
#define PPM_FLAG_on			protocol_flags |= _BV(6)
#define IS_PPM_FLAG_on		( ( protocol_flags & _BV(6) ) !=0 )

//Bind flag for blinking
#define BIND_IN_PROGRESS	protocol_flags &= ~_BV(7)
#define BIND_DONE			protocol_flags |= _BV(7)
#define IS_BIND_DONE_on		( ( protocol_flags & _BV(7) ) !=0 )

#define BLINK_BIND_TIME	100
#define BLINK_SERIAL_TIME	500

//************************
//***  Power settings  ***
//************************
enum {
	TXPOWER_100uW,
	TXPOWER_300uW,
	TXPOWER_1mW,
	TXPOWER_3mW,
	TXPOWER_10mW,
	TXPOWER_30mW,
	TXPOWER_100mW,
	TXPOWER_150mW
};

// A7105 power
//	Power amp is ~+16dBm so:
enum A7105_POWER
{
	A7105_POWER_0 = 0x00<<3 | 0x00,	// TXPOWER_100uW  = -23dBm == PAC=0 TBG=0
	A7105_POWER_1 = 0x00<<3 | 0x01,	// TXPOWER_300uW  = -20dBm == PAC=0 TBG=1
	A7105_POWER_2 = 0x00<<3 | 0x02,	// TXPOWER_1mW    = -16dBm == PAC=0 TBG=2
	A7105_POWER_3 = 0x00<<3 | 0x04,	// TXPOWER_3mW    = -11dBm == PAC=0 TBG=4
	A7105_POWER_4 = 0x01<<3 | 0x05,	// TXPOWER_10mW   =  -6dBm == PAC=1 TBG=5
	A7105_POWER_5 = 0x02<<3 | 0x07,	// TXPOWER_30mW   =   0dBm == PAC=2 TBG=7
	A7105_POWER_6 = 0x03<<3 | 0x07,	// TXPOWER_100mW  =   1dBm == PAC=3 TBG=7
	A7105_POWER_7 = 0x03<<3 | 0x07	// TXPOWER_150mW  =   1dBm == PAC=3 TBG=7
};
#define A7105_HIGH_POWER	A7105_POWER_5
#define	A7105_LOW_POWER		A7105_POWER_3
#define	A7105_BIND_POWER	A7105_POWER_0

// NRF Power
// Power setting is 0..3 for nRF24L01
// Claimed power amp for nRF24L01 from eBay is 20dBm. 
enum NRF_POWER
{						//      Raw            w 20dBm PA
	NRF_POWER_0 = 0x00,	// 0 : -18dBm  (16uW)   2dBm (1.6mW)
	NRF_POWER_1 = 0x01,	// 1 : -12dBm  (60uW)   8dBm   (6mW)
	NRF_POWER_2 = 0x02,	// 2 :  -6dBm (250uW)  14dBm  (25mW)
	NRF_POWER_3 = 0x03	// 3 :   0dBm   (1mW)  20dBm (100mW)
};
#define NRF_HIGH_POWER		NRF_POWER_2
#define	NRF_LOW_POWER		NRF_POWER_1
#define	NRF_BIND_POWER		NRF_POWER_0

// CC2500 power
enum CC2500_POWER
{
	CC2500_POWER_0 = 0xC5,	// -12dbm
	CC2500_POWER_1 = 0x97,	// -10dbm
	CC2500_POWER_2 = 0x6E,	// -8dbm
	CC2500_POWER_3 = 0x7F,	// -6dbm
	CC2500_POWER_4 = 0xA9,	// -4dbm
	CC2500_POWER_5 = 0xBB,	// -2dbm
	CC2500_POWER_6 = 0xFE,	// 0dbm
	CC2500_POWER_7 = 0xFF	// 1.5dbm
};
#define CC2500_HIGH_POWER	CC2500_POWER_6
#define	CC2500_LOW_POWER	CC2500_POWER_3
#define CC2500_BIND_POWER	CC2500_POWER_0

// CYRF power
enum CYRF_POWER
{
	CYRF_POWER_0 = 0x00,	//
	CYRF_POWER_1 = 0x01,	//
	CYRF_POWER_2 = 0x02,	//
	CYRF_POWER_3 = 0x03,	//
	CYRF_POWER_4 = 0x04,	//
	CYRF_POWER_5 = 0x05,	//
	CYRF_POWER_6 = 0x06,	//
	CYRF_POWER_7 = 0x07		//
};
#define CYRF_HIGH_POWER		7
#define	CYRF_LOW_POWER		3
#define	CYRF_BIND_POWER		0

//*******************
//***  CRC Table  ***
//*******************
const uint16_t PROGMEM CRCTable[]=
{
    0x0000,0x1189,0x2312,0x329b,0x4624,0x57ad,0x6536,0x74bf,
    0x8c48,0x9dc1,0xaf5a,0xbed3,0xca6c,0xdbe5,0xe97e,0xf8f7,
    0x1081,0x0108,0x3393,0x221a,0x56a5,0x472c,0x75b7,0x643e,
    0x9cc9,0x8d40,0xbfdb,0xae52,0xdaed,0xcb64,0xf9ff,0xe876,
    0x2102,0x308b,0x0210,0x1399,0x6726,0x76af,0x4434,0x55bd,
    0xad4a,0xbcc3,0x8e58,0x9fd1,0xeb6e,0xfae7,0xc87c,0xd9f5,
    0x3183,0x200a,0x1291,0x0318,0x77a7,0x662e,0x54b5,0x453c,
    0xbdcb,0xac42,0x9ed9,0x8f50,0xfbef,0xea66,0xd8fd,0xc974,
    0x4204,0x538d,0x6116,0x709f,0x0420,0x15a9,0x2732,0x36bb,
    0xce4c,0xdfc5,0xed5e,0xfcd7,0x8868,0x99e1,0xab7a,0xbaf3,
    0x5285,0x430c,0x7197,0x601e,0x14a1,0x0528,0x37b3,0x263a,
    0xdecd,0xcf44,0xfddf,0xec56,0x98e9,0x8960,0xbbfb,0xaa72,
    0x6306,0x728f,0x4014,0x519d,0x2522,0x34ab,0x0630,0x17b9,
    0xef4e,0xfec7,0xcc5c,0xddd5,0xa96a,0xb8e3,0x8a78,0x9bf1,
    0x7387,0x620e,0x5095,0x411c,0x35a3,0x242a,0x16b1,0x0738,
    0xffcf,0xee46,0xdcdd,0xcd54,0xb9eb,0xa862,0x9af9,0x8b70,
    0x8408,0x9581,0xa71a,0xb693,0xc22c,0xd3a5,0xe13e,0xf0b7,
    0x0840,0x19c9,0x2b52,0x3adb,0x4e64,0x5fed,0x6d76,0x7cff,
    0x9489,0x8500,0xb79b,0xa612,0xd2ad,0xc324,0xf1bf,0xe036,
    0x18c1,0x0948,0x3bd3,0x2a5a,0x5ee5,0x4f6c,0x7df7,0x6c7e,
    0xa50a,0xb483,0x8618,0x9791,0xe32e,0xf2a7,0xc03c,0xd1b5,
    0x2942,0x38cb,0x0a50,0x1bd9,0x6f66,0x7eef,0x4c74,0x5dfd,
    0xb58b,0xa402,0x9699,0x8710,0xf3af,0xe226,0xd0bd,0xc134,
    0x39c3,0x284a,0x1ad1,0x0b58,0x7fe7,0x6e6e,0x5cf5,0x4d7c,
    0xc60c,0xd785,0xe51e,0xf497,0x8028,0x91a1,0xa33a,0xb2b3,
    0x4a44,0x5bcd,0x6956,0x78df,0x0c60,0x1de9,0x2f72,0x3efb,
    0xd68d,0xc704,0xf59f,0xe416,0x90a9,0x8120,0xb3bb,0xa232,
    0x5ac5,0x4b4c,0x79d7,0x685e,0x1ce1,0x0d68,0x3ff3,0x2e7a,
    0xe70e,0xf687,0xc41c,0xd595,0xa12a,0xb0a3,0x8238,0x93b1,
    0x6b46,0x7acf,0x4854,0x59dd,0x2d62,0x3ceb,0x0e70,0x1ff9,
    0xf78f,0xe606,0xd49d,0xc514,0xb1ab,0xa022,0x92b9,0x8330,
    0x7bc7,0x6a4e,0x58d5,0x495c,0x3de3,0x2c6a,0x1ef1,0x0f78
};


//****************************************
//*** MULTI protocol serial definition ***
//****************************************
/*
**************************
16 channels serial protocol
**************************
Serial: 100000 Baud 8e2      _ xxxx xxxx p --
  Total of 26 bytes
  Stream[0]   = 0x55
   header
  Stream[1]   = sub_protocol|BindBit|RangeCheckBit|AutoBindBit;
   sub_protocol is 0..31 (bits 0..4)
   =>	Reserved	0
					Flysky		1
					Hubsan		2
					Frsky		3
					Hisky		4
					V2x2		5
					DSM2		6
					Devo		7
					YD717		8
					KN			9
					SymaX		10
					SLT			11
					CX10		12
					CG023		13
					Bayang		14
   BindBit=>		0x80	1=Bind/0=No
   AutoBindBit=>	0x40	1=Yes /0=No
   RangeCheck=>		0x20	1=Yes /0=No
  Stream[2]   = RxNum | Power | Type;
   RxNum value is 0..15 (bits 0..3)
   Type is 0..7 <<4     (bit 4..6)
		sub_protocol==Flysky
			Flysky	0
			V9x9	1
			V6x6	2
			V912	3
		sub_protocol==Hisky
			Hisky	0
			HK310	1
		sub_protocol==DSM2
			DSM2	0
			DSMX	1
		sub_protocol==YD717
			YD717	0
			SKYWLKR	1
			SYMAX2	2
			XINXUN	3
			NIHUI	4
		sub_protocol==SYMAX
			SYMAX	0
			SYMAX5C	1
		sub_protocol==CX10
			CX10_GREEN	0
			CX10_BLUE	1	// also compatible with CX10-A, CX12
			DM007		2
		sub_protocol==CG023
			CG023		0
			YD829		1
   Power value => 0x80	0=High/1=Low
  Stream[3]   = option_protocol;
   option_protocol value is -127..127
  Stream[4] to [25] = Channels
   16 Channels on 11 bits (0..2047)
	0		-125%
    204		-100%
	1024	   0%
	1843	+100%
	2047	+125%
   Channels bits are concatenated to fit in 22 bytes like in SBUS protocol


**************************
8 channels serial protocol
**************************
Serial: 125000 Baud 8n1      _ xxxx xxxx - ---
  Channels:
    Nbr=8
    10bits=0..1023
	0		-125%
    96		-100%
	512		   0%
	928		+100%
	1023	+125%
  Stream[0]   = sub_protocol|BindBit|RangeCheckBit|AutoBindBit;
   sub_protocol is 0..31 (bits 0..4)
				=>	Reserved	0
					Flysky		1
					Hubsan		2
					Frsky		3
					Hisky		4
					V2x2		5
					DSM2		6
					Devo		7
					YD717		8
					KN			9
					SymaX		10
					SLT			11
					CX10		12
					CG023		13
					Bayang		14
   BindBit=>		0x80	1=Bind/0=No
   AutoBindBit=>	0x40	1=Yes /0=No
   RangeCheck=>		0x20	1=Yes /0=No
  Stream[1]   = RxNum | Power | Type;
   RxNum value is 0..15 (bits 0..3)
   Type is 0..7 <<4     (bit 4..6)
		sub_protocol==Flysky
			Flysky	0
			V9x9	1
			V6x6	2
			V912	3
		sub_protocol==Hisky
			Hisky	0
			HK310	1
		sub_protocol==DSM2
			DSM2	0
			DSMX	1
		sub_protocol==YD717
			YD717	0
			SKYWLKR	1
			SYMAX2	2
			XINXUN	3
			NIHUI	4
		sub_protocol==SYMAX
			SYMAX	0
			SYMAX5C	1
		sub_protocol==CX10
			CX10_GREEN	0
			CX10_BLUE	1	// also compatible with CX10-A, CX12
			DM007		2
		sub_protocol==CG023
			CG023		0
			YD829		1
   Power value => 0x80	0=High/1=Low
  Stream[2]   = option_protocol;
   option_protocol value is -127..127
  Stream[i+3] = lowByte(channel[i])		// with i[0..7]
  Stream[11]  = highByte(channel[0])<<6 | highByte(channel[1])<<4 | highByte(channel[2])<<2 | highByte(channel[3])
  Stream[12]  = highByte(channel[4])<<6 | highByte(channel[5])<<4 | highByte(channel[6])<<2 | highByte(channel[7])
  Stream[13]  = lowByte(CRC16(Stream[0..12])


  The option_range is only used by Frsky to fine tune frequency

  The autobind/bind flag are currently identical and are used by FlySky, FrSky, HiSky,
  KN and V2x2 protocol.

  Range flage is protocol idenpendet and controls the send power of the radio module.

  The RxNum is used by all protocol that use a TX ID. Either by a special field in the
  protocol or by using different TX IDs for protocol that have no sepcial field. Protocol using
  TX ID are currently KN, Hisky, DSM2.


*/

