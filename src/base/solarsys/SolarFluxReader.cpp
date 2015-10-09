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

#include "MessageInterface.hpp"

//#define DEBUG_FILE_INDEXING


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
SolarFluxReader::SolarFluxReader():
   obsFileName       (""),
   predictFileName   (""),
   warnEpochBefore   (true),
   warnEpochAfter    (true)
{
   if (!obsFluxData.empty())
      obsFluxData.clear();

   if (!predictFluxData.empty())
      predictFluxData.clear();

   beg_ObsTag = "BEGIN OBSERVED";
   end_ObsTag = "END OBSERVED";
   begObs = endObs = begData = -1;
   
   line = new char[256];
   memset(line, 0, 256);
   
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
   delete [] line;
}


//------------------------------------------------------------------------------
// SolarFluxReader(const SolarFluxReader &)
//------------------------------------------------------------------------------
/**
 * copy constructor
 */
//------------------------------------------------------------------------------
SolarFluxReader::SolarFluxReader(const SolarFluxReader &sfr) :
   warnEpochBefore   (true),
   warnEpochAfter    (true)
{
   obsFileName = sfr.obsFileName;
   predictFileName = sfr.predictFileName;
   obsFluxData = sfr.obsFluxData;
   predictFluxData = sfr.predictFluxData;
   beg_ObsTag = sfr.beg_ObsTag;
   end_ObsTag = sfr.end_ObsTag;
   begObs = sfr.begObs;
   endObs = sfr.endObs;
   begData = sfr.begData;
   line = sfr.line;

   if (sfr.line != NULL)
   {
      line = new char[256];
      strcpy(line, sfr.line);
   }
}


//------------------------------------------------------------------------------
// SolarFluxReader& SolarFluxReader::operator=(const SolarFluxReader& sfr)
//------------------------------------------------------------------------------
/**
 * assignment operator
 */
//------------------------------------------------------------------------------
SolarFluxReader& SolarFluxReader::operator=(const SolarFluxReader &sfr)
{
   if (this == &sfr)
      return *this;

   obsFileName = sfr.obsFileName;
   predictFileName = sfr.predictFileName;
   obsFluxData = sfr.obsFluxData;
   predictFluxData = sfr.predictFluxData;
   beg_ObsTag = sfr.beg_ObsTag;
   end_ObsTag = sfr.end_ObsTag;
   begObs = sfr.begObs;
   endObs = sfr.endObs;
   begData = sfr.begData;
   line = sfr.line;
   
   warnEpochBefore = true;
   warnEpochAfter = true;

   return *this;
}


