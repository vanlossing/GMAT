//------------------------------------------------------------------------------
//                              Norm
//------------------------------------------------------------------------------
// GMAT: General Mission Analysis Tool
//
// Copyright (c) 2002 - 2020 United States Government as represented by the
// Administrator of the National Aeronautics and Space Administration.
// All Other Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License"); 
// You may not use this file except in compliance with the License. 
// You may obtain a copy of the License at:
// http://www.apache.org/licenses/LICENSE-2.0. 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either 
// express or implied.   See the License for the specific language
// governing permissions and limitations under the License.
//
// Developed jointly by NASA/GSFC and Thinking Systems, Inc. under contract
// number S-67573-G
//
// Author: Allison Greene
// Created: 2006/04/20
//
/**
 * Declares Norm class.
 */
//------------------------------------------------------------------------------
#ifndef Norm_hpp
#define Norm_hpp

#include "MathFunction.hpp"

class GMAT_API Norm : public MathFunction
{
public:
   Norm(const std::string &nomme);
   virtual ~Norm();
   Norm(const Norm &copy);
                 
   // inherited from GmatBase
   virtual GmatBase* Clone() const;
   
   virtual void GetOutputInfo(Integer &type, Integer &rowCount, Integer &colCount);
   virtual bool ValidateInputs(); 
   virtual Real Evaluate();
//    virtual Rmatrix MatrixEvaluate();

protected:

  
private:
   
};

#endif // Norm_hpp
