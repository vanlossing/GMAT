//$Header$
//------------------------------------------------------------------------------
//                              ForceModel
//------------------------------------------------------------------------------
// *** File Name : ForceModel.cpp
// *** Created   : October 1, 2002
// **************************************************************************
// ***  Developed By  :  Thinking Systems, Inc. (www.thinksysinc.com)     ***
// ***  For:  Flight Dynamics Analysis Branch (Code 572)                  ***
// ***  Under Contract:  P.O.  GSFC S-66617-G                             ***
// ***                                                                    ***
// ***  Copyright U.S. Government 2002                                    ***
// ***  Copyright United States Government as represented by the          ***
// ***  Administrator of the National Aeronautics and Space               ***
// ***  Administration                                                    ***
// ***                                                                    ***
// ***  This software is subject to the Sofware Usage Agreement described ***
// ***  by NASA Case Number GSC-14735-1.  The Softare Usage Agreement     ***
// ***  must be included in any distribution.  Removal of this header is  ***
// ***  strictly prohibited.                                              ***
// ***                                                                    ***
// ***                                                                    ***
// ***  Header Version: July 12, 2002                                     ***
// **************************************************************************
// Module Type               : ANSI C++ Source
// Development Environment   : Visual C++ 7.0
// Modification History      : 11/26/2002 - D. Conway, Thinking Systems, Inc.
//                             Original delivery
//
//                           : 2/28/2003 - D. Conway, Thinking Systems, Inc.
//                             Filled in some code to model cases with 
//                             discontinuous forces (e.g. SRP)
//
//                           : 10/01/2003 - W. Waktola, Missions Applications Branch
//                              Changes:
//                                - Updated style using GMAT cpp style guide
//                                - Changed FlightDynamicsForces class to ForceModel class
//
//                           : 10/20/2003 - W. Waktola, Missions Applications Branch
//                              Changes:
//                                - All double types to Real types
//                                - All primitive int types to Integer types
//
//                           : 11/9/2003 - D. Conway, Thinking Systems, Inc.
//                             Overrode GetParameterCount so the count 
//                             increases based on the member forces
// **************************************************************************
//===> loj: 3/30/04 NO changes - just commented out the debug statements for delivery

#include "ForceModel.hpp"
#include <string.h> // waw: added 03/10/04
#include "PointMassForce.hpp"
#include "MessageInterface.hpp"

#define normType -2

//---------------------------------
// static data
//---------------------------------

const std::string
ForceModel::PARAMETER_TEXT[ForceModelParamCount - PhysicalModelParamCount] =
{
    "PointMass",
    "FullField",
    "Drag",        //loj: 3/19/04 This is also in PropSetup. Where do we want to handle?
    "MagField",
    "ForceList",
};

const Gmat::ParameterType
ForceModel::PARAMETER_TYPE[ForceModelParamCount - PhysicalModelParamCount] =
{
    Gmat::STRING_TYPE,
    Gmat::STRING_TYPE,
    Gmat::STRING_TYPE,
    Gmat::STRING_TYPE,
    Gmat::STRINGARRAY_TYPE,
};

//---------------------------------
// public
//---------------------------------

//------------------------------------------------------------------------------
// ForceModel::ForceModel(void)
//------------------------------------------------------------------------------
/**
 * The constructor
 */
//------------------------------------------------------------------------------
//ForceModel::ForceModel(void) :
//PhysicalModel     (NULL, NULL, NULL),
//derivatives       (NULL),
//estimationMethod  (2.0)
//{
//    dimension = 6;
//}

//------------------------------------------------------------------------------
// ForceModel::ForceModel(Gmat::ObjectType id, const std::string &typeStr,
//                        const std::string &nomme))
//------------------------------------------------------------------------------
/**
 * The constructor
 */
//------------------------------------------------------------------------------
ForceModel::ForceModel(const std::string &nomme) :
    PhysicalModel     (Gmat::FORCE_MODEL, "ForceModel", nomme),
//    derivatives       (NULL),  waw:  06/03/04
//    forceCount        (0),  waw: 05/06/04
    estimationMethod  (2.0),
    previousState     (NULL)
{
    numForces = 0;
    dimension = 6;
    currentForce = 0;  // waw: added 06/04/04
    parameterCount = ForceModelParamCount;
}

