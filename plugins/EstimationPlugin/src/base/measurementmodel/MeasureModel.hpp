//$Id$
//------------------------------------------------------------------------------
//                           MeasureModel
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
// Created: Jan 31, 2014
/**
 * The Measurement Model base class for estimation
 */
//------------------------------------------------------------------------------

#ifndef MeasureModel_hpp
#define MeasureModel_hpp

#include "estimation_defs.hpp"
#include "GmatBase.hpp"
#include "SolarSystem.hpp"
#include "SignalData.hpp"

// Forward references
class SignalBase;
class ProgressReporter;

class ObservationData;
class RampTableData;
class MeasurementData;
class PropSetup;


/**
 * The estimation measurement model
 *
 * This class is the reworked measurement model for GMAT's estimation subsystem.
 * It uses the signal classes to model the path of a measurement.  The output
 * resulting from the modeling is built by an Adapter that uses the raw data to
 * generate measurement information.
 */
class ESTIMATION_API MeasureModel : public GmatBase
{
public:
   MeasureModel(const std::string &name);
   virtual ~MeasureModel();
   MeasureModel(const MeasureModel& mm);
   MeasureModel& operator=(const MeasureModel& mm);

   virtual GmatBase* Clone() const;
   virtual void SetSolarSystem(SolarSystem *ss);

   // Access methods derived classes can override
   virtual std::string  GetParameterText(const Integer id) const;
   virtual Integer GetParameterID(const std::string &str) const;
   virtual Gmat::ParameterType
                        GetParameterType(const Integer id) const;
   virtual std::string  GetParameterTypeString(const Integer id) const;

   virtual std::string  GetStringParameter(const Integer id) const;
   virtual bool SetStringParameter(const Integer id,
                                           const std::string &value);
   virtual std::string GetStringParameter(const Integer id,
                                           const Integer index) const;
   virtual bool SetStringParameter(const Integer id,
                                           const std::string &value,
                                           const Integer index);
   virtual const StringArray&
                        GetStringArrayParameter(const Integer id) const;
   virtual const StringArray&
                        GetStringArrayParameter(const Integer id,
                                                const Integer index) const;

   virtual std::string GetStringParameter(const std::string &label) const;
   virtual bool SetStringParameter(const std::string &label,
                                           const std::string &value);
   virtual std::string GetStringParameter(const std::string &label,
                                           const Integer index) const;
   virtual bool SetStringParameter(const std::string &label,
                                           const std::string &value,
                                           const Integer index);
   virtual const StringArray&
                        GetStringArrayParameter(const std::string &label) const;
   virtual const StringArray&
                        GetStringArrayParameter(const std::string &label,
                                                const Integer index) const;

   virtual const StringArray&
                        GetRefObjectNameArray(const Gmat::ObjectType type);
//   virtual bool         SetRefObjectName(const Gmat::ObjectType type,
//                                         const std::string &name);
   virtual bool RenameRefObject(const Gmat::ObjectType type,
                                        const std::string &oldName,
                                        const std::string &newName);
//   virtual GmatBase*    GetRefObject(const Gmat::ObjectType type,
//                                     const std::string &name);
//   virtual GmatBase*    GetRefObject(const Gmat::ObjectType type,
//                                     const std::string &name,
//                                     const Integer index);
   virtual bool SetRefObject(GmatBase *obj, const Gmat::ObjectType type,
                                     const std::string &name = "");
   virtual bool SetRefObject(GmatBase *obj, const Gmat::ObjectType type,
                                     const std::string &name,
                                     const Integer index);

   DEFAULT_TO_NO_CLONES

   virtual void SetPropagator(PropSetup* ps);
   virtual bool Initialize();


//   Integer              GetModelID();
//   Integer              GetModelTypeID();
//   void                 SetModelID(Integer newID);

////   virtual const MeasurementData&
////                        CalculateMeasurement(bool withEvents = false);
   // TBD: Do we want something more generic here?
//   virtual const MeasurementData&
   virtual bool CalculateMeasurement(bool withEvents = false,
                       ObservationData* forObservation = NULL,
                       std::vector<RampTableData>* rampTB = NULL);
   virtual const std::vector<RealArray>&
                        CalculateMeasurementDerivatives(GmatBase *obj,
                                                        Integer id);
//   virtual bool         SetMeasurement(CoreMeasurement *meas);
//   virtual const MeasurementData&
//                        GetMeasurement();
//   virtual bool         WriteMeasurements();
//   virtual bool         WriteMeasurement(Integer id);
//
//   // Covariance handling code
//   virtual Integer      HasParameterCovariances(Integer parameterId);
////   virtual Rmatrix*        GetParameterCovariances(Integer parameterId = -1);
//
//   virtual Integer      GetEventCount();
//   virtual Event*       GetEvent(Integer whichOne);
//   virtual bool         SetEventData(Event *locatedEvent = NULL);
//
//   virtual void         SetCorrection(const std::string& correctionName,
//         const std::string& correctionType);

   const std::vector<SignalData*>& GetSignalData();
   bool IsMeasurementFeasible();
   virtual void UsesLightTime(const bool tf);

   // Measurement Model Settings
   virtual bool SetProgressReporter(ProgressReporter* reporter);

protected:
   /// The ordered list of participants in the signal path
   std::vector<StringArray*> participantLists;
   /// Participant pointers.  This list is 1:1 with participantList
   std::vector<ObjectArray*> participants;
   /// Collection of the potential participants
   ObjectArray candidates;
   /// The list of Signals, path by path
   std::vector<SignalBase*> signalPaths;
   /// Measurement data from the Signals
   std::vector<SignalData*> theData;
   /// Most recently calculated measurement derivatives gathered from Signals
   std::vector<RealArray> theDataDerivatives;
   /// @todo: Extend this code to support multiple propagators
   /// The propagator used for light time solutions
   PropSetup *propagator;
   /// Flag checking if the last measurement computed as feasible
   bool feasible;
   /// Flag used to control light time solution generation
   bool withLighttime;

   /// The reporter for status information
   ProgressReporter *navLog;
   /// The log level for measurement models
   UnsignedInt logLevel;

   /// Flag used to indicate if the model is physical or single point
   bool isPhysical;
   /// The solar system
   SolarSystem *solarsys;

   /// Parameter IDs for the BatchEstimators
   enum
   {
      SIGNAL_PATH  = GmatBaseParamCount,
      MeasurementParamCount,
   };

   /// Strings describing the BatchEstimator parameters
   static const std::string PARAMETER_TEXT[MeasurementParamCount -
                                           GmatBaseParamCount];
   /// Types of the BatchEstimator parameters
   static const Gmat::ParameterType PARAMETER_TYPE[MeasurementParamCount -
                                                   GmatBaseParamCount];

//   StringArray* DecomposePathString(const std::string &value);
};

#endif /* MeasureModel_hpp */
