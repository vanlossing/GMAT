//$Id: ITRFAxes.hpp 9513 2012-02-29 21:23:06Z tuandangnguyen $
//------------------------------------------------------------------------------
//                                  ITRFAxes
//------------------------------------------------------------------------------
// GMAT: General Mission Analysis Tool.
//
// Copyright (c) 2002-2011 United States Government as represented by the
// Administrator of The National Aeronautics and Space Administration.
// All Other Rights Reserved.
//
// Developed jointly by NASA/GSFC and Thinking Systems, Inc. under 
// MOMS Task order 124.
//
// Author: Tuan Dang Nguyen (NASA/GSFC)
// Created: 2012/02/29
//
/**
 * Definition of the ITRFAxes class.
 *
 */
//------------------------------------------------------------------------------

#ifndef ITRFAxes_hpp
#define ITRFAxes_hpp

#include "gmatdefs.hpp"
#include "GmatBase.hpp"
#include "AxisSystem.hpp"
#include "DynamicAxes.hpp"
#include "EopFile.hpp"
//#include "DeFile.hpp"
//#include "ItrfCoefficientsFile.hpp"
//#include "CelestialBody.hpp"

#include "EopFile.hpp"
#include "IAUFile.hpp"
#include "Rmatrix33.hpp"

class GMAT_API ITRFAxes : public DynamicAxes
{
public:

   // default constructor
   ITRFAxes(const std::string &itsName = "");
   // copy constructor
   ITRFAxes(const ITRFAxes &itrfAxes);
   // operator = for assignment
   const ITRFAxes& operator=(const ITRFAxes &itrfAxes);
   // destructor
   virtual ~ITRFAxes();
   
   // methods to set files for reading
   // 3 are needed:
   //    leap second file (NOTE - this should be done in the utiltities!!)
   //    EOP file containing polar motion (x,y) and UT1-UTC offset
   //    coefficient file containing nutation and planetary coeffifients
   
   virtual GmatCoordinate::ParameterUsage UsesEopFile(const std::string &forBaseSystem = "ICRF") const;
   virtual GmatCoordinate::ParameterUsage UsesItrfFile() const;
   virtual GmatCoordinate::ParameterUsage UsesNutationUpdateInterval() const;
   
// method to initialize the data
   virtual bool Initialize();
   
   // all classes derived from GmatBase must supply this Clone method;
   // this must be implemented in the 'leaf' classes
   virtual GmatBase*       Clone(void) const;
   // We need to override this method from CoordinateBase
   // to check for a CelestialBody origin only
   virtual bool            SetRefObject(GmatBase *obj, const Gmat::ObjectType type,
                                        const std::string &name = "");

   // Parameter access methods - overridden from GmatBase
   /*
   virtual std::string     GetParameterText(const Integer id) const;     
   virtual Integer         GetParameterID(const std::string &str) const; 
   virtual Gmat::ParameterType
                           GetParameterType(const Integer id) const;
   virtual std::string     GetParameterTypeString(const Integer id) const;
   virtual std::string     GetStringParameter(const Integer id) const;
   virtual bool            SetStringParameter(const Integer id, 
                                              const std::string &value);
   virtual std::string     GetStringParameter(const std::string &label) const;
   virtual bool            SetStringParameter(const std::string &label, 
                                              const std::string &value);
    */

//protected:
public:
   Rmatrix33 R1(Real angle);
   Rmatrix33 R2(Real angle);
   Rmatrix33 R3(Real angle);
   Rmatrix33 Skew(Rvector3 vec);

   enum
   {
      ITRFAxesParamCount = DynamicAxesParamCount,
   };
   
   //static const std::string PARAMETER_TEXT[ITRFAxesParamCount - 
   //                                        DynamicAxesParamCount];
   
   //static const Gmat::ParameterType PARAMETER_TYPE[ITRFAxesParamCount - 
   //                                                DynamicAxesParamCount];
   
   virtual void CalculateRotationMatrix(const A1Mjd &atEpoch,
                                        bool forceComputation = false);

//   DeFile                   *de;
   Real                     prevEpoch;
   Real                     prevUpdateInterval;
   Real                     prevOriginUpdateInterval;
   Gmat::RotationDataSource prevLunaSrc;

   IAUFile*				iauFile;
//   EopFile*			eopFile;

};
#endif // ITRFAxes_hpp