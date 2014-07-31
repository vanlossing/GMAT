//$Id$
//------------------------------------------------------------------------------
//                                 Spacecraft
//------------------------------------------------------------------------------
// GMAT: General Mission Analysis Tool.
//
// Copyright (c) 2002-2014 United States Government as represented by the
// Administrator of The National Aeronautics and Space Administration.
// All Other Rights Reserved.
//
// Developed jointly by NASA/GSFC and Thinking Systems, Inc. under contract
// number S-67573-G
//
// Author:  Joey Gurganus, Reworked by D. Conway
// Created: 2003/10/22
//
/**
 * Definition of the Spacecraft class base
 */
//------------------------------------------------------------------------------

#ifndef Spacecraft_hpp
#define Spacecraft_hpp

#include "SpaceObject.hpp"
#include "Rvector6.hpp"
#include "GmatState.hpp"
#include "FuelTank.hpp"
#include "Thruster.hpp"
#include "PowerSystem.hpp"
#include "CoordinateSystem.hpp"
#include "CoordinateConverter.hpp"
#include "TimeSystemConverter.hpp"
#include "Attitude.hpp"
#include "SPADFileReader.hpp"

class GMAT_API Spacecraft : public SpaceObject
{
public:
   Spacecraft(const std::string &name,
      const std::string &typeStr = "Spacecraft");
   Spacecraft(const Spacecraft &a);
   Spacecraft&          operator=(const Spacecraft &a);

   // Destructor
   virtual              ~Spacecraft();

   virtual void         SetSolarSystem(SolarSystem *ss);
   void                 SetInternalCoordSystem(CoordinateSystem *cs);
   CoordinateSystem*    GetInternalCoordSystem();
   
   std::string          GetModelFile();
   std::string          GetModelFileFullPath();
   int                  GetModelId();
   void                 SetModelId(int id);
   
   void                 SetState(const Rvector6 &cartState);
   void                 SetState(const std::string &elementType, Real *instate);
   void                 SetState(const Real s1, const Real s2, const Real s3,
                                 const Real s4, const Real s5, const Real s6);

   virtual GmatState&   GetState();
   virtual Rvector6     GetState(std::string rep);
   virtual Rvector6     GetState(Integer rep);
   StringArray          GetStateElementLabels(const std::string &stateType);
   Rvector6             GetCartesianState();
   Rvector6             GetKeplerianState();
   Rvector6             GetModifiedKeplerianState();

   Real                 GetAnomaly() const;
   std::string          GetAnomalyType() const;
   
   virtual bool         HasAttitude();
   virtual const Rmatrix33&
                        GetAttitude(Real a1mjdTime);
   const Rvector3&      GetAngularVelocity(Real a1mjdTime) const;
   const UnsignedIntArray&
                        GetEulerAngleSequence() const;
   
   Rvector3             GetSPADSRPArea(const Real ep, const Rvector3 &sunVector);
   
   // inherited from GmatBase
   virtual GmatBase*    Clone(void) const;
   virtual void         Copy(const GmatBase* orig);
   virtual bool         RenameRefObject(const Gmat::ObjectType type,
                                        const std::string &oldName,
                                        const std::string &newName);
   virtual const std::string
                        GetAttributeCommentLine(Integer index);
   virtual void         SetAttributeCommentLine(Integer index,
                                                const std::string &comment);
   virtual const std::string
                        GetInlineAttributeComment(Integer index);
   virtual void         SetInlineAttributeComment(Integer index,
                                                  const std::string &comment);
   
   virtual std::string  GetRefObjectName(const Gmat::ObjectType type) const;

   virtual bool         HasRefObjectTypeArray();
   virtual const        ObjectTypeArray& GetRefObjectTypeArray();
   virtual const StringArray&
                        GetRefObjectNameArray(const Gmat::ObjectType type);
   virtual bool         SetRefObjectName(const Gmat::ObjectType type,
                                         const std::string &name);
   virtual GmatBase*    GetRefObject(const Gmat::ObjectType type,
                                     const std::string &name);
   virtual bool         SetRefObject(GmatBase *obj, const Gmat::ObjectType type,
                                     const std::string &name = "");

