/* USB Keyboard Example for Teensy USB Development Board
 * http://www.pjrc.com/teensy/usb_keyboard.html
 * Copyright (c) 2009 PJRC.COM, LLC
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

// Version 1.0: Initial Release
// Version 1.1: Add support for Teensy 2.0


/**                                                                 description
 * The PJRC USB keyboard implementation
 *
 * History:
 * - Modified 2013, Ben Blazak
 *
 * Notes:
 * - 'description' added
 * - `usb_keyboard_press()` removed
 * - `OPT__` macros added (and other code modified accordingly)
 * - `PROGMEM` code made `const`
 * - removed unused variable `t` from `ISR(USB_GEN_vect)`
 */


// ----------------------------------------------------------------------------

#ifndef OPT__USB__STR_MANUFACTURER
    #error "OPT__USB__STR_MANUFACTURER not defined"
#endif
#ifndef OPT__USB__STR_PRODUCT
    #error "OPT__USB__STR_PRODUCT not defined"
#endif
#ifndef OPT__USB__VENDOR_ID
    #error "OPT__USB__VENDOR_ID not defined"
#endif
#ifndef OPT__USB__PRODUCT_ID
    #error "OPT__USB__PRODUCT_ID not defined"
#endif

/**                                              macros/(group) USB/description
 * USB identifier information
 *
 * Members:
 * - `OPT__USB__STR_MANUFACTURER`
 * - `OPT__USB__STR_PRODUCT`
 * - `OPT__USB__VENDOR_ID`
 * - `OPT__USB__PRODUCT_ID`
 */

// ----------------------------------------------------------------------------

#define USB_SERIAL_PRIVATE_INCLUDE
#include "usb.h"

/**************************************************************************
 *
 *  Configurable Options
 *
 **************************************************************************/

// You can change these to give your code its own name.
#define STR_MANUFACTURER OPT__USB__STR_MANUFACTURER
#define STR_PRODUCT      OPT__USB__STR_PRODUCT


// Mac OS-X and Linux automatically load the correct drivers.  On
// Windows, even though the driver is supplied by Microsoft, an
// INF file is needed to load the driver.  These numbers need to
// match the INF file.
#define VENDOR_ID  OPT__USB__VENDOR_ID
#define PRODUCT_ID OPT__USB__PRODUCT_ID


// USB devices are supposed to implment a halt feature, which is
// rarely (if ever) used.  If you comment this line out, the halt
// code will be removed, saving 102 bytes of space (gcc 4.3.0).
// This is not strictly USB compliant, but works with all major
// operating systems.
#define SUPPORT_ENDPOINT_HALT



/**************************************************************************
 *
 *  Endpoint Buffer Configuration
 *
 **************************************************************************/

#define ENDPOINT0_SIZE		32

static const uint8_t PROGMEM endpoint_config_table[] = {
	1, EP_TYPE_INTERRUPT_IN,  EP_SIZE(KBD_SIZE)      | KBD_BUFFER,      // 1
#ifdef MOUSE_ENABLE
	1, EP_TYPE_INTERRUPT_IN,  EP_SIZE(MOUSE_SIZE)    | MOUSE_BUFFER,    // 2
#else
        0,                                                            // 2
#endif
#ifdef NKRO_ENABLE
	1, EP_TYPE_INTERRUPT_IN,  EP_SIZE(KBD2_SIZE)     | KBD2_BUFFER,     // 3
#else
        0,                                                            // 3
#endif
	0,                                                                  // 4
};


/**************************************************************************
 *
 *  Descriptor Data
 *
 **************************************************************************/

// Descriptors are the data that your computer reads when it auto-detects
// this USB device (called "enumeration" in USB lingo).  The most commonly
// changed items are editable at the top of this file.  Changing things
// in here should only be done by those who've read chapter 9 of the USB
// spec and relevant portions of any USB class specifications!


static const uint8_t PROGMEM device_descriptor[] = {
	18,					// bLength
	1,					// bDescriptorType
	0x00, 0x02,				// bcdUSB
	0,					// bDeviceClass
	0,					// bDeviceSubClass
	0,					// bDeviceProtocol
	ENDPOINT0_SIZE,				// bMaxPacketSize0
	LSB(VENDOR_ID), MSB(VENDOR_ID),		// idVendor
	LSB(PRODUCT_ID), MSB(PRODUCT_ID),	// idProduct
	0x00, 0x01,				// bcdDevice
	1,					// iManufacturer
	2,					// iProduct
	0,					// iSerialNumber
	1					// bNumConfigurations
};

