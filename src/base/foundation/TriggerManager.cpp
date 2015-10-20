//$Id$
//------------------------------------------------------------------------------
//                         TriggerManager
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
// number NNG06CA54C
//
// Author: Darrel J. Conway, Thinking Systems, Inc.
// Created: 2009/12/10
//
/**
 * Interface class defining GMAT's TriggerManager interfaces.
 */
//------------------------------------------------------------------------------


#include "TriggerManager.hpp"

TriggerManager::TriggerManager() :
   triggerType       (Gmat::UNKNOWN_OBJECT),
   triggerTypeString ("Unknown")
{
}

TriggerManager::~TriggerManager()
{
}


TriggerManager::TriggerManager(const TriggerManager& tm) :
   triggerType       (tm.triggerType),
   triggerTypeString (tm.triggerTypeString)
{
}


TriggerManager& TriggerManager::operator=(const TriggerManager& tm)
{
   if (this != &tm)
   {
      triggerType       = tm.triggerType;
      triggerTypeString = tm.triggerTypeString;
   }

   return *this;
}


const Integer TriggerManager::GetTriggerType()
{
   return triggerType;
}


const std::string TriggerManager::GetTriggerTypeString()
{
   return triggerTypeString;
}


void TriggerManager::SetObject(GmatBase *obj)
{
}


void TriggerManager::ClearObject(GmatBase* obj)
{
}