   virtual ObjectArray& GetRefObjectArray(const Gmat::ObjectType type);
   virtual ObjectArray& GetRefObjectArray(const std::string& typeString);

   // Parameter accessor methods -- overridden from GmatBase
   virtual Integer      GetParameterID(const std::string &str) const;

   virtual bool         IsParameterReadOnly(const Integer id) const;
   virtual bool         IsParameterReadOnly(const std::string &label) const;
   virtual bool         IsParameterCommandModeSettable(const Integer id) const;

   virtual bool         ParameterAffectsDynamics(const Integer id) const;
   virtual bool         ParameterDvInitializesNonzero(const Integer id,
                              const Integer r = 0, const Integer c = 0) const;
   virtual Real         ParameterDvInitialValue(const Integer id,
                              const Integer r = 0, const Integer c = 0) const;

   virtual Real         GetRealParameter(const Integer id) const;
   virtual Real         GetRealParameter(const std::string &label) const;
   virtual Real         SetRealParameter(const Integer id, const Real value);
   virtual Real         SetRealParameter(const std::string &label, const Real value);
   virtual Real         SetRealParameter(const Integer id,
                                         const Real value,
                                         const Integer index);
   
   virtual const Rvector& GetRvectorParameter(const Integer id) const;
   virtual const Rvector& GetRvectorParameter(const std::string &label) const;
   virtual const Rvector& SetRvectorParameter(const Integer id,
                                              const Rvector &value);
   virtual const Rvector& SetRvectorParameter(const std::string &label,
                                              const Rvector &value);
   
   virtual std::string  GetStringParameter(const Integer id) const;
   virtual std::string  GetStringParameter(const std::string &label) const;
   virtual bool         SetStringParameter(const Integer id, const std::string &value);
   virtual bool         SetStringParameter(const std::string &label,
                                           const std::string &value);
   virtual bool         SetStringParameter(const Integer id,
                                           const std::string &value,
                                           const Integer index);
   virtual bool         SetStringParameter(const std::string &label,
                                           const std::string &value,
                                           const Integer index);

   virtual const Rmatrix&
                        GetRmatrixParameter(const Integer id) const;
   virtual const Rmatrix&
                        SetRmatrixParameter(const Integer id,
                                            const Rmatrix &value);
   virtual const Rmatrix&
                        GetRmatrixParameter(const std::string &label) const;
   virtual const Rmatrix&
                        SetRmatrixParameter(const std::string &label,
                                            const Rmatrix &value);
   virtual Real         GetRealParameter(const Integer id, const Integer row,
                                         const Integer col) const;
   virtual Real         GetRealParameter(const std::string &label,
                                         const Integer row,
                                         const Integer col) const;
   virtual Real         SetRealParameter(const Integer id, const Real value,
                                         const Integer row, const Integer col);
   virtual Real         SetRealParameter(const std::string &label,
                                         const Real value, const Integer row,
                                         const Integer col);

   const StringArray&   GetStringArrayParameter(const Integer id) const;
   virtual const StringArray&
                        GetStringArrayParameter(const std::string &label) const;
   virtual std::string  GetStringParameter(const Integer id, const Integer index) const;
   virtual std::string  GetStringParameter(const std::string & label, const Integer index) const;

   virtual std::string  GetParameterText(const Integer id) const;
   virtual Gmat::ParameterType
                        GetParameterType(const Integer id) const;
   virtual std::string  GetParameterTypeString(const Integer id) const;
   virtual Gmat::ObjectType
                        GetPropertyObjectType(const Integer id) const;
   virtual bool         CanAssignStringToObjectProperty(const Integer id) const;
   
   virtual bool         Validate();
   virtual bool         Initialize();