//------------------------------------------------------------------------------
// ForceModel::~ForceModel(void)
//------------------------------------------------------------------------------
/**
 * The destructor
 * The destructor deletes the list of PhysicalModel instances.
 */
//------------------------------------------------------------------------------
ForceModel::~ForceModel(void)
{
//  waw: 06/03/04
//    if (derivatives)
//        delete derivatives;
    if (previousState)
        delete [] previousState;
}

//------------------------------------------------------------------------------
// ForceModel::ForceModel(const ForceModel& fdf)
//------------------------------------------------------------------------------
/**
 * Copy constructor
 *
 * NOTE: The ForceModel copy constructor is not yet implemented.  This
 * method should be completed before the class is used in external code.
 *
 * @param fdf   The original of the ForceModel that are copied
 */
//------------------------------------------------------------------------------
ForceModel::ForceModel(const ForceModel& fdf) :
    PhysicalModel    (fdf)
{
}

//------------------------------------------------------------------------------
// ForceModel& ForceModel::operator=(const ForceModel& fdf)
//------------------------------------------------------------------------------
/**
 * The assignment operator
 * 
 * NOTE: The ForceModel assignment operator is not yet implemented.  
 * This method should be completed before the class is used in external code.
 * 
 * @param fdf   The original of the ForceModel that are copied
 */
//------------------------------------------------------------------------------
ForceModel& ForceModel::operator=(const ForceModel& fdf)
{
    if (&fdf == this)
        return *this;

    return *this;
}

//------------------------------------------------------------------------------
// void ForceModel::AddForce(PhysicalModel *pPhysicalModel)
//------------------------------------------------------------------------------
/**
 * Method used to add a new force to the force model
 *
 * This method takes the pointer to the new force and adds it to the force model
 * list for later use.  Each force should supply first derivative information 
 * for elements 4 through 6 of a state vector, and zeros for the first three 
 * elements.  The forces should have the ability to act on state vectors for 
 * formations as well, by filling in elements (6*n+4) through (6*n+6) for larger
 * state vectors.
 * 
 * The force that is passed in is owned by this class (actually, by the member 
 * DerivativeList), and should not be destructed externally.  In addition, every 
 * force that is passed to this class needs to have a copy constructor and an 
 * assignment operator so that it can be cloned for distribution to multiple
 * satellites.
 * 
 * @param pPhysicalModel        The force that is being added to the force model
 */
//------------------------------------------------------------------------------
void ForceModel::AddForce(PhysicalModel *pPhysicalModel)
{
    //MessageInterface::ShowMessage("ForceModel::AddForce() entered\n");
    
    if (pPhysicalModel == NULL)
        return;
// waw: 06/03/04
//    if (derivatives == NULL)
//        derivatives = new DerivativeList;

    pPhysicalModel->SetDimension(dimension);
// waw: 06/03/04
//    derivatives->AddForce(pPhysicalModel);
    initialized = false;

    forceList.push_back(pPhysicalModel);
    numForces = forceList.size();
    
    //++forceCount;  waw: 05/06/04
}

//------------------------------------------------------------------------------
// void DeleteForce(const std::string &name)
//------------------------------------------------------------------------------
/**
 * Deletes force from the force model.
 * @param    name   The force name to delete
 */
//------------------------------------------------------------------------------
void ForceModel::DeleteForce(const std::string &name)
{
    std::vector<PhysicalModel *>::iterator force;
    Integer i = 0;
    for (force = forceList.begin(); force != forceList.end(); force++) 
    {
        PhysicalModel *pm = forceList[i];
        std::string pmName = pm->GetName().c_str();
        
        if ( strcmp(name.c_str(), pmName.c_str()) == 0 )
        {
            forceList.erase(force);
            //delete pm;
            numForces = forceList.size();
            return;
        }
        i++;
    }
}

//------------------------------------------------------------------------------
// bool HasForce(const std::string &name)
//------------------------------------------------------------------------------
/**
 * Search force in the force model.
 * @param  name The force name to look up
 * @return true if force exists, else false
 */