// Keyboard Protocol 1, HID 1.11 spec, Appendix B, page 59-60
static const uint8_t PROGMEM keyboard_hid_report_desc[] = {
        0x05, 0x01,          // Usage Page (Generic Desktop),
        0x09, 0x06,          // Usage (Keyboard),
        0xA1, 0x01,          // Collection (Application),
        0x75, 0x01,          //   Report Size (1),
        0x95, 0x08,          //   Report Count (8),
        0x05, 0x07,          //   Usage Page (Key Codes),
        0x19, 0xE0,          //   Usage Minimum (224),
        0x29, 0xE7,          //   Usage Maximum (231),
        0x15, 0x00,          //   Logical Minimum (0),
        0x25, 0x01,          //   Logical Maximum (1),
        0x81, 0x02,          //   Input (Data, Variable, Absolute), ;Modifier byte
        0x95, 0x01,          //   Report Count (1),
        0x75, 0x08,          //   Report Size (8),
        0x81, 0x03,          //   Input (Constant),                 ;Reserved byte
        0x95, 0x05,          //   Report Count (5),
        0x75, 0x01,          //   Report Size (1),
        0x05, 0x08,          //   Usage Page (LEDs),
        0x19, 0x01,          //   Usage Minimum (1),
        0x29, 0x05,          //   Usage Maximum (5),
        0x91, 0x02,          //   Output (Data, Variable, Absolute), ;LED report
        0x95, 0x01,          //   Report Count (1),
        0x75, 0x03,          //   Report Size (3),
        0x91, 0x03,          //   Output (Constant),                 ;LED report padding
        0x95, KBD_REPORT_KEYS,          //   Report Count (?),
        0x75, 0x08,          //   Report Size (8),
        0x15, 0x00,          //   Logical Minimum (0),
        0x25, 0xFF,          //   Logical Maximum(104),
        0x05, 0x07,          //   Usage Page (Key Codes),
        0x19, 0x00,          //   Usage Minimum (0),
        0x29, 0xFF,          //   Usage Maximum (104),
        0x81, 0x00,          //   Input (Data, Array),
        0xc0                 // End Collection
};

#ifdef MOUSE_ENABLE
// Mouse Protocol 1, HID 1.11 spec, Appendix B, page 59-60, with wheel extension
// http://www.microchip.com/forums/tm.aspx?high=&m=391435&mpage=1#391521
// http://www.keil.com/forum/15671/
// http://www.microsoft.com/whdc/device/input/wheel.mspx
static const uint8_t PROGMEM mouse_hid_report_desc[] = {
    /* mouse */
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x02,                    // USAGE (Mouse)
    0xa1, 0x01,                    // COLLECTION (Application)
    //0x85, REPORT_ID_MOUSE,         //   REPORT_ID (1)
    0x09, 0x01,                    //   USAGE (Pointer)
    0xa1, 0x00,                    //   COLLECTION (Physical)
                                   // ----------------------------  Buttons
    0x05, 0x09,                    //     USAGE_PAGE (Button)
    0x19, 0x01,                    //     USAGE_MINIMUM (Button 1)
    0x29, 0x05,                    //     USAGE_MAXIMUM (Button 5)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //     REPORT_SIZE (1)
    0x95, 0x05,                    //     REPORT_COUNT (5)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    0x75, 0x03,                    //     REPORT_SIZE (3)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x81, 0x03,                    //     INPUT (Cnst,Var,Abs)
                                   // ----------------------------  X,Y position
    0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
    0x09, 0x30,                    //     USAGE (X)
    0x09, 0x31,                    //     USAGE (Y)
    0x15, 0x81,                    //     LOGICAL_MINIMUM (-127)
    0x25, 0x7f,                    //     LOGICAL_MAXIMUM (127)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x02,                    //     REPORT_COUNT (2)
    0x81, 0x06,                    //     INPUT (Data,Var,Rel)
                                   // ----------------------------  Vertical wheel
    0x09, 0x38,                    //     USAGE (Wheel)
    0x15, 0x81,                    //     LOGICAL_MINIMUM (-127)
    0x25, 0x7f,                    //     LOGICAL_MAXIMUM (127)
    0x35, 0x00,                    //     PHYSICAL_MINIMUM (0)        - reset physical
    0x45, 0x00,                    //     PHYSICAL_MAXIMUM (0)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x81, 0x06,                    //     INPUT (Data,Var,Rel)
                                   // ----------------------------  Horizontal wheel
    0x05, 0x0c,                    //     USAGE_PAGE (Consumer Devices)
    0x0a, 0x38, 0x02,              //     USAGE (AC Pan)
    0x15, 0x81,                    //     LOGICAL_MINIMUM (-127)
    0x25, 0x7f,                    //     LOGICAL_MAXIMUM (127)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x81, 0x06,                    //     INPUT (Data,Var,Rel)
    0xc0,                          //   END_COLLECTION
    0xc0,                          // END_COLLECTION
};
#endif