   virtual bool         TakeAction(const std::string &action,
                                   const std::string &actionData = "");
   virtual bool         IsOwnedObject(Integer id) const;
   virtual GmatBase*    GetOwnedObject(Integer whichOne);


   virtual const std::string&
                        GetGeneratingString(Gmat::WriteMode mode = Gmat::SCRIPTING,
                                            const std::string &prefix = "",
                                            const std::string &useName = "");

   std::string GetEpochString();
   void SetDateFormat(const std::string &dateType);
   void SetEpoch(const std::string &ep);
   void SetEpoch(const std::string &type, const std::string &ep, Real a1mjd);
   void SetState(const std::string &type, const Rvector6 &cartState);
   void SetAnomaly(const std::string &type, Real ta);

   virtual Integer         GetPropItemID(const std::string &whichItem);
   virtual Integer         SetPropItem(const std::string &propItem);
   virtual StringArray     GetDefaultPropItems();
   virtual Real*           GetPropItem(const Integer item);
   virtual Integer         GetPropItemSize(const Integer item);
   virtual bool            PropItemNeedsFinalUpdate(const Integer item);

   virtual bool            IsEstimationParameterValid(const Integer id);
   virtual Integer         GetEstimationParameterSize(const Integer id);
   virtual Real*           GetEstimationParameterValue(const Integer id);

   virtual bool            HasDynamicParameterSTM(Integer parameterId);
   virtual Rmatrix*        GetParameterSTM(Integer parameterId);
   virtual Integer         HasParameterCovariances(Integer parameterId);

   // Cloned object update management
   virtual bool HasLocalClones();
   virtual void UpdateClonedObject(GmatBase *obj);
   virtual void UpdateClonedObjectParameter(GmatBase *obj,
                                            Integer updatedParameterId);

   virtual void      UpdateElementLabels();
   virtual void      UpdateElementLabels(const std::string &displayStateType);

protected:
   enum SC_Param_ID
   {
      SC_EPOCH_ID = SpaceObjectParamCount,
      ELEMENT1_ID,
      ELEMENT2_ID,
      ELEMENT3_ID,
      ELEMENT4_ID,
      ELEMENT5_ID,
      ELEMENT6_ID,
      ELEMENT1UNIT_ID,
      ELEMENT2UNIT_ID,
      ELEMENT3UNIT_ID,
      ELEMENT4UNIT_ID,
      ELEMENT5UNIT_ID,
      ELEMENT6UNIT_ID,
      STATE_TYPE_ID,           // deprecated
      DISPLAY_STATE_TYPE_ID,
      ANOMALY_ID,
      COORD_SYS_ID,
      DRY_MASS_ID,
      DATE_FORMAT_ID,
      CD_ID,
      CR_ID,
      DRAG_AREA_ID,
      SRP_AREA_ID,
      FUEL_TANK_ID,
      THRUSTER_ID,
      POWER_SYSTEM_ID,
      TOTAL_MASS_ID,
      SPACECRAFT_ID,
      ATTITUDE,
      ORBIT_STM,
      ORBIT_A_MATRIX,
//      ORBIT_COVARIANCE,

      // SPAD SRP parameters
      SPAD_SRP_FILE,
      SPAD_SRP_FILE_FULL_PATH, // read-only
      SPAD_SRP_SCALE_FACTOR,

      // Hidden parameters used by the PSM
      CARTESIAN_X,
      CARTESIAN_Y,
      CARTESIAN_Z,
      CARTESIAN_VX,
      CARTESIAN_VY,
      CARTESIAN_VZ,
      MASS_FLOW,

      // Hardware for spacecraft
      ADD_HARDWARE,
      // The filename used for the spacecraft's model 
      MODEL_FILE,
      MODEL_FILE_FULL_PATH, // read-only
      // The Offset, rotation, and scale values for the spacecraft's model
      MODEL_OFFSET_X,
      MODEL_OFFSET_Y,
      MODEL_OFFSET_Z,
      MODEL_ROTATION_X,
      MODEL_ROTATION_Y,
      MODEL_ROTATION_Z,
      MODEL_SCALE,
      MODEL_MAX,

