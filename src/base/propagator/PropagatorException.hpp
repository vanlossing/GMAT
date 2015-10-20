//$Id$
//------------------------------------------------------------------------------
//                            PropagatorException
//------------------------------------------------------------------------------
// GMAT: General Mission Analysis Tool
//
// Copyright (c) 2002 - 2015 United States Government as represented by the
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
// number NNG04CI63P
//
// Author: Darrel J. Conway, Thinking Systems, Inc.
// Created: 2003/10/10
//
/**
 * Defines stopping condition exception.
 */
//------------------------------------------------------------------------------
#ifndef PropagatorException_hpp
#define PropagatorException_hpp

#include "BaseException.hpp"

class GMAT_API PropagatorException : public BaseException
{
   public:
      PropagatorException(const std::string& message = "Unspecified",
            // Change to this when the repeated instances are fixed:
   //         Gmat::MessageType mt = Gmat::ERROR_);
            // Change to this if it's problematic:
            Gmat::MessageType mt = Gmat::GENERAL_)
         : BaseException("Propagator Exception: ", message, mt) {};
};

#endif
