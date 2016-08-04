#ifndef _FB01_H_
#define _FB01_H_
#include <mios32.h>

/*
 * The "base"-Address for each of the FM-Operators
 */

u8 op_base_addr[4] = { 0x68, 0x60, 0x58, 0x50 };

/*
 * We need to store all the info locally, because some parameters are
 * transmitted in pairs. Some of them could be limited.
 */
typedef struct {
  u8 volume;
  u8 response_curb;
  u8 velocity_level;
  u8 level_depth;
  u8 volume_fine;

  u8 multiplier;
  u8 fine;

  u8 attack;
  u8 carrier_or_modulator;

  u8 decay1;
  u8 velocity_attack;

  u8 decay2;

  u8 sustain;
  u8 release;
} operator_t;

operator_t operators[4];

/*
 * The available parameters for each FM operator.
 */
enum OPERATOR_PARMS {
  REG_VOLUME = 0,                                       // 0F 07 to 00 00
  REG_RESPONSE_CURB_A_AND_LEVEL_VEL,                    // - Lin = 61: 00 00
                                                        // - Lin = 63: 00 07
                                                        // + Lin = 61: 00 08
                                                        // + Lin = 63: 00 07
                                                        // - Exp = 61: 00 00
                                                        // - Exp = 63: 00 0F
                                                        // + Exp = 61: 00 08
                                                        // + Exp = 63: 00 0F
  REG_LEVEL_DEPTH_AND_ADJ_VOL,                          // LD = {AV x00:F} {LD x00:F}

  REG_MULTIPLIER_AND_FINE,                              // { MULT 00:0F } { FINE }
  REG_ATTACK_RATE,                                      // 0-15: 0F 00
                                                        // 16-31: 0F 01
  REG_CARRIER_MOD_TOGGLE_AND_DECAY_1_RATE_AND_AR_VEL,   // AA BB
                                                        // BB = 0x07 = OFF
                                                        //      0x0F = ON
                                                        // If Decay 1 > 0xF
                                                        // then BB + 1

  REG_DECAY_2_RATE,                                     // 0-15: 0F 00
                                                        // 16-31: 0F 01
  REG_SUSTAIN_LEVEL_AND_RELEASE_RATE,                   // { RR } { SR } both 0-F
};

/*
 * Response curb types
 */
enum RESPONSE_CURB {
  CURB_LINEAR_NEGATIVE,
  CURB_LINEAR_POSITIVE,
  CURB_EXPONENTIAN_NEGATIVE,
  CURB_EXPONENTIAL_POSITIVE,
};

/*
 * Implemented MIDI CC Controls
 */
enum MIDI_OP_TYPES {
  CHANGE_VOLUME,
  CHANGE_VOLUME_FINE,
  CHANGE_RESPONSE_CURB,
  CHANGE_LEVEL_DEPTH,
  CHANGE_MULTIPLIER,
  CHANGE_FINE,
  CHANGE_ATTACK,
  CHANGE_DECAY2,
  CHANGE_SUSTAIN,
  CHANGE_RELEASE
};

#endif // _FB01_H_