      SpacecraftParamCount = MODEL_MAX  // Assumes model params at the end
   };

   enum MultipleReps  // these are IDs for the different representations
   {
      // Cartesian
      CART_X = 10000,
      CART_Y,
      CART_Z,
      CART_VX,
      CART_VY,
      CART_VZ,
      // Keplerian
      KEPL_SMA,
      KEPL_ECC,
      KEPL_INC,
      KEPL_RAAN,
      KEPL_AOP,
      KEPL_TA,
      KEPL_EA,
      KEPL_MA,
      KEPL_HA,
      // Modified Keplerian
      MOD_KEPL_RADPER,
      MOD_KEPL_RADAPO,
      // SphericalAZFPA
      AZFPA_RMAG,
      AZFPA_RA,
      AZFPA_DEC,
      AZFPA_VMAG,
      AZFPA_AZI,
      AZFPA_FPA,
      // SphericalRADEC
      RADEC_RAV,
      RADEC_DECV,
      // Equinoctial
      EQ_PEY,
      EQ_PEX,
      EQ_PNY,
      EQ_PNX,
      EQ_MLONG,
      // Modified Equinoctial (Semi-latus rectum); Modified by M.H.
      MOD_EQ_P,
      MOD_EQ_F,		
      MOD_EQ_G,		
      MOD_EQ_H,		
      MOD_EQ_K,		
      MOD_EQ_TLONG,
      // Alternate Equinoctial by HYKim
      ALT_EQ_P,
      ALT_EQ_Q,
      // Delaunay; Modified by M.H.
      DEL_DELA_l,
      DEL_DELA_g,
      DEL_DELA_h,
      DEL_DELA_L,
      DEL_DELA_G,
      DEL_DELA_H,
      // Planetodetic; Modified by M.H.
      PLD_RMAG,
      PLD_LON,
      PLD_LAT,
      PLD_VMAG,
      PLD_AZI,
      PLD_HFPA,
      // Hyperbolic Incoming Asymptotes mod by YK
      // @note RadPer and TA are shared by Mod_Kep and Kep.
      // Added HyperbolicRadPer (LOJ: 2014.04.28)
      // Added separate Incoming/Outgoing RadPer and C3 (LOJ: 2014.05.07)
      //HYP_RAD_PER,
      //HYP_ASYM_C3,
      HYP_ASYM_INCOMING_RAD_PER,
      HYP_ASYM_INCOMING_C3_ENERGY,
      HYP_ASYM_INCOMING_RHA,
      HYP_ASYM_INCOMING_DHA,
      HYP_ASYM_INCOMING_BVAZI,
      // Hyperbolic Outgoing Asymptotes
      HYP_ASYM_OUTGOING_RAD_PER,
      HYP_ASYM_OUTGOING_C3_ENERGY,
      HYP_ASYM_OUTGOING_RHA,
      HYP_ASYM_OUTGOING_DHA,
      HYP_ASYM_OUTGOING_BVAZI,
      // Brouwer-Lyddane Mean elements (short term only) mod by YK 
      BROLYD_SHORT_SMA,
      BROLYD_SHORT_ECC,
      BROLYD_SHORT_INC,
      BROLYD_SHORT_RAAN,
      BROLYD_SHORT_AOP,
      BROLYD_SHORT_MA,
      // Brouwer-Lyddane Mean elements mod by YK 
      BROLYD_LONG_SMA,
      BROLYD_LONG_ECC,
      BROLYD_LONG_INC,
      BROLYD_LONG_RAAN,
      BROLYD_LONG_AOP,
      BROLYD_LONG_MA,
      EndMultipleReps
   };
   // these are the corresponding strings
   static const std::string MULT_REP_STRINGS[EndMultipleReps - CART_X];
   