//------------------------------------------------------------------------------
bool ForceModel::HasForce(const std::string &name)
{
    std::vector<PhysicalModel *>::iterator force;
    Integer i = 0;
    for (force = forceList.begin(); force != forceList.end(); force++) 
    {
        PhysicalModel *pm = forceList[i];
        std::string pmName = pm->GetName().c_str();
        
        if ( strcmp(name.c_str(), pmName.c_str()) == 0 )
        {
            return true;
        }
        i++;
    }
    return false; 
}

//------------------------------------------------------------------------------
// Integer GetNumForces()
//------------------------------------------------------------------------------
Integer ForceModel::GetNumForces()
{
    return numForces;
}

//------------------------------------------------------------------------------
// StringArray& GetForceTypeNames()
//------------------------------------------------------------------------------
StringArray& ForceModel::GetForceTypeNames()
{
    forceTypeNames.clear();

    for (int i=0; i<numForces; i++)
    {
        forceTypeNames.push_back(forceList[i]->GetTypeName());
    }
        
    return forceTypeNames;
}

//------------------------------------------------------------------------------
// std::string GetForceTypeName(Integer index)
//------------------------------------------------------------------------------
std::string ForceModel::GetForceTypeName(Integer index)
{
    StringArray typeList = GetForceTypeNames();
    
    if (index >= 0 && index < numForces)
    {
        return typeList[index];
    }

    return "UNDEFINED_FORCE_TYPE";
}

//------------------------------------------------------------------------------
// void ClearSpacecraft()
//------------------------------------------------------------------------------
void ForceModel::ClearSpacecraft()
{
    spacecraft.clear();
}

//------------------------------------------------------------------------------
// PhysicalModel* GetForce(Integer index)
//------------------------------------------------------------------------------
PhysicalModel* ForceModel::GetForce(Integer index)
{
    if (index >= 0 && index < numForces)
    {
        return forceList[index];
    }
    
    return NULL;
}


//------------------------------------------------------------------------------
// bool ForceModel::AddSpacecraft(Spacecraft *sc)
//------------------------------------------------------------------------------
/**
 * Sets spacecraft that use this force model.
 *
 * @param sc The spacecraft
 *
 * @return true is the spacecraft is added to the list, false if it was already
 *         in the list, or if it is NULL.
 */
//------------------------------------------------------------------------------
bool ForceModel::AddSpacecraft(Spacecraft *sc)
{
    if (sc == NULL)
        return false;
    if (find(spacecraft.begin(), spacecraft.end(), sc) != spacecraft.end())
        return false;
    spacecraft.push_back(sc);

    // Quick fix for the epoch update
    epoch = sc->GetRealParameter(sc->GetParameterID("Epoch"));
    return true;
}


//------------------------------------------------------------------------------
// void ForceModel::UpdateSpacecraft(void)
//------------------------------------------------------------------------------
/**
 * Updates the state data for the spacecraft that use this force model.
 */
//------------------------------------------------------------------------------
void ForceModel::UpdateSpacecraft(Real newEpoch)
{
    if (spacecraft.size() > 0) {
        Integer j = 0;
        Integer stateSize = 6;
        Integer vectorSize = stateSize * sizeof(Real);
        std::vector<Spacecraft *>::iterator sat;
        Real *state;

        for (sat = spacecraft.begin(); sat != spacecraft.end(); ++sat) {
            state = (*sat)->GetState();
            memcpy(&previousState[j*stateSize], state, vectorSize);
            previousTime = 
                ((*sat)->GetRealParameter((*sat)->GetParameterID("Epoch")) - epoch)
                * 86400.0;
            memcpy(state, &modelState[j*stateSize], vectorSize);
            ++j;            
//            // Update the epoch if it was passed in
//            if (newEpoch != -1.0)
//                (*sat)->SetRealParameter((*sat)->GetParameterID("Epoch"), newEpoch);
              // Quick fix to get the epoch updated
              Real newepoch = epoch + elapsedTime / 86400.0;      
              (*sat)->SetRealParameter((*sat)->GetParameterID("Epoch"), newepoch);
            
        }
    }
}


