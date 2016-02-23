//------------------------------------------------------------------------------
//                           BeginFileThrust
//------------------------------------------------------------------------------
// GMAT: General Mission Analysis Tool
//
// Copyright (c) 2002-2015 United States Government as represented by the
// Administrator of The National Aeronautics and Space Administration.
// All Other Rights Reserved.
//
// Developed jointly by NASA/GSFC and Thinking Systems, Inc. under the FDSS II
// contract, Task Order 08
//
// Author: Darrel J. Conway, Thinking Systems, Inc.
// Created: Jan 13, 2016
/**
 * Toggle command used to tell GMAT to use a thrust data file
 */
//------------------------------------------------------------------------------


#include "BeginFileThrust.hpp"
#include "CommandException.hpp"
#include "MessageInterface.hpp"

//#define DEBUG_BEGIN_MANEUVER
//#define DEBUG_BEGIN_MANEUVER_EXE
//#define DEBUG_INITIALIZATION

BeginFileThrust::BeginFileThrust(const std::string &name) :
   GmatCommand             ("BeginFileThrust"),
   transientForces         (NULL),
   burnForce               (NULL),
   thfName                 (""),
   thrustFile              (NULL)
{
   objectTypeNames.push_back("BurnCommand");
   physicsBasedCommand = true;
}

BeginFileThrust::~BeginFileThrust()
{
}

BeginFileThrust::BeginFileThrust(const BeginFileThrust& bft) :
   GmatCommand             (bft),
   transientForces         (NULL),
   burnForce               (NULL),
   thfName                 (bft.thfName),
   thrustFile              (NULL),
   satNames                (bft.satNames)
{
   sats.clear();
}

BeginFileThrust& BeginFileThrust::operator=(const BeginFileThrust& bft)
{
   if (this != &bft)
   {
      GmatCommand::operator =(bft);

      thfName = bft.thfName;
      thrustFile = NULL;
      satNames = bft.satNames;
      sats.clear();
   }

   return *this;
}

GmatBase* BeginFileThrust::Clone() const
{
   return new BeginFileThrust(*this);
}

void BeginFileThrust::SetTransientForces(std::vector<PhysicalModel*>* tf)
{
   #ifdef DEBUG_TRANSIENT_FORCES
   MessageInterface::ShowMessage
      ("BeginFileThrust::SetTransientForces() tf=<%p>\n", tf);
   #endif
   transientForces = tf;
}


bool BeginFileThrust::TakeAction(const std::string& action,
      const std::string& actionData)
{
   #if DEBUG_TAKE_ACTION
      MessageInterface::ShowMessage
         ("BeginFiniteBurn::TakeAction() action=%s, actionData=%s\n",
          action.c_str(), actionData.c_str());
   #endif

   if (action == "Clear")
   {
      satNames.clear();
      return true;
   }

   return false;
}

std::string BeginFileThrust::GetRefObjectName(const Gmat::ObjectType type) const
{
   switch (type)
   {
//      case Gmat::INTERFACE:
      case Gmat::FINITE_BURN:
         #ifdef DEBUG_BEGIN_MANEUVER
            MessageInterface::ShowMessage
               ("Getting BeginFiniteBurn reference burn names\n");
         #endif
         return thfName;

      default:
         break;
   }

   return GmatCommand::GetRefObjectName(type);
}

const ObjectTypeArray& BeginFileThrust::GetRefObjectTypeArray()
{
   refObjectTypes.clear();
   refObjectTypes.push_back(Gmat::INTERFACE);
   refObjectTypes.push_back(Gmat::SPACECRAFT);
   return refObjectTypes;
}

const StringArray& BeginFileThrust::GetRefObjectNameArray(
      const Gmat::ObjectType type)
{

   refObjectNames.clear();

   if ((type == Gmat::UNKNOWN_OBJECT) ||
       (type == Gmat::SPACECRAFT))
   {
      refObjectNames.insert(refObjectNames.end(), satNames.begin(),
            satNames.end());
   }

   if (type == Gmat::UNKNOWN_OBJECT ||
       type == Gmat::INTERFACE)
   {
      refObjectNames.push_back(thfName);
   }

   return refObjectNames;
}

