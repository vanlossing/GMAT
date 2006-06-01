//$Header$
//------------------------------------------------------------------------------
//                                 StringUtil
//------------------------------------------------------------------------------
// GMAT: Goddard Mission Analysis Tool
//
// **Legal**
//
// Developed jointly by NASA/GSFC and Thinking Systems, Inc. under contract
// number S-67573-G
//
// Author: Linda Jun
// Created: 2006/1/6
//
/**
 * This file provides string utility.
 */
//------------------------------------------------------------------------------

#include "StringUtil.hpp"
#include "MessageInterface.hpp"
#include "GmatBaseException.hpp"

//#define DEBUG_STRING_UTIL 1

using namespace std;


//------------------------------------------------------------------------------
// std::string RemoveAll(const std::string &str, char ch, Integer start = 0)
//------------------------------------------------------------------------------
std::string GmatStringUtil::RemoveAll(const std::string &str, char ch,
                                      Integer start)
{
   std::string str2 = str;

   std::string::iterator iter = str2.begin();
   for (int i=0; i<start; i++)
      iter++;
   
   while (iter != str2.end())
   {
      if (*iter == ch)
         str2.erase(iter);
      else
         ++iter;
   }

   return str2;
}


//------------------------------------------------------------------------------
// std::string Trim(const std::string &str, StripType stype = TRAILING,
//                  bool removeSemicolon = false)
//------------------------------------------------------------------------------
std::string GmatStringUtil::Trim(const std::string &str, StripType stype,
                                 bool removeSemicolon)
{
   std::string str2;

   UnsignedInt index1 = str.find_first_not_of(' ');
   UnsignedInt index2 = str.find_last_not_of(' ');

   if (index1 == str.npos)
      index1 = 0;

   if (stype == LEADING)
      str2 = str.substr(index1);
   else if (stype == TRAILING)
      str2 = str.substr(0, index2);
   else if (stype == BOTH)
      str2.assign(str.substr(index1, index2-index1+1));

   #if DEBUG_STRING_UTIL
   MessageInterface::ShowMessage
      ("==> index1=%d, index2=%d str=<%s>, str2=<%s>\n", index1, index2,
       str.c_str(), str2.c_str());
   #endif

   if (removeSemicolon)
      if (str2[str2.size()-1] == ';')
         str2.erase(str2.size()-1, 1);
   
   return str2;
}


//------------------------------------------------------------------------------
// std::string Strip(const std::string &str, StripType stype = TRAILING)
//------------------------------------------------------------------------------
std::string GmatStringUtil::Strip(const std::string &str, StripType stype)
{
   return Trim(str, stype);
}


//------------------------------------------------------------------------------
// bool ToDouble(const std::string &str, Real *value)
//------------------------------------------------------------------------------
// Note: atof() returns 100.00 for 100.00ABC, but we want it be an error
//------------------------------------------------------------------------------
bool GmatStringUtil::ToDouble(const std::string &str, Real *value)
{
   std::string str2 = Trim(str, BOTH);
   Integer numDot = 0;
   
   if (str2.length() == 0)
      return false;

   if (str2[0] != '-' && !isdigit(str2[0]) && str2[0] != '.')
      return false;

   for (unsigned int i=0; i<str2.length(); i++)
   {
      if (i == 0 && str2[0] == '-')
         continue;
      
      if (str2[i] == '.')
      {
         numDot++;

         if (numDot > 1)
            return false;
         
         continue;
      }
      
      if (!isdigit(str2[i]))
         return false;
   }
   
   *value = atof(str2.c_str());
   return true;
}


//------------------------------------------------------------------------------
// std::string ToUpper(const std::string &str)
//------------------------------------------------------------------------------
std::string GmatStringUtil::ToUpper(const std::string &str)
{
   Integer len = str.length();
   
   if (len == 0)
      return str;

   std::string upper = str;
   for (int i=0; i<len; i++)
      upper[i] = toupper(str[i]);

   return upper;
}


//------------------------------------------------------------------------------
// std::string ToLower(const std::string &str)
//------------------------------------------------------------------------------
std::string GmatStringUtil::ToLower(const std::string &str)
{
   Integer len = str.length();
   
   if (len == 0)
      return str;

   std::string lower = str;
   for (int i=0; i<len; i++)
      lower[i] = tolower(str[i]);

   return lower;
}