   /// Spacecraft parameter types
   static const Gmat::ParameterType
                  PARAMETER_TYPE[SpacecraftParamCount - SpaceObjectParamCount];
   /// Spacecraft parameter labels
   static const std::string
                  PARAMETER_LABEL[SpacecraftParamCount - SpaceObjectParamCount];

   enum STATE_REPS
   {
      CARTESIAN_ID = 0,
      KEPLERIAN_ID,
      MODIFIED_KEPLERIAN_ID,
      SPHERICAL_AZFPA_ID,
      SPHERICAL_RADEC_ID,
      EQUINOCTIAL_ID,
      MODIFIED_EQUINOCTIAL_ID,   // Modified by M.H.
      ALTERNATE_EQUINOCTIAL_ID,  // Modified by HYKim
      DELAUNAY_ID,	            // Modified by M.H.
      PLANETODETIC_ID,           // Modified by M.H.
      IN_ASYM_ID,	               // Mod by YK
      OUT_ASYM_ID,
      BROLYD_SHORT_ID,
      BROLYD_LONG_ID
   };
   
   static const Integer ATTITUDE_ID_OFFSET;
   static const Real    UNSET_ELEMENT_VALUE;
   
   std::map <std::string, std::string> attribCommentLineMap;
   std::map <std::string, std::string> inlineAttribCommentMap;
   std::map<std::string, std::string> defaultStateTypeMap;
   std::map <std::string, StringArray> stateElementLabelsMap;
   std::map <std::string, StringArray> stateElementUnitsMap;
   std::multimap <std::string, std::string> allElementLabelsMultiMap;
   
   /// State element labels
   StringArray       stateElementLabel;
   /// State element units
   StringArray       stateElementUnits;
   /// Possible state representations
   StringArray       representations;
   
   // The ID of the model that the spacecraft uses, and the filename as well
   std::string          modelFile;   
   int                  modelID;
   
   /// Model file path
   std::string          modelFileFullPath;
   
   /// Epoch string, specifying the text form of the epoch
   std::string       scEpochStr;
   Real              dryMass;
   Real              coeffDrag;
   Real              dragArea;
   Real              srpArea;
   Real              reflectCoeff;
   /// String specifying the epoch time system (A1, TAI, UTC, or TT)
   std::string       epochSystem;
   /// String specifying the epoch time format (Gregorian or ModJulian)
   std::string       epochFormat;
   /// String specifying the epoch system and format used for scEpochStr (TAIModJulian, etc)
   std::string       epochType;
   std::string       stateType;
   std::string       displayStateType;
   std::string       anomalyType;
   Real              trueAnomaly;

   
   // The Offset, Rotation, and Scale values for the model
   Real                 modelOffsetX;
   Real                 modelOffsetY;
   Real                 modelOffsetZ;
   Real                 modelRotationX;
   Real                 modelRotationY;
   Real                 modelRotationZ;
   Real                 modelScale;
   
   /// Solar system now needed to set to cloned Thruster
   SolarSystem       *solarSystem;
   /// Base coordinate system for the Spacecraft
   CoordinateSystem  *internalCoordSystem;
   /// Coordinate system used for the input and output to the GUI
   CoordinateSystem  *coordinateSystem;

   std::string       coordSysName;

   /// Gravitational constant for the central body (origin)
   Real              originMu;
   /// GFlattening coefficient for the central body (origin)
   Real              originFlattening;
   /// Equatorial radius for the central body (origin)
   Real              originEqRadius;
   /// Default cartesian state
   Rvector6          defaultCartesian;
   /// Current list of possible input state types
   StringArray       possibleInputTypes;
   /// Flag indicating whether or not the coordinate system has been set by the user
   bool              coordSysSet;
   /// Flag indicating whether or not the epoch has been set by the user
   bool              epochSet;
   /// Flag indicating whether or not unique state element found
   bool              uniqueStateTypeFound;
   
   /// coordinate system map to be used for Thrusters for now
   std::map<std::string, CoordinateSystem*> coordSysMap;