//------------------------------------------------------------------------------
// void ForceModel::UpdateSpacecraft(void)
//------------------------------------------------------------------------------
/**
 * Updates the model state data from the spacecraft state -- useful to revert
 * to a previous step.
 *
 * @note This method will need to be updated when the multi-step integrators are
 *       folded into the code
 */
//------------------------------------------------------------------------------
void ForceModel::UpdateFromSpacecraft(void)
{
    if (spacecraft.size() > 0) 
    {
        Integer j = 0;
        Integer stateSize = 6;
        std::vector<Spacecraft *>::iterator sat;
        Real *state;
        for (sat = spacecraft.begin(); sat != spacecraft.end(); ++sat) 
        {
            state = (*sat)->GetState();
            memcpy(&modelState[j*stateSize], state, stateSize * sizeof(Real));
            ++j;
        }
    }
}


//------------------------------------------------------------------------------
// void ForceModel::RevertSpacecraft(void)
//------------------------------------------------------------------------------
/**
 * Resets the model state data from the previous spacecraft state.
 *
 * @note This method will need to be updated when the multi-step integrators are
 *       folded into the code
 */
//------------------------------------------------------------------------------
void ForceModel::RevertSpacecraft(void)
{
   elapsedTime = previousTime;
   memcpy(modelState, previousState, dimension*sizeof(Real)); 
}


//------------------------------------------------------------------------------
// bool ForceModel::Initialize(void)
//------------------------------------------------------------------------------
/**
 * Initializes model and all contained models
 */
//------------------------------------------------------------------------------
bool ForceModel::Initialize(void)
{
    Integer stateSize = 6;      // Will change if we integrate more variables
    Integer satCount = 1;
    std::vector<Spacecraft *>::iterator sat;

    if (spacecraft.size() > 0)
        satCount = spacecraft.size();
    
    dimension = stateSize * satCount;
    if (!PhysicalModel::Initialize())
        return false;
    previousState = new Real[dimension];

    if (spacecraft.size() == 0) 
    {
        modelState[0] = 7000.0;
        modelState[1] =    0.0;
        modelState[2] = 1000.0;
        modelState[3] =    0.0;
        modelState[4] =    7.4;
        modelState[5] =    0.0;
    }
    else 
    {
        Integer j = 0;
        Real *state;
        for (sat = spacecraft.begin(); sat != spacecraft.end(); ++sat) 
        {
            state = (*sat)->GetState();
            memcpy(&modelState[j*stateSize], state, stateSize * sizeof(Real));
            ++j;
        }
    }
    
    previousTime = 0.0;
    memcpy(previousState, modelState, dimension*sizeof(Real));

//    DerivativeList *current = derivatives;  waw: 06/03/04
    Integer cf = currentForce;
    PhysicalModel *current = GetForce(cf);  // waw: added 06/04/04
    PhysicalModel *currentPm;

    // Variables used to set spacecraft parameters
    std::string parmName, stringParm;
    Real parm;
    Integer id, i;

    while (current) 
    {
//        currentPm = current->GetDerivative();  waw: 06/03/04
        currentPm = current;  // waw: added 06/04/04 
        currentPm->SetDimension(dimension);
        currentPm->SetSolarSystem(solarSystem);
        if (!currentPm->Initialize()) 
        {
           std::string msg = "Component force ";
           msg += currentPm->GetTypeName();
           msg += " failed to initialize";
           throw ForceModelException(msg.c_str());
        }
        currentPm->SetState(modelState);
        
        // Set spacecraft parameters for forces that need them
        i = 0;
        for (sat = spacecraft.begin(); sat != spacecraft.end(); ++sat) 
        {
            parmName = "Epoch";
            id = (*sat)->GetParameterID(parmName);
            parm = (*sat)->GetRealParameter(id);
            id = currentPm->GetParameterID(parmName);
            currentPm->SetRealParameter(id, parm);
        
            parmName = "ReferenceBody";
            id = (*sat)->GetParameterID(parmName);
            stringParm = (*sat)->GetStringParameter(id);
            currentPm->SetSatelliteParameter(i, parmName, stringParm);
        
            parmName = "DryMass";
            id = (*sat)->GetParameterID(parmName);
            parm = (*sat)->GetRealParameter(id);
            currentPm->SetSatelliteParameter(i, parmName, parm);
        
            parmName = "CoefficientDrag";
            id = (*sat)->GetParameterID(parmName);
            parm = (*sat)->GetRealParameter(id);
            currentPm->SetSatelliteParameter(i, parmName, parm);
            
            parmName = "DragArea";
            id = (*sat)->GetParameterID(parmName);
            parm = (*sat)->GetRealParameter(id);
            currentPm->SetSatelliteParameter(i, parmName, parm);
            
            parmName = "ReflectivityCoefficient";
            id = (*sat)->GetParameterID(parmName);
            parm = (*sat)->GetRealParameter(id);
            currentPm->SetSatelliteParameter(i, parmName, parm);
            
            ++i;
        }
        // current = current->Next(); waw: 06/04/04
        // waw: added 06/04/04
        cf++;
        current = GetForce(cf);
    }

    return true;
}

