//$Header$
//------------------------------------------------------------------------------
//                                  GmatBase
//------------------------------------------------------------------------------
// GMAT: Goddard Mission Analysis Tool.
//
// **Legal**
//
// Developed jointly by NASA/GSFC and Thinking Systems, Inc. under contract
// number S-67573-G
//
// Author: Darrel J. Conway
// Created: 2003/09/25
//
//
// Modification History:
//
// 11/9/2003 D. Conway
//   Made GetParameterCount virtual so PropSetup can override it, returning the
//   count for the member ForceModel, Forces, and Propagator.
/**
 * Definition for the base class for all GMAT extensible objects
 *
 * The following class hierarchy trees use this class as their basis:
 *
 *     Asset (hence Spacecraft and GroundStation)
 *     CelestialBody
 *     Propagator
 *     PhysicalModel (hence Force and ForceModel)
 *     PropConfig
 *     Parameter
 *     Command
 *
 * Every class that users can use to derive new classes, or that need to be
 * accessed through generic pointers, should be derived from this class to
 * ensure consistent interfaces accessed by the GMAT control systems (i.e. the
 * Moderator, FactoryManager, Configuration, Interpreter, and Sandbox, along
 * with the GUIInterpreter).
 */
//------------------------------------------------------------------------------


#ifndef GmatBase_hpp
#define GmatBase_hpp

#include "gmatdefs.hpp"


class GMAT_API GmatBase
{
public:
    // The usual suspects
    GmatBase(Gmat::ObjectType typeId, const std::string &typeStr, 
             const std::string &nomme = "");
    virtual ~GmatBase(void) = 0;
    GmatBase(const GmatBase &a);
    GmatBase&           operator=(const GmatBase &a);

    // Access methods called on the base class
    Gmat::ObjectType    GetType(void) const;
    std::string         GetTypeName(void) const;
    virtual Integer     GetParameterCount(void) const;
    std::string         GetName(void) const;
    bool                SetName(const std::string &who);

    // Access methods derived classes can override
    virtual std::string GetParameterText(const Integer id) const;
    virtual Integer     GetParameterID(const std::string &str) const;
    virtual Gmat::ParameterType
                        GetParameterType(const Integer id) const;
    virtual std::string GetParameterTypeString(const Integer id) const;

    virtual Real        GetRealParameter(const Integer id) const;
    virtual Real        SetRealParameter(const Integer id,
                                         const Real value);
    virtual Integer     GetIntegerParameter(const Integer id) const;
    virtual Integer     SetIntegerParameter(const Integer id,
                                            const Integer value);
    virtual std::string GetStringParameter(const Integer id) const;
    virtual bool        SetStringParameter(const Integer id, 
                                           const std::string &value);
    virtual bool        GetBooleanParameter(const Integer id) const;
    virtual bool        SetBooleanParameter(const Integer id,
                                            const bool value);
    
    /// Return value used if the parameter is not accessible as a Real
    static const Real    REAL_PARAMETER_UNDEFINED;
    /// Return value used if the parameter is not accessible as an Integer
    static const Integer INTEGER_PARAMETER_UNDEFINED;
    /// String mappings for the GMAT data types
    static const std::string PARAM_TYPE_STRING[Gmat::TypeCount];

    
protected:
    /// Count of the accessible parameters
    Integer             parameterCount;
    /// Script string used or this class
    std::string         typeName;
    /// Name of the object -- empty if it is nameless
    std::string         instanceName;
    /// Enumerated base type of the object
    Gmat::ObjectType    type;

    void                CopyParameters(const GmatBase &a);
};


#endif // GmatBase_hpp

