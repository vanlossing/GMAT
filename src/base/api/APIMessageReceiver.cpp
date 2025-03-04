//------------------------------------------------------------------------------
//                           APIMessageReceiver
//------------------------------------------------------------------------------
// GMAT: General Mission Analysis Tool
//
// Copyright (c) 2002 - 2020 United States Government as represented by the
// Administrator of the National Aeronautics and Space Administration.
// All Other Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License"); 
// You may not use this file except in compliance with the License. 
// You may obtain a copy of the License at:
// http://www.apache.org/licenses/LICENSE-2.0. 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either 
// express or implied.   See the License for the specific language
// governing permissions and limitations under the License.
//
// Developed jointly by NASA/GSFC and Thinking Systems, Inc. under the FDSS II
// contract, task order 08.
//
// Author: Darrel Conway, based on the ConsoleMessageReceiver
// Created: 2019/08/08
//
/**
 * Declares operations on messages reported to log files for API users.
 */
//------------------------------------------------------------------------------
#include "APIMessageReceiver.hpp"

#include <stdarg.h>                // for va_start(), va_end()
#include <stdio.h>                 // for vsprintf()
#include <string.h>                // for strlen()
#include <iostream>                // for cout, endl
#include <fstream>
//#include <sstream>
#include <queue>                   // for queue
#include "MessageInterface.hpp"    // for MessageInterface functions
#include "BaseException.hpp"
#include "APIException.hpp"
#include "FileManager.hpp"         // for GetFullPathname()
#include "FileUtil.hpp"            // for ParsePathName()
#include "GmatGlobal.hpp"          // for RunBachMode()

// Let's report the run epoch to the log
#include <sstream>
#include <chrono>
#include <ctime>

#include <cstdlib>                  // For GCC4.4 support

//#define DEBUG_SET_LOG_FILE

//---------------------------------
//  static data
//---------------------------------
APIMessageReceiver* APIMessageReceiver::theInstance = NULL;

//---------------------------------
//  public methods
//---------------------------------


//------------------------------------------------------------------------------
// APIMessageReceiver* Instance()
//------------------------------------------------------------------------------
/**
 * Singleton accessor method
 * 
 * This method creates the APIMessageReceiver singleton if it has not been
 * constructed, and returns the singleton instance.
 * 
 * @return The APIMessageReceiver instance.
 */
//------------------------------------------------------------------------------
APIMessageReceiver* APIMessageReceiver::Instance()
{
   if (theInstance == NULL)
      theInstance = new APIMessageReceiver;
   
   return theInstance;
}

//------------------------------------------------------------------------------
//  void ShowMessage(const std::string &msgString)
//------------------------------------------------------------------------------
/**
 * Displays a message passed in as an std::string.
 * 
 * This method sends the message to the user's console and to the log file by 
 * calling the variable argument method, ShowMessage(const char *msg, ...).
 * 
 * @param msgString The message that is displayed.
 */
//------------------------------------------------------------------------------
void APIMessageReceiver::ShowMessage(const std::string &msg)
{
   ShowMessage(msg.c_str());
}

//------------------------------------------------------------------------------
//  void ShowMessage(const char *msg, ...)
//------------------------------------------------------------------------------
/**
 * Displays a message passed in as a char* and a variable argument list.
 * 
 * @param msg The message, possibly including markers for variable argument 
 *            substitution.
 * @param ... The optional list of parameters that are inserted into the msg 
 *            string.
 */
//------------------------------------------------------------------------------
void APIMessageReceiver::ShowMessage(const char *msg, ...)
{
   short    ret;
   short    size;
   va_list  marker;
   char     *msgBuffer;
   
   // msg is vsprintf format
   // actual max message length is MAX_MESSAGE_LENGTH
   size = strlen(msg) + MAX_MESSAGE_LENGTH;
   
   // Note: 'new' throws an exception of type std::bad_alloc on failure.
   // (Note that if an exception is thrown, no memory will have been
   // allocated, so there will be no memory leak.)
   msgBuffer = new char[size];

   // For older C++ compilers, duplicate that behavior by hand.
   if (!msgBuffer)
      throw std::bad_alloc();

   // Process the message
   va_start(marker, msg);
   ret = vsprintf(msgBuffer, msg, marker);
   va_end(marker);
   
   LogMessage(std::string(msgBuffer));
   delete [] msgBuffer;
}

