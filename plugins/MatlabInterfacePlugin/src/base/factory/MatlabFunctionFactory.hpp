//$Id: MatlabFunctionFactory.hpp 9513 2011-04-30 21:23:06Z djcinsb $
//------------------------------------------------------------------------------
//                              MatlabFunctionFactory
//------------------------------------------------------------------------------
// GMAT: General Mission Analysis Tool
//
// Copyright (c) 2002 - 2015 United States Government as represented by the
// Administrator of The National Aeronautics and Space Administration.
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
// number NNG04CC06P.
//
// Author: Allison Greene
// Created: 2004/9/22
//
/**
 * Implementation code for the MatlabFunctionFactory class, responsible for
 * creating MATLAB Function objects.
 */
//------------------------------------------------------------------------------


#ifndef MatlabFunctionFactory_hpp
#define MatlabFunctionFactory_hpp

#include "matlabinterface_defs.hpp"
#include "Factory.hpp"
#include "Function.hpp"

class MATLAB_API MatlabFunctionFactory : public Factory
{
public:
   Function* CreateFunction(const std::string &ofType, const std::string &withName = "");

   MatlabFunctionFactory();
   virtual ~MatlabFunctionFactory();
   MatlabFunctionFactory(StringArray createList);
   MatlabFunctionFactory(const MatlabFunctionFactory& fact);
   MatlabFunctionFactory& operator= (const MatlabFunctionFactory& fact);

};


#endif // MatlabFunctionFactory_hpp
