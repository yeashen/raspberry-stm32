/******************************************************************************
  File			: mpu6050.c
  Description	: mpu6050 accelerometer and gyroscope sensor driver
  Author		: Xiaoming Li
*******************************************************************************
  Modify List:
-------------------------------------------------------------------------------
  2014/8/16 21:00 PM	| Created
******************************************************************************/

#include "mpu6050.h"

uint8_t buffer[14];
static bool isInit = FALSE;

int8_t I2C_ReadBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t *data) 
{
    uint8_t b, count;
#if defined(SW_I2C)
	count = SwI2C_ReadBytes(devAddr, regAddr, 1, &b);
#else 
	count = HwI2C_ReadBytes(devAddr, regAddr, 1, &b);
#endif
	MPU6050_DPDEBUG("[RD] ReadBit reg: 0x%x data: 0x%x\r\n", regAddr, b);
    *data = b & (1 << bitNum);
    return count;
}

int8_t I2C_ReadBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data) 
{
    // 01101001 read byte
    // 76543210 bit numbers
    //    xxx   args: bitStart=4, length=3
    //    010   masked
    //   -> 010 shifted
    uint8_t count, b, mask;
#if defined(SW_I2C)
	count = SwI2C_ReadBytes(devAddr, regAddr, 1, &b);
#else 
	count = HwI2C_ReadBytes(devAddr, regAddr, 1, &b);
#endif
	MPU6050_DPDEBUG("[RD] ReadBit reg: 0x%x data-b: 0x%x\r\n", regAddr, b);
    if (count != 0) {
        mask = ((1 << length) - 1) << (bitStart - length + 1);
        b &= mask;
        b >>= (bitStart - length + 1);
        *data = b;
    }
	MPU6050_DPDEBUG("[RD] ReadBit reg: 0x%x data: 0x%x\r\n", regAddr, *data);
    return count;
}

bool I2C_WriteBit(u8 devAddr, u8 regAddr, u8 bitNum, u8 data) 
{
	u8 b;
	bool res;
#if defined(SW_I2C)
	SwI2C_ReadBytes(devAddr, regAddr, 1, &b);
#else 
	HwI2C_ReadBytes(devAddr, regAddr, 1, &b);
#endif
	MPU6050_DPDEBUG("[RD] WriteBit reg: 0x%x data: 0x%x\r\n", regAddr, b);
	b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
	MPU6050_DPDEBUG("[WR] WriteBit reg: 0x%x data: 0x%x\r\n", regAddr, b);
#if defined(SW_I2C)
	res = SwI2C_WriteByte(devAddr, regAddr, b);
#else 
	res = HwI2C_WriteByte(devAddr, regAddr, b);
#endif
	return res;
}

bool I2C_WriteBits(u8 devAddr, u8 regAddr, u8 bitStart, u8 length, u8 data)
{
	u8 b,mask;
	bool res;
#if defined(SW_I2C)
	if((res = SwI2C_ReadBytes(devAddr, regAddr, 1, &b)) == TRUE)
#else 
	if((res = HwI2C_ReadBytes(devAddr, regAddr, 1, &b)) == TRUE)
#endif
	{
		MPU6050_DPDEBUG("[RD] WriteBits reg: 0x%x data: 0x%x\r\n", regAddr, b);
		mask = (0xFF << (bitStart + 1)) | 0xFF >> ((8 - bitStart) + length - 1);
		data <<= (8 - length);
		data >>= (7 - bitStart);
		b &= mask;
		b |= data;
		MPU6050_DPDEBUG("[WR] WriteBits reg: 0x%x data: 0x%x\r\n", regAddr, b);
#if defined(SW_I2C)
		res = SwI2C_WriteByte(devAddr, regAddr, b);
#else 
		res = HwI2C_WriteByte(devAddr, regAddr, b);
#endif
	}
	return res;
}

/** Power on and prepare for general usage.
 * This will activate the device and take it out of sleep mode (which must be done
 * after start-up). This function also sets both the accelerometer and the gyroscope
 * to their most sensitive settings, namely +/- 2g and +/- 250 degrees/sec, and sets
 * the clock source to use the X Gyro for reference, which is slightly better than
 * the default internal clock source.
 */
