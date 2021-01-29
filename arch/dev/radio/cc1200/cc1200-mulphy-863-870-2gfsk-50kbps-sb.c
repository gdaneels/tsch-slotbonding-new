/**
 * \file
 *          This file is part of the multi-PHY adaptive TSCH implementation.
 *          Only to be used with multi-PHY enabled.
 *          Extra register settings are provided to re-configure to this PHY at runtime,
 *          assuming cc1200-mulphy-863-870-2gfsk-50kbps was the previous PHY.
 *
 * \author  Dries Van Leemput <dries.vanleemput@ugent.be>
 * */

#include "cc1200-rf-cfg.h"
#include "cc1200-const.h"
#include "net/mac/tsch/tsch.h"

/* Base frequency in kHz */
#define RF_CFG_CHAN_CENTER_F0           863125
/* Channel spacing in Hz */
#define RF_CFG_CHAN_SPACING             200000
/* The minimum channel */
#define RF_CFG_MIN_CHANNEL              0
/* The maximum channel */
#define RF_CFG_MAX_CHANNEL              2
//#define RF_CFG_MAX_CHANNEL              33
/* The maximum output power in dBm */
#define RF_CFG_MAX_TXPOWER              CC1200_CONST_TX_POWER_MAX
/* The carrier sense level used for CCA in dBm */
#define RF_CFG_CCA_THRESHOLD            (-91)
/* The RSSI offset in dBm */
#define RF_CFG_RSSI_OFFSET              (-81)
/*---------------------------------------------------------------------------*/
static const char rf_cfg_descriptor[] = "Multi-PHY 863-870MHz 2-GFSK 50kbps SB";
/*---------------------------------------------------------------------------*/

/* 1 byte time: 160 usec */
#define CC1200_TSCH_RF_RECONF_WAIT                600
#define CC1200_TSCH_PREAMBLE_LENGTH               800
#define CC1200_TSCH_CONF_RX_WAIT                 2200
#define CC1200_TSCH_CONF_RX_ACK_WAIT              400
#define CC1200_TSCH_CONF_TX_OFFSET               3800

#define CC1200_TSCH_DEFAULT_TS_CCA_OFFSET        1800
#define CC1200_TSCH_DEFAULT_TS_CCA                128

#define CC1200_TSCH_DEFAULT_TS_TX_OFFSET         CC1200_TSCH_CONF_TX_OFFSET + CC1200_TSCH_RF_RECONF_WAIT
#define CC1200_TSCH_DEFAULT_TS_RX_OFFSET         (CC1200_TSCH_DEFAULT_TS_TX_OFFSET - CC1200_TSCH_PREAMBLE_LENGTH - (CC1200_TSCH_CONF_RX_WAIT / 2))
#define CC1200_TSCH_DEFAULT_TS_TX_ACK_DELAY      3000
#define CC1200_TSCH_DEFAULT_TS_RX_ACK_DELAY      (CC1200_TSCH_DEFAULT_TS_TX_ACK_DELAY - CC1200_TSCH_PREAMBLE_LENGTH - (CC1200_TSCH_CONF_RX_ACK_WAIT / 2))


#define CC1200_TSCH_DEFAULT_TS_RX_WAIT           (CC1200_TSCH_PREAMBLE_LENGTH + CC1200_TSCH_CONF_RX_WAIT)
#define CC1200_TSCH_DEFAULT_TS_ACK_WAIT          (CC1200_TSCH_PREAMBLE_LENGTH + CC1200_TSCH_CONF_RX_ACK_WAIT)
#define CC1200_TSCH_DEFAULT_TS_RX_TX              192
//#define CC1200_TSCH_DEFAULT_TS_MAX_ACK           1760 /* 11 bytes at 50 kbps */
#define CC1200_TSCH_DEFAULT_TS_MAX_ACK           3360 /* 21 bytes at 50 kbps */
#define CC1200_TSCH_DEFAULT_TS_MAX_TX           20480 /* 128 bytes at 50 kbps */

/*#define CC1200_TSCH_DEFAULT_SLACK_TIME            500 */
/* fill up from 31740 to 32000 us */
#define CC1200_TSCH_DEFAULT_SLACK_TIME            500 + 260
/* Timeslot length: 30140 usec - is not correct?*/
#define CC1200_TSCH_DEFAULT_TS_TIMESLOT_LENGTH  \
                                                  (CC1200_TSCH_DEFAULT_TS_TX_OFFSET \
                                                  + CC1200_TSCH_DEFAULT_TS_MAX_TX \
                                                  + CC1200_TSCH_DEFAULT_TS_TX_ACK_DELAY \
                                                  + CC1200_TSCH_DEFAULT_TS_MAX_ACK \
                                                  + CC1200_TSCH_DEFAULT_SLACK_TIME \
                                                  )

/* TSCH timeslot timing (mircoseconds) */
static const tsch_timeslot_timing_usec cc1200_mulphy_2GFSK_50kbps_tsch_timing_usec = {
		CC1200_TSCH_DEFAULT_TS_CCA_OFFSET,
		CC1200_TSCH_DEFAULT_TS_CCA,
		CC1200_TSCH_DEFAULT_TS_TX_OFFSET,
		CC1200_TSCH_DEFAULT_TS_RX_OFFSET,
		CC1200_TSCH_DEFAULT_TS_RX_ACK_DELAY,
		CC1200_TSCH_DEFAULT_TS_TX_ACK_DELAY,
		CC1200_TSCH_DEFAULT_TS_RX_WAIT,
		CC1200_TSCH_DEFAULT_TS_ACK_WAIT,
		CC1200_TSCH_DEFAULT_TS_RX_TX,
		CC1200_TSCH_DEFAULT_TS_MAX_ACK,
		CC1200_TSCH_DEFAULT_TS_MAX_TX,
		CC1200_TSCH_DEFAULT_TS_TIMESLOT_LENGTH,
};