#ifdef NKRO_ENABLE
// http://geekhack.org/index.php?topic=13162.msg259955#msg259955
static const uint8_t PROGMEM keyboard2_hid_report_desc[] = {
        0x05, 0x01,                     // Usage Page (Generic Desktop),
        0x09, 0x06,                     // Usage (Keyboard),
        0xA1, 0x01,                     // Collection (Application),
        // bitmap of modifiers
        0x75, 0x01,                     //   Report Size (1),
        0x95, 0x08,                     //   Report Count (8),
        0x05, 0x07,                     //   Usage Page (Key Codes),
        0x19, 0xE0,                     //   Usage Minimum (224),
        0x29, 0xE7,                     //   Usage Maximum (231),
        0x15, 0x00,                     //   Logical Minimum (0),
        0x25, 0x01,                     //   Logical Maximum (1),
        0x81, 0x02,                     //   Input (Data, Variable, Absolute), ;Modifier byte
        // LED output report
        0x95, 0x05,                     //   Report Count (5),
        0x75, 0x01,                     //   Report Size (1),
        0x05, 0x08,                     //   Usage Page (LEDs),
        0x19, 0x01,                     //   Usage Minimum (1),
        0x29, 0x05,                     //   Usage Maximum (5),
        0x91, 0x02,                     //   Output (Data, Variable, Absolute),
        0x95, 0x01,                     //   Report Count (1),
        0x75, 0x03,                     //   Report Size (3),
        0x91, 0x03,                     //   Output (Constant),
        // bitmap of keys
        0x95, KBD2_REPORT_KEYS*8,       //   Report Count (),
        0x75, 0x01,                     //   Report Size (1),
        0x15, 0x00,                     //   Logical Minimum (0),
        0x25, 0x01,                     //   Logical Maximum(1),
        0x05, 0x07,                     //   Usage Page (Key Codes),
        0x19, 0x00,                     //   Usage Minimum (0),
        0x29, KBD2_REPORT_KEYS*8-1,     //   Usage Maximum (),
        0x81, 0x02,                     //   Input (Data, Variable, Absolute),
        0xc0                            // End Collection
};
#endif

#define KBD_HID_DESC_NUM                0
#define KBD_HID_DESC_OFFSET             (9+(9+9+7)*KBD_HID_DESC_NUM+9)

#ifdef MOUSE_ENABLE
#   define MOUSE_HID_DESC_NUM           (KBD_HID_DESC_NUM + 1)
#   define MOUSE_HID_DESC_OFFSET        (9+(9+9+7)*MOUSE_HID_DESC_NUM+9)
#else
#   define MOUSE_HID_DESC_NUM           (KBD_HID_DESC_NUM + 0)
#endif

#ifdef NKRO_ENABLE
#   define KBD2_HID_DESC_NUM            (MOUSE_HID_DESC_NUM + 1)
#   define KBD2_HID_DESC_OFFSET         (9+(9+9+7)*KBD2_HID_DESC_NUM+9)
#else
#   define KBD2_HID_DESC_NUM            (MOUSE_HID_DESC_NUM + 0)
#endif

#define NUM_INTERFACES                  (KBD2_HID_DESC_NUM + 1)
#define CONFIG1_DESC_SIZE               (9+(9+9+7)*NUM_INTERFACES)