//------------------------------------------------------------------------------
// void ForceModel::IncrementTime(Real dt)
//------------------------------------------------------------------------------
void ForceModel::IncrementTime(Real dt)
{
    PhysicalModel::IncrementTime(dt);
// waw: 06/03/04
//    DerivativeList *current = derivatives;
//    while (current) 
//    {
//        current->GetDerivative()->IncrementTime(dt);
//        current = current->Next();
//    }
}

//------------------------------------------------------------------------------
// void ForceModel::SetTime(Real t)
//------------------------------------------------------------------------------
void ForceModel::SetTime(Real t)
{
    PhysicalModel::SetTime(t);
// waw: 06/03/04
//    DerivativeList *current = derivatives;
//
//    while (current) 
//    {
//        current->GetDerivative()->SetTime(t);
//        current = current->Next();
//    }
}

//------------------------------------------------------------------------------
// bool ForceModel::GetDerivatives(Real * state, Real dt, Integer order)
//------------------------------------------------------------------------------
/**
 * Returns the accumulated superposition of forces 
 * 
 * This method applies superposition of forces in order to calculate the total
 * acceleration applied to the state vector.
 * 
 * NOTE: GetDerivatives is not yet implemented.
 * 
 * @param    state   The current state vector
 * @param    dt      The current time interval from epoch
 * @param    order   Order of the derivative to be taken
 */
//------------------------------------------------------------------------------
bool ForceModel::GetDerivatives(Real * state, Real dt, Integer order)
{
    if (order > 2)
        return false;
    if (!initialized)
        return false;

    if (order == 1) //loj: changed from =
    {
        deriv[0] = state[3];
        deriv[1] = state[4];
        deriv[2] = state[5];
        deriv[3] = deriv[4] = deriv[5] = 0.0;
    }
    else 
    {
        deriv[0] = deriv[1] = deriv[2] = 
        deriv[3] = deriv[4] = deriv[5] = 0.0;
    }
// waw: 06/03/04
//    DerivativeList *current = derivatives;
    // waw: added 06/04/04
    Integer cf = currentForce;
    PhysicalModel *current = GetForce(cf);  

    const Real * ddt;
    while (current) 
    {
        // waw: 06/04/04
        //ddt = current->GetDerivative()->GetDerivativeArray();
        ddt = GetForce(cf)->GetDerivativeArray();
        //if (!current->GetDerivative()->GetDerivatives(state, dt, order)) 
        if (!current->GetDerivatives(state, dt, order))
            return false;

        if (order == 1) //loj: changed from =
        {
            deriv[3] += ddt[3];
            deriv[4] += ddt[4];
            deriv[5] += ddt[5];
        }
        else 
        {
            deriv[0] += ddt[0];
            deriv[1] += ddt[1];
            deriv[2] += ddt[2];
        }
        //current = current->Next(); waw: 06/04/04
        // waw: added 06/04/04
        cf++;
        current = GetForce(cf);
    }

    return true;
}

