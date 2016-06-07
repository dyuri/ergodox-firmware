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
// Apple-specific keycode events found at
// /System/Library/Frameworks/Carbon.framework/Versions/A/Frameworks/HIToolbox.framework/Versions/A/Headers/Events.h

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
usb__kb__set_key(true, KEYBOARD__v_V); usb__kb__send_report();

usb__kb__set_key(false, KEYBOARD__v_V);
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


// Browser refresh
void P(m_BrowserRefresh)(void){
  usb__kb__set_key(true, KEYBOARD__LeftGUI); usb__kb__send_report(); 
  usb__kb__set_key(true, KEYBOARD__LeftShift); usb__kb__send_report(); 
  usb__kb__set_key(true, KEYBOARD__r_R); usb__kb__send_report();
  usb__kb__set_key(false, KEYBOARD__r_R); 
  usb__kb__set_key(false, KEYBOARD__LeftShift);
  usb__kb__set_key(false, KEYBOARD__LeftGUI);
}
void R(m_BrowserRefresh)(void){}

// mac restart
void P(mac_restart)(void){
  usb__kb__set_key(true, KEYBOARD__LeftGUI); usb__kb__send_report(); 
  usb__kb__set_key(true, KEYBOARD__LeftControl ); usb__kb__send_report();
  usb__kb__set_key(true, KEYBOARD__Power ); usb__kb__send_report();
  usb__kb__set_key(false, KEYBOARD__LeftGUI); 
  usb__kb__set_key(false, KEYBOARD__LeftControl ); 
  usb__kb__set_key(false, KEYBOARD__Power );
}
void R(mac_restart)(void){}


// forward one word in mac terminal
void P(fwd_terminal)(void){
  usb__kb__set_key(true, KEYBOARD__Escape); usb__kb__send_report();
  usb__kb__set_key(true, KEYBOARD__f_F); usb__kb__send_report();
  usb__kb__set_key(false, KEYBOARD__f_F); 
  usb__kb__set_key(false, KEYBOARD__Escape);
}
void R(fwd_terminal)(void){}

//back one word in mac terminal
void P(back_terminal)(void){
  usb__kb__set_key(true, KEYBOARD__Escape); usb__kb__send_report();
  usb__kb__set_key(true, KEYBOARD__b_B); usb__kb__send_report();
  usb__kb__set_key(false, KEYBOARD__b_B); 
  usb__kb__set_key(false, KEYBOARD__Escape);
}
void R(back_terminal)(void){}

//forward one word ipythong
void P(fwd_ipy)(void){
  usb__kb__set_key(true, KEYBOARD__LeftAlt); usb__kb__send_report(); 
  usb__kb__set_key(true, KEYBOARD__LeftShift); usb__kb__send_report();
  usb__kb__set_key(true, KEYBOARD__f_F); usb__kb__send_report();
  usb__kb__set_key(false, KEYBOARD__f_F); 
  usb__kb__set_key(false, KEYBOARD__LeftShift); 
  usb__kb__set_key(false, KEYBOARD__LeftAlt); 
}
void R(fwd_ipy)(void){}

//back one word in ipython
void P(back_ipy)(void){
  usb__kb__set_key(true, KEYBOARD__RightAlt); usb__kb__send_report(); 
  usb__kb__set_key(true, KEYBOARD__RightShift); usb__kb__send_report();
  usb__kb__set_key(true, KEYBOARD__RightGUI); usb__kb__send_report();
  usb__kb__set_key(true, KEYBOARD__b_B); usb__kb__send_report();
  usb__kb__set_key(false, KEYBOARD__b_B);
  usb__kb__set_key(false, KEYBOARD__RightGUI); 
  usb__kb__set_key(false, KEYBOARD__RightShift); 
  usb__kb__set_key(false, KEYBOARD__RightAlt); 
}
void R(back_ipy)(void){}

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
    esc,       1,        2,        3,        4,        5,     m_screenCap,
    pound,      q,       w,        e,        r,        t,     brktL,
    tab,        a,       s,        d,        f,        g,
    shL2kcap,   z,       x,        c,        v,        b,     parenL,
    lpu2l2,    grave,  ctrlL,   altL,   guiL,
                                                               back_terminal,     fwd_terminal,
                                                       nop,      nop,     arrowL,
                                                      enter,      bs,      arrowR,