void mpu6050_init()
{
	MPU6050_DEBUG("Initial...\r\n");
	if(isInit){
		MPU6050_DEBUG("MPU6050 has already Initialed!\r\n");
		return;
	}
#if defined(SW_I2C)
	SwI2C_init();
#else 
	HwI2C_Init();
#endif
	delay_ms(1000);

	if(!mpu6050_reset()){
		MPU6050_DEBUG("Reset Failed...\r\n");
	}
	
	delay_ms(100);
	mpu6050_setSleepEnabled(DISABLE);
	delay_ms(5);

	mpu6050_setTempSensorEnabled(ENABLE);
	delay_ms(5);

	mpu6050SetIntEnabled(DISABLE);
	delay_ms(5);

	mpu6050_setI2CBypassEnabled(ENABLE);
	delay_ms(5);
	
	mpu6050_setClockSource(MPU6050_CLOCK_PLL_XGYRO);
	delay_ms(5);
	mpu6050_setFullScaleGyroRange(MPU6050_GYRO_FS_500);
	delay_ms(5);
	mpu6050_setFullScaleAccelRange(MPU6050_ACCEL_FS_2);
	delay_ms(5);
	
	mpu6050_setRate(0x7);
	delay_ms(5);
	
	mpu6050_setDLPFMode(MPU6050_DLPF_BW_256);
	delay_ms(5);

	MPU6050_DEBUG("Initial OK\r\n");
	isInit = TRUE;
	
	delay_ms(5);
}

/** Set clock source setting.
 * An internal 8MHz oscillator, gyroscope based clock, or external sources can
 * be selected as the MPU-60X0 clock source. When the internal 8 MHz oscillator
 * or an external source is chosen as the clock source, the MPU-60X0 can operate
 * in low power modes with the gyroscopes disabled.
 *
 * Upon power up, the MPU-60X0 clock source defaults to the internal oscillator.
 * However, it is highly recommended that the device be configured to use one of
 * the gyroscopes (or an external clock source) as the clock reference for
 * improved stability. The clock source can be selected according to the following table:
 *
 * <pre>
 * CLK_SEL | Clock Source
 * --------+--------------------------------------
 * 0       | Internal oscillator
 * 1       | PLL with X Gyro reference
 * 2       | PLL with Y Gyro reference
 * 3       | PLL with Z Gyro reference
 * 4       | PLL with external 32.768kHz reference
 * 5       | PLL with external 19.2MHz reference
 * 6       | Reserved
 * 7       | Stops the clock and keeps the timing generator in reset
 * </pre>
 *
 * @param source New clock source setting
 * @see getClockSource()
 * @see MPU6050_RA_PWR_MGMT_1
 * @see MPU6050_PWR1_CLKSEL_BIT
 * @see MPU6050_PWR1_CLKSEL_LENGTH
 */
void mpu6050_setClockSource(uint8_t source)
{
	I2C_WriteBits(DEVADDR, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_CLKSEL_BIT, MPU6050_PWR1_CLKSEL_LENGTH, source);
}

/** Get clock source setting.
 * @return Current clock source setting
 * @see MPU6050_RA_PWR_MGMT_1
 * @see MPU6050_PWR1_CLKSEL_BIT
 * @see MPU6050_PWR1_CLKSEL_LENGTH
 */
uint8_t mpu6050_getClockSource() {
    I2C_ReadBits(DEVADDR, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_CLKSEL_BIT, MPU6050_PWR1_CLKSEL_LENGTH, buffer);
    return buffer[0];
}

//-----------------GYRO_CONFIG register-----------------------------

/** Get full-scale gyroscope range.
 * The FS_SEL parameter allows setting the full-scale range of the gyro sensors,
 * as described in the table below.
 *
 * <pre>
 * 0 = +/- 250 degrees/sec
 * 1 = +/- 500 degrees/sec
 * 2 = +/- 1000 degrees/sec
 * 3 = +/- 2000 degrees/sec
 * </pre>
 *
 * @return Current full-scale gyroscope range setting
 * @see MPU6050_GYRO_FS_250
 * @see MPU6050_RA_GYRO_CONFIG
 * @see MPU6050_GCONFIG_FS_SEL_BIT
 * @see MPU6050_GCONFIG_FS_SEL_LENGTH
 */
uint8_t mpu6050_getFullScaleGyroRange() 
{
    I2C_ReadBits(DEVADDR, MPU6050_RA_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, buffer);
    return buffer[0];
}

/** Set full-scale gyroscope range.
 * @param range New full-scale gyroscope range value
 * @see getFullScaleRange()
 * @see MPU6050_GYRO_FS_250
 * @see MPU6050_RA_GYRO_CONFIG
 * @see MPU6050_GCONFIG_FS_SEL_BIT
 * @see MPU6050_GCONFIG_FS_SEL_LENGTH
 */
void mpu6050_setFullScaleGyroRange(uint8_t range) 
{
	I2C_WriteBits(DEVADDR, MPU6050_RA_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, range);
}

