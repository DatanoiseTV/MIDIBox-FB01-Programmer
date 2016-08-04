/////////////////////////////////////////////////////////////////////////////
// Include files
/////////////////////////////////////////////////////////////////////////////

#include <mios32.h>
#include "app.h"
#include "fb01.h"

// TODO: use malloc function instead of a global array to save RAM
static u8 sysex_buffer[1024];

// SysEx Header for Yamaha FB-01
static const u8 sysex_header[5] = { 0xf0, 0x43, 0x75, 0x00, 0x18 };

/////////////////////////////////////////////////////////////////////////////
// This function sends a SysEx dump of the patch structure
/////////////////////////////////////////////////////////////////////////////
s32 SYSEX_Send(mios32_midi_port_t port, u8 op, u8 cmd, u8 param)
{
  int i;
  int sysex_buffer_ix = 0;

  // send header
  for(i=0; i<sizeof(sysex_header); ++i)
    sysex_buffer[sysex_buffer_ix++] = sysex_header[i];

  int base_addr = op_base_addr[op];

  switch(cmd)
  {
    case CHANGE_VOLUME:
        operators[op].volume = param & 0x7F;
        sysex_buffer[sysex_buffer_ix++] = base_addr + REG_VOLUME;
        sysex_buffer[sysex_buffer_ix++] = (operators[op].volume  & 0x0F);
        sysex_buffer[sysex_buffer_ix++] = ((operators[op].volume  >> 4) & 0x0F);
        sysex_buffer[sysex_buffer_ix++] = 0xF7;
        break;
    case CHANGE_VOLUME_FINE:
        operators[op].volume = param;
        sysex_buffer[sysex_buffer_ix++] = base_addr + REG_LEVEL_DEPTH_AND_ADJ_VOL;
        sysex_buffer[sysex_buffer_ix++] = (operators[op].level_depth  & 0x0F);
        sysex_buffer[sysex_buffer_ix++] = (operators[op].volume_fine  & 0x0F);
        sysex_buffer[sysex_buffer_ix++] = 0xF7;
        break;
    case CHANGE_RESPONSE_CURB:
        operators[op].response_curb = param & 0x4;
        if(operators[op].response_curb == CURB_LINEAR_NEGATIVE)
        {
            // Implement me!
        } else if(operators[op].response_curb == CURB_LINEAR_POSITIVE)
        {
            // Implement me!
        } else if(operators[op].response_curb == CURB_EXPONENTIAN_NEGATIVE)
        {
            // Implement me!
        } else if(operators[op].response_curb == CURB_EXPONENTIAL_POSITIVE)
        {
            // Implement me!
        }
        break;
    case CHANGE_LEVEL_DEPTH:
        operators[op].level_depth = param & 0x0F;
        sysex_buffer[sysex_buffer_ix++] = base_addr + REG_LEVEL_DEPTH_AND_ADJ_VOL;
        sysex_buffer[sysex_buffer_ix++] = (operators[op].level_depth & 0x0F);
        sysex_buffer[sysex_buffer_ix++] = (operators[op].volume_fine & 0x0F);
        sysex_buffer[sysex_buffer_ix++] = 0xF7;
        break;
    case CHANGE_MULTIPLIER:
        operators[op].multiplier = param;
        sysex_buffer[sysex_buffer_ix++] = base_addr + REG_MULTIPLIER_AND_FINE;
        sysex_buffer[sysex_buffer_ix++] = (operators[op].multiplier & 0x0F);
        sysex_buffer[sysex_buffer_ix++] = (operators[op].fine & 0x0F);
        sysex_buffer[sysex_buffer_ix++] = 0xF7;
        break;
    case CHANGE_FINE:
        operators[op].fine = param;
        sysex_buffer[sysex_buffer_ix++] = base_addr + REG_MULTIPLIER_AND_FINE;
        sysex_buffer[sysex_buffer_ix++] = (operators[op].multiplier & 0x0F);
        sysex_buffer[sysex_buffer_ix++] = (operators[op].fine & 0x0F);
        sysex_buffer[sysex_buffer_ix++] = 0xF7;
        break;
    case CHANGE_ATTACK:
        operators[op].attack = param & 0x0F;
        sysex_buffer[sysex_buffer_ix++] = base_addr + REG_ATTACK_RATE;
        sysex_buffer[sysex_buffer_ix++] = (operators[op].attack  & 0x0F);
        sysex_buffer[sysex_buffer_ix++] = ((operators[op].attack  >> 4) & 0x0F);
        sysex_buffer[sysex_buffer_ix++] = 0xF7;
        break;
    case CHANGE_DECAY2:
        operators[op].decay2 = param;
        sysex_buffer[sysex_buffer_ix++] = base_addr + REG_DECAY_2_RATE;
        sysex_buffer[sysex_buffer_ix++] = (operators[op].decay2  & 0x0F);
        sysex_buffer[sysex_buffer_ix++] = ((operators[op].decay2  >> 4) & 0x0F);
        sysex_buffer[sysex_buffer_ix++] = 0xF7;
        break;
    case CHANGE_SUSTAIN:
        operators[op].sustain = param;
        sysex_buffer[sysex_buffer_ix++] = base_addr + REG_SUSTAIN_LEVEL_AND_RELEASE_RATE;
        sysex_buffer[sysex_buffer_ix++] = (operators[op].release & 0x0F);
        sysex_buffer[sysex_buffer_ix++] = (operators[op].sustain & 0x0F);
        sysex_buffer[sysex_buffer_ix++] = 0xF7;
        break;
    case CHANGE_RELEASE:
        operators[op].release = param;
        sysex_buffer[sysex_buffer_ix++] = base_addr + REG_SUSTAIN_LEVEL_AND_RELEASE_RATE;
        sysex_buffer[sysex_buffer_ix++] = (operators[op].release & 0x0F);
        sysex_buffer[sysex_buffer_ix++] = (operators[op].sustain & 0x0F);
        sysex_buffer[sysex_buffer_ix++] = 0xF7;
        break;



  }

  return MIOS32_MIDI_SendSysEx(port, (u8 *)sysex_buffer, sysex_buffer_ix);
}

