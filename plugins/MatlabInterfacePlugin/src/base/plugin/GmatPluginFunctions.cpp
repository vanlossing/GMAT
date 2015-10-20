//$Id: GmatPluginFunctions.cpp 9460 2011-04-21 22:03:28Z lindajun $
//------------------------------------------------------------------------------
//                            GmatPluginFunctions
//------------------------------------------------------------------------------
// GMAT: General Mission Analysis Tool
//
// Copyright (c) 2002 - 2015 United States Government as represented by the
// Administrator of The National Aeronautics and Space Administration.
// All Other Rights Reserved.
//
// Developed jointly by NASA/GSFC and Thinking Systems, Inc. under contract
// number NNG06CA54C
//
// Author: Linda Jun (NASA/GSFC)
// Created: 2010/03/30
//
/**
 * Implementation for library code interfaces.
 * 
 * This is prototype code.
 */
//------------------------------------------------------------------------------

#include "GmatPluginFunctions.hpp"
#include "MessageInterface.hpp"

#include "MatlabInterfaceFactory.hpp"
#include "MatlabWorkspaceFactory.hpp"
#include "CallMatlabFunctionFactory.hpp"
#include "MatlabFunctionFactory.hpp"

//#define __INCLUDE_MATLAB_WORKSPACE__

extern "C"
{
   //------------------------------------------------------------------------------
   // Integer GetFactoryCount()
   //------------------------------------------------------------------------------
   /**
    * Returns the number of plug-in factories in this module
    *
    * @return The number of factories
    */
   //------------------------------------------------------------------------------
   Integer GetFactoryCount()
   {
      #ifdef __INCLUDE_MATLAB_WORKSPACE__
      return 4;
      #else
      return 3;
      #endif
   }
   
   //------------------------------------------------------------------------------
   // Factory* GetFactoryPointer(Integer index)
   //------------------------------------------------------------------------------
   /**
    * Retrieves a pointer to a specific factory
    *
    * @param index The index to the Factory
    *
    * @return The Factory pointer
    */
   //------------------------------------------------------------------------------
   Factory* GetFactoryPointer(Integer index)
   {
      Factory* factory = NULL;

      switch (index)
      {
         case 0:
            factory = new MatlabInterfaceFactory;
            break;
            
         case 1:
            factory = new CallMatlabFunctionFactory;
            break;
            
         case 2:
            factory = new MatlabFunctionFactory;
            break;

         #ifdef __INCLUDE_MATLAB_WORKSPACE__
         case 3:
            factory = new MatlabWorkspaceFactory;
            break;
         #endif
            
         default:
            break;
      }

      return factory;
   }
   
   //------------------------------------------------------------------------------
   // void SetMessageReceiver(MessageReceiver* mr)
   //------------------------------------------------------------------------------
   /**
    * Sets the messaging interface used for GMAT messages
    *
    * @param mr The message receiver
    */
   //------------------------------------------------------------------------------
   void SetMessageReceiver(MessageReceiver* mr)
   {
      MessageInterface::SetMessageReceiver(mr);
   }
};
