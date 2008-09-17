//$Id$
//------------------------------------------------------------------------------
//                                  FunctionRunner
//------------------------------------------------------------------------------
// GMAT: Goddard Mission Analysis Tool
//
// **Legal**
//
// Developed jointly by NASA/GSFC and Thinking Systems, Inc. under contract
// number NNG06CCA54C
//
// Author: Wendy Shoan
// Created: 2008.04.21
//
/**
 * Implements FunctionRunner class.
 */
//------------------------------------------------------------------------------

#include "FunctionRunner.hpp"
#include "MessageInterface.hpp"

//#define DEBUG_FUNCTION
//#define DEBUG_FINALIZE

//---------------------------------
// public methods
//---------------------------------

//------------------------------------------------------------------------------
// FunctionRunner()
//------------------------------------------------------------------------------
/**
 * Constructor.
 */
//------------------------------------------------------------------------------
FunctionRunner::FunctionRunner(const std::string &nomme)
   : MathFunction("FunctionRunner", nomme)
{
   objectTypeNames.push_back("FunctionRunner");
   theObjectMap = NULL;
   theGlobalObjectMap = NULL;
   theFunction = NULL;
   callingFunction = NULL;
}


//------------------------------------------------------------------------------
// ~FunctionRunner()
//------------------------------------------------------------------------------
/**
 * Destructor.
 */
//------------------------------------------------------------------------------
FunctionRunner::~FunctionRunner()
{
}


//------------------------------------------------------------------------------
//  FunctionRunner(const FunctionRunner &copy)
//------------------------------------------------------------------------------
/**
 * Constructs the FunctionRunner object (copy constructor).
 * 
 * @param <copy> Object that is copied
 */
//------------------------------------------------------------------------------
FunctionRunner::FunctionRunner(const FunctionRunner &copy) :
   MathFunction(copy)
{
   callingFunction = copy.callingFunction;
}


//------------------------------------------------------------------------------
// void SetFunctionName(const std::string &fname)
//------------------------------------------------------------------------------
/*
 * Sets function name to the FunctionManager.
 *
 * @param  fname  The function name to set
 */
//------------------------------------------------------------------------------
void FunctionRunner::SetFunctionName(const std::string &fname)
{
   #ifdef DEBUG_FUNCTION
   MessageInterface::ShowMessage
      ("FunctionRunner::SetFunctionName() passingArgs='%s', fname='%s'\n",
       GetName().c_str(), fname.c_str());
   #endif
   
   theFunctionName = fname;
   theFunctionManager.SetFunctionName(fname);
}


//------------------------------------------------------------------------------
// void SetFunction(Function *function)
//------------------------------------------------------------------------------
/*
 * Sets function pointer to the FunctionManager.
 *
 * @param  function  The function pointer to set
 */
//------------------------------------------------------------------------------
void FunctionRunner::SetFunction(Function *function)
{
   #ifdef DEBUG_FUNCTION
   MessageInterface::ShowMessage
      ("FunctionRunner::SetFunction() passingArgs='%s', function=%p, name='%s'\n",
       GetName().c_str(), function, function->GetName().c_str());
   #endif
   
   if (theFunctionManager.GetFunctionName() == function->GetName())
      theFunctionManager.SetFunction(function);
}


//------------------------------------------------------------------------------
// void AddFunctionInput(const std::string &name)
//------------------------------------------------------------------------------
/*
 * Adds function input argument name to the input names.
 *
 * @param  name  The name to add the list
 */
//------------------------------------------------------------------------------
void FunctionRunner::AddFunctionInput(const std::string &name)
{
   #ifdef DEBUG_FUNCTION
   MessageInterface::ShowMessage
      ("FunctionRunner::AddFunctionInput() passingArgs='%s', adding name='%s'\n",
       GetName().c_str(), name.c_str());
   #endif
   
   theInputNames.push_back(name);
}


//------------------------------------------------------------------------------
// void AddFunctionOutput(const std::string &name)
//------------------------------------------------------------------------------
/*
 * Adds function output argument name to the output names.
 *
 * @param  name  The name to add the list
 */
//------------------------------------------------------------------------------
void FunctionRunner::AddFunctionOutput(const std::string &name)
{
   #ifdef DEBUG_FUNCTION
   MessageInterface::ShowMessage
      ("FunctionRunner::AddFunctionOutput() passingArgs='%s', adding name='%s'\n",
       GetName().c_str(), name.c_str());
   #endif
   
   theOutputNames.push_back(name);
}


