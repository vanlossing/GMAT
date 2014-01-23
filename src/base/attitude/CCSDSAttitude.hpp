//$Id:$
//------------------------------------------------------------------------------
//                                 CCSDSAttitude
//------------------------------------------------------------------------------
// GMAT: General Mission Analysis Tool.
//
// Copyright (c) 2002-2011 United States Government as represented by the
// Administrator of The National Aeronautics and Space Administration.
// All Other Rights Reserved.
//
// Author: Wendy C. Shoan/GSFC
// Created: 2013.11.26
//
/**
 * Class definition for the CCSDS-AEM (CCSDSAttitude) attitude class.
 * This class calls the CCSDS utilities to get the attitude data
 * for an object (currently, for spacecraft only).
 *
 * @note The time passed in for the epoch or passed into the methods
 *       should be an A1Mjd (though its type is currently Real).
 * @note Methods called to set/get parameter values will require/return angle
 *       values in degrees and rate values in degrees/sec.  All other methods
 *       assume/expect radians and radians/sec.
 */
//------------------------------------------------------------------------------

#ifndef CCSDSAttitude_hpp
#define CCSDSAttitude_hpp

#include "gmatdefs.hpp"
#include "GmatBase.hpp"
#include "Attitude.hpp"
#include "Rmatrix33.hpp"
#include "CCSDSAEMReader.hpp"


class GMAT_API CCSDSAttitude : public Attitude
{
public:
   /// Constructor
   CCSDSAttitude(const std::string &attName = "");
   /// copy constructor
   CCSDSAttitude(const CCSDSAttitude& att);
   /// operator =
   CCSDSAttitude& operator=(const CCSDSAttitude& att);
   /// destructor
   virtual ~CCSDSAttitude();

   /// Initialize the CCSDSAttitude attitude
   virtual bool Initialize();

   /// inherited from GmatBase
   virtual GmatBase* Clone() const;

protected:
   enum
   {
      CCSDSAttitudeParamCount = AttitudeParamCount,
   };

   CCSDSAEMReader *reader;

   virtual void ComputeCosineMatrixAndAngularVelocity(Real atTime);

private:
   // Default constructor - not implemented
   //CCSDSAttitude(); // MSVC compiler gives warning: multiple default constructors specified

};
#endif /*CCSDSAttitude_hpp*/