//------------------------------------------------------------------------------
// SolarFluxReader::FluxData& SolarFluxReader::FluxData::operator=()
//------------------------------------------------------------------------------
/**
* Operator=() for FluxData data structure
*/
//------------------------------------------------------------------------------
SolarFluxReader::FluxData& SolarFluxReader::FluxData::operator=(const SolarFluxReader::FluxData &fD)
{
   if (this == &fD)
      return *this;

   epoch = fD.epoch;
   for (Integer j = 0; j < 8; j++)
   {
      kp[j] = fD.kp[j];
      ap[j] = fD.ap[j];
   }
   apAvg = fD.apAvg;
   adjF107 = fD.adjF107;
   adjCtrF107a = fD.adjCtrF107a;
   obsF107 = fD.obsF107;
   obsCtrF107a = fD.obsCtrF107a;
   for (Integer j = 0; j < 9; j++)
      F107a[j] = fD.F107a[j];
   for (Integer j = 0; j < 3; j++)
      apSchatten[j] = fD.apSchatten[j];
   index = fD.index;
   id = fD.id;

   return *this;
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
   if (!obsFileName.empty())
   {
      inObs.open(obsFileName.c_str(), std::ios_base::in);
   }
   if (!predictFileName.empty())
   {
      inPredict.open(predictFileName.c_str(), std::ios_base::in);
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
bool SolarFluxReader::LoadFluxData(const std::string &obsFile, const std::string &predictFile)
{
   if (!obsFile.empty())
      obsFileName = obsFile;
   if (!predictFile.empty())
      predictFileName = predictFile;

   // Open the files to load
   Open();
   
   if (obsFileName != "")
   {
      if (inObs.is_open())
      {
         inObs.seekg(0, std::ios_base::beg);

         while (true)
         {
            inObs.getline(line, 256);

            if (std::string(line).find(beg_ObsTag) != std::string::npos)
            {
               begObs = inObs.tellg();
            }
            else if (std::string(line).find(end_ObsTag) != std::string::npos)
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
         throw SolarSystemException("SolarFluxReader: Historic/Observed File " +
               obsFileName + " could not be opened.\n");
      }
   }

   if (predictFileName != "")
   {
      if (inPredict.is_open())
      {
         inPredict.seekg(0, std::ios_base::beg);
         std::string strLine = "";

         while (true)
         {
            inPredict.getline(line, 256);

            strLine = line;
            // if we read the line with "Nominal Timing" and "Early Timing" content, continue reading two more lines to get
            // into the data lines.
            if (strLine.find("NOMINAL TIMING") != std::string::npos && strLine.find("EARLY TIMING") != std::string::npos)
            {
               inPredict.getline(line, 256);
               inPredict.getline(line, 256);

               begData = inPredict.tellg();

               break;
            }
         }

         LoadPredictData();
      }
      else
      {
         //throw an exception
         throw SolarSystemException("SolarFluxReader: The Schatten predict "
               "file " + predictFileName + " could not be opened.\n");
      }
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
   bool ret = false;
   if (inObs.is_open())
   {
      inObs.close();
      ret = true;
   }
   if (inPredict.is_open())
   {
      inPredict.close();
      ret = true;
   }

   return ret;
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

   // FORMAT(I4, I3, I3, I5, I3, 8I3, I4, 8I4, I4, F4.1, I2, I4, F6.1, I2, 5F6.1)
   //-------------------------------------------------------------------------------------------Adj     Adj   Adj   Obs   Obs   Obs---
   //yy mm dd BSRN ND Kp Kp Kp Kp Kp Kp Kp Kp Sum Ap  Ap  Ap  Ap Ap  Ap  Ap  Ap  Avg Cp C9 ISN F10.7 Q Ctr81 Lst81 F10.7 Ctr81 Lst81
   //--------------------------------------------------------------------------------------------------------------------------------- 
   // Divide each line to two sections : Geomagnetics (s1) and F107 (s2) data 
   char s1[92], s2[38];
   const char * format = "%92c %38c";
   // first section
   //char s11[4], s12[2], s13[2], *s6[8], *s8[8], s9[3];
   char s11[5], s12[3], s13[3], *s6[8], *s8[8], s9[4];      // Increasing the size of arrays s11, s12, s13, and s9 by 1 in order to store end of string
   //const char * format1 = "%4c %2c %2c %*7c %2c %2c %2c %2c %2c %2c %2c %2c %*3c %3c %3c %3c %3c %3c %3c %3c %3c %3c";
   const char * format1 = "%4c %2c %2c %*5c %*3c %2c %2c %2c %2c %2c %2c %2c %2c %*3c %3c %3c %3c %3c %3c %3c %3c %3c %3c";

   s11[4] = s12[2] = s13[2] = s9[3] = '\0';      // set the last array's element to be end of string char
   for (Integer i = 0; i < 8; i++)
   {
	   //s6[i] = new char[2];
	   //memset(s6[i], 0, 2);
      s6[i] = new char[3];               // Increasing the size of array s6[i] by 1 in order to store end of string
      s6[i][2] = '\0';                   // set the last array's element to be end of string char
	   //s8[i] = new char[3];
	   //memset(s8[i], 0, 3);
	   s8[i] = new char[4];               // Increasing the size of array s8[i] by 1 in order to store end of string
	   s8[i][3] = '\0';                   // set the last array's element to be end of string char
   }
	  
   // second section
   //char s21[5], *s22[5];
   char s21[6], *s22[5];                 // Increasing the size of arrays s21 by 1 in order to store end of string
   const char * format2 = "%5c %*1c %5c %5c %5c %5c %5c";

   s21[5] = '\0';                        // set the last array's element to be end of string char
   for (Integer i = 0; i < 5; i++)
   {
	   //s22[i] = new char[5];
	   //memset(s22[i], 0, 5);
	   s22[i] = new char[6];              // Increasing the size of array s22[i] by 1 in order to store end of string
	   s22[i][5] = '\0';                  // set the last array's element to be end of string char
   }

   inObs.seekg(begObs, std::ios_base::beg);
   while (true)
   {
      inObs.getline(line, 256);
      if (std::string(line).find(end_ObsTag) != std::string::npos)
            break; 
      
      if (strlen(line) > 8)
      {
		 // Read geomagnetic and F107 data
		 sscanf(line, format, s1, s2);
		 // s1 corresponds to geomagnetics data with formatted string (format1) defined above.
		 sscanf(s1, format1, s11, s12, s13, s6[0], s6[1], s6[2], s6[3], s6[4], s6[5], s6[6], s6[7], s8[0], s8[1], s8[2], s8[3], s8[4], s8[5], s8[6], s8[7], s9);
		 // s2 corresponds to F107 data with formatted string (format2) defined above.
		 sscanf(s2, format2, s21, s22[0], s22[1], s22[2], s22[3], s22[4]);

		 /*
		 std::istringstream buf(line);
		 std::istream_iterator<std::string> beg(buf), end;
		 std::vector<std::string> tokens(beg, end);

         Real mjd = ModifiedJulianDate(atoi(tokens[0].c_str()), atoi(tokens[1].c_str()), atoi(tokens[2].c_str()), hour, minute, sec);
         // because it starts from noon, we subtract it by 0.5 to move it back a half a day.
         fD.epoch = mjd - 0.5;

         // The CSSI fole conains Kp * 10, then rouded to an int.  Undo that here.
         for (Integer l=0; l<8; l++)
            fD.kp[l] = atof(tokens[tokens.size()-28+l].c_str()) / 10.0;
         for (Integer l=0; l<8; l++)
            fD.ap[l] = atof(tokens[tokens.size()-19+l].c_str());
         fD.apAvg = atof(tokens[tokens.size()-19+8].c_str());
         fD.adjF107 = atof(tokens[tokens.size()-7].c_str());
         fD.adjCtrF107a = atof(tokens[tokens.size()-5].c_str());
         fD.obsF107 = atof(tokens[tokens.size()-3].c_str());
         fD.obsCtrF107a = atof(tokens[tokens.size()-2].c_str());
		 */

		 FluxData fD;
		 Real mjd = ModifiedJulianDate((YearNumber)atof(s11), (MonthOfYear)atof(s12), (DayOfMonth)atof(s13), hour, minute, sec);
		 // because it starts from noon, we subtract it by 0.5 to move it back a half a day.
		 fD.epoch = mjd - 0.5;

		 // The CSSI fole conains Kp * 10, then rouded to an int. Undo that here.
		 for (Integer l = 0; l < 8; l++)
		 {
			 fD.kp[l] = atof(s6[l]) / 10.0;
			 //memset(s6[l], 0, 2);
		 }
		 for (Integer l = 0; l < 8; l++)
		 {
			 fD.ap[l] = atof(s8[l]);
			 //memset(s8[l], 0, 3);
		 }
		 fD.apAvg = atof(s9);
		 fD.adjF107 = atof(s21);
		 fD.adjCtrF107a = atof(s22[0]);
		 fD.obsF107 = atof(s22[2]);
		 fD.obsCtrF107a = atof(s22[3]);
		 //memset(s22[0], 0, 5);
		 //memset(s22[2], 0, 5);
		 //memset(s22[3], 0, 5);
		 //memset(s9, 0, 3);
		 //memset(s21, 0, 5);

         fD.index = -1;
         for (Integer l = 0; l<9; l++)
            fD.F107a[l] = -1;
         for (Integer l =0; l<3; l++)
            fD.apSchatten[l] = -1;
	
         obsFluxData.push_back(fD);

      }
   }

   //delete newed array
   for (Integer i = 0; i < 8; i++)
   {
	   if (s6[i] != NULL)
	   {
		   delete[] s6[i];
		   s6[i] = NULL;
	   }
	   if (s8[i] != NULL)
	   {
		   delete[] s8[i];
		   s8[i] = NULL;
	   }
   }

   //delete newed array
   for (Integer i = 0; i < 5; i++)
   {
	   if (s22[i] != NULL)
	   {
		   delete[] s22[i];
		   s22[i] = NULL;
	   }
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
   Integer hour = 0, minute = 0;
   Real sec = 0.0;

   inPredict.seekg(begData, std::ios_base::beg);

   while (!inPredict.eof())
   {
      inPredict.getline(line, 256);
      //last line in file may or may not have "END DATA" tag,
      // if it has, break 
      if(std::string(line).find("END_DATA") != std::string::npos)
         break;

      std::istringstream buf(line);
      std::istream_iterator<std::string> beg(buf), end;
      std::vector<std::string> tokens(beg, end);

      FluxData fD;
      Real mjd = ModifiedJulianDate(atoi(tokens[1].c_str()), atoi(tokens[0].c_str()), 0, hour, minute, sec); 
      // because it starts from noon, we subtract it by 0.5 to move it back a half a day.
      fD.epoch = mjd - 0.5;

      for (Integer l=0; l<3; l++)
         fD.F107a[l] = atof(tokens[l+2].c_str());
      fD.apSchatten[0] = atof(tokens[5].c_str());
      for (Integer l=0; l<3; l++)
         fD.F107a[l+3] = atof(tokens[l+6].c_str());
      fD.apSchatten[1] = atof(tokens[9].c_str());
      for (Integer l=0; l<3; l++)
         fD.F107a[l+6] = atof(tokens[l+10].c_str());
      fD.apSchatten[2] = atof(tokens[13].c_str());
      fD.index = -1;
      fD.id = -1;

      for (Integer l = 0; l<8; l++)
         fD.kp[l] = -1;
      for (Integer l=0; l<8; l++)
         fD.ap[l] = -1;
      fD.apAvg = -1;
      fD.adjF107 = -1;
      fD.adjCtrF107a = -1;
      fD.obsF107 = -1;
      fD.obsCtrF107a = -1;

      predictFluxData.push_back(fD);

   }

   std::vector<FluxData>::iterator it = predictFluxData.begin();
   it->index = 0;
   it->id = 0;
   for (it = it+1; it != predictFluxData.end(); it++)
   {
      it->index = (Integer) (it->epoch - (it-1)->epoch);
      it->index += (it-1)->index;
      it->id = (it-1)->id + 1;
   }

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
   Integer index;
   GmatEpoch epoch_1st;
   FluxData fD;

   // Requirement list in google docs:
   // HistoricAndNearTerm source is always given precedence if requested epoch
   // is in the historic data section.
   // Otherwise, predicted data is used from the predicted source.
   // If requested data is not on a file (regardless of the file type), 
   // use closest data from the requested epoch and issue a single warning message 
   // that data was not available so using closest data and from which file source.
   epoch_1st = obsFluxData.at(0).epoch;
   index = (Integer) (epoch - 0.5 - epoch_1st);

   if (index < 0)
   {
      // Warn the user that the epoch is too early
      if (warnEpochBefore)
      {
         MessageInterface::ShowMessage("Warning: Requested epoch for solar flux "
            "data is earlier than the starting epoch on the flux file.  GMAT "
            "is using the first file entry.\n");
         warnEpochBefore = false;
      }
      index = 0;
   }

   // if the requested epoch fall beyond the 
   // last item in the obsFluxData, then search in predictFluxData
   if (index >= (Integer) obsFluxData.size())
   {
      if ((predictFluxData.size() > 0))
      {
         epoch_1st = predictFluxData.at(0).epoch;
         index = (Integer) (epoch - epoch_1st) - 1;
      
         /// @todo: if earlier than 1st use 1st
         if (index < 0)
            throw SolarSystemException("\"SolarFluxReader::GetInputs()\" Index can not be less than zero.\n");
   
         std::vector<FluxData>::iterator it;
         for ( it = predictFluxData.begin(); it != predictFluxData.end(); ++it)
         {
            if ( index >= it->index && (it+1) != predictFluxData.end() && index < (it+1)->index)
            {
               fD = predictFluxData[it->id];
               break;
            }
            else if ( (it+1) == predictFluxData.end())
            {
               // Warn the user that the epoch is too late
               if (warnEpochAfter)
               {
                  MessageInterface::ShowMessage("Warning: Requested epoch for solar flux "
                     "data is later than the ending epoch on the flux file.  GMAT "
                     "is using the last file entry.\n");
                  warnEpochAfter = false;
               }
               fD = predictFluxData[it->id];
            }
         }
      }
      else // Off the CSSI file and there is no predict data read
      {
         // Warn the user that the epoch is too late
         if (warnEpochAfter)
         {
            MessageInterface::ShowMessage("Warning: Requested epoch for solar flux "
               "data is later than the ending epoch on the flux file.  GMAT "
               "is using the last file entry.\n");
            warnEpochAfter = false;
         }                                          // fix bug GMT-5304
         index = obsFluxData.size() - 1;            // fix bug GMT-5304
         fD = obsFluxData[index];                   // fix bug GMT-5304
         fD.index = index;                          // fix bug GMT-5304
         //}                                        // fix bug GMT-5304
      }
   }
   else
   {
      fD = obsFluxData[index];
      fD.index = index;
   }

   return fD;
}

//------------------------------------------------------------------------------
// PrepareApData(GmatEpoch epoch, Integer index, FluxData &fD)
//------------------------------------------------------------------------------
/**
 * Prepares the data used by the MSISE models
 *
 * Function that replaces the ap data with data MSISE models need, and the F10.7
 * obs value with the value from the previous day
 *
 * @param fD The data record that contains raw data for the requested epoch
 *
 * @return void
*/
//------------------------------------------------------------------------------
void SolarFluxReader::PrepareApData(SolarFluxReader::FluxData &fD, GmatEpoch epoch )
{
   #ifdef DEBUG_FILE_INDEXING
      MessageInterface::ShowMessage("Reading flux data; epoch = %12lf, "
         "fD.Epoch = %.12lf, index = %d\n", epoch, fD.epoch, fD.index);
   #endif
   
   Integer f107index = fD.index;
   
   if (fD.index < (Integer)obsFluxData.size())
   {
      // Fill in fD.ap so it contains these data:
      /*  (1) DAILY AP */
      /*  (2) 3 HR AP INDEX FOR CURRENT TIME */
      /*  (3) 3 HR AP INDEX FOR 3 HRS BEFORE CURRENT TIME */   
      /*  (4) 3 HR AP INDEX FOR 6 HRS BEFORE CURRENT TIME */
      /*  (5) 3 HR AP INDEX FOR 9 HRS BEFORE CURRENT TIME */
      /*  (6) AVERAGE OF EIGHT 3 HR AP INDICIES FROM 12 TO 33 HRS PRIOR */
      /*        TO CURRENT TIME */
      /*  (7) AVERAGE OF EIGHT 3 HR AP INDICIES FROM 36 TO 57 HRS PRIOR */
      /*        TO CURRENT TIME */
      Real fracEpoch = (epoch - 0.5) - fD.epoch;
      Integer subIndex = (Integer)floor(fracEpoch * 8);
      
      // F10.7 is measured at 8 pm (5pm before ???), so we use the current row for 
      // data from 8 am on the current day to 8 am the next day.  f107index is 
      // used to track that piece.
      if (fracEpoch < 8.0/24.0)
         f107index = (f107index > 0 ? f107index - 1 : 0);
      
      if (subIndex >= 8) // Off the end of the array
      {
         subIndex = 7;
      }

      Real apValues[32];
      Integer i = 0,j = 0;
      
      #ifdef DEBUG_FILE_INDEXING
         MessageInterface::ShowMessage("   subindex = %d\n", subIndex);
      #endif
      for (i = subIndex, j = 0; i >= 0; --i, ++j)
         apValues[j] = fD.ap[i];
      if (fD.index > 0)
      {
         FluxData fD_OneBefore = obsFluxData[fD.index - 1];
         for (i = 7; i >= 0; j++,i--)
            apValues[j] = fD_OneBefore.ap[i];
      }
      else
      {
         for (i = 7; i >= 0; j++,i--)
            apValues[j] = obsFluxData[0].ap[0];
      }
      if (fD.index > 1)
      {
         FluxData fD_TwoBefore = obsFluxData[fD.index - 2];
         for (i = 7; i >= 0; j++,i--)
            apValues[j] = fD_TwoBefore.ap[i];
      }
      else
      {
         for (i = 7; i >= 0; j++,i--)
            apValues[j] = obsFluxData[0].ap[0];
      }
      
      if ( fD.index > 2)
      {
         FluxData fD_ThreeBefore = obsFluxData[fD.index - 3];
         for (i = 7; i >= 0; j++,i--)
            apValues[j] = fD_ThreeBefore.ap[i];
      }
      else
      {
         for (i = 7; i >= 0; j++,i--)
            apValues[j] = obsFluxData[0].ap[0];
      }
            

      for (i = 0; i < 7; i++)
      {
         if (i == 0)
            fD.ap[i] = fD.apAvg;
         if (i >= 1 && i <= 4)
            fD.ap[i] = apValues[i-1];
         if (i == 5)
         {
            fD.ap[i] = 0;
            for (Integer l = 0; l < 8; l++)
               fD.ap[i] += apValues[i+l-1];
            fD.ap[i] /= 8.0;
         }
         if (i == 6)
         {
            fD.ap[i] = 0;
            for (Integer l = 8; l < 16; l++)
               if ( apValues[i+l-2] >= 0)
                  fD.ap[i] += apValues[i+l-2];
            fD.ap[i] /= 8.0;
         }
      }
      
      // Update the F10.7 data
      // Daily value from previous day
      fD.obsF107 = (f107index > 0 ? obsFluxData[f107index-1].obsF107 : 
                                    obsFluxData[f107index].obsF107);
      // Average value from detected day
      fD.obsCtrF107a = obsFluxData[f107index].obsCtrF107a;
   }

   return;
}


//------------------------------------------------------------------------------
// PrepareKpData(GmatEpoch epoch, Integer index, FluxData &fD)
//------------------------------------------------------------------------------
/**
 * Function that replaces the Kp data with data the Jacchia Roberts model needs
 *
 * @param fD The data record that contains raw data for the requested epoch
 *
 * @return void
*/
//------------------------------------------------------------------------------
void SolarFluxReader::PrepareKpData(SolarFluxReader::FluxData &fD, GmatEpoch epoch )
{
   Integer f107index = fD.index;
   if ((fD.index < (Integer)obsFluxData.size()) && ((fD.index - 2) >= 0))
   {
      FluxData fD_OneBefore = obsFluxData[fD.index - 1];

      // Fill in fD.kp[0] so it contains the reading at epoch - 6.7 Hrs, per
      // Vallado and Finkleman
      Real fracEpoch = (epoch - 0.5) - fD.epoch - 6.7/24.0;
      Integer subIndex = (Integer)floor(fracEpoch * 8);
      
      // F10.7 is measured at 8 pm (5pm before ???), so we use the current row for 
      // data from 8 am on the current day to 8 am the next day.  f107index is 
      // used to track that piece.
      if (fracEpoch < 8.0/24.0)
         f107index = (f107index > 0 ? f107index - 1 : 0);
      
      if (subIndex >= 8)
         subIndex = 7;

      if (subIndex > 0)
         fD.kp[0] = fD.kp[subIndex];

      if (subIndex < 0)
         fD.kp[0] = fD_OneBefore.kp[8+subIndex];

      fD.adjF107 = obsFluxData[f107index].adjF107;
      fD.adjCtrF107a = obsFluxData[f107index].adjCtrF107a;
   }

   return;
}
