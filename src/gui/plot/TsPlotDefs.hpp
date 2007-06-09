// $Header$
//------------------------------------------------------------------------------
//                           MessageInterface
//------------------------------------------------------------------------------
//
// Author: Darrel Conway, Thinking Systems, Inc.
// Created: 2005/05/20
//
// This code is Copyright (c) 2005, Thinking Systems, Inc.
// Licensed under the Academic Free License version 3.0.
/**
 * Definitions used for compatibility issues and other items used in TsPlot.
 */
//------------------------------------------------------------------------------


#ifndef TSPLOTDEFS_HPP_
#define TSPLOTDEFS_HPP_

#include <wx/wx.h>

// The following block manages the cahange in deprecated functions between 
// wx 2.6 and 2.8
#if wxCHECK_VERSION(2, 8, 0)

    #define wxRECT_CONTAINS Contains
    #define wxBEGIN_DRAWING
    #define wxEND_DRAWING  

#else // replacement code for old version
    
    #define wxRECT_CONTAINS Inside
    #define wxBEGIN_DRAWING dc.BeginDrawing();
    #define wxEND_DRAWING   dc.BeginDrawing();

#endif

// VC++ Omits definitions for some math numbers
#ifndef M_PI_2
#define M_PI_2 M_PI/2.0
#endif

#endif /*TSPLOTDEFS_HPP_*/