   /// Spacecraft ID Used in estimation, measurement data files, etc
   std::string       spacecraftId;

   /// The model type of the attitude
   std::string       attitudeModel;
   /// Pointer to the object that manages the attitude of the spacecraft
   Attitude          *attitude;

   // for non-internal spacecraft information
   CoordinateConverter coordConverter;

   // Lists of hardware elements added 11/12/04, djc
   /// Fuel tank names
   StringArray       tankNames;
   /// Thruster names
   StringArray       thrusterNames;
   /// Name of the PowerSystem
   std::string       powerSystemName;
   /// Pointers to the fuel tanks
   ObjectArray       tanks;
   /// Pointers to the spacecraft thrusters
   ObjectArray       thrusters;
   /// Pointer to the PowerSystem
   PowerSystem       *powerSystem;
   /// Dry mass plus fuel masses, a calculated parameter
   Real              totalMass;

   /// New constructs needed to preserve interfaces
   Rvector6          rvState;

   bool              initialDisplay;
   bool              csSet;
   bool              isThrusterSettingMode;

   /// The orbit State Transition Matrix
   Rmatrix           orbitSTM;
   /// The orbit State A Matrix
   Rmatrix           orbitAMatrix;

   /// The name of the SPAD SRP file
   std::string       spadSRPFile;
   /// SPAD SRP full path file name
   std::string       spadSrpFileFullPath;
   /// the scale factor to use for the SSPAD SRP data
   Real              spadSRPScaleFactor;
   /// the SPADFileReader
   SPADFileReader    *spadSRPReader;
   /// Body-fixed coordinate system used for SPAD SRP calculations
   CoordinateSystem  *spadBFCS;

   /// Toggle to making Cart state dynamic; Integer to handle multiple includes
   Integer           includeCartesianState;

   // Hardware
   /// List of hardware names used in the spacecraft
   StringArray       hardwareNames;
   /// List of hardware objects used in the spacecraft
   ObjectArray       hardwareList;

   Real              UpdateTotalMass();
   Real              UpdateTotalMass() const;
   bool              ApplyTotalMass(Real newMass);
   void              DeleteOwnedObjects(bool deleteAttitude, bool deleteTanks,
                                        bool deleteThrusters, bool deletePowerSystem,
                                        bool otherHardware);
   void              CloneOwnedObjects(Attitude *att, const ObjectArray &tnks,
                                       const ObjectArray &thrs, PowerSystem *pwr);
   void              AttachTanksToThrusters();
   bool              SetHardware(GmatBase *obj, StringArray &hwNames,
                                 ObjectArray &hwArray);
   bool              SetPowerSystem(GmatBase *obj, std::string &psName);
   virtual void      WriteParameters(Gmat::WriteMode mode, std::string &prefix,
                        std::stringstream &stream);

   Rvector6          GetStateInRepresentation(const std::string &rep = "", bool useDefaultCartesian = false);
   Rvector6          GetStateInRepresentation(Integer rep = CARTESIAN_ID, bool useDefaultCartesian = false);
   void              SetStateFromRepresentation(const std::string &rep, Rvector6 &st, const std::string &label = "");

   Real              GetElement(const std::string &label);
   bool              SetElement(const std::string &label, const Real &value);
   Integer           LookUpLabel(const std::string &label, std::string &rep);
   void              RecomputeStateAtEpoch(const GmatEpoch &toEpoch);

private:
   bool              VerifyAddHardware();
   Integer           NumStateElementsSet();
   void              SetPossibleInputTypes(const std::string& label, const std::string &rep);
   bool              ValidateOrbitStateValue(const std::string &forRep, const std::string &withLabel, Real andValue, bool checkCoupled = true);

   bool              SetAttitudeAndCopyData(Attitude *oldAtt, Attitude *newAtt, bool deleteOldAtt = false);
   void              BuildStateElementLabelsAndUnits();
};

#endif // Spacecraft_hpp