static const uint8_t PROGMEM config1_descriptor[CONFIG1_DESC_SIZE] = {
	// configuration descriptor, USB spec 9.6.3, page 264-266, Table 9-10
	9, 					// bLength;
	2,					// bDescriptorType;
	LSB(CONFIG1_DESC_SIZE),			// wTotalLength
	MSB(CONFIG1_DESC_SIZE),
	NUM_INTERFACES,				// bNumInterfaces
	1,					// bConfigurationValue
	0,					// iConfiguration
	0xA0,					// bmAttributes
	50,					// bMaxPower

	// interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
	9,					// bLength
	4,					// bDescriptorType
	KBD_INTERFACE,				// bInterfaceNumber
	0,					// bAlternateSetting
	1,					// bNumEndpoints
	0x03,					// bInterfaceClass (0x03 = HID)
	0x01,					// bInterfaceSubClass (0x01 = Boot)
	0x01,					// bInterfaceProtocol (0x01 = Keyboard)
	0,					// iInterface
	// HID descriptor, HID 1.11 spec, section 6.2.1
	9,					// bLength
	0x21,					// bDescriptorType
	0x11, 0x01,				// bcdHID
	0,					// bCountryCode
	1,					// bNumDescriptors
	0x22,					// bDescriptorType
	sizeof(keyboard_hid_report_desc),     	// wDescriptorLength
	0,
	// endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
	7,					// bLength
	5,					// bDescriptorType
	KBD_ENDPOINT | 0x80,			// bEndpointAddress
	0x03,					// bmAttributes (0x03=intr)
	KBD_SIZE, 0,				// wMaxPacketSize
	10,					// bInterval

#ifdef MOUSE_ENABLE
	// interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
	9,					// bLength
	4,					// bDescriptorType
	MOUSE_INTERFACE,			// bInterfaceNumber
	0,					// bAlternateSetting
	1,					// bNumEndpoints
	0x03,					// bInterfaceClass (0x03 = HID)
        // ThinkPad T23 BIOS doesn't work with boot mouse.
	0x00,					// bInterfaceSubClass (0x01 = Boot)
	0x02,					// bInterfaceProtocol (0x02 = Mouse)
/*
	0x01,					// bInterfaceSubClass (0x01 = Boot)
	0x02,					// bInterfaceProtocol (0x02 = Mouse)
*/
	0,					// iInterface
	// HID descriptor, HID 1.11 spec, section 6.2.1
	9,					// bLength
	0x21,					// bDescriptorType
	0x11, 0x01,				// bcdHID
	0,					// bCountryCode
	1,					// bNumDescriptors
	0x22,					// bDescriptorType
	sizeof(mouse_hid_report_desc),		// wDescriptorLength
	0,
	// endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
	7,					// bLength
	5,					// bDescriptorType
	MOUSE_ENDPOINT | 0x80,			// bEndpointAddress
	0x03,					// bmAttributes (0x03=intr)
	MOUSE_SIZE, 0,				// wMaxPacketSize
	1,					// bInterval
#endif

#ifdef NKRO_ENABLE
	// interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
	9,					// bLength
	4,					// bDescriptorType
	KBD2_INTERFACE,				// bInterfaceNumber
	0,					// bAlternateSetting
	1,					// bNumEndpoints
	0x03,					// bInterfaceClass (0x03 = HID)
	0x00,					// bInterfaceSubClass (0x01 = Boot)
	0x00,					// bInterfaceProtocol (0x01 = Keyboard)
	0,					// iInterface
	// HID descriptor, HID 1.11 spec, section 6.2.1
	9,					// bLength
	0x21,					// bDescriptorType
	0x11, 0x01,				// bcdHID
	0,					// bCountryCode
	1,					// bNumDescriptors
	0x22,					// bDescriptorType
	sizeof(keyboard2_hid_report_desc),     	// wDescriptorLength
	0,
	// endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
	7,					// bLength
	5,					// bDescriptorType
	KBD2_ENDPOINT | 0x80,			// bEndpointAddress
	0x03,					// bmAttributes (0x03=intr)
	KBD2_SIZE, 0,				// wMaxPacketSize
	1,					// bInterval
#endif
};