/////////////////////////////////////////////////////////////////////////////
// This hook is called after startup to initialize the application
/////////////////////////////////////////////////////////////////////////////
void APP_Init(void)
{
  // initialize all LEDs
  MIOS32_BOARD_LED_Init(0xffffffff);
}


/////////////////////////////////////////////////////////////////////////////
// This task is running endless in background
/////////////////////////////////////////////////////////////////////////////
void APP_Background(void)
{
}


/////////////////////////////////////////////////////////////////////////////
// This hook is called each mS from the main task which also handles DIN, ENC
// and AIN events. You could add more jobs here, but they shouldn't consume
// more than 300 uS to ensure the responsiveness of buttons, encoders, pots.
// Alternatively you could create a dedicated task for application specific
// jobs as explained in $MIOS32_PATH/apps/tutorials/006_rtos_tasks
/////////////////////////////////////////////////////////////////////////////
void APP_Tick(void)
{
}


/////////////////////////////////////////////////////////////////////////////
// This hook is called each mS from the MIDI task which checks for incoming
// MIDI events. You could add more MIDI related jobs here, but they shouldn't
// consume more than 300 uS to ensure the responsiveness of incoming MIDI.
/////////////////////////////////////////////////////////////////////////////
void APP_MIDI_Tick(void)
{
}


/////////////////////////////////////////////////////////////////////////////
// This hook is called when a MIDI package has been received
/////////////////////////////////////////////////////////////////////////////
void APP_MIDI_NotifyPackage(mios32_midi_port_t port, mios32_midi_package_t midi_package)
{
  // toggle Status LED on each incoming MIDI package
  MIOS32_BOARD_LED_Set(0x0001, ~MIOS32_BOARD_LED_Get());

  // Just forward Note On and Note Off messages
  if( midi_package.type == NoteOn || midi_package.type == NoteOff )
  {
    MIOS32_MIDI_SendPackage(MIDI_OUTPUT_PORT, midi_package);
  }
  else if( midi_package.type == CC )
  {
    switch(midi_package.cc_number)
    {
      case 50 ... 53: SYSEX_Send(MIDI_OUTPUT_PORT, midi_package.cc_number-50, CHANGE_VOLUME, midi_package.value);
      break;
      case 54 ... 57: SYSEX_Send(MIDI_OUTPUT_PORT, midi_package.cc_number-54, CHANGE_VOLUME_FINE, midi_package.value);
      break;
      case 58 ... 61: SYSEX_Send(MIDI_OUTPUT_PORT, midi_package.cc_number-58, CHANGE_RESPONSE_CURB, midi_package.value);
      break;
      case 62 ... 65: SYSEX_Send(MIDI_OUTPUT_PORT, midi_package.cc_number-62, CHANGE_LEVEL_DEPTH, midi_package.value);
      break;
      case 66 ... 69: SYSEX_Send(MIDI_OUTPUT_PORT, midi_package.cc_number-66, CHANGE_MULTIPLIER, midi_package.value);
      break;
      case 70 ... 73: SYSEX_Send(MIDI_OUTPUT_PORT, midi_package.cc_number-70, CHANGE_FINE, midi_package.value);
      break;
      case 74 ... 77: SYSEX_Send(MIDI_OUTPUT_PORT, midi_package.cc_number-74, CHANGE_ATTACK, midi_package.value);
      break;
      case 78 ... 81: SYSEX_Send(MIDI_OUTPUT_PORT, midi_package.cc_number-78, CHANGE_DECAY2, midi_package.value);
      break;
      case 82 ... 85: SYSEX_Send(MIDI_OUTPUT_PORT, midi_package.cc_number-82, CHANGE_SUSTAIN, midi_package.value);
      break;
      case 86 ... 89: SYSEX_Send(MIDI_OUTPUT_PORT, midi_package.cc_number-86, CHANGE_RELEASE, midi_package.value);
      break;
    }
  }

}


/////////////////////////////////////////////////////////////////////////////
// This hook is called before the shift register chain is scanned
/////////////////////////////////////////////////////////////////////////////
void APP_SRIO_ServicePrepare(void)
{
}


/////////////////////////////////////////////////////////////////////////////
// This hook is called after the shift register chain has been scanned
/////////////////////////////////////////////////////////////////////////////
void APP_SRIO_ServiceFinish(void)
{
}


/////////////////////////////////////////////////////////////////////////////
// This hook is called when a button has been toggled
// pin_value is 1 when button released, and 0 when button pressed
/////////////////////////////////////////////////////////////////////////////
void APP_DIN_NotifyToggle(u32 pin, u32 pin_value)
{
}


/////////////////////////////////////////////////////////////////////////////
// This hook is called when an encoder has been moved
// incrementer is positive when encoder has been turned clockwise, else
// it is negative
/////////////////////////////////////////////////////////////////////////////
void APP_ENC_NotifyChange(u32 encoder, s32 incrementer)
{
}


/////////////////////////////////////////////////////////////////////////////
// This hook is called when a pot has been moved
/////////////////////////////////////////////////////////////////////////////
void APP_AIN_NotifyChange(u32 pin, u32 pin_value)
{
}
