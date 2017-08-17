/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : usb_desc.c
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : Descriptors for Joystick Mouse Demo
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
#include "usb_desc.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* USB Standard Device Descriptor */
const u8 Joystick_DeviceDescriptor[JOYSTICK_SIZ_DEVICE_DESC] =
  {
    0x12,                       /*bLength */
    USB_DEVICE_DESCRIPTOR_TYPE, /*bDescriptorType*/
    0x00,                       /*bcdUSB */
    0x02,
    0x00,                       /*bDeviceClass*/
    0x00,                       /*bDeviceSubClass*/
    0x00,                       /*bDeviceProtocol*/
    0x40,                       /*bMaxPacketSize40*/
    0x11,                       /*idVendor (0x1234)*/
    0x11,
    0x11,                       /*idProduct = 0x4321*/
    0x11,
    0x00,                       /*bcdDevice rel. 2.00*/
    0x02,
    1,                          /*Index of string descriptor describing
                                              manufacturer */
    2,                          /*Index of string descriptor describing
                                             product*/
    3,                          /*Index of string descriptor describing the
                                             device serial number */
    0x01                        /*bNumConfigurations*/
  }
  ; /* Joystick_DeviceDescriptor */


const u8 Joystick_ConfigDescriptor[JOYSTICK_SIZ_CONFIG_DESC] =
{
 /***************ÅäÖÃÃèÊö·û***********************/
 0x09,		//bLength×Ö¶Î
 USB_CONFIGURATION_DESCRIPTOR_TYPE,		//bDescriptorType×Ö¶Î
 //wTotalLength×Ö¶Î
 JOYSTICK_SIZ_CONFIG_DESC,
 /* wTotalLength: Bytes returned */
 0x00,
 
 0x02,	//bNumInterfaces×Ö¶Î
 0x01,	//bConfiguration×Ö¶Î
 0x00,	//iConfigurationz×Ö¶Î
 0x80,	//bmAttributes×Ö¶Î
 0x32,	//bMaxPower×Ö¶Î
 
 /*******************µÚÒ»¸ö½Ó¿ÚÃèÊö·û*********************/
 0x09,	//bLength×Ö¶Î
 0x04,	//bDescriptorType×Ö¶Î
 0x00,	//bInterfaceNumber×Ö¶Î
 0x00,	//bAlternateSetting×Ö¶Î
 0x02,	//bNumEndpoints×Ö¶Î
 0x03,	//bInterfaceClass×Ö¶Î
 0x01,	//bInterfaceSubClass×Ö¶Î
 0x01,	//bInterfaceProtocol×Ö¶Î
 0x00,	//iConfiguration×Ö¶Î
 
 /******************HIDÃèÊö·û************************/
 0x09,	//bLength×Ö¶Î
 0x21,	//bDescriptorType×Ö¶Î
 0x10,	//bcdHID×Ö¶Î
 0x01,
 0x21,	//bCountyCode×Ö¶Î
 0x01,	//bNumDescriptors×Ö¶Î
 0x22,	//bDescriptorType×Ö¶Î
 
 //bDescriptorLength×Ö¶Î¡£
 //ÏÂ¼¶ÃèÊö·ûµÄ³¤¶È¡£ÏÂ¼¶ÃèÊö·ûÎª¼üÅÌ±¨¸æÃèÊö·û¡£
 sizeof(KeyboardReportDescriptor)&0xFF,
 (sizeof(KeyboardReportDescriptor)>>8)&0xFF,
 
 /**********************ÊäÈë¶ËµãÃèÊö·û***********************/
 0x07,	//bLength×Ö¶Î
 0x05,	//bDescriptorType×Ö¶Î
 0x81,	//bEndpointAddress×Ö¶Î
 0x03,	//bmAttributes×Ö¶Î
 0x10,	//wMaxPacketSize×Ö¶Î
 0x00,
 0x0A,	//bInterval×Ö¶Î
 
 /**********************Êä³ö¶ËµãÃèÊö·û***********************/
 0x07,	//bLength×Ö¶Î
 0x05,	//bDescriptorType×Ö¶Î
 0x01,	//bEndpointAddress×Ö¶Î
 0x03,	//bmAttributes×Ö¶Î
 0x10,	//wMaxPacketSize×Ö¶Î
 0x00,
 0x0A,	//bInterval×Ö¶Î

 /*******************µÚ¶þ¸ö½Ó¿ÚÃèÊö·û*********************/
 0x09,	//bLength×Ö¶Î
 0x04,	//bDescriptorType×Ö¶Î
 0x01,	//bInterfaceNumber×Ö¶Î
 0x00,	//bAlternateSetting×Ö¶Î
 0x01,	//bNumEndpoints×Ö¶Î
 0x03,	//bInterfaceClass×Ö¶Î
 0x01,	//bInterfaceSubClass×Ö¶Î
 0x02,	//bInterfaceProtocol×Ö¶Î
 0x00,	//iConfiguration×Ö¶Î
 
 /******************HIDÃèÊö·û************************/
 0x09,	//bLength×Ö¶Î
 0x21,	//bDescriptorType×Ö¶Î
 0x10,	//bcdHID×Ö¶Î
 0x01,
 0x21,	//bCountyCode×Ö¶Î
 0x01,	//bNumDescriptors×Ö¶Î
 0x22,	//bDescriptorType×Ö¶Î
 sizeof(MouseReportDescriptor)&0xFF,		//bDescriptorLength×Ö¶Î
 (sizeof(MouseReportDescriptor)>>8)&0xFF,
 
 /**********************ÊäÈë¶ËµãÃèÊö·û***********************/
 0x07,	//bLength×Ö¶Î
 0x05,	//bDescriptorType×Ö¶Î
 0x82,	//bEndpointAddress×Ö¶Î
 0x03,	//bmAttributes×Ö¶Î¡£D1~D0Îª¶Ëµã´«ÊäÀàÐÍÑ¡Ôñ
 0x40,	//wMaxPacketSize×Ö¶Î
 0x00,
 0x0A 	//bInterval×Ö¶Î
};
////////////////////////ÅäÖÃÃèÊö·û¼¯ºÏÍê±Ï//////////////////////////
//USB¼üÅÌ±¨¸æÃèÊö·ûµÄ¶¨Òå
const u8 KeyboardReportDescriptor[KP_ReportDescriptor_Size]=
{
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)	//63
    0x09, 0x06,                    // USAGE (Keyboard)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x08,                    //   REPORT_COUNT (8)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)
    0x95, 0x05,                    //   REPORT_COUNT (5)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x05, 0x08,                    //   USAGE_PAGE (LEDs)
    0x19, 0x01,                    //   USAGE_MINIMUM (Num Lock)
    0x29, 0x05,                    //   USAGE_MAXIMUM (Kana)
    0x91, 0x02,                    //   OUTPUT (Data,Var,Abs)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x03,                    //   REPORT_SIZE (3)
    0x91, 0x03,                    //   OUTPUT (Cnst,Var,Abs)
    0x95, 0x06,                    //   REPORT_COUNT (6)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x65,                    //   LOGICAL_MAXIMUM (101)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0x65,                    //   USAGE_MAXIMUM (Keyboard Application)
    0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
    0xc0,                           // END_COLLECTION
	//0xc0,
  }; /* Joystick_ReportDescriptor */