//-----------------ACCEL_CONFIG register-----------------------------
/** Get full-scale accelerometer range.
 * The FS_SEL parameter allows setting the full-scale range of the accelerometer
 * sensors, as described in the table below.
 *
 * <pre>
 * 0 = +/- 2g
 * 1 = +/- 4g
 * 2 = +/- 8g
 * 3 = +/- 16g
 * </pre>
 *
 * @return Current full-scale accelerometer range setting
 * @see MPU6050_ACCEL_FS_2
 * @see MPU6050_RA_ACCEL_CONFIG
 * @see MPU6050_ACONFIG_AFS_SEL_BIT
 * @see MPU6050_ACONFIG_AFS_SEL_LENGTH
 */
uint8_t mpu6050_getFullScaleAccelRange() 
{
    I2C_ReadBits(DEVADDR, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, buffer);
    return buffer[0];
}
/** Set full-scale accelerometer range.
 * @param range New full-scale accelerometer range setting
 * @see getFullScaleAccelRange()
 */
void mpu6050_setFullScaleAccelRange(uint8_t range) 
{
	I2C_WriteBits(DEVADDR, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, range);
}

/** Get I2C bypass enabled status.
 * When this bit is equal to 1 and I2C_MST_EN (Register 106 bit[5]) is equal to
 * 0, the host application processor will be able to directly access the
 * auxiliary I2C bus of the MPU-60X0. When this bit is equal to 0, the host
 * application processor will not be able to directly access the auxiliary I2C
 * bus of the MPU-60X0 regardless of the state of I2C_MST_EN (Register 106
 * bit[5]).
 * @return Current I2C bypass enabled status
 * @see MPU6050_RA_INT_PIN_CFG
 * @see MPU6050_INTCFG_I2C_BYPASS_EN_BIT
 */
u8 mpu6050_getI2CBypassEnabled() {
    I2C_ReadBit(DEVADDR, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_I2C_BYPASS_EN_BIT, buffer);
    return buffer[0];
}

/** Set I2C bypass enabled status.
 * When this bit is equal to 1 and I2C_MST_EN (Register 106 bit[5]) is equal to
 * 0, the host application processor will be able to directly access the
 * auxiliary I2C bus of the MPU-60X0. When this bit is equal to 0, the host
 * application processor will not be able to directly access the auxiliary I2C
 * bus of the MPU-60X0 regardless of the state of I2C_MST_EN (Register 106
 * bit[5]).
 * @param enabled New I2C bypass enabled status
 * @see MPU6050_RA_INT_PIN_CFG
 * @see MPU6050_INTCFG_I2C_BYPASS_EN_BIT
 */
void mpu6050_setI2CBypassEnabled(uint8_t enabled) 
{
	I2C_WriteBit(DEVADDR, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_I2C_BYPASS_EN_BIT, enabled);
}


/** Get digital low-pass filter configuration.
 * The DLPF_CFG parameter sets the digital low pass filter configuration. It
 * also determines the internal sampling rate used by the device as shown in
 * the table below.
 *
 * Note: The accelerometer output rate is 1kHz. This means that for a Sample
 * Rate greater than 1kHz, the same accelerometer sample may be output to the
 * FIFO, DMP, and sensor registers more than once.
 *
 * <pre>
 *          |   ACCELEROMETER    |           GYROSCOPE
 * DLPF_CFG | Bandwidth | Delay  | Bandwidth | Delay  | Sample Rate
 * ---------+-----------+--------+-----------+--------+-------------
 * 0        | 260Hz     | 0ms    | 256Hz     | 0.98ms | 8kHz
 * 1        | 184Hz     | 2.0ms  | 188Hz     | 1.9ms  | 1kHz
 * 2        | 94Hz      | 3.0ms  | 98Hz      | 2.8ms  | 1kHz
 * 3        | 44Hz      | 4.9ms  | 42Hz      | 4.8ms  | 1kHz
 * 4        | 21Hz      | 8.5ms  | 20Hz      | 8.3ms  | 1kHz
 * 5        | 10Hz      | 13.8ms | 10Hz      | 13.4ms | 1kHz
 * 6        | 5Hz       | 19.0ms | 5Hz       | 18.6ms | 1kHz
 * 7        |   -- Reserved --   |   -- Reserved --   | Reserved
 * </pre>
 *
 * @return DLFP configuration
 * @see MPU6050_RA_CONFIG
 * @see MPU6050_CFG_DLPF_CFG_BIT
 * @see MPU6050_CFG_DLPF_CFG_LENGTH
 */
