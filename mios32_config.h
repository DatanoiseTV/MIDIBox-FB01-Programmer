// $Id: mios32_config.h 682 2009-07-31 21:55:26Z tk $
/*
 * Local MIOS32 configuration file
 *
 * this file allows to disable (or re-configure) default functions of MIOS32
 * available switches are listed in $MIOS32_PATH/modules/mios32/MIOS32_CONFIG.txt
 *
 */

#ifndef _MIOS32_CONFIG_H
#define _MIOS32_CONFIG_H

// The boot message which is print during startup and returned on a SysEx query
#define MIOS32_LCD_BOOT_MSG_LINE1 "Tutorial #004"
#define MIOS32_LCD_BOOT_MSG_LINE2 "(C) 2009 T.Klose"

#define MIDI_OUTPUT_PORT UART1

#endif /* _MIOS32_CONFIG_H */