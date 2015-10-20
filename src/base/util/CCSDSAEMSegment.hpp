//------------------------------------------------------------------------------
//                                  CCSDSAEMSegment
//------------------------------------------------------------------------------
// GMAT: General Mission Analysis Tool
//
// Copyright (c) 2002 - 2015 United States Government as represented by the
// Administrator of the National Aeronautics and Space Administration.
// All Other Rights Reserved.
//
// Author: Wendy Shoan / NASA
// Created: 2013.11.19
//
/**
 * Stores, validates, and manages meta data and real data read from, or
 * to be written to, a CCSDS Attitude Ephemeris Message (AEM) file.
 * This is the attitude base class from which other attitude classes must
 * derive (e.g. a class to handle quaternion segments).
 * This class currently handles the following meta data fields:
 *    REF_FRAME_A
 *    REF_FRAME_B
 *    ATTITUDE_DIR
 *    ATTITUDE_TYPE
 *    INTERPOLATION_METHOD
 */
//------------------------------------------------------------------------------
#ifndef CCSDSAEMSegment_hpp
#define CCSDSAEMSegment_hpp

#include "gmatdefs.hpp"
#include "Rvector.hpp"
#include "Rmatrix33.hpp"
#include "CCSDSEMSegment.hpp"

class GMAT_API CCSDSAEMSegment : public CCSDSEMSegment
{
public:
   /// class methods
   CCSDSAEMSegment(Integer segNum);
   CCSDSAEMSegment(const CCSDSAEMSegment &copy);
   CCSDSAEMSegment& operator=(const CCSDSAEMSegment &copy);

   virtual ~CCSDSAEMSegment();

   virtual Rmatrix33        GetState(Real atEpoch) = 0;

   virtual bool             Validate(bool checkData = true);
   // GetState should be added to leaf classes, returning the
   // necessary state data

   virtual bool             SetMetaData(const std::string &fieldName,
                                        const std::string &value);

   /// Returns true of the rotation is inertial-to-body; false
   /// if the rotation is body-to-inertial
   virtual bool             IsRotationInertialToBody();


protected:
   // Required metadata fields for an Attitude Ephemeris Message
   // (InterpolationMethod handled by parent class; validated here)
   std::string refFrameA;
   std::string refFrameB;
   std::string attitudeDirection;
   std::string attitudeType;

   /// Optional (stored but not currently used) fields
   /// none

   // other data
   /// flag indicating whether or not the rotation is inertial-to-body;
   /// if false, the rotation for the data on the file is body-to-inertial
   bool        inertialToBody;
};

#endif // CCSDSAEMSegment_hpp