// If you're desperate for a little extra code memory, these strings
// can be completely removed if iManufacturer, iProduct, iSerialNumber
// in the device desciptor are changed to zeros.
struct usb_string_descriptor_struct {
	uint8_t bLength;
	uint8_t bDescriptorType;
	int16_t wString[];
};
static const struct usb_string_descriptor_struct PROGMEM string0 = {
	4,
	3,
	{0x0409}
};
static const struct usb_string_descriptor_struct PROGMEM string1 = {
	sizeof(STR_MANUFACTURER),
	3,
	STR_MANUFACTURER
};
static const struct usb_string_descriptor_struct PROGMEM string2 = {
	sizeof(STR_PRODUCT),
	3,
	STR_PRODUCT
};

// This table defines which descriptor data is sent for each specific
// request from the host (in wValue and wIndex).
static const struct descriptor_list_struct {
	uint16_t	wValue;     // descriptor type
	uint16_t	wIndex;
	const uint8_t	*addr;
	uint8_t		length;
} PROGMEM descriptor_list[] = {
        // DEVICE descriptor
	{0x0100, 0x0000, device_descriptor, sizeof(device_descriptor)},
        // CONFIGURATION descriptor
	{0x0200, 0x0000, config1_descriptor, sizeof(config1_descriptor)},
        // HID/REPORT descriptors
	{0x2100, KBD_INTERFACE, config1_descriptor+KBD_HID_DESC_OFFSET, 9},
	{0x2200, KBD_INTERFACE, keyboard_hid_report_desc, sizeof(keyboard_hid_report_desc)},
#ifdef MOUSE_ENABLE
	{0x2100, MOUSE_INTERFACE, config1_descriptor+MOUSE_HID_DESC_OFFSET, 9},
	{0x2200, MOUSE_INTERFACE, mouse_hid_report_desc, sizeof(mouse_hid_report_desc)},
#endif
#ifdef NKRO_ENABLE
	{0x2100, KBD2_INTERFACE, config1_descriptor+KBD2_HID_DESC_OFFSET, 9},
	{0x2200, KBD2_INTERFACE, keyboard2_hid_report_desc, sizeof(keyboard2_hid_report_desc)},
#endif
        // STRING descriptors
	{0x0300, 0x0000, (const uint8_t *)&string0, 4},
	{0x0301, 0x0409, (const uint8_t *)&string1, sizeof(STR_MANUFACTURER)},
	{0x0302, 0x0409, (const uint8_t *)&string2, sizeof(STR_PRODUCT)}
};
#define NUM_DESC_LIST (sizeof(descriptor_list)/sizeof(struct descriptor_list_struct))


/**************************************************************************
 *
 *  Variables - these are the only non-stack RAM usage
 *
 **************************************************************************/

// zero when we are not configured, non-zero when enumerated
static volatile uint8_t usb_configuration=0;

// which modifier keys are currently pressed
// 1=left ctrl,    2=left shift,   4=left alt,    8=left gui
// 16=right ctrl, 32=right shift, 64=right alt, 128=right gui
uint8_t keyboard_modifier_keys=0;

// which keys are currently pressed, up to 6 keys may be down at once
uint8_t keyboard_keys[REPORT_KEYS]; // ={0,0,0,0,0,0};

// protocol setting from the host.  We use exactly the same report
// either way, so this variable only stores the setting since we
// are required to be able to report which setting is in use.
static uint8_t keyboard_protocol=1;

// the idle configuration, how often we send the report to the
// host (ms * 4) even when it hasn't changed
static uint8_t keyboard_idle_config=125;

// count until idle timeout
static uint8_t keyboard_idle_count=0;

// 1=num lock, 2=caps lock, 4=scroll lock, 8=compose, 16=kana
volatile uint8_t keyboard_leds=0;

// nkro, disabled by default
bool keyboard_nkro_enabled=false;

// mouse
uint8_t usb_mouse_protocol=1;
uint8_t mouse_buttons = 0;

/**************************************************************************
 *
 *  Public Functions - these are the API intended for the user
 *
 **************************************************************************/


// initialize USB
void usb_init(void)
{
  uint8_t i;

	HW_CONFIG();
	USB_FREEZE();	// enable USB
	PLL_CONFIG();				// config PLL
  while (!(PLLCSR & (1<<PLOCK))) ;	// wait for PLL lock
  USB_CONFIG();				// start USB clock
  UDCON = 0;				// enable attach resistor
	usb_configuration = 0;
  UDIEN = (1<<EORSTE)|(1<<SOFE);
	sei();

  // clear key buffer
	for (i=0; i<REPORT_KEYS; i++) {
		keyboard_keys[i] = 0;
	}
}

