/* ----------------------------------------------------------------------------
 * Copyright (c) 2013 Ben Blazak <benblazak.dev@gmail.com>
 * Released under The MIT License (see "doc/licenses/MIT.md")
 * Project located at <https://github.com/benblazak/ergodox-firmware>
 * ------------------------------------------------------------------------- */

/**                                                                 description
 * A Dvorak layout adapted from the default Kinesis layout.  The position of
 * the symbol keys on the function layer was (roughly) taken from the Arensito
 * layout.
 *
 * Implements the "layout" section of '.../firmware/keyboard.h'
 */


#include "./common/definitions.h"

// ----------------------------------------------------------------------------
// matrix control
// ----------------------------------------------------------------------------

#include "./common/exec_key.c.h"


// ----------------------------------------------------------------------------
// LED control
// ----------------------------------------------------------------------------

void kb__led__logical_on(char led) {
    switch(led) {
        case 'N': kb__led__on(1); break;  // numlock
        case 'C': kb__led__on(2); break;  // capslock
        case 'S': kb__led__on(3); break;  // scroll lock
        case 'O':                 break;  // compose
        case 'K':                 break;  // kana
    };
}

void kb__led__logical_off(char led) {
    switch(led) {
        case 'N': kb__led__off(1); break;  // numlock
        case 'C': kb__led__off(2); break;  // capslock
        case 'S': kb__led__off(3); break;  // scroll lock
        case 'O':                  break;  // compose
        case 'K':                  break;  // kana
    };
}


// ----------------------------------------------------------------------------
// keys
// ----------------------------------------------------------------------------

#include "./common/keys.c.h"

KEYS__LAYER__NUM_PUSH(10, 3);
KEYS__LAYER__NUM_POP(10);


// ----------------------------------------------------------------------------
// layout
// ----------------------------------------------------------------------------

#include "./common/matrix.h"

// TODO
// - tnkro
// - mouse layer

static _layout_t _layout = {

// ............................................................................

    MATRIX_LAYER(  // layer 0 : default
// macro, unused,
       K,    nop,
// left hand ...... ......... ......... ......... ......... ......... .........
     esc,        1,        2,        3,        4,        5,   lpu2l2,
     tab,    quote,    comma,   period,        p,        y, lpupo2l2,
    altR,        a,        o,        e,        u,        i,
shL2kcap,  semicol,        q,        j,        k,        x, lpupo1l1,
   ctrlL,    grave,  bkslash,    brktL,    brktR,
                                                                guiL,     altL,
                                                       nop,      nop,     home,
                                                     space,       bs,      end,
// right hand ..... ......... ......... ......... ......... ......... .........
            lpu1l1,        6,        7,        8,        9,        0,     equal,
          lpupo2l2,        f,        g,        c,        r,        l,    slash,
                           d,        h,        t,        n,        s,     dash,
          lpupo1l1,        b,        m,        w,        v,        z, shR2kcap,
                                arrowL,   arrowD,   arrowU,   arrowR,    ctrlR,
    altR,     guiR,
   pageU,      nop,      nop,
   pageD,    enter,    space  ),

// ............................................................................

    MATRIX_LAYER(  // layer 1 : function and symbol keys
// macro, unused,
       K,    nop,
// left hand ...... ......... ......... ......... ......... ......... .........
    menu,       F1,       F2,       F3,       F4,       F5,      esc,
    scrl,    prScr,    pause,     stop,      nop,      nop,   lreset,
     num,      nop,  volumeU,      ins,     home,    pageU,
    caps,     mute,  volumeD,      del,      end,    pageD, lpupo3l3,
  transp,   transp,   transp,   transp,   transp,
                                                              transp,   transp,
                                                    transp,   transp,   transp,
                                                    transp,   transp,   transp,
// right hand ..... ......... ......... ......... ......... ......... .........
            lpo1l1,       F6,       F7,       F8,       F9,      F10,      F11,
            lreset,      kp7,      kp8,      kp9,    kpDiv,    kpMul,      F12,
                         kp4,      kp5,      kp6,    kpSub,    kpAdd,  kpEnter,
          lpupo2l2,      kp1,      kp2,      kp3,    space,   arrowU,   transp,
                                   kp0,    kpDec,   arrowL,   arrowD,   arrowR,
  transp,   transp,
  transp,   transp,   transp,
  transp,   transp,      kp0  ),

// ............................................................................

    MATRIX_LAYER(  // layer 2 : keyboard functions
// macro, unused,
       K,    nop,
// left hand ...... ......... ......... ......... ......... ......... .........
   btldr,      nop,      nop,      nop,      nop,      nop,   lreset,
     nop,      nop,      nop,      nop,      nop,      nop,   lreset,
     nop,      nop,      nop,      nop,      nop,      nop,
     nop,      nop,      nop,      nop,      nop,      nop,   lreset,
     nop,      nop,      nop,      nop,      nop,
                                                                 nop,      nop,
                                                       nop,      nop,      nop,
                                                       nop,      nop,      nop,
// right hand ..... ......... ......... ......... ......... ......... .........
            lreset,      nop,      nop,      nop,      nop,      nop,    tnkro,
            lreset,      nop,      nop,      nop,      nop,      nop,      nop,
                         nop,      nop,      nop,      nop,      nop,      nop,
            lreset,      nop,      nop,      nop,      nop,      nop,      nop,
                                   nop,      nop,      nop,      nop,      nop,
     nop,      nop,
     nop,      nop,      nop,
     nop,      nop,      nop  ),

// ............................................................................

    MATRIX_LAYER(  // layer 3 : numpad
// macro, unused,
       K,    nop,
// left hand ...... ......... ......... ......... ......... ......... .........
  transp,   transp,   transp,   transp,   transp,   transp,   transp,
  transp,   transp,   transp,   transp,   transp,   transp,   transp,
  transp,   transp,   transp,   transp,   transp,   transp,
  transp,   transp,   transp,   transp,   transp,   transp,   transp,
  transp,      ins,   transp,   transp,   transp,
                                                              transp,   transp,
                                                    transp,   transp,   transp,
                                                    transp,   transp,   transp,
// right hand ..... ......... ......... ......... ......... ......... .........
            numPop,   transp,   numPop,    equal,    kpDiv,    kpMul,   transp,
            transp,   transp,      kp7,      kp8,      kp9,    kpSub,   transp,
                      transp,      kp4,      kp5,      kp6,    kpAdd,   transp,
            transp,   transp,      kp1,      kp2,      kp3,  kpEnter,   transp,
                                transp,   transp,   period,  kpEnter,   transp,
  transp,   transp,
  transp,   transp,   transp,
  transp,   transp,      kp0  ),

// ............................................................................
};

