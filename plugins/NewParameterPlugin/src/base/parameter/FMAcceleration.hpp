//$Id$
//------------------------------------------------------------------------------
//                             File: EnvParameters.hpp
//------------------------------------------------------------------------------
// GMAT: General Mission Analysis Tool
//
// Copyright (c) 2002 - 2015 United States Government as represented by the
// Administrator of The National Aeronautics and Space Administration.
// All Other Rights Reserved.
//
// Developed jointly by NASA/GSFC and Thinking Systems, Inc. under the FDSS
// contract task order 28
//
// Author: Darrel Conway
// Created: 2013/05/29
//
/**
 * Declares ODEModel based acceleration parameter class.
 */
//------------------------------------------------------------------------------
#ifndef FMAcceleration_hpp
#define FMAcceleration_hpp

#include "newparameter_defs.hpp"
#include "OdeReal.hpp"

//==============================================================================
//                              FMAcceleration
//==============================================================================
/**
 * Declares atmospheric density parameter class.
 */
//------------------------------------------------------------------------------

class NEW_PARAMETER_API FMAcceleration : public OdeReal
{
public:

   FMAcceleration(const std::string &name = "", GmatBase *obj = NULL);
   FMAcceleration(const FMAcceleration &copy);
   const FMAcceleration& operator=(const FMAcceleration &right);
   virtual ~FMAcceleration();

   // methods inherited from Parameter
   virtual bool Evaluate();

   // methods inherited from GmatBase
   virtual GmatBase* Clone() const;
   
protected:

};

class NEW_PARAMETER_API FMAccelerationX : public OdeReal
{
public:

   FMAccelerationX(const std::string &name = "", GmatBase *obj = NULL);
   FMAccelerationX(const FMAccelerationX &copy);
   const FMAccelerationX& operator=(const FMAccelerationX &right);
   virtual ~FMAccelerationX();

   // methods inherited from Parameter
   virtual bool Evaluate();

   // methods inherited from GmatBase
   virtual GmatBase* Clone() const;

protected:

};

class NEW_PARAMETER_API FMAccelerationY : public OdeReal
{
public:

   FMAccelerationY(const std::string &name = "", GmatBase *obj = NULL);
   FMAccelerationY(const FMAccelerationY &copy);
   const FMAccelerationY& operator=(const FMAccelerationY &right);
   virtual ~FMAccelerationY();

   // methods inherited from Parameter
   virtual bool Evaluate();

   // methods inherited from GmatBase
   virtual GmatBase* Clone() const;

protected:

};

class NEW_PARAMETER_API FMAccelerationZ : public OdeReal
{
public:

   FMAccelerationZ(const std::string &name = "", GmatBase *obj = NULL);
   FMAccelerationZ(const FMAccelerationZ &copy);
   const FMAccelerationZ& operator=(const FMAccelerationZ &right);
   virtual ~FMAccelerationZ();

   // methods inherited from Parameter
   virtual bool Evaluate();

   // methods inherited from GmatBase
   virtual GmatBase* Clone() const;

protected:

};

#endif //FMAcceleration_hpp