//------------------------------------------------------------------------------
//  void PopupMessage(Gmat::MessageType msgType, const std::string &msg)
//------------------------------------------------------------------------------
/**
 * Pops up a message in a message box.
 * 
 * This method logs informational messages directed at pop-up message boxes.  
 * The Console application does not support pop-ups, so the message cannot be 
 * shown as a pop-up.
 * 
 * This method calls the variable argument version of the method to perform the 
 * actual logging.
 * 
 * @param msgType The type of message that is displayed, selected from the set
 *                {ERROR_, WARNING_, INFO_} enumerated in the Gmat namespace.
 * @param msg The message.
 */
//------------------------------------------------------------------------------
void APIMessageReceiver::PopupMessage(Gmat::MessageType msgType,
      const std::string &msg)
{
   popupMessage = msg;
   messageType = msgType;
   
   PopupMessage(msgType, msg.c_str());
}

//------------------------------------------------------------------------------
//  void PopupMessage(Gmat::MessageType msgType, const char *msg, ...)
//------------------------------------------------------------------------------
/**
 * Pops up a message in a message box.
 * 
 * This method logs informational messages directed at pop-up message boxes.  
 * The Console application does not support pop-ups, so the message cannot be 
 * shown as a pop-up.
 * 
 * @param msgType The type of message that is displayed, selected from the set
 *                {ERROR_, WARNING_, INFO_} enumerated in the Gmat namespace.
 * @param msg The message, possibly including markers for variable argument 
 *            substitution.
 * @param ... The optional list of parameters that are inserted into the msg 
 *            string.
 */
//------------------------------------------------------------------------------
void APIMessageReceiver::PopupMessage(Gmat::MessageType msgType,
      const char *msg, ...)
{
   short    ret;
   short    size;
   va_list  marker;
   char     *msgBuffer;
   
   // msg is vsprintf format
   // actual max message length is MAX_MESSAGE_LENGTH
   size = strlen(msg) + MAX_MESSAGE_LENGTH;
   
   // Note: 'new' throws an exception of type std::bad_alloc on failure.
   // (Note that if an exception is thrown, no memory will have been
   // allocated, so there will be no memory leak.)
   msgBuffer = new char[size];

   // For older C++ compilers, duplicate that behavior by hand.
   if (!msgBuffer)
      throw std::bad_alloc();

   // Process the message
   va_start(marker, msg);
   ret = vsprintf(msgBuffer, msg, marker);
   va_end(marker);

   // if no EOL then append it
   if (msgBuffer[strlen(msgBuffer)-1] != '\n')
       msgBuffer[strlen(msgBuffer)] = '\n';

   LogMessage(std::string(msgBuffer) + "\n");
   delete [] msgBuffer;
}

//------------------------------------------------------------------------------
// std::string GetLogFileName()
//------------------------------------------------------------------------------
/**
 * Retrieves the fully qualified name of the log file.
 * 
 * @return The name of the log file, including path information.
 */
//------------------------------------------------------------------------------
std::string APIMessageReceiver::GetLogFileName()
{
   FileManager *fm = FileManager::Instance();
   std::string fname;
   try
   {
      if (logFileName == "")
      {
         fname = fm->GetFullPathname("LOG_FILE");
      }
      else
      {
         std::string outputPath = fm->GetPathname(FileManager::LOG_FILE);
         
         // add output path if there is no path
         if (logFileName.find("/") == logFileName.npos &&
             logFileName.find("\\") == logFileName.npos)
         {
            fname = outputPath + logFileName;
         }
      }
   }
   catch (BaseException &e)
   {
      ShowMessage
         ("**** ERROR **** " + e.GetFullMessage() + 
          "So setting log file name to GmatLog.txt");
      
      fname = "GmatLog.txt";
   }
   
   return fname;
}

//------------------------------------------------------------------------------
// void SetLogEnable(bool flag)
//------------------------------------------------------------------------------
/**
 * Turns logging on or off.
 * 
 * @param flag The new logging state -- true enables logging, and false disables 
 *             it.  The logging state is idempotent.
 */
//------------------------------------------------------------------------------
void APIMessageReceiver::SetLogEnable(bool flag)
{
   logEnabled = flag;
}

//------------------------------------------------------------------------------
// bool GetLogEnable()
//------------------------------------------------------------------------------
/**
* returns if logging is on or off.
*
*/
//------------------------------------------------------------------------------
bool APIMessageReceiver::GetLogEnable()
{
   return logEnabled;
}

