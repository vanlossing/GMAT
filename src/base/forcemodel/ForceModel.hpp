//$Header$
//------------------------------------------------------------------------------
//                              ForceModel
//------------------------------------------------------------------------------
// *** File Name : ForceModel.hpp
// *** Created   : October 1, 2002
// **************************************************************************
// ***  Developed By  :  Thinking Systems, Inc. (www.thinksysinc.com)     ***
// ***  For:  Flight Dynamics Analysis Branch (Code 572)                  ***
// ***  Under Contract:  P.O.  GSFC S-66617-G                             ***
// ***                                                                    ***
// ***  Copyright U.S. Government 2002                                    ***
// ***  Copyright United States Government as represented by the          ***
// ***  Administrator of the National Aeronautics and Space               ***
// ***  Administration                                                    ***
// ***                                                                    ***
// ***  This software is subject to the Sofware Usage Agreement described ***
// ***  by NASA Case Number GSC-14735-1.  The Softare Usage Agreement     ***
// ***  must be included in any distribution.  Removal of this header is  ***
// ***  strictly prohibited.                                              ***
// ***                                                                    ***
// ***                                                                    ***
// ***  Header Version: July 12, 2002                                     ***
// **************************************************************************
// Module Type               : ANSI C++ Source
// Development Environment   : Visual C++ 7.0
// Modification History      : 11/26/2002 - D. Conway, Thinking Systems, Inc.
//                             Original delivery
//
//                           : 2/28/2003 - D. Conway, Thinking Systems, Inc.
//                             Filled in some code to model cases with 
//                             discontinuous forces (e.g. SRP)
//
//                           : 10/01/2003 - W. Waktola, Missions Applications Branch
//                              Changes:
//                                - Updated style using GMAT cpp style guide
//                                - Changed FlightDynamicsForces class to ForceModel class
//
//                           : 10/20/2003 - W. Waktola, Missions Applications Branch
//                              Changes:
//                                - All double types to Real types
//                                - All primitive int types to Integer types
//
//                           : 11/9/2003 - D. Conway, Thinking Systems, Inc.
//                             Overrode GetParameterCount so the count 
//                             increases based on the member forces
// **************************************************************************
/**
 * Container for forces acting on spacecraft.
 * 
 * ForceModel is a class that accumulates the forces acting on one or
 * more spacecraft
*/

#ifndef ForceModel_hpp
#define ForceModel_hpp

#include "PhysicalModel.hpp"
#include "DerivativeList.hpp"
#include "Spacecraft.hpp"

#include <fstream>
#include <vector>

class GMAT_API ForceModel : public PhysicalModel
{
public:
//    ForceModel(void);
    ForceModel(const std::string &nomme = "");
    virtual ~ForceModel(void);

    ForceModel(const ForceModel& fdf);

    ForceModel&   operator=(const ForceModel& fdf);

    void AddForce(PhysicalModel *pPhyscialModel);
    bool AddSpacecraft(Spacecraft *sc);
    void UpdateSpacecraft(Real newEpoch = -1.0);
    void UpdateFromSpacecraft(void);
    
    virtual bool Initialize(void);
    virtual void IncrementTime(Real dt);
    virtual void SetTime(Real t);

    bool GetDerivatives(Real * state, Real dt = 0.0, Integer order = 1);
    Real EstimateError(Real *diffs, Real *answer) const;

    Integer GetNumForces();
    StringArray& GetForceTypeNames(); //loj: 3/4/04 added
    std::string GetForceTypeName(Integer index);
    void ClearSpacecraft(); //loj: 4/1/04 added
    
    PhysicalModel* GetForce(Integer index);
      
    virtual Integer GetParameterCount(void) const;
    
    // Access methods derived classes can override
    virtual std::string GetParameterText(const Integer id) const;
    virtual Integer     GetParameterID(const std::string &str) const;
    virtual Gmat::ParameterType
                        GetParameterType(const Integer id) const;
    virtual std::string GetParameterTypeString(const Integer id) const;

    virtual std::string GetStringParameter(const Integer id) const;
    virtual std::string GetStringParameter(const std::string &value) const;
    virtual bool        SetStringParameter(const Integer id,
                                           const std::string &value);
    virtual bool        SetStringParameter(const std::string &label,
                                           const std::string &value);
    
    virtual const StringArray& GetStringArrayParameter(const Integer id) const;
    virtual const StringArray& GetStringArrayParameter(const std::string &label) const;

protected:

    DerivativeList * derivatives;
    Integer          forceCount;
    Integer          numForces;
    
    Real estimationMethod;
    /// List of spacecraft that get propagated
    std::vector<Spacecraft *> spacecraft;
    StringArray forceTypeNames;
    std::vector<PhysicalModel *> forceList; //loj: 2/11/04 added
    
    enum
    {
        POINT_MASS = PhysicalModelParamCount,
        FULL_FIELD,
        DRAG,
        MAG_FIELD,
        FORCE_LIST,
        ForceModelParamCount,
    };
    
    static const std::string PARAMETER_TEXT[ForceModelParamCount - PhysicalModelParamCount];
    static const Gmat::ParameterType PARAMETER_TYPE[ForceModelParamCount - PhysicalModelParamCount];
};

#endif  // ForceModel_hpp