uint8_t mpu6050_getDLPFMode() {
    I2C_ReadBits(DEVADDR, MPU6050_RA_CONFIG, MPU6050_CFG_DLPF_CFG_BIT, MPU6050_CFG_DLPF_CFG_LENGTH, buffer);
    return buffer[0];
}
/** Set digital low-pass filter configuration.
 * @param mode New DLFP configuration setting
 * @see getDLPFBandwidth()
 * @see MPU6050_DLPF_BW_256
 * @see MPU6050_RA_CONFIG
 * @see MPU6050_CFG_DLPF_CFG_BIT
 * @see MPU6050_CFG_DLPF_CFG_LENGTH
 */
void mpu6050_setDLPFMode(uint8_t mode) {
    I2C_WriteBits(DEVADDR, MPU6050_RA_CONFIG, MPU6050_CFG_DLPF_CFG_BIT, MPU6050_CFG_DLPF_CFG_LENGTH, mode);
}

// ACCEL_*OUT_* registers

/** Get raw 9-axis motion sensor readings (accel/gyro/compass).
 * FUNCTION NOT FULLY IMPLEMENTED YET.
 * @param ax 16-bit signed integer container for accelerometer X-axis value
 * @param ay 16-bit signed integer container for accelerometer Y-axis value
 * @param az 16-bit signed integer container for accelerometer Z-axis value
 * @param gx 16-bit signed integer container for gyroscope X-axis value
 * @param gy 16-bit signed integer container for gyroscope Y-axis value
 * @param gz 16-bit signed integer container for gyroscope Z-axis value
 * @param mx 16-bit signed integer container for magnetometer X-axis value
 * @param my 16-bit signed integer container for magnetometer Y-axis value
 * @param mz 16-bit signed integer container for magnetometer Z-axis value
 * @see getMotion6()
 * @see getAcceleration()
 * @see getRotation()
 * @see MPU6050_RA_ACCEL_XOUT_H
 */
void mpu6050_getMotion9(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz, int16_t* mx, int16_t* my, int16_t* mz) {
    mpu6050_getMotion6(ax, ay, az, gx, gy, gz);
    // TODO: magnetometer integration
}
/** Get raw 6-axis motion sensor readings (accel/gyro).
 * Retrieves all currently available motion sensor values.
 * @param ax 16-bit signed integer container for accelerometer X-axis value
 * @param ay 16-bit signed integer container for accelerometer Y-axis value
 * @param az 16-bit signed integer container for accelerometer Z-axis value
 * @param gx 16-bit signed integer container for gyroscope X-axis value
 * @param gy 16-bit signed integer container for gyroscope Y-axis value
 * @param gz 16-bit signed integer container for gyroscope Z-axis value
 * @see getAcceleration()
 * @see getRotation()
 * @see MPU6050_RA_ACCEL_XOUT_H
 */
void mpu6050_getMotion6(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz) {
#if defined(SW_I2C)
	SwI2C_ReadBytes(DEVADDR, MPU6050_RA_ACCEL_XOUT_H, 14, buffer);
#else 
	HwI2C_ReadBytes(DEVADDR, MPU6050_RA_ACCEL_XOUT_H, 14, buffer);
#endif
    *ax = (((int16_t)buffer[0]) << 8) | buffer[1];
    *ay = (((int16_t)buffer[2]) << 8) | buffer[3];
    *az = (((int16_t)buffer[4]) << 8) | buffer[5];
    *gx = (((int16_t)buffer[8]) << 8) | buffer[9];
    *gy = (((int16_t)buffer[10]) << 8) | buffer[11];
    *gz = (((int16_t)buffer[12]) << 8) | buffer[13];
}

