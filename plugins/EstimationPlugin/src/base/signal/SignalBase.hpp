//$Id$
//------------------------------------------------------------------------------
//                           SignalBase
//------------------------------------------------------------------------------
// GMAT: General Mission Analysis Tool
//
// Copyright (c) 2002-2011 United States Government as represented by the
// Administrator of The National Aeronautics and Space Administration.
// All Other Rights Reserved.
//
// Developed jointly by NASA/GSFC and Thinking Systems, Inc. under the FDSS 
// contract, Task Order 28
//
// Author: Darrel J. Conway, Thinking Systems, Inc.
// Created: Jan 9, 2014
/**
 * Base class for signals between two measurement participants
 */
//------------------------------------------------------------------------------

#ifndef SignalBase_hpp
#define SignalBase_hpp

#include "GmatBase.hpp"
#include "SpacePoint.hpp"
#include "GmatState.hpp"
#include "MeasurementData.hpp"
#include "CoordinateSystem.hpp"
#include "CoordinateConverter.hpp"
#include "ProgressReporter.hpp"
#include "SignalData.hpp"

class PropSetup;


/**
 * Base class for signals between two measurement participants
 *
 * This class sets the nodes for measurement signals, and defines the interfaces
 * used by measurements when calculating signal data.
 *
 * SignalBase objects are arranged in a linked list, so that a full signal path
 * can be defined through the list.
 */
class ESTIMATION_API SignalBase: public GmatBase
{
public:
   SignalBase(const std::string &typeStr, const std::string &name = "");
   virtual ~SignalBase();
   SignalBase(const SignalBase& sb);
   SignalBase& operator=(const SignalBase& sb);

   DEFAULT_TO_NO_CLONES

   bool                 SetProgressReporter(ProgressReporter* reporter);

   bool                 SetTransmitParticipantName(std::string name);
   bool                 SetReceiveParticipantName(std::string name);

   virtual void         SetSolarSystem(SolarSystem *ss);
   virtual const StringArray&
                        GetRefObjectNameArray(const Gmat::ObjectType type);
   virtual bool         SetRefObject(GmatBase *obj, const Gmat::ObjectType type,
                                     const std::string &name = "");
   virtual bool         RenameRefObject(const Gmat::ObjectType type,
                                        const std::string &oldName,
                                        const std::string &newName);

   SignalBase           *GetStart(bool epochIsAtEnd);
   virtual SignalBase*  GetNext();
   virtual bool         Add(SignalBase *signalToAdd);

   virtual void         SetPropagator(PropSetup *propagator,
                                      GmatBase *forObj = NULL);
   virtual bool         Initialize();
   virtual bool         LoadParticipantData();
   virtual bool         ModelSignal(const GmatEpoch atEpoch,
                                    bool EpochAtReceive = true) = 0;
   virtual const std::vector<RealArray>&
                        ModelSignalDerivative(GmatBase *obj,
                              Integer forId) = 0;

   virtual std::string  GetPathDescription(bool fullList = true);

   SignalData&          GetSignalData();
   bool                 IsSignalFeasible();
   virtual void         UsesLighttime(const bool tf);

protected:
   /// The next node in the list of signals (NULL at the end of the list)
   SignalBase                 *next;
   /// The next node in the list of signals (NULL at the start of the list)
   SignalBase                 *previous;

   /// State data for the transmitting participant
   GmatState                  tState;
   /// State data for the receiving participant
   GmatState                  rState;
   // Participant data that can be passed to callers
   SignalData                 theData;
   // Participant data that can be passed to callers
   std::vector<RealArray>     theDataDerivatives;

   // Member data used
   // Coordinate Systems
   /// Coordinate system of the transmitting participant
   CoordinateSystem           *tcs;
   /// Coordinate system of the receiving participant
   CoordinateSystem           *rcs;
   /// Coordinate system in which observation is expressed
   CoordinateSystem           *ocs;
   /// J2K coordinate system
   CoordinateSystem           *j2k;

   // Storage buffers
   /// Epoch of most recent calculation
   GmatEpoch                  satEpoch;

   /// Parameter ID used to retrieve internal epoch data
   Integer                    satEpochID;
   /// Rotation matrix from receiver to J2K (Identity by default)
   Rmatrix33                  R_j2k_Receiver;
//   /// Rotation matrix from J2K to receiver (Identity by default)
//   Rmatrix33                  R_Receiver_j2k;
   /// Rotation matrix from transmitter to J2K (Identity by default)
   Rmatrix33                  R_j2k_Transmitter;
//   /// Rotation matrix from J2K to F1 (Identity by default)
//   Rmatrix33                  R_Transmitter_j2k;
   /// Rotation matrix from receiver to observation frame (Identity by default)
   Rmatrix33                  R_Obs_Receiver;
   /// Rotation matrix from F1 to observation frame (Identity by default)
   Rmatrix33                  R_Obs_Transmitter;
   /// Rotation matrix from J2K to observation frame (Identity by default)
   Rmatrix33                  R_Obs_j2k;
   /// Rotation Dot matrix from J2K to observation frame (Identity by default)
   Rmatrix33                  RDot_Obs_j2k;
   /// Rotation Dot matrix from F2 to observation frame (Identity by default)
   Rmatrix33                  RDot_Obs_Receiver;
   /// Rotation Dot matrix from transmitter to observation frame (Identity by default)
   Rmatrix33                  RDot_Obs_Transmitter;
   /// Feasibility for the signal, based on information from the signal nodes
   bool                       signalIsFeasible;
   /// Flag triggering inclusion of light time solution
   bool                       includeLightTime;

   // Utility vectors and matrices
   /// Identity matrix, 3x3
   static const Rmatrix33     I33;
   /// Zero matrix, 3x3
   static const Rmatrix33     zero33;
   /// zero-vector
   static const Rvector3      zeroVector;
   /// [1,0,0]T
   static const Rvector3      xUnit;
   /// [0,1,0]T
   static const Rvector3      yUnit;
   /// [0,0,1]T
   static const Rvector3      zUnit;

   // Utility objects
   /// Converter to convert between F1, F2, Fo frames
   CoordinateConverter        converter;
   /// the Solar System in use
   SolarSystem                *solarSystem;

   /// The progress reporter that allows user control over the data logging
   ProgressReporter           *navLog;
   /// The current logging level for signals
   UnsignedInt                logLevel;

   void                       SetPrevious(SignalBase *prev);
   virtual void               InitializeSignal();

   // Some useful methods
   // Coordinate System methods based on methods in GeometricMeasurement
   virtual void               CalculateRangeVectorInertial();
   virtual void               CalculateRangeVectorObs();
   virtual void               CalculateRangeRateVectorObs();
   virtual void               UpdateRotationMatrix(Real atEpoch,
                                    const std::string &whichOne = "All");

   virtual void GetRangeDerivative(GmatBase *forObj, bool wrtR, bool wrtV,
         Rvector &deriv);
   virtual void GetRangeVectorDerivative(GmatBase *forObj, bool wrtR, bool wrtV,
         Rmatrix& derivMatrix);

   Integer                    GetParmIdFromEstID(Integer forId, GmatBase *obj);

   void                       MoveToEpoch(const GmatEpoch theEpoch,
                                          bool epochAtReceive,
                                          bool moveAll = true);
   bool                       StepParticipant(Real stepToTake,
                                              bool forTransmitter);
};

#endif /* SignalBase_hpp */