//------------------------------------------------------------------------------
// Real ForceModel::EstimateError(Real *diffs, Real *answer) const
//------------------------------------------------------------------------------
/**
 * Interface used to estimate the error in the current step
 * 
 * The method calculates the largest local estimate of the error from the 
 * integration given the components of the differences calculated bt the 
 * integrator.  It returns the largest error estimate found.  
 *   
 * The default implementation returns the largest single relative error 
 * component found based on the input arrays.  In other words, the 
 * implementation provided here returns the largest component of the following
 * vector:
 * 
 * \f[\vec \epsilon = |{{{EE}_n}\over{x_n^f - x_n^i}}|\f]
 *   
 * There are several alternatives that users of this class can implement: the 
 * error could be calculated based on the largest error in the individual 
 * components of the state vector, as the magnitude of the state vector (that 
 * is, the L2 (rss) norm of the error estimate vector).  The estimated error 
 * should never be negative, so a return value less than 0.0 can be used to 
 * indicate an error condition.
 *     
 * @param diffs         Array of differences calculated by the integrator.  This array 
 *                  must be the same size as the state vector
 * @param answer        Candidate new state from the integrator
 */
//------------------------------------------------------------------------------
Real ForceModel::EstimateError(Real *diffs, Real *answer) const
{
    if (estimationMethod == 1.0)
        return PhysicalModel::EstimateError(diffs, answer);

    Real retval = 0.0, err, mag, vec[3];

    for (int i = 0; i < dimension; i += 3) 
    {
        switch (normType) 
        {

            case -2:
                // Code for the L2 norm, based on sep from central body
                vec[0] = 0.5 * (answer[ i ] + modelState[ i ]); 
                vec[1] = 0.5 * (answer[i+1] + modelState[i+1]); 
                vec[2] = 0.5 * (answer[i+2] + modelState[i+2]);

                mag = vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2];        
                err = diffs[i]*diffs[i] + diffs[i+1]*diffs[i+1] + diffs[i+2]*diffs[i+2];
                if (mag >relativeErrorThreshold) 
                    err = sqrt(err / mag);
                else
                    err = sqrt(err);
                break;

            case -1:
                // L1 norm, based on sep from central body
                vec[0] = fabs(0.5 * (answer[ i ] + modelState[ i ])); 
                vec[1] = fabs(0.5 * (answer[i+1] + modelState[i+1])); 
                vec[2] = fabs(0.5 * (answer[i+2] + modelState[i+2]));

                mag = vec[0] + vec[1] + vec[2];        
                err = fabs(diffs[i]) + fabs(diffs[i+1]) + fabs(diffs[i+2]);
                if (mag >relativeErrorThreshold) 
                    err = err / mag;
                break;

            case 0:         // Report no error here
                return 0.0;

            case 1:
                // L1 norm
                vec[0] = fabs(answer[ i ] - modelState[ i ]); 
                vec[1] = fabs(answer[i+1] - modelState[i+1]); 
                vec[2] = fabs(answer[i+2] - modelState[i+2]);

                mag = vec[0] + vec[1] + vec[2];        
                err = fabs(diffs[i]) + fabs(diffs[i+1]) + fabs(diffs[i+2]);
                if (mag >relativeErrorThreshold) 
                    err = err / mag;
                break;

            case 2:
            default:
                // Code for the L2 norm
                vec[0] = answer[ i ] - modelState[ i ]; 
                vec[1] = answer[i+1] - modelState[i+1]; 
                vec[2] = answer[i+2] - modelState[i+2];

                mag = vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2];        
                err = diffs[i]*diffs[i] + diffs[i+1]*diffs[i+1] + diffs[i+2]*diffs[i+2];
                if (mag >relativeErrorThreshold) 
                    err = sqrt(err / mag);
                else
                    err = sqrt(err);
        }

        if (err > retval)
        {
            retval = err;
        }
    }

    return retval;
}

//------------------------------------------------------------------------------
// Integer GetParameterCount(void) const
//------------------------------------------------------------------------------
Integer ForceModel::GetParameterCount(void) const
{
    return parameterCount;
}


