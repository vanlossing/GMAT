//$Id: IncomingAsymptoteParameters.cpp 2013-05-24 Youngkwang Kim $
//------------------------------------------------------------------------------
//                            File: IncomingAsymptoteParameters.cpp
//------------------------------------------------------------------------------
// GMAT: General Mission Analysis Tool
//
// Copyright (c) 2002-2011 United States Government as represented by the
// Administrator of The National Aeronautics and Space Administration.
// All Other Rights Reserved.
//
// Developed jointly by NASA/GSFC and Thinking Systems, Inc. under contract
// number S-67573-G
//
// Author: Youngkwang Kim
// Created: 2013/09/15
//
/**
 * Declares Incoming Asymptote related parameter classes.
 * IncAsym_RHA, IncAsym_DHA, IncAsym_BVAZI, IncAsym_State
 */
//------------------------------------------------------------------------------

#include "IncomingAsymptoteParameters.hpp"
#include "ColorTypes.hpp"


//==============================================================================
//                              IncAsymRHA
//==============================================================================
IncAsymRHA::IncAsymRHA(const std::string &name, GmatBase *obj)
   : OrbitReal(name, "IncomingRHA", obj, "RA of Incoming Asymptote", "Deg",
               GmatParam::COORD_SYS, INCASYM_RHA, true)
{
   mDepObjectName = "EarthMJ2000Eq";
   SetRefObjectName(Gmat::COORDINATE_SYSTEM, mDepObjectName);
   SetRequiresCelestialBodyCSOrigin(true);
   mIsAngleParam = true;
   mCycleType = GmatParam::ZERO_360;
}


//------------------------------------------------------------------------------
// IncAsymRHA(const IncAsymRHA &copy)
//------------------------------------------------------------------------------
/**
 * Copy constructor.
 *
 * @param <copy> the parameter to make copy of
 */
//------------------------------------------------------------------------------
IncAsymRHA::IncAsymRHA(const IncAsymRHA &copy)
   : OrbitReal(copy)
{
}


//------------------------------------------------------------------------------
// const IncAsymRHA& operator=(const IncAsymRHA &right)
//------------------------------------------------------------------------------
/**
 * Assignment operator.
 *
 * @param <right> the parameter to make copy of
 */
//------------------------------------------------------------------------------
IncAsymRHA& IncAsymRHA::operator=(const IncAsymRHA &right)
{
   if (this != &right)
      OrbitReal::operator=(right);

   return *this;
}


//------------------------------------------------------------------------------
// ~IncAsymRHA()
//------------------------------------------------------------------------------
/**
 * Destructor.
 */
//------------------------------------------------------------------------------
IncAsymRHA::~IncAsymRHA()
{
}


//-------------------------------------
// Inherited methods from Parameter
//-------------------------------------

//------------------------------------------------------------------------------
// virtual bool Evaluate()
//------------------------------------------------------------------------------
/**
 * Evaluates value of the parameter.
 *
 * @return true if parameter value successfully evaluated; false otherwise
 */
//------------------------------------------------------------------------------
bool IncAsymRHA::Evaluate()
{
   mRealValue = OrbitData::GetIncAsymReal(INCASYM_RHA);    
    
   if (mRealValue == GmatOrbitConstants::ORBIT_REAL_UNDEFINED)
      return false;
   else
      return true;
}


//-------------------------------------
// methods inherited from GmatBase
//-------------------------------------

//------------------------------------------------------------------------------
// virtual GmatBase* Clone(void) const
//------------------------------------------------------------------------------
/**
 * Method used to create a copy of the object
 */
//------------------------------------------------------------------------------
GmatBase* IncAsymRHA::Clone(void) const
{
   return new IncAsymRHA(*this);
}


//==============================================================================
//                              IncAsymDHA
//==============================================================================
IncAsymDHA::IncAsymDHA(const std::string &name, GmatBase *obj)
   : OrbitReal(name, "IncomingDHA", obj, "DEC of Incoming Asymptote", "Deg",
               GmatParam::COORD_SYS, INCASYM_DHA, true)
{
   mDepObjectName = "EarthMJ2000Eq";
   SetRefObjectName(Gmat::COORDINATE_SYSTEM, mDepObjectName);
   SetRequiresCelestialBodyCSOrigin(true);
   mIsAngleParam = true;
   mCycleType = GmatParam::ZERO_360;
}


//------------------------------------------------------------------------------
// IncAsymDHA(const IncAsymDHA &copy)
//------------------------------------------------------------------------------
/**
 * Copy constructor.
 *
 * @param <copy> the parameter to make copy of
 */
//------------------------------------------------------------------------------
IncAsymDHA::IncAsymDHA(const IncAsymDHA &copy)
   : OrbitReal(copy)
{
}


//------------------------------------------------------------------------------
// const IncAsymDHA& operator=(const IncAsymDHA &right)
//------------------------------------------------------------------------------
/**
 * Assignment operator.
 *
 * @param <right> the parameter to make copy of
 */
//------------------------------------------------------------------------------
IncAsymDHA& IncAsymDHA::operator=(const IncAsymDHA &right)
{
   if (this != &right)
      OrbitReal::operator=(right);

   return *this;
}


//------------------------------------------------------------------------------
// ~IncAsymDHA()
//------------------------------------------------------------------------------
/**
 * Destructor.
 */
//------------------------------------------------------------------------------
IncAsymDHA::~IncAsymDHA()
{
}


//-------------------------------------
// Inherited methods from Parameter
//-------------------------------------

//------------------------------------------------------------------------------
// virtual bool Evaluate()
//------------------------------------------------------------------------------
/**
 * Evaluates value of the parameter.
 *
 * @return true if parameter value successfully evaluated; false otherwise
 */
