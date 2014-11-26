//$Id$
//------------------------------------------------------------------------------
//                            SolarFluxReader
//------------------------------------------------------------------------------
// GMAT: General Mission Analysis Tool.
//
// Copyright (c) 2002-2011 United States Government as represented by the
// Administrator of The National Aeronautics and Space Administration.
// All Other Rights Reserved.
//
// Author: Farideh Farahnak
// Created: 2014/11/14
//
// Developed jointly by NASA/GSFC and Thinking Systems, Inc. under contract
// FDSS II.
//
/**
 * SolarFluxReader class will be used by the AtomsphereModel class.
 */
//------------------------------------------------------------------------------

#include "SolarFluxReader.hpp"
#include "SolarSystemException.hpp"


//------------------------------------------------------------------------------
// Public Methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// SolarFluxReader()
//------------------------------------------------------------------------------
/**
 * Constructor
 */
//------------------------------------------------------------------------------
SolarFluxReader::SolarFluxReader()
{
   if (!obsFluxData.empty())
      obsFluxData.clear();

   if (!predictFluxData.empty())
      predictFluxData.clear();

   obsFileName = NULL;
   predictFileName = NULL;
   beg_ObsTag = "BEGIN OBSERVED";
   end_ObsTag = "END OBSERVED";
   begObs = endObs = -1;
   line = new char[256];;
}


//------------------------------------------------------------------------------
// ~SolarFluxReader()
//------------------------------------------------------------------------------
/**
 * Destructor
 */
//------------------------------------------------------------------------------
SolarFluxReader::~SolarFluxReader(void)
{
   delete[] line;
}


//------------------------------------------------------------------------------
// bool Open()
//------------------------------------------------------------------------------
/**
 * Opens both Observed and Schatten input file streams.
 *
 * This method will open input file streams
 * 
 * @param none
 *
 * @return bool
 */
//------------------------------------------------------------------------------
bool SolarFluxReader::Open()
{
   if (obsFileName != NULL)
   {
      inObs.open(obsFileName, std::ios_base::in);
   }
   if (predictFileName != NULL)
   {
      inPredict.open(predictFileName, std::ios_base::in);
   }

   return true;
}


//------------------------------------------------------------------------------
// bool LoadFluxData()
//------------------------------------------------------------------------------
/**
 * Loads both Observed and Schatten input file data.
 *
 * This method will load input file data.
 * 
 * @param Observed file name and Predicted(Schatten) file name.
 *
 * @return bool
 */
//------------------------------------------------------------------------------
bool SolarFluxReader::LoadFluxData(const char *obsFileName, const char *predictFileName)
{
   if (obsFileName != NULL)
      this->obsFileName = obsFileName;
   if (predictFileName != NULL)
      this->predictFileName = predictFileName;

   // Open the files to load
   Open();
   
   if (inObs.is_open())
   {
      inObs.seekg(0, std::ios_base::beg);
  
      while (true)
      {
         inObs.getline(line, 256, '\n');
      
         if (strcmp(line, beg_ObsTag) == 0)
         {
            begObs = inObs.tellg();
         }
         else if (strcmp(line, end_ObsTag) == 0)
         {
            endObs = inObs.tellg();
            endObs = endObs - strlen(line) - 2 ;
            break;
         }
      }
     
      LoadObsData();
   }
   else
   {
      //throw an exception
      throw SolarSystemException("SolarFluxReader: Historic/Observed File is NOT opened.\n");
   }

   if (inPredict.is_open())
   {
      inPredict.seekg(0, std::ios_base::beg);
      std::string strLine = "";

      while (true)
      {
         inPredict.getline(line, 256, '\n');
         
         strLine = line;
         // if we read the line with "Nominal Timing" and "Early Timing" content, continue reading two more lines to get
         // into the data lines.
         if (strLine.find("NOMINAL TIMING") != std::string::npos && strLine.find("EARLY TIMING") != std::string::npos)
         {
            inPredict.getline(line, 256, '\n');
            inPredict.getline(line, 256, '\n');
            begData = inPredict.tellg();

            break;
         }
      }

      LoadPredictData();
   }
   else
   {
      //throw an exception
      throw SolarSystemException("SolarFluxReader: Predicted File is NOT opened.\n");
   }


   return(Close());
}


//------------------------------------------------------------------------------
// bool Close()
//------------------------------------------------------------------------------
/**
 * Closes both Observed and Schatten input file streams.
 *
 * This method will close input file streams
 * 
 * @param none
 *
 * @return bool
 */
//------------------------------------------------------------------------------
bool SolarFluxReader::Close()
{
   if (inObs.is_open())
      inObs.close();
   if (inPredict.is_open())
      inPredict.close();

   return true;
}


//------------------------------------------------------------------------------
// bool LoadObsData()
//------------------------------------------------------------------------------
/**
 * Loads Observed input file data.
 *
 * This method will load Observed input file data.
 * 
 * @param none
 *
 * @return bool
 */
//------------------------------------------------------------------------------
bool SolarFluxReader::LoadObsData()
{
   Integer hour = 0, minute = 0;
   Real sec = 0.0;

   inObs.seekg(begObs, std::ios_base::beg);
   while (true)
   {
      inObs.getline(line, 256, '\n');
      std::istringstream buf(line);
      std::istream_iterator<std::string> beg(buf), end;
      std::vector<std::string> tokens(beg, end);

      FluxData fD;
      Real mjd = ModifiedJulianDate(atoi(tokens[0].c_str()), atoi(tokens[1].c_str()), atoi(tokens[2].c_str()), hour, minute, sec); 
      // because it starts from noon, we subtract it by 0.5 to move it back a half a day.
      fD.epoch = mjd - 0.5;
    
      for (Integer l=0; l<8; l++)
         fD.kp[l] = atof(tokens[tokens.size()-28+l].c_str());
      for (Integer l=0; l<8; l++)
         fD.ap[l] = atof(tokens[tokens.size()-19+l].c_str());
      fD.adjF107 = atof(tokens[tokens.size()-7].c_str());
      fD.adjCtrF107a = atof(tokens[tokens.size()-5].c_str());
      fD.obsF107 = atof(tokens[tokens.size()-3].c_str());
      fD.obsCtrF107a = atof(tokens[tokens.size()-2].c_str());

      obsFluxData.push_back(fD);

      if (inObs.tellg() == endObs)
         break;
   }

   return true;
}


//------------------------------------------------------------------------------
// bool LoadPredictData()
//------------------------------------------------------------------------------
/**
 * Loads Predict input file data.
 *
 * This method will load Predict input file data.
 * 
 * @param none
 *
 * @return bool
 */
//------------------------------------------------------------------------------
bool SolarFluxReader::LoadPredictData()
{ 
   return true;
}

//------------------------------------------------------------------------------
// bool GetInputs()
//------------------------------------------------------------------------------
/**
 * Gets Observed or Predicted input file data.
 *
 * This method will get Observed or Predicted input file data.
 * 
 * @param float
 *
 * @return bool
 */
//------------------------------------------------------------------------------
SolarFluxReader::FluxData SolarFluxReader::GetInputs(GmatEpoch epoch)
{
   GmatEpoch epoch_1st = obsFluxData.at(0).epoch;
   
   FluxData fD;
   // do some calculation off the epoch passed to index to the obsFluxData

   return fD;
}