// Access methods 
//------------------------------------------------------------------------------
// std::string GetParameterText(const Integer id) const
//------------------------------------------------------------------------------
std::string ForceModel::GetParameterText(const Integer id) const
{
    if (id >= PhysicalModelParamCount && id < ForceModelParamCount)
        return PARAMETER_TEXT[id - PhysicalModelParamCount];
    else
        return PhysicalModel::GetParameterText(id);
}

//------------------------------------------------------------------------------
// Integer GetParameterID(const std::string &str) const
//------------------------------------------------------------------------------
Integer ForceModel::GetParameterID(const std::string &str) const
{
    for (int i = PhysicalModelParamCount; i < ForceModelParamCount; i++)
    {
        if (str == PARAMETER_TEXT[i - PhysicalModelParamCount])
            return i;
    }

    return PhysicalModel::GetParameterID(str);
}

//------------------------------------------------------------------------------
// Gmat::ParameterType GetParameterType(const Integer id) const
//------------------------------------------------------------------------------
Gmat::ParameterType ForceModel::GetParameterType(const Integer id) const
{
    if (id >= PhysicalModelParamCount && id < ForceModelParamCount)
        return PARAMETER_TYPE[id - PhysicalModelParamCount];
    else
        return PhysicalModel::GetParameterType(id);
}

//------------------------------------------------------------------------------
// std::string GetParameterTypeString(const Integer id) const
//------------------------------------------------------------------------------
std::string ForceModel::GetParameterTypeString(const Integer id) const
{

    if (id >= PhysicalModelParamCount && id < ForceModelParamCount)
        return GmatBase::PARAM_TYPE_STRING[GetParameterType(id - PhysicalModelParamCount)];
    else
        return PhysicalModel::GetParameterTypeString(id);
}

//------------------------------------------------------------------------------
// std::string GetStringParameter(const Integer id) const
//------------------------------------------------------------------------------
std::string ForceModel::GetStringParameter(const Integer id) const
{
    switch (id)
    {
    case POINT_MASS:
        //loj: what should we return here?
        return "PointMassForce";
    case FULL_FIELD:
        return "TBD-FullFieldForce";
    case DRAG:
        return "TBD-DragForce";
    case MAG_FIELD:
        return "TBD-MagFieldForce";
    default:
        return PhysicalModel::GetStringParameter(id);
    }
}

//------------------------------------------------------------------------------
// std::string GetStringParameter(const std::string &label) const
//------------------------------------------------------------------------------
std::string ForceModel::GetStringParameter(const std::string &label) const
{
    return GetStringParameter(GetParameterID(label));
}

//------------------------------------------------------------------------------
// bool SetStringParameter(const Integer id, const std::string &value)
//------------------------------------------------------------------------------
bool ForceModel::SetStringParameter(const Integer id, const std::string &value)
{
    switch (id)
    {
    case POINT_MASS:
        {
            PhysicalModel *pmf = new PointMassForce();
            if (pmf != NULL)
            {
                if (pmf->SetStringParameter("Body", value))
                {
                    AddForce(pmf);
                    return true;
                }
            }
            return false;
        }
    case FULL_FIELD:
        // build 3
        return false;
    case DRAG:
        // build 3
        return false;
    case MAG_FIELD:
        // build 3
        return false;
    default:
        return PhysicalModel::SetStringParameter(id, value);
    }
}

//------------------------------------------------------------------------------
// bool SetStringParameter(const std::string &label,const std::string &value)
//------------------------------------------------------------------------------
bool ForceModel::SetStringParameter(const std::string &label,
                                    const std::string &value)
{
    return SetStringParameter(GetParameterID(label), value);
}

//------------------------------------------------------------------------------
// const StringArray& GetStringArrayParameter(const Integer id) const
//------------------------------------------------------------------------------
const StringArray& ForceModel::GetStringArrayParameter(const Integer id) const
{
    switch (id)
    {
    case FORCE_LIST:
        return forceTypeNames;
    default:
        return PhysicalModel::GetStringArrayParameter(id);
    }
}

//------------------------------------------------------------------------------
// StringArray& GetStringArrayParameter(const std::string &label) const
//------------------------------------------------------------------------------
const StringArray& ForceModel::GetStringArrayParameter(const std::string &label) const
{
    return GetStringArrayParameter(GetParameterID(label));
}
