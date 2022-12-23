/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h" /* FatFs lower layer API */
#include "app_flash.h"
/* Definitions of physical drive number for each drive */
// #define DEV_RAM 0 /* Example: Map Ramdisk to physical drive 0 */
// #define DEV_MMC 1 /* Example: Map MMC/SD card to physical drive 1 */
// #define DEV_USB 2 /* Example: Map USB MSD to physical drive 2 */
#define DEV_SPI 0

void spi_disk_read(UINT8 *buff, UINT32 sector, UINT16 count);

void spi_disk_write(UINT8 *buff, UINT32 sector, UINT16 count);

void spi_disk_erase(UINT32 sector);

DRESULT SPI_disk_ioctl(BYTE cmd, void *buff);

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status(
	BYTE pdrv /* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;

	DSTATUS status = STA_NOINIT;

	switch (pdrv)
	{
		// case DEV_RAM :
		// 	result = RAM_disk_status();

		// 	// translate the reslut code here

		// 	return stat;

		// case DEV_MMC :
		// 	result = MMC_disk_status();

		// 	// translate the reslut code here

		// 	return stat;

		// case DEV_USB :
		// 	result = USB_disk_status();

		// 	// translate the reslut code here

		// 	return stat;

	case DEV_SPI:

		if (app_flash_read_flashID() == 0XEF16)
		{
			/* 设备ID读取结果正确 */
			status &= ~STA_NOINIT;
		}
		else
		{
			/* 设备ID读取结果错误 */
			status = STA_NOINIT;
		}
		return status;
	}
	return STA_NOINIT;
}

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize(
	BYTE pdrv /* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;
	DSTATUS status = STA_NOINIT;	

	switch (pdrv)
	{
		// case DEV_RAM :
		// 	result = RAM_disk_initialize();

		// 	// translate the reslut code here

		// 	return stat;

		// case DEV_MMC :
		// 	result = MMC_disk_initialize();

		// 	// translate the reslut code here

		// 	return stat;

		// case DEV_USB :
		// 	result = USB_disk_initialize();

		// 	// translate the reslut code here

		// 	return stat;

	case DEV_SPI:
		app_flash_init();
		status = disk_status(DEV_SPI);
		break;
	default:
		status = STA_NOINIT;
	}
	return status;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read(
	BYTE pdrv,	  /* Physical drive nmuber to identify the drive */
	BYTE *buff,	  /* Data buffer to store read data */
	DWORD sector, /* Start sector in LBA */
	UINT count	  /* Number of sectors to read */
)
{
	DRESULT res;
	int result;

	switch (pdrv)
	{
		// case DEV_RAM :
		// 	// translate the arguments here

		// 	result = RAM_disk_read(buff, sector, count);

		// 	// translate the reslut code here

		// 	return res;

		// case DEV_MMC :
		// 	// translate the arguments here

		// 	result = MMC_disk_read(buff, sector, count);

		// 	// translate the reslut code here

		// 	return res;

		// case DEV_USB :
		// 	// translate the arguments here

		// 	result = USB_disk_read(buff, sector, count);

		// 	// translate the reslut code here

		// 	return res;

	case DEV_SPI:

		spi_disk_read((UINT8 *)buff, sector << 12, count << 12);

		return RES_OK;
	}

	return RES_PARERR;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_write(
	BYTE pdrv,		  /* Physical drive nmuber to identify the drive */
	const BYTE *buff, /* Data to be written */
	DWORD sector,	  /* Start sector in LBA */
	UINT count		  /* Number of sectors to write */
)
{
	DRESULT res;
	int result;

	switch (pdrv)
	{
	// case DEV_RAM :
	// 	// translate the arguments here

	// 	result = RAM_disk_write(buff, sector, count);

	// 	// translate the reslut code here

	// 	return res;

	// case DEV_MMC :
	// 	// translate the arguments here

	// 	result = MMC_disk_write(buff, sector, count);

	// 	// translate the reslut code here

	// 	return res;

	// case DEV_USB :
	// 	// translate the arguments here

	// 	result = USB_disk_write(buff, sector, count);

	// 	// translate the reslut code here

	// 	return res;
	case DEV_SPI:
		spi_disk_erase(sector << 12);
		spi_disk_write((UINT8 *)buff, sector << 12, count << 12);

		return RES_OK;
	}

	return RES_PARERR;
}

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl(
	BYTE pdrv, /* Physical drive nmuber (0..) */
	BYTE cmd,  /* Control code */
	void *buff /* Buffer to send/receive control data */
)
{
	// DRESULT res;
	// int result;

	DRESULT status = RES_PARERR;

	switch (pdrv)
	{
		// case DEV_RAM :

		// 	// Process of the command for the RAM drive

		// 	return res;

		// case DEV_MMC :

		// 	// Process of the command for the MMC/SD card

		// 	return res;

		// case DEV_USB :

		// 	// Process of the command the USB drive

		// 	return res;

	case DEV_SPI:
		switch (cmd)
		{
		/* 扇区数量：1536*4096/1024/1024=6(MB) */
		case GET_SECTOR_COUNT:
			// *(DWORD *)buff = 1536;
			*(DWORD *)buff = 2048;
			break;
		/* 扇区大小  */
		case GET_SECTOR_SIZE:
			*(WORD *)buff = 4096;
			break;
		/* 同时擦除扇区个数 */
		case GET_BLOCK_SIZE:
			*(DWORD *)buff = 1;
			break;
		}
		status = RES_OK;
		break;
	}

	return status;
}

void spi_disk_read(UINT8 *buff, UINT32 sector, UINT16 count)
{
	app_flash_read(sector, count, (PUINT8)buff);
}

void spi_disk_write(UINT8 *buff, UINT32 sector, UINT16 count)
{
	app_flash_write(sector, count, (PUINT8)buff);

	DelayMs(20);
}
void spi_disk_erase(UINT32 sector)
{
	app_flash_erase_4k_flash(sector);
}