// return 0 if the USB is not configured, or the configuration
// number selected by the HOST
uint8_t usb_configured(void)
{
	return usb_configuration;
}

// toggle nkro
// 1 - nkro enabled, disabled otherwise
void usb_keyboard_nkro_enable(bool status)
{
#ifdef NKRO_ENABLE
  uint8_t i;
  
  keyboard_nkro_enabled = status;

  // clear key buffer
	for (i=0; i<REPORT_KEYS; i++) {
		keyboard_keys[i] = 0;
	}
#endif
}

// send the contents of keyboard_keys and keyboard_modifier_keys
int8_t usb_keyboard_send(void)
{
	uint8_t i, intr_state, timeout, keys_end, endpoint;

	if (!usb_configuration) return -1;

#ifdef NKRO_ENABLE
  if (!keyboard_nkro_enabled) {
    endpoint = KBD_ENDPOINT;
    keys_end = KBD_REPORT_KEYS;
  } else {
    endpoint = KBD2_ENDPOINT;
    keys_end = KBD2_REPORT_KEYS;
  }
#else
	endpoint = KBD_ENDPOINT;
  keys_end = KBD_REPORT_KEYS;
#endif

	intr_state = SREG;
	cli();
	UENUM = endpoint;
	timeout = UDFNUML + 50;
	while (1) {
		// are we ready to transmit?
		if (UEINTX & (1<<RWAL)) break;
		SREG = intr_state;
		// has the USB gone offline?
		if (!usb_configuration) return -1;
		// have we waited too long?
		if (UDFNUML == timeout) return -1;
		// get ready to try checking again
		intr_state = SREG;
		cli();
		UENUM = endpoint;
	}
	UEDATX = keyboard_modifier_keys;
#ifdef NKRO_ENABLE
  if (!keyboard_nkro_enabled) {
	  UEDATX = 0;
  }
#else
  UEDATX = 0;
#endif
	for (i=0; i<keys_end; i++) {
		UEDATX = keyboard_keys[i];
	}
	UEINTX = 0x3A;
	keyboard_idle_count = 0;
	SREG = intr_state;
	return 0;
}

// send mouse movement, buttons
int8_t usb_mouse_send(int8_t x, int8_t y, int8_t wheel_v, int8_t wheel_h, uint8_t buttons)
{
	uint8_t intr_state, timeout;

  mouse_buttons = buttons;

	if (!usb_configured()) return -1;
	if (x == -128) x = -127;
	if (y == -128) y = -127;
	if (wheel_v == -128) wheel_v = -127;
	if (wheel_h == -128) wheel_h = -127;
	intr_state = SREG;
	cli();
	UENUM = MOUSE_ENDPOINT;
	timeout = UDFNUML + 50;
	while (1) {
		// are we ready to transmit?
		if (UEINTX & (1<<RWAL)) break;
		SREG = intr_state;
		// has the USB gone offline?
		if (!usb_configured()) return -1;
		// have we waited too long?
		if (UDFNUML == timeout) return -1;
		// get ready to try checking again
		intr_state = SREG;
		cli();
		UENUM = MOUSE_ENDPOINT;
	}
	UEDATX = buttons;
	UEDATX = x;
	UEDATX = y;
        if (usb_mouse_protocol) {
            UEDATX = wheel_v;
            UEDATX = wheel_h;
        }
        
	UEINTX = 0x3A;
	SREG = intr_state;
	return 0;
}

void usb_mouse_buttons(uint8_t buttons)
{
  usb_mouse_send(0, 0, 0, 0, buttons);
}

/**************************************************************************
 *
 *  Private Functions - not intended for general user consumption....
 *
 **************************************************************************/



