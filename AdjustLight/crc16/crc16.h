#ifndef __CRC16_H__
#define __CRC16_H__


/**
 * @brief getCRC16  获取CRC16
 * @param data  计算的数据
 * @param len  数据的长度
 * @param oldCRC16  上一个CRC16的值，用于循环计算大文件的CRC16。第一个数据的CRC16则传入0x0。
 * @return
 */
unsigned short getCRC16(const char *pData, unsigned long iLen, unsigned long iOldCRC16);


#endif /* __CRC16_H__ */