//------------------------------------------------------------------------------
// void SetFunctionInputs()
//------------------------------------------------------------------------------
/*
 * Sets function input list to the FunctionManager.
 */
//------------------------------------------------------------------------------
void FunctionRunner::SetFunctionInputs()
{
   theFunctionManager.SetInputs(theInputNames);
}


//------------------------------------------------------------------------------
// void SetFunctionOutputs()
//------------------------------------------------------------------------------
/*
 * Sets function output list to the FunctionManager.
 */
//------------------------------------------------------------------------------
void FunctionRunner::SetFunctionOutputs()
{
   theFunctionManager.SetOutputs(theOutputNames);
}


//------------------------------------------------------------------------------
// const StringArray& GetInputs()
//------------------------------------------------------------------------------
/*
 * @return the input list
 */
//------------------------------------------------------------------------------
const StringArray& FunctionRunner::GetInputs()
{
   return theInputNames;
}
//------------------------------------------------------------------------------
// void SetCallingFunction();
//------------------------------------------------------------------------------
void FunctionRunner::SetCallingFunction(FunctionManager *fm)
{
   callingFunction = fm;
}


//------------------------------------------------------------------------------
//  void SetObjectMap(ObjectMap *map)
//------------------------------------------------------------------------------
/**
 * Called by the MathTree to set the local asset store used by the GmatCommand
 * 
 * @param <map> Pointer to the local object map
 */
//------------------------------------------------------------------------------
void FunctionRunner::SetObjectMap(ObjectMap *map)
{
   #ifdef DEBUG_FUNCTION
   MessageInterface::ShowMessage
      ("FunctionRunner::SetObjectMap() entered, theFunctionName='%s', "
       "map=<%p>\n", theFunctionName.c_str(), map);
   #endif

   theObjectMap = map;
   theFunctionManager.SetObjectMap(map);
}


//------------------------------------------------------------------------------
//  void SetGlobalObjectMap(ObjectMap *map)
//------------------------------------------------------------------------------
/**
 * Called by the MathTree to set the global asset store used by the GmatCommand
 * 
 * @param <map> Pointer to the global object map
 */
//------------------------------------------------------------------------------
void FunctionRunner::SetGlobalObjectMap(ObjectMap *map)
{
   #ifdef DEBUG_FUNCTION
   MessageInterface::ShowMessage
      ("FunctionRunner::SetGlobalObjectMap() entered, theFunctionName='%s', "
       "map=<%p>\n", theFunctionName.c_str(), map);
   #endif
   
   theGlobalObjectMap = map;
   
   // Now, find the function object
   GmatBase *mapObj = FindObject(theFunctionName);
   
   #ifdef DEBUG_FUNCTION
   MessageInterface::ShowMessage
      ("=> found the function: <%p><%s>\n", mapObj, mapObj ? mapObj->GetName().c_str() : "NULL");
   #endif
   
   if (mapObj == NULL)
   {
      throw MathException("FunctionRunner cannot find the Function \"" +
                          theFunctionName + "\"");
   }
   else
   {
      theFunction = (Function *)mapObj;
      theFunctionManager.SetFunction(theFunction);
   }
   
   theFunctionManager.SetGlobalObjectMap(map);
   
   #ifdef DEBUG_FUNCTION
   MessageInterface::ShowMessage("FunctionRunner::SetGlobalObjectMap() exiting\n");
   #endif
}


//------------------------------------------------------------------------------
//  void SetSolarSystem(SolarSystem *ss)
//------------------------------------------------------------------------------
/**
 * Called by the MathTree to set the SolarSystem used by the GmatCommand
 * 
 * @param <ss> Pointer to the SolarSystem
 */
//------------------------------------------------------------------------------
void FunctionRunner::SetSolarSystem(SolarSystem *ss)
{
   #ifdef DEBUG_FUNCTION
   MessageInterface::ShowMessage
      ("FunctionRunner::SetSolarSystem() entered, theFunctionName='%s', "
       "ss=<%p>\n", theFunctionName.c_str(), ss);
   #endif
   
   theFunctionManager.SetSolarSystem(ss);
   
   #ifdef DEBUG_FUNCTION
   MessageInterface::ShowMessage("FunctionRunner::SetSolarSystem() exiting\n");
   #endif
}


//------------------------------------------------------------------------------
//  void SetInternalCoordSystem(CoordinateSystem *cs)
//------------------------------------------------------------------------------
/**
 * Called by the MathTree to set the CoordinateSystem used by the GmatCommand and
 * function objects.
 * 
 * @param <cs> Pointer to the CoordinateSystem
 */