//------------------------------------------------------------------------------
// std::string Capitalize(const std::string &str)
//------------------------------------------------------------------------------
std::string GmatStringUtil::Capitalize(const std::string &str)
{
   Integer len = str.length();
   
   if (len == 0)
      return str;

   std::string newstr = str;
   if (newstr.length() == 0)
      return str;
   
   newstr = Trim(newstr, BOTH);
   newstr[0] = toupper(newstr[0]);

   return newstr;
}


//------------------------------------------------------------------------------
// void ParseParameter(const std::string &str, std::string &type,
//                     std::string &owner, std::string &depobj)
//------------------------------------------------------------------------------
void GmatStringUtil::ParseParameter(const std::string &str, std::string &type,
                                    std::string &owner, std::string &depObj)
{
   //find owner.depObj.type
   std::string str1 = str;
   std::string::size_type pos1 = str1.find(".");
   std::string::size_type pos2 = str1.find_last_of(".");
   
   if (pos1 != str1.npos && pos2 != str1.npos)
   {
      owner = str1.substr(0, pos1);
      type = str1.substr(pos2+1, str1.npos-pos2);

      depObj = "";
      if (pos2 > pos1)
         depObj = str1.substr(pos1+1, pos2-pos1-1);
   }

   #if DEBUG_STRING_UTIL
   MessageInterface::ShowMessage
      ("GmatStringUtil::ParseParameter() str=%s, type=%s, owner=%s, depObj=%s\n",
       str.c_str(), type.c_str(), owner.c_str(), depObj.c_str());
   #endif
   
}


//------------------------------------------------------------------------------
// void GetArrayIndex(const std::string &str, Integer &row, Integer &col,
//                    std::string &name)
//------------------------------------------------------------------------------
void GmatStringUtil::GetArrayIndex(const std::string &str, Integer &row,
                                   Integer &col, std::string &name)
{
   #if DEBUG_STRING_UTIL
   MessageInterface::ShowMessage
      ("Report::GetArrayIndex() str=%s\n", str.c_str());
   #endif
   
   row = -1;
   col = -1;
   
   // Handle Array indexing
   UnsignedInt openParen = str.find('(');
   if (openParen != str.npos)
   {
      UnsignedInt comma = str.find(',');
      if (comma == str.npos)
         throw GmatBaseException("Expecting \",\" for Array " + str);
         
      UnsignedInt closeParen = str.find(')');
      if (closeParen == str.npos)
         throw GmatBaseException("Expecting \")\" for Array " + str);
         
      std::string rowStr = str.substr(openParen+1, comma-openParen-1);
      std::string colStr = str.substr(comma+1, closeParen-comma-1);

      // array index starts at 0
      row = atoi(rowStr.c_str()) - 1;
      col = atoi(colStr.c_str()) - 1;
   }

   name = str.substr(0, openParen);
   
   #if DEBUG_STRING_UTIL
   MessageInterface::ShowMessage
      ("Report::GetArrayIndex() row=%d, col=%d, name=%s\n", row, col, name.c_str());
   #endif
}


//------------------------------------------------------------------------------
// void FindFirstAndLast(const std::string &str, char ch, Integer &first,
//                       Integer &last)
//------------------------------------------------------------------------------
/*
 * Finds first and last index of given chracter from the string.
 * if given chracter is not found it sets to -1
 */
//------------------------------------------------------------------------------
void GmatStringUtil::FindFirstAndLast(const std::string &str, char ch,
                                      Integer &first, Integer &last)
{
   first = str.find_first_of(ch);
   if (first == (Integer)str.npos)
      first = -1;

   last = str.find_last_of(ch);
   if (last == (Integer)str.npos)
      last = -1;
}


//------------------------------------------------------------------------------
// void FindParenMatch(const std::string &str, Integer &openParen,
//                     Integer &closePardn)
//------------------------------------------------------------------------------
/*
 * Finds open and close parenthesis index from the string.
 * if parhenthesis is not found it sets to -1
 */
//------------------------------------------------------------------------------
void GmatStringUtil::FindParenMatch(const std::string &str, Integer &openParen,
                                    Integer &closeParen, bool &isOuterParen)
{
   openParen = str.find_first_of('(');
   if (openParen == (Integer)str.npos)
      openParen = -1;

   closeParen = str.find_last_of(')');
   if (closeParen == (Integer)str.npos)
      closeParen = -1;
   
   isOuterParen = false;
   if (openParen != -1 && closeParen == (Integer)str.size()-1)
      isOuterParen = true;
   
}