/** Get 3-axis accelerometer readings.
 * These registers store the most recent accelerometer measurements.
 * Accelerometer measurements are written to these registers at the Sample Rate
 * as defined in Register 25.
 *
 * The accelerometer measurement registers, along with the temperature
 * measurement registers, gyroscope measurement registers, and external sensor
 * data registers, are composed of two sets of registers: an internal register
 * set and a user-facing read register set.
 *
 * The data within the accelerometer sensors' internal register set is always
 * updated at the Sample Rate. Meanwhile, the user-facing read register set
 * duplicates the internal register set's data values whenever the serial
 * interface is idle. This guarantees that a burst read of sensor registers will
 * read measurements from the same sampling instant. Note that if burst reads
 * are not used, the user is responsible for ensuring a set of single byte reads
 * correspond to a single sampling instant by checking the Data Ready interrupt.
 *
 * Each 16-bit accelerometer measurement has a full scale defined in ACCEL_FS
 * (Register 28). For each full scale setting, the accelerometers' sensitivity
 * per LSB in ACCEL_xOUT is shown in the table below:
 *
 * <pre>
 * AFS_SEL | Full Scale Range | LSB Sensitivity
 * --------+------------------+----------------
 * 0       | +/- 2g           | 8192 LSB/mg
 * 1       | +/- 4g           | 4096 LSB/mg
 * 2       | +/- 8g           | 2048 LSB/mg
 * 3       | +/- 16g          | 1024 LSB/mg
 * </pre>
 *
 * @param x 16-bit signed integer container for X-axis acceleration
 * @param y 16-bit signed integer container for Y-axis acceleration
 * @param z 16-bit signed integer container for Z-axis acceleration
 * @see MPU6050_RA_GYRO_XOUT_H
 */
void mpu6050_getAcceleration(int16_t* x, int16_t* y, int16_t* z) {
#if defined(SW_I2C)
	SwI2C_ReadBytes(DEVADDR, MPU6050_RA_ACCEL_XOUT_H, 6, buffer);
#else 
	HwI2C_ReadBytes(DEVADDR, MPU6050_RA_ACCEL_XOUT_H, 6, buffer);
#endif
    *x = (((int16_t)buffer[0]) << 8) | buffer[1];
    *y = (((int16_t)buffer[2]) << 8) | buffer[3];
    *z = (((int16_t)buffer[4]) << 8) | buffer[5];
}
/** Get X-axis accelerometer reading.
 * @return X-axis acceleration measurement in 16-bit 2's complement format
 * @see getMotion6()
 * @see MPU6050_RA_ACCEL_XOUT_H
 */
int16_t mpu6050_getAccelerationX() {
#if defined(SW_I2C)
	SwI2C_ReadBytes(DEVADDR, MPU6050_RA_ACCEL_XOUT_H, 2, buffer);
#else
	HwI2C_ReadBytes(DEVADDR, MPU6050_RA_ACCEL_XOUT_H, 2, buffer);
#endif
    return (((int16_t)buffer[0]) << 8) | buffer[1];
}
/** Get Y-axis accelerometer reading.
 * @return Y-axis acceleration measurement in 16-bit 2's complement format
 * @see getMotion6()
 * @see MPU6050_RA_ACCEL_YOUT_H
 */
int16_t mpu6050_getAccelerationY() {
#if defined(SW_I2C)
	SwI2C_ReadBytes(DEVADDR, MPU6050_RA_ACCEL_YOUT_H, 2, buffer);
#else
	HwI2C_ReadBytes(DEVADDR, MPU6050_RA_ACCEL_YOUT_H, 2, buffer);
#endif
    return (((int16_t)buffer[0]) << 8) | buffer[1];
}
/** Get Z-axis accelerometer reading.
 * @return Z-axis acceleration measurement in 16-bit 2's complement format
 * @see getMotion6()
 * @see MPU6050_RA_ACCEL_ZOUT_H
 */
int16_t mpu6050_getAccelerationZ() {
#if defined(SW_I2C)
	SwI2C_ReadBytes(DEVADDR, MPU6050_RA_ACCEL_ZOUT_H, 2, buffer);
#else
	HwI2C_ReadBytes(DEVADDR, MPU6050_RA_ACCEL_ZOUT_H, 2, buffer);
#endif
    return (((int16_t)buffer[0]) << 8) | buffer[1];
}

// TEMP_OUT_* registers

/** Get current internal temperature.
 * @return Temperature reading in 16-bit 2's complement format
 * @see MPU6050_RA_TEMP_OUT_H
 */
int16_t mpu6050_getTemperature() {
#if defined(SW_I2C)
	SwI2C_ReadBytes(DEVADDR, MPU6050_RA_TEMP_OUT_H, 2, buffer);
#else 
	HwI2C_ReadBytes(DEVADDR, MPU6050_RA_TEMP_OUT_H, 2, buffer);
#endif
    return (((int16_t)buffer[0]) << 8) | buffer[1];
}

// GYRO_*OUT_* registers