//------------------------------------------------------------------------------
void FunctionRunner::SetInternalCoordSystem(CoordinateSystem *cs)
{
   #ifdef DEBUG_FUNCTION
   MessageInterface::ShowMessage
      ("FunctionRunner::SetInternalCoordSystem() entered, theFunctionName='%s', "
       "cs=<%p>\n", theFunctionName.c_str(), cs);
   #endif
   
   theFunctionManager.SetInternalCoordinateSystem(cs);
   
   #ifdef DEBUG_FUNCTION
   MessageInterface::ShowMessage("FunctionRunner::SetInternalCoordSystem() leaving\n");
   #endif
}


//------------------------------------------------------------------------------
//  void SetTransientForces(std::vector<PhysicalModel*> *tf)
//------------------------------------------------------------------------------
/**
 * Called by the MathTree to set the transient forces used by the GmatCommand
 * 
 * @param <map> Pointer to the vector of transient forces
 */
//------------------------------------------------------------------------------
void FunctionRunner::SetTransientForces(std::vector<PhysicalModel*> *tf)
{
   #ifdef DEBUG_FUNCTION
   MessageInterface::ShowMessage
      ("FunctionRunner::SetTransientForces() entered, theFunctionName='%s', "
       "tf=<%p>\n", theFunctionName.c_str(), tf);
   #endif
   
   theFunctionManager.SetTransientForces(tf);
   
   #ifdef DEBUG_FUNCTION
   MessageInterface::ShowMessage("FunctionRunner::SetTransientForces() exiting\n");
   #endif
}


//------------------------------------------------------------------------------
// void GetOutputInfo(Integer &type, Integer &rowCount, Integer &colCount)
//------------------------------------------------------------------------------
void FunctionRunner::GetOutputInfo(Integer &type,
                                   Integer &rowCount, Integer &colCount)
{
   Function *function = theFunctionManager.GetFunction();
   if (function == NULL)
      throw MathException("FunctionRunner::GetOutputInfo() function is NULL\n");
   
   #ifdef DEBUG_FUNCTION
   MessageInterface::ShowMessage
      ("FunctionRunner::GetOutputInfo() entered, this=<%p><%s>, function=<%s><%p>\n",
       this, GetName().c_str(), function->GetName().c_str(), function);
   #endif
   
   // check for function output type
   IntegerArray rowCounts, colCounts;
   WrapperTypeArray outputTypes = function->GetOutputTypes(rowCounts, colCounts);
   std::string errMsg;
   
   if (outputTypes.size() == 0)
   {
      errMsg = "The function \"" + function->GetName() + "\" does not return"
         " any value";
   }
   else if (outputTypes.size() > 1)
   {
      errMsg = "The function \"" + function->GetName() + "\" returns more than "
         " one value";
   }
   else
   {
      if (outputTypes[0] == Gmat::VARIABLE)
      {
         type = Gmat::REAL_TYPE;
         rowCount = 1;
         colCount = 1;
      }
      else if (outputTypes[0] == Gmat::ARRAY)
      {
         type = Gmat::RMATRIX_TYPE;
         rowCount = rowCounts[0];
         colCount = colCounts[0];
         matrix.SetSize(rowCount, colCount);
      }
   }
   
   elementType = type;
   
   #ifdef DEBUG_FUNCTION
   MessageInterface::ShowMessage
      ("FunctionRunner::GetOutputInfo() type=%d, rowCount=%d, colCount=%d\n",
       type, rowCount, colCount);
   #endif
   
   if (errMsg != "")
   {
      throw MathException("FunctionRunner::GetOutputInfo() " + errMsg);
   }
}


//------------------------------------------------------------------------------
// bool ValidateInputs()
//------------------------------------------------------------------------------
/**
 * This method calls its subnodes and checks to be sure that the subnodes return
 * compatible data for the function.
 */
//------------------------------------------------------------------------------
bool FunctionRunner::ValidateInputs()
{
   Function *function = theFunctionManager.GetFunction();
   if (function == NULL)
      throw MathException("FunctionRunner::ValidateInputs() function is NULL\n");
   
   #ifdef DEBUG_FUNCTION
   MessageInterface::ShowMessage
      ("FunctionRunner::ValidateInputs() entered, this=<%p><%s>, function=<%s><%p>\n",
       this, GetName().c_str(), function->GetName().c_str(), function);
   #endif
   
   // How can we validate input here? Just return true for now.
   return true;
}