bool BeginFileThrust::SetRefObjectName(const Gmat::ObjectType type,
      const std::string& name)
{
   switch (type)
   {
      case Gmat::SPACECRAFT:
         #ifdef DEBUG_BEGIN_MANEUVER
            MessageInterface::ShowMessage
               ("Setting BeginFileThrust reference spacecraft \"%s\"\n",
                name.c_str());
         #endif
         if (find(satNames.begin(), satNames.end(), name) != satNames.end())
         {
            MessageInterface::ShowMessage("In the BeginFileThrust command %s, "
                  "the spacecraft %s is set more than once.  Only one instance "
                  "will be used.\n",
                  GetGeneratingString(Gmat::NO_COMMENTS).c_str(), name.c_str());
            return true;
         }
         satNames.push_back(name);
         if (satNames.size() > 1)
            throw CommandException("BeginFileThrust commands do not currently "
                  "support multiple Spacecraft; please toggle thrust history "
                  "file burns on one spacecraft at a time.");
         return true;

      // Note:  ThrustHistoryFile looks like FINITE_BURN to the Interpreter
      case Gmat::FINITE_BURN:
      case Gmat::INTERFACE:
         #ifdef DEBUG_BEGIN_MANEUVER
            MessageInterface::ShowMessage
               ("Setting BeginFileThrust reference burn \"%s\"\n",
                     name.c_str());
         #endif
         thfName = name;
         return true;

      default:
         #ifdef DEBUG_BEGIN_MANEUVER
            MessageInterface::ShowMessage
               ("BeginFileThrust reference object \"%s\" not set!\n",
                name.c_str());
         #endif
         break;
   }

   return GmatCommand::SetRefObjectName(type, name);
}

GmatBase* BeginFileThrust::GetGmatObject(const Gmat::ObjectType type,
      const std::string objName)
{
   if (type == Gmat::TRANSIENT_FORCE)
      return burnForce;
   return GmatCommand::GetGmatObject(type, objName);
}

bool BeginFileThrust::RenameRefObject(const Gmat::ObjectType type,
      const std::string& oldName, const std::string& newName)
{
   // BeginFiniteBurn needs to know about Burn and Spacecraft only
   if (type != Gmat::FINITE_BURN && type != Gmat::SPACECRAFT)
      return true;

   if (thfName == oldName)
      thfName = newName;

   for (UnsignedInt i = 0; i < satNames.size(); ++i)
      if (satNames[i] == oldName)
         satNames[i] = newName;

   return true;
}

const std::string& BeginFileThrust::GetGeneratingString(Gmat::WriteMode mode,
      const std::string& prefix, const std::string& useName)
{
   generatingString = prefix + "BeginFileThrust " + thfName + "(";
   for (StringArray::iterator satIt = satNames.begin();
        satIt != satNames.end(); ++satIt)
   {
      if (satIt != satNames.begin())
         generatingString += ", ";
      generatingString += *satIt;
   }

   generatingString += ");";

   return GmatCommand::GetGeneratingString(mode, prefix, useName);
}


//------------------------------------------------------------------------------
// bool BeginFileThrust::Initialize()
//------------------------------------------------------------------------------
/**
 * Prepares for inserting a thrust history file into the force model
 *
 * @return True is all of the membner structures are in place, false if not
 */
//------------------------------------------------------------------------------
bool BeginFileThrust::Initialize()
{
   bool retval = GmatCommand::Initialize();

   #ifdef DEBUG_INITIALIZATION
      MessageInterface::ShowMessage("BeginFileThrust::Initialize() entered; "
            "GmatCommand %s\n", (retval ? "initialized" :
             "failed initialization"));
   #endif

   GmatBase *mapObj = NULL;
   if (retval)
   {
      // Look up the ThrustHistoryFile object
      if ((mapObj = FindObject(thfName)) == NULL)
         throw CommandException("Unknown ThrustHistoryFile \"" + thfName + "\"\n");
      if (mapObj->IsOfType("ThrustHistoryFile") == false)
         throw CommandException(thfName + " is not a ThrustHistoryFile\n");

      thrustFile = (ThrustHistoryFile*)mapObj;

      #ifdef DEBUG_INITIALIZATION
         MessageInterface::ShowMessage("Initializing the thrust history file "
               "object\n");
      #endif

      if (thrustFile->Initialize())
      {
         GmatBase *burn = thrustFile->GetForce();
         if (burn != NULL)
         {
            burnForce = (FileThrust*)(burn->Clone());
            retval = true;
         }
         else
            throw CommandException("Unable to load the thrust history file "
                  "force");

         if (retval)
         {
            // find all of the spacecraft
            StringArray::iterator scName;
            Spacecraft *sc;
            sats.clear();
            for (scName = satNames.begin(); scName != satNames.end(); ++scName)
            {
               if ((mapObj = FindObject(*scName)) == NULL)
                  throw CommandException("Unknown SpaceObject \"" +
                        (*scName) + "\"");

               if (mapObj->IsOfType(Gmat::SPACECRAFT) == false)
                  throw CommandException((*scName) + " is not a Spacecraft");
               sc = (Spacecraft*)mapObj;

               #ifdef DEBUG_BEGIN_MANEUVER
                  MessageInterface::ShowMessage(
                     "BeginFiniteBurn::Initialize() found %s with type %s\n",
                     scName->c_str(), sc->GetTypeName().c_str());
               #endif

               sats.push_back(sc);
            }

            #ifdef DEBUG_MEMORY
            MemoryTracker::Instance()->Remove
               (burnForce, burnForce->GetName(), "BeginFiniteBurn::Initialize()",
                "deleting burn force");
            #endif

            if (transientForces != NULL)
            {
               std::vector<PhysicalModel *>::iterator transient = find(
                  transientForces->begin(), transientForces->end(), burnForce);
               if (transient != transientForces->end())
                  transientForces->erase(transient);
            }
         }

         Gmat::ObjectType type = Gmat::SPACECRAFT;
         StringArray::iterator iter;

         // load up the spacecraft name list
         for (iter = satNames.begin(); iter != satNames.end(); ++iter)
         {
            #ifdef DEBUG_BEGIN_MANEUVER
               MessageInterface::ShowMessage(
                  "BeginFiniteBurn::Initialize() setting %s on %s\n",
                  iter->c_str(), thrustName.c_str());
            #endif
            burnForce->SetRefObjectName(type, *iter);
         }
      }
   }
   return retval;
}

