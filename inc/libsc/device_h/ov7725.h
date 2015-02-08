/*
 * ov7725.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma	once

#define OV7725_W_SLAVE_ADDR 0x42 // Device slave address for write
#define OV7725_R_SLAVE_ADDR 0x43 // Device slave address for read

// Registers
#define OV7725_GAIN			0x00 // AGC - Gain control gain setting
#define OV7725_BLUE			0x01 // AWB - Blue channel gain setting
#define OV7725_RED			0x02 // AWB - Red channel gain setting
#define OV7725_GREEN		0x03 // AWB - Green channel gain setting
#define OV7725_BAVG			0x05 // U/B Average Level
#define OV7725_GAVG			0x06 // Y/Gb Average Level
#define OV7725_RAVG			0x07 // V/R Average Level
#define OV7725_AECH			0x08 // Exposure Value - AEC MSBs
#define OV7725_COM2			0x09 // Common Control 2
#define OV7725_PID			0x0A // Product ID Number MSB (Read only)
#define OV7725_VER			0x0B // Product ID Number LSB (Read only)
#define OV7725_COM3			0x0C // Common Control 3
#define OV7725_COM4			0x0D // Common Control 4
#define OV7725_COM5			0x0E // Common Control 5
#define OV7725_COM6			0x0F // Common Control 6
#define OV7725_AEC			0x10 // Exposure Value
#define OV7725_CLKRC		0x11 // Internal Clock
#define OV7725_COM7			0x12 // Common Control 7
#define OV7725_COM8			0x13 // Common Control 8
#define OV7725_COM9			0x14 // Common Control 9
#define OV7725_COM10		0x15 // Common Control 10
#define OV7725_REG16		0x16 // Register 16
#define OV7725_HSTART		0x17 // Horizontal Frame (HREF column) Start 8 MSBs
#define OV7725_HSIZE		0x18 // Horizontal Sensor Size
#define OV7725_VSTRT		0x19 // Vertical Frame (row) Start 8 MSBs
#define OV7725_VSIZE		0x1A // Vertical Sensor Size
#define OV7725_PSHFT		0x1B // Data Format - Pixel Delay Select
#define OV7725_MIDH			0x1C // Manufacturer ID Byte - High (Read only)
#define OV7725_MIDL			0x1D // Manufacturer ID Byte - Low (Read only)
#define OV7725_LAEC			0x1F // Fine AEC Value - defines exposure value less than one row period
#define OV7725_COM11		0x20 // Common Control 11
#define OV7725_BDBASE		0x22 // Banding Filter Minimum AEC Value
#define OV7725_BDMSTEP		0x23 // Banding Filter Maximum Step
#define OV7725_AEW			0x24 // AGC/AEC - Stable Operating Region (Upper Limit)
#define OV7725_AEB			0x25 // AGC/AEC - Stable Operating Region (Lower Limit)
#define OV7725_VPT			0x26 // AGC/AEC Fast Mode Operating Region
#define OV7725_REG28		0x28 // Register 28
#define OV7725_HOUTSIZE		0x29 // Horizontal Data Output size MSBs
#define OV7725_EXHCH		0x2A // Dummy Pixel Insert MSB
#define OV7725_EXHCL		0x2B // Dummy Pixel Insert LSB
#define OV7725_VOUTSIZE		0x2C // Vertical Data Output Size MSBs
#define OV7725_ADVFL		0x2D // LSB of Insert Dummy Rows in Vertical Sync (1 bit equals 1 row)
#define OV7725_ADVFH		0x2E // MSB of Insert Dummy Rows in Vertical Sync
#define OV7725_YAVE			0x2F // Y/G Channel Average Value
#define OV7725_LUMHTH		0x30 // Histogram AEC/AGC Luminance High Level Threshold
#define OV7725_LUMLTH		0x31 // Histogram AEC/AGC Luminance Low Level Threshold
#define OV7725_HREF			0x32 // Image Start and Size Control
#define OV7725_DM_LNL		0x33 // Dummy Row Low 8 Bits
#define OV7725_DM_LNH		0x34 // Dummy Row High 8 Bits
#define OV7725_ADOFF_B		0x35 // AD Offset Compensation Value for B Channel
#define OV7725_ADOFF_R		0x36 // AD Offset Compensation Value for R Channel
#define OV7725_ADOFF_GB		0x37 // AD Offset Compensation Value for Gb Channel
#define OV7725_ADOFF_GR		0x38 // AD Offset Compensation Value for Gr Channel
#define OV7725_OFF_B		0x39 // Analog Process B Channel Offset Compensation Value
#define OV7725_OFF_R		0x3A // Analog Process R Channel Offset Compensation Value
#define OV7725_OFF_GB		0x3B // Analog Process Gb Channel Offset Compensation Value
#define OV7725_OFF_GR		0x3C // Analog Process Gr Channel Offset Compensation Value
#define OV7725_COM12		0x3D // Common Control 12
#define OV7725_COM13		0x3E // Common Control 13
#define OV7725_COM14		0x3F // Common Control 14
#define OV7725_COM15		0x40 // Common Control 15
#define OV7725_COM16		0x41 // Common Control 16
#define OV7725_TGT_B		0x42 // BLC Blue Channel Target Value
#define OV7725_TGT_R		0x43 // BLC Red Channel Target Value
#define OV7725_TGT_GB		0x44 // BLC Gb Channel Target Value
#define OV7725_TGT_GR		0x45 // BLC Gr Channel Target Value
#define OV7725_LC_CTR		0x46 // Lens Correction Control
#define OV7725_LC_XC		0x47 // X Coordinate of Lens Correction Center Relative to Array Center
#define OV7725_LC_YC		0x48 // Y Coordinate of Lens Correction Center Relative to Array Center
#define OV7725_LC_COEF		0x49 // Lens Correction Coefficient
#define OV7725_LC_RADI		0x4A // Lens Correction Radius - radius of the circular section where no compensation applies
#define OV7725_LC_COEFB		0x4B // Lens Correction B Channel Compensation Coefficient
#define OV7725_LC_COEFR		0x4C // Lens Correction R Channel Compensation Coefficient
#define OV7725_FIXGAIN		0x4D // Analog Fix Gain Amplifer
#define OV7725_AREF0		0x4E // Sensor Reference Control
#define OV7725_AREF1		0x4F // Sensor Reference Current Control
#define OV7725_AREF2		0x50 // Analog Reference Control
#define OV7725_AREF3		0x51 // ADC Reference Control
#define OV7725_AREF4		0x52 // ADC Reference Control
#define OV7725_AREF5		0x53 // ADC Reference Control
#define OV7725_AREF6		0x54 // Analog Reference Control
#define OV7725_AREF7		0x55 // Analog Reference Control
#define OV7725_UFIX			0x60 // U Channel Fixed Value Output
#define OV7725_VFIX			0x61 // V Channel Fixed Value Output
#define OV7725_AWBB_BLK		0x62 // AWB Option for Advanced AWB
#define OV7725_AWB_CTRL0	0x63 // AWB Control Byte 0
#define OV7725_DSP_CTRL1	0x64 // DSP Control Byte 1
#define OV7725_DSP_CTRL2	0x65 // DSP Control Byte 2
#define OV7725_DSP_CTRL3	0x66 // DSP Control Byte 3
#define OV7725_DSP_CTRL4	0x67 // DSP Control Byte 4
#define OV7725_AWB_BIAS		0x68 // AWB BLC Level Clip
#define OV7725_AWB_CTRL1	0x69 // AWB Control Byte 1
#define OV7725_AWB_CTRL2	0x6A // AWB Control Byte 2
#define OV7725_AWB_CTRL3	0x6B // AWB Control Byte 3
#define OV7725_AWB_CTRL4	0x6C // AWB Control Byte 4
#define OV7725_AWB_CTRL5	0x6D // AWB Control Byte 5
#define OV7725_AWB_CTRL6	0x6E // AWB Control Byte 6
#define OV7725_AWB_CTRL7	0x6F // AWB Control Byte 7
#define OV7725_AWB_CTRL8	0x70 // AWB Control Byte 8
#define OV7725_AWB_CTRL9	0x71 // AWB Control Byte 9
#define OV7725_AWB_CTRL10	0x72 // AWB Control Byte 10
#define OV7725_AWB_CTRL11	0x73 // AWB Control Byte 11
#define OV7725_AWB_CTRL12	0x74 // AWB Control Byte 12
#define OV7725_AWB_CTRL13	0x75 // AWB Control Byte 13
#define OV7725_AWB_CTRL14	0x76 // AWB Control Byte 14
#define OV7725_AWB_CTRL15	0x77 // AWB Control Byte 15
#define OV7725_AWB_CTRL16	0x78 // AWB Control Byte 16
#define OV7725_AWB_CTRL17	0x79 // AWB Control Byte 17
#define OV7725_AWB_CTRL18	0x7A // AWB Control Byte 18
#define OV7725_AWB_CTRL19	0x7B // AWB Control Byte 19
#define OV7725_AWB_CTRL20	0x7C // AWB Control Byte 20
#define OV7725_AWB_CTRL21	0x7D // AWB Control Byte 21
#define OV7725_GAM1			0x7E // Gamma Curve 1st Segment Input End Point 0x04 Output Value
#define OV7725_GAM2			0x7F // Gamma Curve 2nd Segment Input End Point 0x08 Output Value
#define OV7725_GAM3			0x80 // Gamma Curve 3rd Segment Input End Point 0x10 Output Value
#define OV7725_GAM4			0x81 // Gamma Curve 4th Segment Input End Point 0x20 Output Value
#define OV7725_GAM5			0x82 // Gamma Curve 5th Segment Input End Point 0x28 Output Value
#define OV7725_GAM6			0x83 // Gamma Curve 6th Segment Input End Point 0x30 Output Value
#define OV7725_GAM7			0x84 // Gamma Curve 7th Segment Input End Point 0x38 Output Value
#define OV7725_GAM8			0x85 // Gamma Curve 8th Segment Input End Point 0x40 Output Value
#define OV7725_GAM9			0x86 // Gamma Curve 9th Segment Input End Point 0x48 Output Value
#define OV7725_GAM10		0x87 // Gamma Curve 10th Segment Input End Point 0x50 Output Value
#define OV7725_GAM11		0x88 // Gamma Curve 11th Segment Input End Point 0x60 Output Value
#define OV7725_GAM12		0x89 // Gamma Curve 12th Segment Input End Point 0x70 Output Value
#define OV7725_GAM13		0x8A // Gamma Curve 13th Segment Input End Point 0x90 Output Value
#define OV7725_GAM14		0x8B // Gamma Curve 14th Segment Input End Point 0xB0 Output Value
#define OV7725_GAM15		0x8C // Gamma Curve 15th Segment Input End Point 0xD0 Output Value
#define OV7725_SLOP			0x8D // Gamma Curve Highest Segment Slope
#define OV7725_DNSTH		0x8E // De-noise Threshold
#define OV7725_EDGE0		0x8F // Sharpness (Edge Enhancement) Control 0
#define OV7725_EDGE1		0x90 // Sharpness (Edge Enhancement) Control 1
#define OV7725_DNSOFF		0x91 // Auto De-noise Threshold Control
#define OV7725_EDGE2		0x92 // Sharpness (Edge Enhancement) Strength Upper Limit
#define OV7725_EDGE3		0x93 // Sharpness (Edge Enhancement) Strength Lower Limit
#define OV7725_MTX1			0x94 // Matrix Coefficient 1
#define OV7725_MTX2			0x95 // Matrix Coefficient 2
#define OV7725_MTX3			0x96 // Matrix Coefficient 3
#define OV7725_MTX4			0x97 // Matrix Coefficient 4
#define OV7725_MTX5			0x98 // Matrix Coefficient 5
#define OV7725_MTX6			0x99 // Matrix Coefficient 6
#define OV7725_MTX_CTRL		0x9A // Matrix Control
#define OV7725_BRIGHT		0x9B // Brightness Control
#define OV7725_CNST			0x9C // Contrast Gain
#define OV7725_UVADJ0		0x9E // Auto UV Adjust Control 0
#define OV7725_UVADJ1		0x9F // Auto UV Adjust Control 1
#define OV7725_SCAL0		0xA0 // DCW Ratio Control
#define OV7725_SCAL1		0xA1 // Horizontal Zoom Out Control
#define OV7725_SCAL2		0xA2 // Vertical Zoom Out Control
#define OV7725_FIFODLYM		0xA3 // FIFO Manual Mode Delay Control
#define OV7725_FIFODLYA		0xA4 // FIFO Auto Mode Delay Control
#define OV7725_SDE			0xA6 // Special Digital Effect Control
#define OV7725_USAT			0xA7 // U Component Saturation Gain
#define OV7725_VSAT			0xA8 // V Component Saturation Gain
#define OV7725_HUECOS		0xA9 // Cosine value
#define OV7725_HUESIN		0xAA // Sine value
#define OV7725_SIGN			0xAB // Sign Bit for Hue and Brightness
#define OV7725_DSPAUTO		0xAC // DSP Auto Function ON/OFF Control

// Compatibility
#define OV7725_AWBCTRL0		OV7725_AWB_CTRL0
#define OV7725_AWBCTRL1		OV7725_AWB_CTRL1
#define OV7725_AWBCTRL2		OV7725_AWB_CTRL2
#define OV7725_AWBCTRL3		OV7725_AWB_CTRL3
#define OV7725_AWBCTRL4		OV7725_AWB_CTRL4
#define OV7725_AWBCTRL5		OV7725_AWB_CTRL5
#define OV7725_AWBCTRL6		OV7725_AWB_CTRL6
#define OV7725_AWBCTRL7		OV7725_AWB_CTRL7
#define OV7725_AWBCTRL8		OV7725_AWB_CTRL8
#define OV7725_AWBCTRL9		OV7725_AWB_CTRL9
#define OV7725_AWBCTRL10	OV7725_AWB_CTRL10
#define OV7725_AWBCTRL11	OV7725_AWB_CTRL11
#define OV7725_AWBCTRL12	OV7725_AWB_CTRL12
#define OV7725_AWBCTRL13	OV7725_AWB_CTRL13
#define OV7725_AWBCTRL14	OV7725_AWB_CTRL14
#define OV7725_AWBCTRL15	OV7725_AWB_CTRL15
#define OV7725_AWBCTRL16	OV7725_AWB_CTRL16
#define OV7725_AWBCTRL17	OV7725_AWB_CTRL17
#define OV7725_AWBCTRL18	OV7725_AWB_CTRL18
#define OV7725_AWBCTRL19	OV7725_AWB_CTRL19
#define OV7725_AWBCTRL20	OV7725_AWB_CTRL20
#define OV7725_AWBCTRL21	OV7725_AWB_CTRL21
