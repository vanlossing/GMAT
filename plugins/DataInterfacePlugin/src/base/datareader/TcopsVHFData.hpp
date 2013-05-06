//$Id$
//------------------------------------------------------------------------------
//                           TcopsVHFData
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
// Created: May 3, 2013
/**
 * 
 */
//------------------------------------------------------------------------------

#ifndef TcopsVHFData_hpp
#define TcopsVHFData_hpp

#include "FileReader.hpp"

class TcopsVHFData: public FileReader
{
public:
   TcopsVHFData(const std::string& theTypeName, const std::string& theName);
   virtual ~TcopsVHFData();
   TcopsVHFData(const TcopsVHFData& vhf);
   TcopsVHFData& operator=(const TcopsVHFData& vhf);

   const StringArray& GetSupportedFieldNames();

protected:



   /// Parameter IDs
   enum
   {
      TVHFDataParamCount = FileReaderParamCount,
   };

//   /// TCOPS VHF parameter types
//   static const Gmat::ParameterType PARAMETER_TYPE[TVHFDataParamCount - FileReaderParamCount];
//   /// TCOPS VHF parameter labels
//   static const std::string PARAMETER_LABEL[TVHFDataParamCount - FileReaderParamCount];
};

#endif /* TcopsVHFData_hpp */