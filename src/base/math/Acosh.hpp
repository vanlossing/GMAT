//$Id$
//------------------------------------------------------------------------------
//                              Acosh
//------------------------------------------------------------------------------
// GMAT: General Mission Analysis Tool
//
// Copyright (c) 2002 - 2015 United States Government as represented by the
// Administrator of the National Aeronautics and Space Administration.
// All Other Rights Reserved.
//
// Developed jointly by NASA/GSFC and Thinking Systems, Inc. under contract
// number S-67573-G
//
// Author: Allison Greene
// Created: 2006/06/27
//
/**
 * Declares Acosh class.
 */
//------------------------------------------------------------------------------
#ifndef Acosh_hpp
#define Acosh_hpp

#include "MathFunction.hpp"
#include "RealUtilities.hpp"

class GMAT_API Acosh : public MathFunction
{
public:
   Acosh(const std::string &nomme);
   virtual ~Acosh();
   Acosh(const Acosh &copy);
                 
   // inherited from GmatBase
   virtual GmatBase* Clone() const;

   virtual void GetOutputInfo(Integer &type, Integer &rowCount, Integer &colCount);
   virtual bool ValidateInputs(); 
   virtual Real Evaluate();
};

#endif // Acosh_hpp