//------------------------------------------------------------------------------
bool IncAsymDHA::Evaluate()
{
   mRealValue = OrbitData::GetIncAsymReal(INCASYM_DHA);    
    
   if (mRealValue == GmatOrbitConstants::ORBIT_REAL_UNDEFINED)
      return false;
   else
      return true;
}


//-------------------------------------
// methods inherited from GmatBase
//-------------------------------------

//------------------------------------------------------------------------------
// virtual GmatBase* Clone(void) const
//------------------------------------------------------------------------------
/**
 * Method used to create a copy of the object
 */
//------------------------------------------------------------------------------
GmatBase* IncAsymDHA::Clone(void) const
{
   return new IncAsymDHA(*this);
}


//==============================================================================
//                              IncAsymBVAZI
//==============================================================================
IncAsymBVAZI::IncAsymBVAZI(const std::string &name, GmatBase *obj)
   : OrbitReal(name, "IncomingBVAZI", obj, "B-vector AZI at Infinity of Incoming Asymptote", "Deg",
               GmatParam::COORD_SYS, INCASYM_BVAZI, true)
{
   mDepObjectName = "EarthMJ2000Eq";
   SetRefObjectName(Gmat::COORDINATE_SYSTEM, mDepObjectName);
   SetRequiresCelestialBodyCSOrigin(true);
   mIsAngleParam = true;
   mCycleType = GmatParam::ZERO_360;
}


//------------------------------------------------------------------------------
// IncAsymBVAZI(const IncAsymBVAZI &copy)
//------------------------------------------------------------------------------
/**
 * Copy constructor.
 *
 * @param <copy> the parameter to make copy of
 */
//------------------------------------------------------------------------------
IncAsymBVAZI::IncAsymBVAZI(const IncAsymBVAZI &copy)
   : OrbitReal(copy)
{
}


//------------------------------------------------------------------------------
// const IncAsymBVAZI& operator=(const IncAsymBVAZI &right)
//------------------------------------------------------------------------------
/**
 * Assignment operator.
 *
 * @param <right> the parameter to make copy of
 */
//------------------------------------------------------------------------------
IncAsymBVAZI& IncAsymBVAZI::operator=(const IncAsymBVAZI &right)
{
   if (this != &right)
      OrbitReal::operator=(right);

   return *this;
}


//------------------------------------------------------------------------------
// ~IncAsymBVAZI()
//------------------------------------------------------------------------------
/**
 * Destructor.
 */
//------------------------------------------------------------------------------
IncAsymBVAZI::~IncAsymBVAZI()
{
}


//-------------------------------------
// Inherited methods from Parameter
//-------------------------------------

//------------------------------------------------------------------------------
// virtual bool Evaluate()
//------------------------------------------------------------------------------
/**
 * Evaluates value of the parameter.
 *
 * @return true if parameter value successfully evaluated; false otherwise
 */
//------------------------------------------------------------------------------
bool IncAsymBVAZI::Evaluate()
{
   mRealValue = OrbitData::GetIncAsymReal(INCASYM_BVAZI);    
    
   if (mRealValue == GmatOrbitConstants::ORBIT_REAL_UNDEFINED)
      return false;
   else
      return true;
}


//-------------------------------------
// methods inherited from GmatBase
//-------------------------------------

//------------------------------------------------------------------------------
// virtual GmatBase* Clone(void) const
//------------------------------------------------------------------------------
/**
 * Method used to create a copy of the object
 */
//------------------------------------------------------------------------------
GmatBase* IncAsymBVAZI::Clone(void) const
{
   return new IncAsymBVAZI(*this);
}



//------------------------------------------------------------------------------
// IncAsymState::IncAsymState(const std::string &name, GmatBase *obj)
//------------------------------------------------------------------------------
IncAsymState::IncAsymState(const std::string &name, GmatBase *obj)
   : OrbitRvec6(name, "IncomingAsymptote", obj, "Incoming Asymptote State Vector",
                "", GmatParam::COORD_SYS)
{
   // Parameter member data
   mDepObjectName = "EarthMJ2000Eq";
   SetRefObjectName(Gmat::COORDINATE_SYSTEM, mDepObjectName);
   SetRequiresCelestialBodyCSOrigin(true);
   mIsPlottable = false;
}

//------------------------------------------------------------------------------
// IncAsymState::IncAsymState(const IncAsymState &copy)
//------------------------------------------------------------------------------
IncAsymState::IncAsymState(const IncAsymState &copy)
   : OrbitRvec6(copy)
{
}

//------------------------------------------------------------------------------
// IncAsymState& IncAsymState::operator=(const IncAsymState &right)
//------------------------------------------------------------------------------
IncAsymState& IncAsymState::operator=(const IncAsymState &right)
{
   if (this != &right)
      OrbitRvec6::operator=(right);

   return *this;
}

//------------------------------------------------------------------------------
// IncAsymState::~IncAsymState()
//------------------------------------------------------------------------------
IncAsymState::~IncAsymState()
{
}

//------------------------------------------------------------------------------
// bool IncAsymState::Evaluate()
//------------------------------------------------------------------------------
bool IncAsymState::Evaluate()
{
   mRvec6Value = GetIncAsymState();
   
   #if DEBUG_CARTESIAN_PARAM
   MessageInterface::ShowMessage
      ("EquinState::Evaluate() mRvec6Value =\n%s\n",
       mRvec6Value.ToString().c_str());
   #endif
   
   return mRvec6Value.IsValid(GmatOrbitConstants::ORBIT_REAL_UNDEFINED);
}

//------------------------------------------------------------------------------
// GmatBase* IncAsymState::Clone(void) const
//------------------------------------------------------------------------------
GmatBase* IncAsymState::Clone(void) const
{
   return new IncAsymState(*this);
}