//------------------------------------------------------------------------------
// void SetLogPath(const std::string &pathname, bool append = false)
//------------------------------------------------------------------------------
/*
 * Sets log file path with keeping log file name as is.
 *
 * @param  pathname  log file path name, such as "/newpath/test1/"
 * @param  append  true if appending log message (false)
 */
//------------------------------------------------------------------------------
void APIMessageReceiver::SetLogPath(const std::string &pathname,
      bool append)
{
   FileManager *fm = FileManager::Instance();
   std::string fname;
   try
   {
      std::string filename = fm->GetFilename(FileManager::LOG_FILE);
      fname = pathname + filename;
   }
   catch (BaseException &e)
   {
      ShowMessage
         ("**** ERROR **** " + e.GetFullMessage() + 
          "So setting log file name to GmatLog.txt");
      
      fname = "GmatLog.txt";
   }
   
   OpenLogFile(fname, append);
}

//------------------------------------------------------------------------------
// void SetLogFile(const std::string &filename)
//------------------------------------------------------------------------------
/*
 * Calls OpenLogFile() to set the log file path and name and then open the log.
 *
 * @param  filename  log file name, such as "/newpath/test1/GmatLog.txt"
 */
//------------------------------------------------------------------------------
void APIMessageReceiver::SetLogFile(const std::string &filename)
{
   #ifdef DEBUG_SET_LOG_FILE
      ShowMessage("Log file being set to: %s\n", filename.c_str());
   #endif
   std::string fname = filename;
   FileManager *fm   = FileManager::Instance();
   
   if (GmatFileUtil::ParsePathName(fname) == "")
   {
      std::string outPath = fm->GetFullPathname(FileManager::OUTPUT_PATH);
      fname = outPath + fname;
   }
   OpenLogFile(fname);
}

//------------------------------------------------------------------------------
// void OpenLogFile(const std::string &filename, bool append)
//------------------------------------------------------------------------------
/*
 * Sets the log file name and opens the log file.
 *
 * @param filename  log file name, such as "/newpath/test1/GmatLog.txt"
 * @param append  true if appending log message
 */
//------------------------------------------------------------------------------
void APIMessageReceiver::OpenLogFile(const std::string &filename,
      bool append)
{
   // Make sure we aren't stomping on an already existing non-log file
   if (logFileName != filename)
   {
      if (!IsValidLogFile(filename))
      {
         throw APIException(
               "ERROR - specified log file is not a valid log file.\n");
      }
   }
   
   logFileName = filename;
   
   if (logFile)
      CloseLogFile();
   
   if (append)
      logFile = fopen(logFileName.c_str(), "a");
   else
      logFile = fopen(logFileName.c_str(), "w");
   
   if (!logFile)
   {
      logFileName = "GmatAPILog.txt";
      
      if (append)
         logFile = fopen(logFileName.c_str(), "a");
      else
         logFile = fopen(logFileName.c_str(), "w");
   }

   if (logFile)
   {
      auto time = std::chrono::system_clock::now();
      std::stringstream epoch;
      std::time_t theTime = std::chrono::system_clock::to_time_t(time);
      epoch << std::ctime(&theTime);

      std::string logFileStart = GetLogFileText();
      fprintf(logFile, "%s %s %s\n\n",  logFileStart.c_str(), __DATE__, __TIME__);
      fprintf(logFile, "GMAT API Log: Created %s\n\n", epoch.str().c_str());
      logFileSet = true;
   }
}

//------------------------------------------------------------------------------
// void CloseLogFile()
//------------------------------------------------------------------------------
/**
 * Closes the log file.
 */
//------------------------------------------------------------------------------
void APIMessageReceiver::CloseLogFile()
{
   if (logFile)
   {
      auto time = std::chrono::system_clock::now();
      std::stringstream epoch;
      std::time_t theTime = std::chrono::system_clock::to_time_t(time);
      epoch << std::ctime(&theTime);

      std::string logFileStart = GetLogFileText();
      fprintf(logFile, "\nLog closed: %s\n\n", epoch.str().c_str());
      fclose(logFile);
   }
   
   logFile = NULL;
   logFileSet = false;
}


//------------------------------------------------------------------------------
//  void LogMessage(const std::string &msg)
//------------------------------------------------------------------------------
/**
 * Logs the message to the log file.
 * 
 * This method displays the input message on the console and writes it to the 
 * log file.
 * 
 * @param msg The message.
 */
