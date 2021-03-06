/**
 * common.h
 *
 * created by Kamil Cukrowski
 * under beerware license
 *
 * declaration staged to move elsewhere
 */
#ifndef COMMON_H
#define	COMMON_H

#include <stdint.h>
#include <stdbool.h>
#include <GenericTypeDefs.h>
#include <stdlib.h>

/**
 * from http://stackoverflow.com/questions/4234004/is-maxa-b-defined-in-stdlib-h-or-not:
 *
Any C library which defines a macro named max in its standard headers is broken beyond imagination. Fortunately, an easy workaround if you need to support such platforms is to #undef max (and any other problematic macros it defines) after including the system headers and before any of your own headers/code.

Note that everyone else is saying to wrap your definition in #ifndef max ... #endif. This is not a good idea. Defining max in a system header is an indication that the implementor was incompetent, and it's possible that certain versions of the environment have incorrect macros (for example, ones which do not properly protect arguments with parentheses, but I've even seen a max macro that was incorrectly performing min instead of max at least once in my life!). Just use #undef and be safe.

As for why it's so broken for stdlib.h to define max, the C standard is very specific about what names are reserved for the application and what names are reserved for standard functions and/or internal use by the implementation. There are very good reasons for this. Defining macro names in system headers that could clash with variable/function names used in the application program is dangerous. In the best case it leads to compile-time errors with an obvious cause, but in other cases it can cause very strange behavior that's hard to debug. In any case it makes it very difficult to write portable code because you never know what names will already be taken by the library.
 */
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

#define ARRAY_SIZE(x)                ( sizeof(x) / sizeof((x)[0]) )

// sadly no typeof in xc8
#define MIN(a,b)                     (((a)<(b))?(a):(b))
#define MAX(a,b)                     (((a)>(b))?(a):(b))

#define CONFIG(num)                  (*(const __far unsigned char *)(0x300000+num))
#define  DEVID(num)                  (*(const __far unsigned char *)(0x3ffffe+num))

#pragma warning disable 1404 // __attribute__((unsupported)) -- used mostly for plib functions that i use

/**
 * PORT1 and TRIS1 usage example:
 * #define LIB_PORT A
 * #define LIB_PIN  5
 * TRIS(LIB_PORT, LIB_PIN) = 0;
 * PORT(LIB_PORT, LIB_PIN) = 1;
 *
 * PORT and TRIS usage example:
 * #define LIB_PORTPIN A, 5
 * TRIS(LIB_PORTPIN) = 0;
 * PORT(LIB_PORTPIN) = 1;
 */
#define PORT1(PORTNAME, PINNUMBER)    ( PORT ## PORTNAME ## bits.R ## PORTNAME ## PINNUMBER )
#define TRIS1(PORTNAME, PINNUMBER)    ( TRIS ## PORTNAME ## bits.R ## PORTNAME ## PINNUMBER )
#define LAT1( PORTNAME, PINNUMBER)    (  LAT ## PORTNAME ## bits.R ## PORTNAME ## PINNUMBER )
#define PORT(PORTNAMEPINPORT)         PORT(PORTNAMEPINPORT)
#define TRIS(PORTNAMEPINPORT)         TRIS(PORTNAMEPINPORT)
#define LAT( PORTNAMEPINPORT)         LAT(PORTNAMEPINPORT)

#define PORTPIN_PORT1(PORTNAME, PINNUMBER)  PORTNAME
#define PORTPIN_PORT(PORTNAMEPINPORT)       PORTPIN_PORT1(PORTNAMEPINPORT)
#define PORTPIN_PIN1(PORTNAME, PINNUMBER)   PINNUMBER
#define PORTPIN_PIN(PORTNAMEPINPORT)        PORTPIN_PIN1(PORTNAMEPINPORT)

#define PORT_LOW                     0
#define PORT_HIGH                    1
#define TRIS_OUT                     0
#define TRIS_IN                      1
#define LAT_LOW                      0
#define LAT_HIGH                     1
#define INTERRUPT_FLAG_CLEAR         0
#define INTERRUPT_FLAG_SET           1
#define INTERRUPT_PRIORITY_BIT_LOW   0
#define INTERRUPT_PRIORITY_BIT_HIGH  1

/**
 * PIC18f2550 documentation page 111 to 120:
On a Power-on Reset, RA5 and RA3:RA0
are configured as analog inputs and read
as ‘0’. RA4 is configured as a digital input.

On a Power-on Reset, RB4:RB0 are
configured as analog inputs by default and
read as ‘0’; RB7:RB5 are configured as
digital inputs.
By programming the Configuration bit,
PBADEN (CONFIG3H<1>), RB4:RB0 will
alternatively be configured as digital inputs
on POR

On a Power-on Reset, these pins, except
RC4 and RC5, are configured as digital
inputs. To use pins RC4 and RC5 as digital
inputs, the USB module must be disabled
(UCON<3> = 0) and the on-chip
USB transceiver must be disabled
(UCFG<3> = 1).
 */
#define CONFIG_ALL_PORTS_AS_INPUTS() do{ \
	LATA = PORTA = LATB = PORTB = LATC = PORTC = 0x00; \
	ADCON1 = 0x0f; CMCON = 0x07; /* all A/D Ports as Digital I/O */ \
	/* UCON = 0x00; UCFG = 0x03; *//* disable usb - RC4 and RC5 as digital */ \
	TRISA = TRISB = TRISC = 0xff; \
}while(0)
#define CONFIG_ALL_PORTS_AS_OUTPUTS() do{ \
	LATA = PORTA = LATB = PORTB = LATC = PORTC = 0x00; \
	ADCON1 = 0x0f; CMCON = 0x07; /* all A/D Ports as Digital I/O */ \
	/* UCON = 0x00; UCFG = 0x03; *//* disable usb - RC4 and RC5 as digital */ \
	TRISA = TRISB = TRISC = 0x00; \
}while(0)

#define COMMON_MAIN_PREINIT() do{ \
	/* wait for PLL HS oscillator */ \
	/* while( OSCCONbits.OSTS == 0 ); */ \
	/* set all ports as inputs */ \
	CONFIG_ALL_PORTS_AS_INPUTS(); \
	/* enable priority interrupts */ \
	RCONbits.IPEN = 1; \
	/* enable interrupts */ \
	INTCONbits.GIEH = 1; \
	INTCONbits.GIEL = 1; \
}while(0)


#endif	/* COMMON_H */