//------------------------------------------------------------------------------
// Real Evaluate()
//------------------------------------------------------------------------------
/**
 * @return the FunctionRunner of left node
 *
 */
//------------------------------------------------------------------------------
Real FunctionRunner::Evaluate()
{
   Function *function = theFunctionManager.GetFunction();
   if (function == NULL)
      throw MathException("FunctionRunner::Evaluate() function is NULL\n");
   
   #ifdef DEBUG_FUNCTION
   MessageInterface::ShowMessage
      ("FunctionRunner::Evaluate() entered, this=<%p><%s>, function=<%s><%p>\n",
       this, GetName().c_str(), function->GetName().c_str(), function);
   #endif
   
   if (elementType == Gmat::RMATRIX_TYPE)
      throw MathException
         ("The function \"" + function->GetName() + "\" returns matrix value");
   
   return theFunctionManager.Evaluate(callingFunction);
}


//------------------------------------------------------------------------------
// Rmatrix MatrixEvaluate()
//------------------------------------------------------------------------------
/**
 * @return the FunctionRunner of left node
 *
 */
//------------------------------------------------------------------------------
Rmatrix FunctionRunner::MatrixEvaluate()
{
   Function *function = theFunctionManager.GetFunction();
   if (function == NULL)
      throw MathException("FunctionRunner::Evaluate() function is NULL\n");
   
   #ifdef DEBUG_FUNCTION
   MessageInterface::ShowMessage
      ("FunctionRunner::MatrixEvaluate() entered, this=<%p><%s>, function=<%s><%p>\n",
       this, GetName().c_str(), function->GetName().c_str(), function);
   #endif
   
   if (elementType == Gmat::REAL_TYPE)
      throw MathException
         ("The function \"" + function->GetName() + "\" returns Real value");
   
   return theFunctionManager.MatrixEvaluate(callingFunction);
}


//------------------------------------------------------------------------------
// void Finalize()
//------------------------------------------------------------------------------
void FunctionRunner::Finalize()
{
   #ifdef DEBUG_FINALIZE
   MessageInterface::ShowMessage
      ("FunctionRunner::RunComplete() calling FunctionManager::Finalize()\n");
   #endif
   theFunctionManager.Finalize();
}


//------------------------------------------------------------------------------
//  GmatBase* Clone() const
//------------------------------------------------------------------------------
/**
 * Clone of the FunctionRunner operation.
 *
 * @return clone of the FunctionRunner operation.
 *
 */
//------------------------------------------------------------------------------
GmatBase* FunctionRunner::Clone() const
{
   return (new FunctionRunner(*this));
}


//------------------------------------------------------------------------------
// GmatBase* FunctionRunner::FindObject(const std::string &name)
//------------------------------------------------------------------------------
GmatBase* FunctionRunner::FindObject(const std::string &name)
{
   std::string newName = name;
   
   // Ignore array indexing of Array
   std::string::size_type index = name.find('(');
   if (index != name.npos)
      newName = name.substr(0, index);
   
   #ifdef DEBUG_FUNCTION
   MessageInterface::ShowMessage
      ("FunctionRunner::FindObject() theObjectMap=<%p>, theGlobalObjectMap=<%p>, "
       "newName='%s'\n", theObjectMap, theGlobalObjectMap, newName.c_str());
   if (theObjectMap)
   {
      MessageInterface::ShowMessage("Here is the local object map:\n");
      for (std::map<std::string, GmatBase *>::iterator i = theObjectMap->begin();
           i != theObjectMap->end(); ++i)
         MessageInterface::ShowMessage("   %s\n", i->first.c_str());
   }
   if (theGlobalObjectMap)
   {
      MessageInterface::ShowMessage("Here is the global object map:\n");
      for (std::map<std::string, GmatBase *>::iterator i = theGlobalObjectMap->begin();
           i != theGlobalObjectMap->end(); ++i)
         MessageInterface::ShowMessage("   %s\n", i->first.c_str());
   }
   #endif
   
   // Check for the object in the Local Object Store (LOS) first
   if (theObjectMap && theObjectMap->find(newName) != theObjectMap->end())
      return (*theObjectMap)[newName];
   
   // If not found in the LOS, check the Global Object Store (GOS)
   if (theGlobalObjectMap &&
       theGlobalObjectMap->find(newName) != theGlobalObjectMap->end())
      return (*theGlobalObjectMap)[newName];
   
   return NULL;
}