//------------------------------------------------------------------------------
void APIMessageReceiver::LogMessage(const std::string &msg)
{
   if (echoMode)
      std::cout << msg;
   
   if (!logEnabled) return;

   if (logEnabled)
   {
      if (logFile == NULL)
      {
         SetLogFile(GetLogFileName());
      }
   }
   else if (!logFileSet)
   {
      OpenLogFile(logFileName);
   }
   
   if (logFile)
   {
      //std::string tempStr = GmatStringUtil::Replace(msg, "%", "%%");
      //fprintf(logFile, "%s", tempStr.c_str());
      fprintf(logFile, "%s", msg.c_str());
      fflush(logFile);
   }
}

//------------------------------------------------------------------------------
//  void LogMessage(const char *msg, ...)
//------------------------------------------------------------------------------
/**
 * Logs a variable argument formatted message to the log file.
 * 
 * This method displays the input message on the console and writes it to the 
 * log file.
 * 
 * @param msg The message, possibly including markers for variable argument 
 *            substitution.
 * @param ... The optional list of parameters that are inserted into the msg 
 *            string.
 */
//------------------------------------------------------------------------------
void APIMessageReceiver::LogMessage(const char *msg, ...)
{
   if (!logEnabled) return;

   short    ret;
   short    size;
   va_list  marker;
   char     *msgBuffer;
   
   // msg is vsprintf format
   // actual max message length is MAX_MESSAGE_LENGTH
   size = strlen(msg) + MAX_MESSAGE_LENGTH;
   
   // Note: 'new' throws an exception of type std::bad_alloc on failure.
   // (Note that if an exception is thrown, no memory will have been
   // allocated, so there will be no memory leak.)
   msgBuffer = new char[size];

   // For older C++ compilers, duplicate that behavior by hand.
   if (!msgBuffer)
      throw std::bad_alloc();

   // Process the message
   va_start(marker, msg);
   ret = vsprintf(msgBuffer, msg, marker);
   va_end(marker);

   LogMessage(std::string(msgBuffer));
   delete [] msgBuffer;
}

//------------------------------------------------------------------------------
//  void ClearMessage()
//------------------------------------------------------------------------------
/**
 * Clears the message window.  This console version does nothing.
 */  
//------------------------------------------------------------------------------
void APIMessageReceiver::ClearMessage()
{
}

//------------------------------------------------------------------------------
//  std::string GetMessage()
//------------------------------------------------------------------------------
/**
 * Pops the messages off the message queue and concatenates them together.
 * 
 * @return The concatenated messages.
 */
//------------------------------------------------------------------------------
std::string APIMessageReceiver::GetMessage()
{
   return "";
}

//------------------------------------------------------------------------------
// void PutMessage(const std::string &msg)
//------------------------------------------------------------------------------
/**
 * Push the message into queue
 */
//------------------------------------------------------------------------------
void APIMessageReceiver::PutMessage(const std::string &msg)
{
   ; // do nothing here
}

//------------------------------------------------------------------------------
// void ClearMessageQueue()
//------------------------------------------------------------------------------
/**
 * Tells the MessageReceiver to clear the message queue.
 */
//------------------------------------------------------------------------------
void APIMessageReceiver::ClearMessageQueue()
{
   ; // do nothing here
}

//------------------------------------------------------------------------------
// void SetEchoMode(bool echo)
//------------------------------------------------------------------------------
/**
 * Sets the echo mode for derived classes that support echoing
 *
 * The APIMessageReceiver uses this setting to echo log messages to the user
 *
 * @param echo The mode setting
 */
//------------------------------------------------------------------------------
void APIMessageReceiver::SetEchoMode(bool echo)
{
   echoMode = echo;
}

//---------------------------------
//  private methods
//---------------------------------

//------------------------------------------------------------------------------
// APIMessageReceiver()
//------------------------------------------------------------------------------
/**
 * Constructor, called from the Instance method to create the singleton.
 */
//------------------------------------------------------------------------------
APIMessageReceiver::APIMessageReceiver() :
   MAX_MESSAGE_LENGTH      (10000),
   logFile                 (NULL),
   logEnabled              (false),
   logFileSet              (false),
   echoMode                (false)
{
   messageQueue.push("APIMessageReceiver: Starting GMAT ...");
}

//------------------------------------------------------------------------------
// ~APIMessageReceiver()
//------------------------------------------------------------------------------
/**
 * Class destructor.
 */
//------------------------------------------------------------------------------
APIMessageReceiver::~APIMessageReceiver()
{
}
