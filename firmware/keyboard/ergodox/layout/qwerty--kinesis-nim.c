/* ----------------------------------------------------------------------------
 * Copyright (c) 2013, 2014 Ben Blazak <benblazak.dev@gmail.com>
 * Released under The MIT License (see "doc/licenses/MIT.md")
 * Project located at <https://github.com/benblazak/ergodox-firmware>
 * ------------------------------------------------------------------------- */

/**                                                                 description
 * A layout with the home layer adapted from the default Kinesis layout
 * (staying as close the original as possible).  The position of the symbol
 * keys on the function layer was (roughly) taken from the Arensito layout.
 *
 * Implements the "layout" section of '.../firmware/keyboard.h'
 *
 * The template key prefix is ``, with the rest of the name indicating the
 * key's position in the QWERTY layout.
 */


#include "./fragments/includes.part.h"
#include "./fragments/macros.part.h"
#include "./fragments/types.part.h"
#include "./fragments/variables.part.h"


// ----------------------------------------------------------------------------
// keys
// ----------------------------------------------------------------------------

#include "./fragments/keys.part.h"


// ----------------------------------------------------------------------------
// LED control
// ----------------------------------------------------------------------------

#include "./fragments/led-control.part.h"


// ----------------------------------------------------------------------------
// matrix control
// ----------------------------------------------------------------------------

#include "./fragments/matrix-control.part.h"



// Extra keynames

// ----------------------------------------------------------------------------
// custom
// ----------------------------------------------------------------------------

// Send a Cut (command-X), (mac)
void keys__press__m_cut(void) { 
usb__kb__set_key(true, KEYBOARD__LeftGUI); usb__kb__send_report();
usb__kb__set_key(true, KEYBOARD__x_X); usb__kb__send_report();
usb__kb__set_key(false, KEYBOARD__x_X);
usb__kb__set_key(false, KEYBOARD__LeftGUI);
}
void R(m_cut)(void) {}

// Send a Copy (command-C), (mac)
void keys__press__m_copy(void) { 
usb__kb__set_key(true, KEYBOARD__LeftGUI); usb__kb__send_report();
usb__kb__set_key(true, KEYBOARD__c_C); usb__kb__send_report();
usb__kb__set_key(false, KEYBOARD__c_C);
usb__kb__set_key(false, KEYBOARD__LeftGUI);
}
void R(m_copy)(void) {}

// Send a Paste without text formatting (shift-alt-command-V), (mac)
void keys__press__m_paste(void) { 
usb__kb__set_key(true, KEYBOARD__LeftGUI); usb__kb__send_report();
usb__kb__set_key(true, KEYBOARD__LeftShift); usb__kb__send_report();
usb__kb__set_key(true, KEYBOARD__LeftAlt); usb__kb__send_report();
usb__kb__set_key(true, KEYBOARD__v_V); usb__kb__send_report();

usb__kb__set_key(false, KEYBOARD__v_V);
usb__kb__set_key(false, KEYBOARD__LeftAlt);
usb__kb__set_key(false, KEYBOARD__LeftShift);
usb__kb__set_key(false, KEYBOARD__LeftGUI);
}
void R(m_paste)(void) {}

// Send a Screencap Snapshot (Command Shift 4)
void keys__press__m_screenCap(void) { 
usb__kb__set_key(true, KEYBOARD__LeftGUI); usb__kb__send_report();
usb__kb__set_key(true, KEYBOARD__LeftShift); usb__kb__send_report(); 
usb__kb__set_key(true, KEYBOARD__4_Dollar ); usb__kb__send_report();
//_delay_ms(333);
usb__kb__set_key(false, KEYBOARD__4_Dollar);
usb__kb__set_key(false, KEYBOARD__LeftShift);
usb__kb__set_key(false, KEYBOARD__LeftGUI);
}
void R(m_screenCap)(void) {}



// type a :-), to demonstrate how to do a macro
 void P(m_smiley)(void) { KF(type_string)( PSTR(":-)") ); }
 void R(m_smiley)(void) {}

// send a mouse click
// defined in firmware/keyboard/ergodox/layout/common/keys.c.h
// void P(MclkL) (void) { KF(mouse_buttons)(1, 0, 0); }
// void R(MclkL) (void) { KF(mouse_buttons)(0, 0, 0); }

void P(Mup10)(void){ KF(mouse_direction)(0,10);}
void R(Mup10)(void){ KF(mouse_stop)();} 
// ----------------------------------------------------------------------------
// layout
// ----------------------------------------------------------------------------

