//$Header$
//------------------------------------------------------------------------------
//                                Optimize 
//------------------------------------------------------------------------------
// GMAT: Goddard Mission Analysis Tool.
//
// **Legal**
//
// Developed jointly by NASA/GSFC and Thinking Systems, Inc. under contract
// number NNG04CC06P
//
// Author:  Daniel Hunter/GSFC/MAB (CoOp)
// Created: 2006.07.20
//
/**
 * Implementation for the Optimize command class
 */
//------------------------------------------------------------------------------

#include <sstream>
#include "Optimize.hpp"
#include "ExternalOptimizer.hpp"
#if defined __USE_MATLAB__
   #include "MatlabInterface.hpp"
   #include "GmatInterface.hpp"
   
   static GmatInterface *gmatInt = GmatInterface::Instance();
#endif

//#define DEBUG_OPTIMIZER_PARSING
//#define DEBUG_OPTIMIZE_COMMANDS
//#define DEBUG_OPTIMIZER
//#define DEBUG_CALLBACK

//------------------------------------------------------------------------------
// static data
//------------------------------------------------------------------------------
const std::string
Optimize::PARAMETER_TEXT[OptimizeParamCount - BranchCommandParamCount] =
{
   "OptimizerName",
   "OptimizerConverged",
};

const Gmat::ParameterType
Optimize::PARAMETER_TYPE[OptimizeParamCount - BranchCommandParamCount] =
{
   Gmat::STRING_TYPE,
   Gmat::BOOLEAN_TYPE,
};

//------------------------------------------------------------------------------
// public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// constructor
//------------------------------------------------------------------------------
Optimize::Optimize() :
   BranchCommand      ("Optimize"),
   optimizerName       (""),
   optimizer           (NULL),
   optimizerConverged  (false),
   optimizerInDebugMode(false)
{
   parameterCount = OptimizeParamCount;
}

//------------------------------------------------------------------------------
// copy constructor
//------------------------------------------------------------------------------
Optimize::Optimize(const Optimize& o) :
   BranchCommand        (o),
   optimizerName        (o.optimizerName),
   optimizer            (NULL),
   optimizerConverged   (false),
   optimizerInDebugMode (o.optimizerInDebugMode)
{
	//parameterCount = OptimizeParamCount;  // this is set in GmatBase copy constructor
   localStore.clear();
}

//------------------------------------------------------------------------------
// operator =
//------------------------------------------------------------------------------
Optimize& Optimize::operator=(const Optimize& o)
{
   if (this == &o)
      return *this;
    
   GmatCommand::operator=(o);

   optimizerName        = o.optimizerName;
   optimizer            = NULL;
   optimizerConverged   = false;
   optimizerInDebugMode = o.optimizerInDebugMode;
   localStore.clear();

   return *this;
}

//------------------------------------------------------------------------------
// destructor
//------------------------------------------------------------------------------
Optimize::~Optimize()
{
   if (optimizer)
      delete optimizer;
}


//------------------------------------------------------------------------------
// Append
//------------------------------------------------------------------------------
bool Optimize::Append(GmatCommand *cmd)
{
   #ifdef DEBUG_OPTIMIZER_PARSING
       MessageInterface::ShowMessage("\nOptimize::Append received \"%s\" command",
                                     cmd->GetTypeName().c_str());
   #endif
    
   if (!BranchCommand::Append(cmd))
        return false;
    
   // If at the end of a optimizer branch, point that end back to this comand.
   if (cmd->GetTypeName() == "EndOptimize") 
   {
      if ((nestLevel == 0) && (branchToFill != -1))  
      {
         cmd->Append(this);
         // Optimizer loop is complete; -1 pops to the next higher sequence.
         branchToFill = -1;
         #ifdef DEBUG_OPTIMIZER_PARSING
             MessageInterface::ShowMessage("\nOptimize::Append closing \"%s\"",
                                           generatingString.c_str());
         #endif
      }
      else
         --nestLevel;
   }

   // If it's a nested optimizer branch, add to the nest level.
   // 2006.09.13 wcs - as of today, nested optimizers are not allowed
   if (cmd->GetTypeName() == "Optimize")
      ++nestLevel;

   #ifdef DEBUG_OPTIMIZER_PARSING
       MessageInterface::ShowMessage("\nOptimize::Append for \"%s\" nest level = %d",
                                     generatingString.c_str(), nestLevel);
   #endif

   return true;
}