//------------------------------------------------------------------------------
// bool Execute()
//------------------------------------------------------------------------------
/**
 * The method that is fired to turn on thrusters.
 *
 * @return true if the GmatCommand runs to completion, false if an error
 *         occurs.
 */
//------------------------------------------------------------------------------
bool BeginFileThrust::Execute()
{
   bool retval = true; // false;

   // Tell active spacecraft that they are now firing
   for (std::vector<Spacecraft*>::iterator s=sats.begin(); s!=sats.end(); ++s)
   {
      #ifdef DEBUG_EXECUTION
         MessageInterface::ShowMessage("Setting maneuvering state on %s\n",
               (*s)->GetName().c_str());
      #endif
      (*s)->IsManeuvering(true);
   }

   if (transientForces == NULL)
      throw CommandException("Transient force list was NOT initialized; "
            "ABORTING RUN!!!\n\n");

   if (burnForce == NULL)
      throw CommandException("Thrust history file burn was NOT initialized; "
            "ABORTING RUN!!!\n\n");

   // Insert the force into the list of transient forces if not found
   if (transientForces->size() == 0)
   {
      #ifdef DEBUG_TRANSIENT_FORCES
      MessageInterface::ShowMessage
         ("BeginFiniteBurn::Execute() Adding first burnForce<%p>'%s' to "
               "transientForces\n", burnForce, burnForce->GetName().c_str());
      #endif
      transientForces->push_back(burnForce);
   }
   else
   {
      bool alreadyThere = false;
      for (std::vector<PhysicalModel*>::iterator i = transientForces->begin();
            i !=transientForces->end(); ++i)
      {
         if ((*i)->IsOfType("FileThrust"))
         {
            FileThrust *transient = (FileThrust*)(*i);
            if (transient == burnForce)
            {
               alreadyThere = true;
            }

            if ((*transient) == (*burnForce))
            {
               MessageInterface::ShowMessage("Burn activated by the line\n   "
                     "%s\noverlaps with an active file burn.  No new file "
                     "burn will be applied.\n",
                     GetGeneratingString(Gmat::NO_COMMENTS).c_str());
               alreadyThere = true;
            }
         }
      }
      if (alreadyThere == false)
      {
         #ifdef DEBUG_TRANSIENT_FORCES
         MessageInterface::ShowMessage("BeginFileBurn::Execute() Adding "
               "burnForce<%p>'%s' to transientForces\n", burnForce,
               burnForce->GetName().c_str());
         #endif
         transientForces->push_back(burnForce);
      }
   }

   // Set maneuvering to Publisher so that any subscriber can do its own action
   if (!sats.empty())
   {
      Real epoch = sats[0]->GetEpoch();
      publisher->SetManeuvering(this, true, epoch, satNames,
            "begin of thrust history file maneuver");
   }

   #ifdef DEBUG_BEGIN_MANEUVER_EXE
      MessageInterface::ShowMessage
         ("There are %d transient force(s)\n", transientForces->size());
      MessageInterface::ShowMessage("Current TransientForces list:\n");
      for (std::vector<PhysicalModel*>::iterator j = transientForces->begin();
           j != transientForces->end(); ++j)
         MessageInterface::ShowMessage("   %s\n", (*j)->GetName().c_str());
   #endif

   BuildCommandSummary(true);
   return retval;
}