static layout_t layout PROGMEM = {
// ............................................................................

    MATRIX_LAYER(  // layer 0 : default
// macro, unused,
       K,    nop,
// left hand ...... ......... ......... ......... ......... ......... .........
    esc,       1,        2,        3,        4,        5,      brktL,
    ins,      q,      w,      e,      r,      t,   tab,
    del,      a,      s,      d,      f,      g,
shL2kcap,      z,      x,      c,      v,      b, parenL,
    lpu1l1,    grave,  ctrlL,   altL,   guiL,
                                                               volumeU,     mute,
                                                       nop,      nop,     tab,
                                                      enter,      bs,      slash,
// right hand ..... ......... ......... ......... ......... ......... .........
            brktR,        6,        7,        8,        9,        0,     pageU,
            dash,         y,      u,      i,      o,      p,                 pageD,
                         h,      j,      k,      l,       semicol,  home,
          parenR,      n,      m,  comma, period,  bkslash, end,
                                guiR,   altR,    ctrlR,    plus,     lpu2l2,
    volumeD,    mute,
   quote,      nop,      nop,
   period,    shR2kcap,    space  ),

// ............................................................................

    MATRIX_LAYER(  // layer 1 : number pad
// macro, unused,
       K,    nop,
// left hand ...... ......... ......... ......... ......... ......... .........
  lpo1l1,   F1,   F2,   F3,   F4,   F5,   MclkL,
  transp,   F11,   F12,   F13,   F14,   F15,   transp,
  transp,   F21,   F22,   F23,   F24,   transp,
  transp,   transp,   transp,   transp,   transp,   transp,   transp,
  lpo1l1,      ins,   transp,   transp,   transp,
                                                              transp,   transp,
                                                    transp,   transp,   transp,
                                                    transp,   transp,   transp,
// right hand ..... ......... ......... ......... ......... ......... .........
            transp,   F6,   F7,    F8,    F9, F10,   btldr,
            transp,   F16,        F17,        F18,        F19,     F20,   power,
                      arrowL,        arrowD,        arrowU,        arrowR,     transp,   transp,
            transp,   transp,        transp,        transp,        transp,    transp,   transp,
                                transp,   transp,   period,    transp,   transp,
  transp,   transp,
  transp,   transp,   transp,
  transp,   transp,        0  ),

// ............................................................................

    MATRIX_LAYER(  // layer 2 : symbols and function keys
// macro, unused,
       K,    nop,
// left hand ...... ......... ......... ......... ......... ......... .........
  transp,       F1,       F2,       F3,       F4,       F5,      F11,
  transp,   braceL,   braceR,    brktL,    brktR,      nop,   lpo2l2,
  transp,  semicol,    slash,     dash,        0,    colon,
  transp,        6,        7,        8,        9,     plus, lpupo3l3,
  transp,   transp,   transp,   transp,   transp,
                                                              transp,   transp,
                                                    transp,   transp,   transp,
                                                    transp,   transp,   transp,
// right hand ..... ......... ......... ......... ......... ......... .........
               F12,       F6,       F7,       F8,       F9,      F10,    power,
            lpo2l2,      nop,  undersc, lessThan, grtrThan,   dollar,  volumeU,
                     bkslash,        1,   parenL,   parenR,    equal,  volumeD,
          lpupo3l3, asterisk,        2,        3,        4,        5,     mute,
                                transp,   transp,   transp,   transp,   transp,
  transp,   transp,
  transp,   transp,   transp,
  transp,   transp,   transp  ),

// ............................................................................

    MATRIX_LAYER(  // layer 3 : keyboard functions
// macro, unused,
       K,    nop,
// left hand ...... ......... ......... ......... ......... ......... .........
   btldr,      nop,      nop,      nop,      nop,      nop,      nop,
     nop,      nop,      nop,      nop,      nop,      nop,      nop,
     nop,      nop,      nop,      nop,      nop,      nop,
     nop,      nop,      nop,      nop,      nop,      nop,      nop,
     nop,      nop,      nop,      nop,      nop,
                                                                 nop,      nop,
                                                       nop,      nop,      nop,
                                                       nop,      nop,      nop,
// right hand ..... ......... ......... ......... ......... ......... .........
               nop,      nop,      nop,      nop,      nop,      nop, dmp_sram,
               nop,      nop,      nop,      nop,      nop,      nop, dmp_prog,
                         nop,      nop,      nop,      nop,      nop, dmp_eepr,
               nop,      nop,      nop,      nop,      nop,      nop,      nop,
                                   nop,      nop,      nop,      nop,      nop,
     nop,      nop,
     nop,      nop,      nop,
     nop,      nop,      nop  ),

// ............................................................................
};