/** Get 3-axis gyroscope readings.
 * These gyroscope measurement registers, along with the accelerometer
 * measurement registers, temperature measurement registers, and external sensor
 * data registers, are composed of two sets of registers: an internal register
 * set and a user-facing read register set.
 * The data within the gyroscope sensors' internal register set is always
 * updated at the Sample Rate. Meanwhile, the user-facing read register set
 * duplicates the internal register set's data values whenever the serial
 * interface is idle. This guarantees that a burst read of sensor registers will
 * read measurements from the same sampling instant. Note that if burst reads
 * are not used, the user is responsible for ensuring a set of single byte reads
 * correspond to a single sampling instant by checking the Data Ready interrupt.
 *
 * Each 16-bit gyroscope measurement has a full scale defined in FS_SEL
 * (Register 27). For each full scale setting, the gyroscopes' sensitivity per
 * LSB in GYRO_xOUT is shown in the table below:
 *
 * <pre>
 * FS_SEL | Full Scale Range   | LSB Sensitivity
 * -------+--------------------+----------------
 * 0      | +/- 250 degrees/s  | 131 LSB/deg/s
 * 1      | +/- 500 degrees/s  | 65.5 LSB/deg/s
 * 2      | +/- 1000 degrees/s | 32.8 LSB/deg/s
 * 3      | +/- 2000 degrees/s | 16.4 LSB/deg/s
 * </pre>
 *
 * @param x 16-bit signed integer container for X-axis rotation
 * @param y 16-bit signed integer container for Y-axis rotation
 * @param z 16-bit signed integer container for Z-axis rotation
 * @see getMotion6()
 * @see MPU6050_RA_GYRO_XOUT_H
 */
void mpu6050_getRotation(int16_t* x, int16_t* y, int16_t* z) {
#if defined(SW_I2C)
	SwI2C_ReadBytes(DEVADDR, MPU6050_RA_GYRO_XOUT_H, 6, buffer);
#else 
	HwI2C_ReadBytes(DEVADDR, MPU6050_RA_GYRO_XOUT_H, 6, buffer);
#endif
    *x = (((int16_t)buffer[0]) << 8) | buffer[1];
    *y = (((int16_t)buffer[2]) << 8) | buffer[3];
    *z = (((int16_t)buffer[4]) << 8) | buffer[5];
}
/** Get X-axis gyroscope reading.
 * @return X-axis rotation measurement in 16-bit 2's complement format
 * @see getMotion6()
 * @see MPU6050_RA_GYRO_XOUT_H
 */
int16_t mpu6050_getRotationX() {
#if defined(SW_I2C)
	SwI2C_ReadBytes(DEVADDR, MPU6050_RA_GYRO_XOUT_H, 2, buffer);
#else 
	HwI2C_ReadBytes(DEVADDR, MPU6050_RA_GYRO_XOUT_H, 2, buffer);
#endif
    return (((int16_t)buffer[0]) << 8) | buffer[1];
}
/** Get Y-axis gyroscope reading.
 * @return Y-axis rotation measurement in 16-bit 2's complement format
 * @see getMotion6()
 * @see MPU6050_RA_GYRO_YOUT_H
 */
int16_t mpu6050_getRotationY() {
#if defined(SW_I2C)
	SwI2C_ReadBytes(DEVADDR, MPU6050_RA_GYRO_YOUT_H, 6, buffer);
#else
	HwI2C_ReadBytes(DEVADDR, MPU6050_RA_GYRO_YOUT_H, 6, buffer);
#endif
    return (((int16_t)buffer[0]) << 8) | buffer[1];
}
/** Get Z-axis gyroscope reading.
 * @return Z-axis rotation measurement in 16-bit 2's complement format
 * @see getMotion6()
 * @see MPU6050_RA_GYRO_ZOUT_H
 */
int16_t mpu6050_getRotationZ() {
#if defined(SW_I2C)
	SwI2C_ReadBytes(DEVADDR, MPU6050_RA_GYRO_ZOUT_H, 6, buffer);
#else 
	HwI2C_ReadBytes(DEVADDR, MPU6050_RA_GYRO_ZOUT_H, 6, buffer);
#endif
    return (((int16_t)buffer[0]) << 8) | buffer[1];
}

/** Get I2C Master Mode enabled status.
 * When this mode is enabled, the MPU-60X0 acts as the I2C Master to the
 * external sensor slave devices on the auxiliary I2C bus. When this bit is
 * cleared to 0, the auxiliary I2C bus lines (AUX_DA and AUX_CL) are logically
 * driven by the primary I2C bus (SDA and SCL). This is a precondition to
 * enabling Bypass Mode. For further information regarding Bypass Mode, please
 * refer to Register 55.
 * @return Current I2C Master Mode enabled status
 * @see MPU6050_RA_USER_CTRL
 * @see MPU6050_USERCTRL_I2C_MST_EN_BIT
 */