// right hand ..... ......... ......... ......... ......... ......... .........
            m_BrowserRefresh,     6,        7,        8,        9,        0,       m_paste,
            brktR,                y,        u,        i,        o,        p,       dash,
                                  h,        j,        k,        l,       semicol,  quote,
            parenR,               n,        m,       comma,     period,  slash,    shR2kcap,
                                undersc,   equal,    altR,   ctrlR,     lpu1l1,
    mute,    bkslash,
   arrowU,      nop,      nop,
   arrowD,    tab,    space  ),

// ............................................................................

    MATRIX_LAYER(  // layer 1 : number pad
// macro, unused,
       K,    nop,
// left hand ...... ......... ......... ......... ......... ......... .........
  lpo1l1,   F1,        F2,      F3,       F4,       F5,       transp,
  transp,   F11,       F12,     F13,      F14,      F15,      transp,
  transp,   transp,   transp,   transp,   transp,   transp,
  transp,   transp,   transp,   transp,   transp,   transp,   transp,
  transp,   transp,   transp,   transp,   transp,
                                                              transp,   transp,
                                                    transp,   transp,   transp,
                                                    MclkL,   transp,   transp,
// right hand ..... ......... ......... ......... ......... ......... .........
            transp,   F6,           F7,            F8,             F9,     F10,     pageU,
            transp,   transp,        7,              8,              9,     transp,   pageD,
                      arrowL,       4,              5,              6,     transp,   home,
            transp,   transp,        1,             2,              3,     transp,   end,
                                period,   0,   transp,    transp,   lpo1l1,
  transp,   transp,
  transp,   transp,   transp,
  transp,   transp,    enter  ),

// ............................................................................

    MATRIX_LAYER(  // layer 2 : hardware control + left hand layer
// macro, unused,
       K,    nop,
// left hand ...... ......... ......... ......... ......... ......... .........
  lpo2l2,   transp,  transp,    transp,   transp,  transp,  btldr,
  power,   transp,   transp,    transp,    transp, transp,   transp,
  mac_restart,  transp,    transp,     transp,   transp,  transp,
  transp,  transp,    transp,     transp,   transp, transp, transp,
  lpo2l2,   transp,   transp,   transp,   transp,
                                                              back_ipy,   fwd_ipy,
                                                    transp,   transp,   transp,
                                                    transp,   transp,   transp,
// right hand ..... ......... ......... ......... ......... ......... .........
              transp, transp,   transp,  transp,    transp,   dmp_sram,    power,
              transp,  transp,  transp,  transp,    transp,   dmp_prog,  volumeU,
                      transp,   transp,  transp,    transp,   dmp_eepr,  volumeD,
              transp, transp,   transp,   transp,   transp,   transp,     mute,
                                transp,   transp,   transp,   transp,   transp,
  transp,   transp,
  transp,   transp,   transp,
  transp,   transp,   transp  ),

// ............................................................................

    MATRIX_LAYER(  // layer 3 : TBD
// macro, unused,
       K,    nop,
// left hand ...... ......... ......... ......... ......... ......... .........
     nop,      nop,      nop,      nop,      nop,      nop,      nop,
     nop,      nop,      nop,      nop,      nop,      nop,      nop,
     nop,      nop,      nop,      nop,      nop,      nop,
     nop,      nop,      nop,      nop,      nop,      nop,      nop,
     nop,      nop,      nop,      nop,      nop,
                                                                 nop,      nop,
                                                       nop,      nop,      nop,
                                                       nop,      nop,      nop,
// right hand ..... ......... ......... ......... ......... ......... .........
               nop,      nop,      nop,      nop,      nop,      nop,      nop,
               nop,      nop,      nop,      nop,      nop,      nop,      nop,
                         nop,      nop,      nop,      nop,      nop,      nop,
               nop,      nop,      nop,      nop,      nop,      nop,      nop,
                                   nop,      nop,      nop,      nop,      nop,
     nop,      nop,
     nop,      nop,      nop,
     nop,      nop,      nop  ),

// ............................................................................
};