//------------------------------------------------------------------------------
// Clone
//------------------------------------------------------------------------------
GmatBase* Optimize::Clone() const
{
   return (new Optimize(*this));
}

//------------------------------------------------------------------------------
// GetGeneratingString
//------------------------------------------------------------------------------
const std::string& Optimize::GetGeneratingString(Gmat::WriteMode mode,
                                                 const std::string &prefix,
                                                 const std::string &useName)
{
   generatingString = "Optimize " + optimizerName + ";";
   return BranchCommand::GetGeneratingString(mode, prefix, useName);
}

//------------------------------------------------------------------------------
// RenameRefObject
//------------------------------------------------------------------------------
bool Optimize::RenameRefObject(const Gmat::ObjectType type,
                               const std::string &oldName,
                               const std::string &newName)
{
   if (type == Gmat::SOLVER)
   {
      if (optimizerName == oldName)
         optimizerName = newName;
   }
   
   return true;
}

//------------------------------------------------------------------------------
// GetParameterText
//------------------------------------------------------------------------------
std::string Optimize::GetParameterText(const Integer id) const
{
   if (id >= BranchCommandParamCount && id < OptimizeParamCount)
      return PARAMETER_TEXT[id - BranchCommandParamCount];
    
   return BranchCommand::GetParameterText(id);
}

//------------------------------------------------------------------------------
// GetParameterID
//------------------------------------------------------------------------------
Integer Optimize::GetParameterID(const std::string &str) const
{
   for (Integer i = BranchCommandParamCount; i < OptimizeParamCount; i++)
   {
      if (str == PARAMETER_TEXT[i - BranchCommandParamCount])
         return i;
   }
    
   return BranchCommand::GetParameterID(str);
}

//------------------------------------------------------------------------------
// GetParameterType
//------------------------------------------------------------------------------
Gmat::ParameterType Optimize::GetParameterType(const Integer id) const
{
   if (id >= BranchCommandParamCount && id < OptimizeParamCount)
      return PARAMETER_TYPE[id - BranchCommandParamCount];
    
   return BranchCommand::GetParameterType(id);
}

//------------------------------------------------------------------------------
// GetParameterTypeString
//------------------------------------------------------------------------------
std::string Optimize::GetParameterTypeString(const Integer id) const
{    
   return BranchCommand::PARAM_TYPE_STRING[GetParameterType(id)];
}

//------------------------------------------------------------------------------
// GetStringParameter
//------------------------------------------------------------------------------
std::string Optimize::GetStringParameter(const Integer id) const
{
   if (id == OPTIMIZER_NAME)
      return optimizerName;
    
   return BranchCommand::GetStringParameter(id);
}

//------------------------------------------------------------------------------
// SetStringParameter
//------------------------------------------------------------------------------
bool Optimize::SetStringParameter(const Integer id, const std::string &value)
{
   if (id == OPTIMIZER_NAME) 
   {
      optimizerName = value;
      return true;
   }
    
   return BranchCommand::SetStringParameter(id, value);
}

//------------------------------------------------------------------------------
// GetBooleanParameter
//------------------------------------------------------------------------------
bool Optimize::GetBooleanParameter(const Integer id) const
{
   if (id == OPTIMIZER_CONVERGED)
      return optimizerConverged;
      
   return BranchCommand::GetBooleanParameter(id);
}

//------------------------------------------------------------------------------
// GetRefObjectName
//------------------------------------------------------------------------------
std::string Optimize::GetRefObjectName(const Gmat::ObjectType type) const
{
   if (type == Gmat::SOLVER)
      return optimizerName;
   return BranchCommand::GetRefObjectName(type);
}

//------------------------------------------------------------------------------
// SetRefObjectName
//------------------------------------------------------------------------------
bool Optimize::SetRefObjectName(const Gmat::ObjectType type,
                                const std::string &name)
{
   if (type == Gmat::SOLVER) 
   {
      optimizerName = name;
      return true;
   }
   return BranchCommand::SetRefObjectName(type, name);
}

