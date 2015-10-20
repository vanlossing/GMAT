//$Id$
//------------------------------------------------------------------------------
//                         ClassName
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
// Created: 2009/ /
//
/**
 * File description here.
 */
//------------------------------------------------------------------------------


#ifndef RunSimulator_hpp
#define RunSimulator_hpp

#include "RunSolver.hpp"

#include "PropSetup.hpp"
#include "Simulator.hpp"

/**
 * Mission Control Sequence Command that runs data simulation
 *
 * This command uses a simulator to generate simulated measurement data that can
 * be processed by GMAT's estimators.  The Simulator class contains the elements
 * that actually manage the simulation process, including a finite state machine
 * that walks the system through the process of data simulation.
 *
 * Simulated data generated by this command is generated through propagation
 * using a pre-configured propagator.  The ODEModel used in this process is set
 * prior to execution of the command.  That means that the simulated data
 * generated in this process does not include any transient forces.
 */
class RunSimulator : public RunSolver
{
public:
   RunSimulator();
   virtual ~RunSimulator();
   RunSimulator(const RunSimulator& rs);
   RunSimulator& operator=(const RunSimulator& rs);

   virtual GmatBase* Clone() const;

   virtual std::string GetRefObjectName(const Gmat::ObjectType type) const;
   virtual bool SetRefObjectName(const Gmat::ObjectType type,
         const std::string &name);
   virtual bool RenameRefObject(const Gmat::ObjectType type,
         const std::string &oldName, const std::string &newName);
   virtual const std::string& GetGeneratingString(Gmat::WriteMode mode,
         const std::string &prefix, const std::string &useName);

   virtual bool Initialize();
   virtual bool Execute();
   virtual void RunComplete();
   virtual bool TakeAction(const std::string &action,
                           const std::string &actionData = "");

   virtual GmatCommand* GetNext();

protected:
   /// The simulator that drives this process
   Simulator      *theSimulator;
   /// Flag indicating if command execution is started
   bool commandRunning;
//   /// Flag indicating if propagation is running an needs reentrance
//   bool commandPropagating;
   /// Flag indicating if command execution is done
   bool commandComplete;

   void PrepareToSimulate();
   void Propagate();
   void Calculate();
   void LocateEvent();
   void Simulate();
   void Finalize();
};

#endif /* RunSimulator_hpp */