///////////////////////////¼üÅÌ±¨¸æÃèÊö·ûÍê±Ï////////////////////////////

//USBÊó±ê±¨¸æÃèÊö·ûµÄ¶¨Òå
const u8 MouseReportDescriptor[Mouse_ReportDescriptor_Size]=
{
 0x05, 0x01, // USAGE_PAGE (Generic Desktop)
 0x09, 0x02, // USAGE (Mouse)
 0xa1, 0x01, // COLLECTION (Application)
 0x85, 0x01, //Report ID (1)
 0x09, 0x01, //   USAGE (Pointer)
 0xa1, 0x00, //   COLLECTION (Physical)
 0x05, 0x09, //     USAGE_PAGE (Button)
 0x19, 0x01, //     USAGE_MINIMUM (Button 1)
 0x29, 0x03, //     USAGE_MAXIMUM (Button 3)
 0x15, 0x00, //     LOGICAL_MINIMUM (0)
 0x25, 0x01, //     LOGICAL_MAXIMUM (1)
 0x95, 0x03, //     REPORT_COUNT (3)
 0x75, 0x01, //     REPORT_SIZE (1)
 0x81, 0x02, //     INPUT (Data,Var,Abs)
 0x95, 0x01, //     REPORT_COUNT (1)
 0x75, 0x05, //     REPORT_SIZE (5)
 0x81, 0x03, //     INPUT (Cnst,Var,Abs)
 0x05, 0x01, //     USAGE_PAGE (Generic Desktop)
 0x09, 0x30, //     USAGE (X)
 0x09, 0x31, //     USAGE (Y)
 0x09, 0x38, //     USAGE (Wheel)
 0x15, 0x81, //     LOGICAL_MINIMUM (-127)
 0x25, 0x7f, //     LOGICAL_MAXIMUM (127)
 0x75, 0x08, //     REPORT_SIZE (8)
 0x95, 0x03, //     REPORT_COUNT (3)
 0x81, 0x06, //     INPUT (Data,Var,Rel)
 0xc0,       //   END_COLLECTION
 0xc0        // END_COLLECTION
};
///////////////////////////±¨¸æÃèÊö·ûÍê±Ï////////////////////////////
/* USB String Descriptors (optional) */
const u8 Joystick_StringLangID[JOYSTICK_SIZ_STRING_LANGID] =
  {
    JOYSTICK_SIZ_STRING_LANGID,
    USB_STRING_DESCRIPTOR_TYPE,
    0x09,
    0x04
  }
  ; /* LangID = 0x0409: U.S. English */

const u8 Joystick_StringVendor[JOYSTICK_SIZ_STRING_VENDOR] =
  {
    JOYSTICK_SIZ_STRING_VENDOR, /* Size of Vendor string */
    USB_STRING_DESCRIPTOR_TYPE,  /* bDescriptorType*/
    /* Manufacturer: "STMicroelectronics" */
    'S', 0, 'T', 0, 'M', 0, 'i', 0, 'c', 0, 'r', 0, 'o', 0, 'e', 0,
    'l', 0, 'e', 0, 'c', 0, 't', 0, 'r', 0, 'o', 0, 'n', 0, 'i', 0,
    'c', 0, 's', 0
  };

const u8 Joystick_StringProduct[JOYSTICK_SIZ_STRING_PRODUCT] =
  {
    JOYSTICK_SIZ_STRING_PRODUCT,          /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    'S', 0, 'T', 0, 'M', 0, '3', 0, '2', 0, ' ', 0, 'J', 0,
    'o', 0, 'y', 0, 's', 0, 't', 0, 'i', 0, 'c', 0, 'k', 0
  };
u8 Joystick_StringSerial[JOYSTICK_SIZ_STRING_SERIAL] =
  {
    JOYSTICK_SIZ_STRING_SERIAL,           /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    'S', 0, 'T', 0, 'M', 0, '3', 0, '2', 0, '1', 0, '0', 0
  };

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/