//------------------------------------------------------------------------------
// Initialize
//------------------------------------------------------------------------------
bool Optimize::Initialize()
{
	if (objectMap->find(optimizerName) == objectMap->end()) 
   {
      std::string errorString = "Optimize command cannot find optimizer \"";
      errorString += optimizerName;
      errorString += "\"";
      throw CommandException(errorString);
   }

   // Clone the optimizer for local use
   optimizer = (Solver *)((*objectMap)[optimizerName])->Clone();
   optimizer->TakeAction("IncrementInstanceCount");
   ((*objectMap)[optimizerName])->TakeAction("IncrementInstanceCount");
   
   if (optimizer->GetStringParameter("ReportStyle") == "Debug")
      optimizerInDebugMode = true;      
    
   // Set the local copy of the optimizer on each node
   std::vector<GmatCommand*>::iterator node;
   GmatCommand *currentCmd;

   for (node = branch.begin(); node != branch.end(); ++node)
   {
      currentCmd = *node;

      #ifdef DEBUG_OPTIMIZE_COMMANDS
         Integer nodeNum = 0;
      #endif
      while ((currentCmd != NULL) && (currentCmd != this))
      {
         #ifdef DEBUG_OPTIMIZE_COMMANDS
            MessageInterface::ShowMessage(
               "   Optimize Command %d:  %s\n", ++nodeNum, 
               currentCmd->GetTypeName().c_str());       
         #endif
         if ((currentCmd->GetTypeName() == "Vary") || 
             (currentCmd->GetTypeName() == "Minimize") ||
             (currentCmd->GetTypeName() == "NonLinearConstraint"))
            currentCmd->SetRefObject(optimizer, Gmat::SOLVER, optimizerName);
         currentCmd = currentCmd->GetNext();
      }
   }

   bool retval = BranchCommand::Initialize();

   if (retval == true) 
   {
      // Optimizer specific initialization goes here: // wcs - why is this done twice?
      if (objectMap->find(optimizerName) == objectMap->end()) 
      {
         std::string errorString = "Optimize command cannot find optimizer \"";
         errorString += optimizerName;
         errorString += "\"";
         throw CommandException(errorString);
      }

      retval = optimizer->Initialize();
   }
   // NOTE that in the future we may have a callback to/from a non_MATLAB
   // external optimizer
   #if defined __USE_MATLAB__
      if (optimizer->IsOfType("ExternalOptimizer") &&
         (optimizer->GetStringParameter("SourceType") == "MATLAB"))
         gmatInt->RegisterCallbackServer(this);
   #endif

   return retval;
}