// Address Config = No address check
// Bit Rate = 50
// Carrier Frequency = 867.999878
// Deviation = 24.948120
// Device Address = 0
// Manchester Enable = false
// Modulation Format = 2-GFSK
// Packet Bit Length = 0
// Packet Length = 255
// Packet Length Mode = Variable
// RX Filter BW = 104.166667
// Symbol rate = 50
// Whitening = false

static const registerSetting_t preferredSettings[]=
		{
				{CC1200_SYNC_CFG1,         0xE5},
				{CC1200_SYNC_CFG0,         0x23},
				{CC1200_DEVIATION_M,       0x47},
				{CC1200_MODCFG_DEV_E,      0x0B},
				{CC1200_DCFILT_CFG,        0x56},
				{CC1200_PREAMBLE_CFG1,     0x14},
				{CC1200_PREAMBLE_CFG0,     0xBA},
				{CC1200_IQIC,              0xC8},
				{CC1200_CHAN_BW,           0x84},
				{CC1200_MDMCFG1,           0x42},
				{CC1200_MDMCFG0,           0x05},
				{CC1200_SYMBOL_RATE2,      0x94},
				{CC1200_SYMBOL_RATE1,      0x7A},
				{CC1200_SYMBOL_RATE0,      0xE1},
				{CC1200_AGC_REF,           0x27},
				{CC1200_AGC_CS_THR,        0xF1},
				{CC1200_AGC_CFG1,          0x11},
				{CC1200_AGC_CFG0,          0x90},
				{CC1200_FIFO_CFG,          0x00},
				{CC1200_FS_CFG,            0x12},
				{CC1200_PKT_CFG2,          0x00},
				{CC1200_PKT_CFG0,          0x20},
				{CC1200_PKT_LEN,           0xFF},
				{CC1200_IF_MIX_CFG,        0x18},
				{CC1200_TOC_CFG,           0x03},
				{CC1200_MDMCFG2,           0x02},
				{CC1200_FREQ2,             0x56},
				{CC1200_FREQ1,             0xCC},
				{CC1200_FREQ0,             0xCC},
				{CC1200_IF_ADC1,           0xEE},
				{CC1200_IF_ADC0,           0x10},
				{CC1200_FS_DIG1,           0x04},
				{CC1200_FS_DIG0,           0x50},
				{CC1200_FS_CAL1,           0x40},
				{CC1200_FS_CAL0,           0x0E},
				{CC1200_FS_DIVTWO,         0x03},
				{CC1200_FS_DSM0,           0x33},
				{CC1200_FS_DVC1,           0xF7},
				{CC1200_FS_DVC0,           0x0F},
				{CC1200_FS_PFD,            0x00},
				{CC1200_FS_PRE,            0x6E},
				{CC1200_FS_REG_DIV_CML,    0x1C},
				{CC1200_FS_SPARE,          0xAC},
				{CC1200_FS_VCO0,           0xB5},
				{CC1200_IFAMP,             0x05},
				{CC1200_XOSC5,             0x0E},
				{CC1200_XOSC1,             0x03},
		};

static const uint8_t reconfSettingsBurst[]=
		{0xE5,0x23,0x47,0x0B,0x56,0x14,0xBA,0xC8,0x84,0x42,0x05,0x94,0x7A,0xE1,0x27,0xF1,0x00,0xB1,0x20,0x11,0x90};

static const registerSetting_t reconfSettings[]=
		{
				{CC1200_IF_MIX_CFG,        0x18},
				{CC1200_TOC_CFG,           0x03},
				{CC1200_MDMCFG2,           0x02},
				{CC1200_FS_DIG0,           0x50},
				{CC1200_IFAMP,             0x05},
		};

/*---------------------------------------------------------------------------*/
/* Global linkage: symbol name must be different in each exported file! */
const cc1200_rf_cfg_t cc1200_mulphy_863_870_2GFSK_50kbps_sb = {
		.cfg_descriptor = rf_cfg_descriptor,
		.reconf_settings_burst = reconfSettingsBurst,
		.size_of_burst = sizeof(reconfSettingsBurst),
		.reconf_settings = reconfSettings,
		.size_of_reconfig_settings = sizeof(reconfSettings),
		.register_settings = preferredSettings,
		.size_of_register_settings = sizeof(preferredSettings),
		.tx_pkt_lifetime = (RTIMER_SECOND / 20),
		.tx_rx_turnaround = (RTIMER_SECOND / 100),
		/* Includes 3 Bytes preamble + 2 Bytes SFD, at 160usec per byte = 800 usec */
		/* Includes time to completion of "Wait for TX to start" if cc1200.c: 397 usec */
		.delay_before_tx = ((unsigned)US_TO_RTIMERTICKS(800 + 397 + 423)),
		.delay_before_rx = (unsigned)US_TO_RTIMERTICKS(400),
		.delay_before_detect = 0,
		.chan_center_freq0 = RF_CFG_CHAN_CENTER_F0,
		.chan_spacing = RF_CFG_CHAN_SPACING,
		.min_channel = RF_CFG_MIN_CHANNEL,
		.max_channel = RF_CFG_MAX_CHANNEL,
		.max_txpower = RF_CFG_MAX_TXPOWER,
		.cca_threshold = RF_CFG_CCA_THRESHOLD,
		.rssi_offset = RF_CFG_RSSI_OFFSET,
		.bitrate = 50000,
		.tsch_timing = cc1200_mulphy_2GFSK_50kbps_tsch_timing_usec,
};
/*---------------------------------------------------------------------------*/