u8 mpu6050_getI2CMasterModeEnabled() {
    I2C_ReadBit(DEVADDR, MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_I2C_MST_EN_BIT, buffer);
    return buffer[0];
}
/** Set I2C Master Mode enabled status.
 * @param enabled New I2C Master Mode enabled status
 * @see getI2CMasterModeEnabled()
 * @see MPU6050_RA_USER_CTRL
 * @see MPU6050_USERCTRL_I2C_MST_EN_BIT
 */
void mpu6050_setI2CMasterModeEnabled(u8 enabled) {
    I2C_WriteBit(DEVADDR, MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_I2C_MST_EN_BIT, enabled);
}

// PWR_MGMT_1 register

/** Trigger a full device reset.
 * A small delay of ~50ms may be desirable after triggering a reset.
 * @see MPU6050_RA_PWR_MGMT_1
 * @see MPU6050_PWR1_DEVICE_RESET_BIT
 */
bool mpu6050_reset() {
	return I2C_WriteBit(DEVADDR, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_DEVICE_RESET_BIT, 1);
}
/** Get sleep mode status.
 * Setting the SLEEP bit in the register puts the device into very low power
 * sleep mode. In this mode, only the serial interface and internal registers
 * remain active, allowing for a very low standby current. Clearing this bit
 * puts the device back into normal mode. To save power, the individual standby
 * selections for each of the gyros should be used if any gyro axis is not used
 * by the application.
 * @return Current sleep mode enabled status
 * @see MPU6050_RA_PWR_MGMT_1
 * @see MPU6050_PWR1_SLEEP_BIT
 */
u8 mpu6050_getSleepEnabled() {
    I2C_ReadBit(DEVADDR, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, buffer);
    return buffer[0];
}
/** Set sleep mode status.
 * @param enabled New sleep mode enabled status
 * @see getSleepEnabled()
 * @see MPU6050_RA_PWR_MGMT_1
 * @see MPU6050_PWR1_SLEEP_BIT
 */
void mpu6050_setSleepEnabled(u8 enabled) {
    I2C_WriteBit(DEVADDR, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, enabled);
}

/** Get temperature sensor enabled status.
 * Control the usage of the internal temperature sensor.
 *
 * Note: this register stores the *disabled* value, but for consistency with the
 * rest of the code, the function is named and used with standard true/false
 * values to indicate whether the sensor is enabled or disabled, respectively.
 *
 * @return Current temperature sensor enabled status
 * @see MPU6050_RA_PWR_MGMT_1
 * @see MPU6050_PWR1_TEMP_DIS_BIT
 */
u8 mpu6050_getTempSensorEnabled() {
    I2C_ReadBit(DEVADDR, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_TEMP_DIS_BIT, buffer);
    return buffer[0] == 0; // 1 is actually disabled here
}
/** Set temperature sensor enabled status.
 * Note: this register stores the *disabled* value, but for consistency with the
 * rest of the code, the function is named and used with standard true/false
 * values to indicate whether the sensor is enabled or disabled, respectively.
 *
 * @param enabled New temperature sensor enabled status
 * @see getTempSensorEnabled()
 * @see MPU6050_RA_PWR_MGMT_1
 * @see MPU6050_PWR1_TEMP_DIS_BIT
 */
void mpu6050_setTempSensorEnabled(u8 enabled) {
    // 1 is actually disabled here
    I2C_WriteBit(DEVADDR, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_TEMP_DIS_BIT, !enabled);
}

// SMPLRT_DIV register

/** Get gyroscope output rate divider.
 * The sensor register output, FIFO output, DMP sampling, Motion detection, Zero
 * Motion detection, and Free Fall detection are all based on the Sample Rate.
 * The Sample Rate is generated by dividing the gyroscope output rate by
 * SMPLRT_DIV:
 *
 * Sample Rate = Gyroscope Output Rate / (1 + SMPLRT_DIV)
 *
 * where Gyroscope Output Rate = 8kHz when the DLPF is disabled (DLPF_CFG = 0 or
 * 7), and 1kHz when the DLPF is enabled (see Register 26).
 *
 * Note: The accelerometer output rate is 1kHz. This means that for a Sample
 * Rate greater than 1kHz, the same accelerometer sample may be output to the
 * FIFO, DMP, and sensor registers more than once.
 *
 * For a diagram of the gyroscope and accelerometer signal paths, see Section 8
 * of the MPU-6000/MPU-6050 Product Specification document.
 *
 * @return Current sample rate
 * @see MPU6050_RA_SMPLRT_DIV
 */