// USB Device Interrupt - handle all device-level events
// the transmit buffer flushing is triggered by the start of frame
//
ISR(USB_GEN_vect)
{
	uint8_t intbits, i;
	static uint8_t div4=0;

        intbits = UDINT;
        UDINT = 0;
        if (intbits & (1<<EORSTI)) {
		UENUM = 0;
		UECONX = 1;
		UECFG0X = EP_TYPE_CONTROL;
		UECFG1X = EP_SIZE(ENDPOINT0_SIZE) | EP_SINGLE_BUFFER;
		UEIENX = (1<<RXSTPE);
		usb_configuration = 0;
        }
	if ((intbits & (1<<SOFI)) && usb_configuration) {
#ifdef NKRO_ENABLE
		if (!keyboard_nkro_enabled && keyboard_idle_config && (++div4 & 3) == 0) {
#else
		if (keyboard_idle_config && (++div4 & 3) == 0) {
#endif
			UENUM = KBD_ENDPOINT;
			if (UEINTX & (1<<RWAL)) {
				keyboard_idle_count++;
				if (keyboard_idle_count == keyboard_idle_config) {
					keyboard_idle_count = 0;
					UEDATX = keyboard_modifier_keys;
					UEDATX = 0;
					for (i=0; i<6; i++) {
						UEDATX = keyboard_keys[i];
					}
					UEINTX = 0x3A;
				}
			}
		}
	}
}



// Misc functions to wait for ready and send/receive packets
static inline void usb_wait_in_ready(void)
{
	while (!(UEINTX & (1<<TXINI))) ;
}
static inline void usb_send_in(void)
{
	UEINTX = ~(1<<TXINI);
}
static inline void usb_wait_receive_out(void)
{
	while (!(UEINTX & (1<<RXOUTI))) ;
}
static inline void usb_ack_out(void)
{
	UEINTX = ~(1<<RXOUTI);
}



// USB Endpoint Interrupt - endpoint 0 is handled here.  The
// other endpoints are manipulated by the user-callable
// functions, and the start-of-frame interrupt.
//
ISR(USB_COM_vect)
{
        uint8_t intbits;
	const uint8_t *list;
        const uint8_t *cfg;
	uint8_t i, n, len, en;
	uint8_t bmRequestType;
	uint8_t bRequest;
	uint16_t wValue;
	uint16_t wIndex;
	uint16_t wLength;
	uint16_t desc_val;
	const uint8_t *desc_addr;
	uint8_t	desc_length;

        UENUM = 0;
	intbits = UEINTX;
        if (intbits & (1<<RXSTPI)) {
                bmRequestType = UEDATX;
                bRequest = UEDATX;
                wValue = UEDATX;
                wValue |= (UEDATX << 8);
                wIndex = UEDATX;
                wIndex |= (UEDATX << 8);
                wLength = UEDATX;
                wLength |= (UEDATX << 8);
                UEINTX = ~((1<<RXSTPI) | (1<<RXOUTI) | (1<<TXINI));
                if (bRequest == GET_DESCRIPTOR) {
			list = (const uint8_t *)descriptor_list;
			for (i=0; ; i++) {
				if (i >= NUM_DESC_LIST) {
					UECONX = (1<<STALLRQ)|(1<<EPEN);  //stall
					return;
				}
				desc_val = pgm_read_word(list);
				if (desc_val != wValue) {
					list += sizeof(struct descriptor_list_struct);
					continue;
				}
				list += 2;
				desc_val = pgm_read_word(list);
				if (desc_val != wIndex) {
					list += sizeof(struct descriptor_list_struct)-2;
					continue;
				}
				list += 2;
				desc_addr = (const uint8_t *)pgm_read_word(list);
				list += 2;
				desc_length = pgm_read_byte(list);
				break;
			}
			len = (wLength < 256) ? wLength : 255;
			if (len > desc_length) len = desc_length;
			do {
				// wait for host ready for IN packet
				do {
					i = UEINTX;
				} while (!(i & ((1<<TXINI)|(1<<RXOUTI))));
				if (i & (1<<RXOUTI)) return;	// abort
				// send IN packet
				n = len < ENDPOINT0_SIZE ? len : ENDPOINT0_SIZE;
				for (i = n; i; i--) {
					UEDATX = pgm_read_byte(desc_addr++);
				}
				len -= n;
				usb_send_in();
			} while (len || n == ENDPOINT0_SIZE);
			return;
                }
		if (bRequest == SET_ADDRESS) {
			usb_send_in();
			usb_wait_in_ready();
			UDADDR = wValue | (1<<ADDEN);
			return;
		}
		if (bRequest == SET_CONFIGURATION && bmRequestType == 0) {
			usb_configuration = wValue;
			usb_send_in();
			cfg = endpoint_config_table;
			for (i=1; i<5; i++) {
				UENUM = i;
				en = pgm_read_byte(cfg++);
				UECONX = en;
				if (en) {
					UECFG0X = pgm_read_byte(cfg++);
					UECFG1X = pgm_read_byte(cfg++);
				}
			}
        		UERST = 0x1E;
        		UERST = 0;
			return;
		}
		if (bRequest == GET_CONFIGURATION && bmRequestType == 0x80) {
			usb_wait_in_ready();
			UEDATX = usb_configuration;
			usb_send_in();
			return;
		}

		if (bRequest == GET_STATUS) {
			usb_wait_in_ready();
			i = 0;
			#ifdef SUPPORT_ENDPOINT_HALT
			if (bmRequestType == 0x82) {
				UENUM = wIndex;
				if (UECONX & (1<<STALLRQ)) i = 1;
				UENUM = 0;
			}
			#endif
			UEDATX = i;
			UEDATX = 0;
			usb_send_in();
			return;
		}
		#ifdef SUPPORT_ENDPOINT_HALT
		if ((bRequest == CLEAR_FEATURE || bRequest == SET_FEATURE)
		  && bmRequestType == 0x02 && wValue == 0) {
			i = wIndex & 0x7F;
			if (i >= 1 && i <= MAX_ENDPOINT) {
				usb_send_in();
				UENUM = i;
				if (bRequest == SET_FEATURE) {
					UECONX = (1<<STALLRQ)|(1<<EPEN);
				} else {
					UECONX = (1<<STALLRQC)|(1<<RSTDT)|(1<<EPEN);
					UERST = (1 << i);
					UERST = 0;
				}
				return;
			}
		}
		#endif
		if (wIndex == KBD_INTERFACE) {
			if (bmRequestType == 0xA1) {
				if (bRequest == HID_GET_REPORT) {
					usb_wait_in_ready();
					UEDATX = keyboard_modifier_keys;
					UEDATX = 0;
					for (i=0; i<6; i++) {
						UEDATX = keyboard_keys[i];
					}
					usb_send_in();
					return;
				}
				if (bRequest == HID_GET_IDLE) {
					usb_wait_in_ready();
					UEDATX = keyboard_idle_config;
					usb_send_in();
					return;
				}
				if (bRequest == HID_GET_PROTOCOL) {
					usb_wait_in_ready();
					UEDATX = keyboard_protocol;
					usb_send_in();
					return;
				}
			}
			if (bmRequestType == 0x21) {
				if (bRequest == HID_SET_REPORT) {
					usb_wait_receive_out();
					keyboard_leds = UEDATX;
					usb_ack_out();
					usb_send_in();
					return;
				}
				if (bRequest == HID_SET_IDLE) {
					keyboard_idle_config = (wValue >> 8);
					keyboard_idle_count = 0;
					usb_send_in();
					return;
				}
				if (bRequest == HID_SET_PROTOCOL) {
					keyboard_protocol = wValue;
					usb_send_in();
					return;
				}
			}
		}
#ifdef MOUSE_ENABLE
		if (wIndex == MOUSE_INTERFACE) {
			if (bmRequestType == 0xA1) {
				if (bRequest == HID_GET_REPORT) {
          if (wValue == HID_REPORT_INPUT) {
	  			  usb_wait_in_ready();
		  	  	UEDATX = mouse_buttons;
			    	UEDATX = 0;
	   			 	UEDATX = 0;
	     			UEDATX = 0;
		  	  	usb_send_in();
		    	  return;
          }
          if (wValue == HID_REPORT_FEATURE) {
			    	usb_wait_in_ready();
		    		UEDATX = 0x05;
	    			usb_send_in();
    				return;
          }
				}
				if (bRequest == HID_GET_PROTOCOL) {
					usb_wait_in_ready();
					UEDATX = usb_mouse_protocol;
					usb_send_in();
					return;
				}
			}
			if (bmRequestType == 0x21) {
				if (bRequest == HID_SET_PROTOCOL) {
					usb_mouse_protocol = wValue;
					usb_send_in();
					return;
				}
			}
		}
#endif
	}
	UECONX = (1<<STALLRQ) | (1<<EPEN);	// stall
}