//------------------------------------------------------------------------------
// Execute
//------------------------------------------------------------------------------
bool Optimize::Execute()
{
   bool retval = true;

   // Drive through the state machine.
   Solver::SolverState state = optimizer->GetState();
   
   #ifdef DEBUG_OPTIMIZE_COMMANDS
      MessageInterface::ShowMessage("Optimize::Execute(%c%c%d)\n",
         (commandExecuting?'Y':'N'),
         (commandComplete?'Y':'N'),
         state);
   #endif

   // Attempt to reset if recalled   
   if (commandComplete)
   {
      commandComplete  = false;
      commandExecuting = false;
   }  

   if (!commandExecuting) 
   {
	   #ifdef DEBUG_OPTIMIZE_COMMANDS
   	   MessageInterface::ShowMessage(
      	   "Entered Optimizer while command is not executing\n");
      #endif

      FreeLoopData();
      StoreLoopData();

      retval = BranchCommand::Execute();

      #ifdef DEBUG_OPTIMIZER
         MessageInterface::ShowMessage("Resetting the Optimizer\n");
      #endif

      optimizer->TakeAction("Reset");
      state = optimizer->GetState();
      
   }
   /*  Revisit this if/when we add non-external optimizers !!!!!!!!!!!!!!!!!!!!!
   if (branchExecuting)
   {
      retval = ExecuteBranch();
      if (!branchExecuting && (state == Solver::FINISHED))
      {
         commandComplete = true;
      }  
   }
   else
   {
   */
      GmatCommand *currentCmd;
   
      switch (state) 
      {
         case Solver::INITIALIZING:
            // Finalize initialization of the optimizer data
            currentCmd = branch[0];
            optimizerConverged = false;
            while (currentCmd != this)  
            {
               std::string type = currentCmd->GetTypeName();
               if ((type == "Optimize") || (type == "Vary") ||
                   (type == "Minimize") || (type == "NonLinearConstraint"))
                  currentCmd->Execute();
               currentCmd = currentCmd->GetNext();
            }
            StoreLoopData();
            break;

         case Solver::RUNEXTERNAL:
            // Execute the nominal sequence
            if (!commandComplete) 
            {
               branchExecuting = true;
               ResetLoopData();
            }
            break;
               
          /*     
         case Solver::NOMINAL:
            // Execute the nominal sequence
            if (!commandComplete) 
            {
               branchExecuting = true;
               ResetLoopData();
            }
            break;
               
         case Solver::CHECKINGRUN:
            // Check for convergence; this is done in the optimizer state
            // machine, so this case is a NoOp for the Optimize command
            break;
   
         case Solver::PERTURBING:
            branchExecuting = true;
            ResetLoopData();
            break;
               
         case Solver::CALCULATING:
            // Calculate the next set of variables to use; this is performed in
            // the optimizer -- nothing to be done here
            break;
         */      
         case Solver::FINISHED:
            // Final clean-up
//            commandComplete = true;
            optimizerConverged = true;

            // Run once more to publish the data from the converged state
            if (!commandComplete)
            {
               ResetLoopData();
               branchExecuting = true;
            }
            break;
          /*     
         case Solver::ITERATING:     // Intentional fall-through
         */
         default:
            throw CommandException(
               "Invalid state in the Optimize state machine");

      }
   //}

   if (!branchExecuting)
   {
      optimizer->AdvanceState();

      if (optimizer->GetState() == Solver::FINISHED) 
      {
         optimizerConverged = true;
      }
   }

   // Pass spacecraft data to the optimizer for reporting in debug mode
   if (optimizerInDebugMode)
   {
      std::string dbgData = "";
      for (ObjectArray::iterator i = localStore.begin(); i < localStore.end(); ++i)
      {
         dbgData += (*i)->GetGeneratingString() + "\n---\n";
      }
      optimizer->SetDebugString(dbgData);
   }
   BuildCommandSummary(true);
   return retval;
}

bool Optimize::ExecuteCallback()
{
   #ifdef DEBUG_CALLBACK
      MessageInterface::ShowMessage("Entering Optimize::ExecuteCallback");
   #endif
   // NOTE that in the future we may have a callback to/from a non_MATLAB
   // external optimizer
   if (!optimizer || 
      (!(optimizer->IsOfType("ExternalOptimizer"))) || 
      (((ExternalOptimizer*)optimizer)->GetStringParameter("SourceType")
      != "MATLAB"))
   {
      throw CommandException(
      "Optimize::ExecuteCallback not yet implemented for non_MATLAB optimizers");
   }
   #ifndef __USE_MATLAB__ 
      throw CommandException("Optimize: ERROR - MATLAB required for Callback");
   #endif
   #ifdef __USE_MATLAB__
   if (!MatlabInterface::IsOpen())
      throw CommandException(
         "Optimize:: ERROR - Matlab Interface not yet open");
   
   callbackExecuting = true;
   // ask Matlab for the value of X
   Integer     n = optimizer->GetIntegerParameter(
                   optimizer->GetParameterID("NumberOfVariables"));
   //double      X[n];
   //std::string xName = "X";
   //MatlabInterface::GetVariable(xName, n, X);
   Real X[n];
   // read X values from the callback data string here
   std::stringstream ins(callbackData.c_str());
   for (Integer i=0; i<n; i++)
      ins >> X[i];
   std::vector<Real> vars;
   for (Integer i=0;i<n;i++)
      vars.push_back(X[i]);
   // get the state of the Optimizer
   Solver::SolverState nState = optimizer->GetNestedState(); 
   if (nState == Solver::INITIALIZING)
   {
      StoreLoopData();
      // advance to NOMINAL
      callbackResults = optimizer->AdvanceNestedState(vars);
      nState          = optimizer->GetNestedState();
   }
   if (nState != Solver::NOMINAL)
      throw CommandException(
         "Optimize::ExecuteCallback - error in optimizer state");
   // this call should just advance the state to CALCULATING
   callbackResults = optimizer->AdvanceNestedState(vars);
   ResetLoopData();
   try
   {
      if (!ExecuteBranch())
         throw CommandException("Optimize: ERROR executing branch");
   }
   catch (BaseException &be)
   {
      std::string errorStr = "Optimize:ExecuteCallback: ERROR - " +
         be.GetMessage() + "\n";
      throw CommandException(errorStr);
   }
   // this call should just advance the state back to NOMINAL
   // and return results
   callbackResults = optimizer->AdvanceNestedState(vars); 
   // send results to MATLAB
   //for (Integer i=0;i<(Integer)results.size();i++)
   //   MatlabInterface::RunMatlabString(results.at(i));
   #endif
   callbackExecuting = false;
   //return false;  
   return true;
}