uint8_t mpu6050_getRate(void) {
#if defined(SW_I2C)
	SwI2C_ReadBytes(DEVADDR, MPU6050_RA_SMPLRT_DIV, 1, buffer);
#else
	HwI2C_ReadBytes(DEVADDR, MPU6050_RA_SMPLRT_DIV, 1, buffer);
#endif
    return buffer[0];
}
/** Set gyroscope sample rate divider.
 * @param rate New sample rate divider
 * @see getRate()
 * @see MPU6050_RA_SMPLRT_DIV
 */
void mpu6050_setRate(uint8_t rate) {
#if defined(SW_I2C)
	SwI2C_WriteByte(DEVADDR, MPU6050_RA_SMPLRT_DIV, rate);
#else
	HwI2C_WriteByte(DEVADDR, MPU6050_RA_SMPLRT_DIV, rate);
#endif
}

// INT_ENABLE register

/** Get full interrupt enabled status.
 * Full register byte for all interrupts, for quick reading. Each bit will be
 * set 0 for disabled, 1 for enabled.
 * @return Current interrupt enabled status
 * @see MPU6050_RA_INT_ENABLE
 * @see MPU6050_INTERRUPT_FF_BIT
 **/
uint8_t mpu6050_getIntEnabled()
{
#if defined(SW_I2C)
	SwI2C_ReadBytes(DEVADDR, MPU6050_RA_INT_ENABLE, 1, buffer);
#else
	HwI2C_ReadBytes(DEVADDR, MPU6050_RA_INT_ENABLE, 1, buffer);
#endif
  return buffer[0];
}
/** Set full interrupt enabled status.
 * Full register byte for all interrupts, for quick reading. Each bit should be
 * set 0 for disabled, 1 for enabled.
 * @param enabled New interrupt enabled status
 * @see getIntFreefallEnabled()
 * @see MPU6050_RA_INT_ENABLE
 * @see MPU6050_INTERRUPT_FF_BIT
 **/
void mpu6050SetIntEnabled(uint8_t enabled)
{
#if defined(SW_I2C)
	SwI2C_WriteByte(DEVADDR, MPU6050_RA_INT_ENABLE, enabled);
#else
	HwI2C_WriteByte(DEVADDR, MPU6050_RA_INT_ENABLE, enabled);
#endif
}

// WHO_AM_I register

/** Get Device ID.
 * This register is used to verify the identity of the device (0b110100).
 * @return Device ID (should be 0x68, 104 dec, 150 oct)
 * @see MPU6050_RA_WHO_AM_I
 * @see MPU6050_WHO_AM_I_BIT
 * @see MPU6050_WHO_AM_I_LENGTH
 */
uint8_t mpu6050_getDeviceID(void) {
    I2C_ReadBits(DEVADDR, MPU6050_RA_WHO_AM_I, MPU6050_WHO_AM_I_BIT, MPU6050_WHO_AM_I_LENGTH, buffer);
	return buffer[0];
}
/** Set Device ID.
 * Write a new ID into the WHO_AM_I register (no idea why this should ever be
 * necessary though).
 * @param id New device ID to set.
 * @see getDeviceID()
 * @see MPU6050_RA_WHO_AM_I
 * @see MPU6050_WHO_AM_I_BIT
 * @see MPU6050_WHO_AM_I_LENGTH
 */
void mpu6050_setDeviceID(uint8_t id) {
    I2C_WriteBits(DEVADDR, MPU6050_RA_WHO_AM_I, MPU6050_WHO_AM_I_BIT, MPU6050_WHO_AM_I_LENGTH, id);
}

/** Check MPU6050
 * get ID and is or not WHO_AM_I 
 */
bool mpu6050_check()
{
	uint8_t id;
	id = mpu6050_getDeviceID() << 1;
	if(id == 0x68){
		MPU6050_DEBUG("Check OK, DEV_ID: 0x%x\r\n", id);
		return TRUE;
	}else{
		MPU6050_DEBUG("Check NG! DEV_ID: 0x%x\r\n", id);
		return FALSE;
	}
}


/** Get angle
 */
uint8_t mpu6050_getAngle(float x, float y, float z, AXIS dir)
{
	float temp;
	float ang = 0;
	switch(dir)
	{
		case AXIS_X: //X Axis
			//temp = x/sqrt((y*y + z*z));
			temp = x;
			break;
		case AXIS_Y: //Y Axis
			//temp = y/sqrt((x*x + z*z));
			temp = y;
			break;
		case AXIS_Z: //Z Axis
			//temp = sqrt((x*x + y*y))/z;
			temp = z;
			break;
	}
	ang = acos(temp);//atan(temp);
	ang = ang * 1800/3.14;
	return ang;
}