bool Optimize::PutCallbackData(std::string &data)
{
   callbackData = data;
   #ifdef DEBUG_CALLBACK
      MessageInterface::ShowMessage("Entering Optimize::PutCallbackData");
      MessageInterface::ShowMessage("-- callback data are:\n");
      for (Integer i=0; i< (Integer) data.size(); i++)
         MessageInterface::ShowMessage("---- %f ", data[i]);
      MessageInterface::ShowMessage("-------------\n");
   #endif
   return true;
}

std::string Optimize::GetCallbackResults()
{
   std::string allResults;
   for (Integer i=0; i< (Integer) callbackResults.size(); i++)
      allResults += callbackResults.at(i) + ";";
   #ifdef DEBUG_CALLBACK
      MessageInterface::ShowMessage("Exiting Optimize::GetCallbackResults");
      MessageInterface::ShowMessage("-- callback results are: %s\n",
      allResults.c_str());
   #endif
   return allResults;
}


//------------------------------------------------------------------------------
// protected methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// StoreLoopData
//------------------------------------------------------------------------------
void Optimize::StoreLoopData()
{
   // Make local copies of all of the objects that may be affected by optimize
   // loop iterations
   std::map<std::string, GmatBase *>::iterator pair = objectMap->begin();
   GmatBase *obj;
    
   // Loop through the object map, looking for objects we'll need to restore.
   while (pair != objectMap->end()) {
      obj = (*pair).second;
      // Save copies of all of the spacecraft
      if (obj->GetType() == Gmat::SPACECRAFT)
      {
         Spacecraft *orig = (Spacecraft*)(obj);
         Spacecraft *sc = new Spacecraft(*orig);
         // Handle CoordinateSystems
         if (orig->GetInternalCoordSystem() == NULL)
   			MessageInterface::ShowMessage(
      			"Internal CS is NULL on spacecraft %s prior to optimizer cloning\n",
      			orig->GetName().c_str());
			if (orig->GetRefObject(Gmat::COORDINATE_SYSTEM, "") == NULL)
   			MessageInterface::ShowMessage(
      			"Coordinate system is NULL on spacecraft %s prior to optimizer cloning\n",
    			   orig->GetName().c_str());
         sc->SetInternalCoordSystem(orig->GetInternalCoordSystem());
         sc->SetRefObject(orig->GetRefObject(Gmat::COORDINATE_SYSTEM, ""),
            Gmat::COORDINATE_SYSTEM, "");
         
         localStore.push_back(sc);
      }
      ++pair;
   }
}

//------------------------------------------------------------------------------
// ResetLoopData
//------------------------------------------------------------------------------
void Optimize::ResetLoopData()
{
   Spacecraft *sc;
   std::string name;
    
   for (std::vector<GmatBase *>::iterator i = localStore.begin();
        i != localStore.end(); ++i) {
      name = (*i)->GetName();
      GmatBase *gb = (*objectMap)[name];
      if (gb != NULL) {
         sc = (Spacecraft*)gb;
         *sc = *((Spacecraft*)(*i));
      }
   }
}

//------------------------------------------------------------------------------
// FreeLoopData
//------------------------------------------------------------------------------
void Optimize::FreeLoopData()
{
   GmatBase *obj;
   while (!localStore.empty()) {
      obj = *(--localStore.end());
      localStore.pop_back();
      delete obj;
   }
}
