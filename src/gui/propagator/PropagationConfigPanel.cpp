//$Header$
//------------------------------------------------------------------------------
//                              PropagationConfigPanel
//------------------------------------------------------------------------------
// GMAT: Goddard Mission Analysis Tool
//
// Author: Waka Waktola
// Created: 2003/08/29
//
/**
 * This class contains the Propagation Configuration window.
 */
//------------------------------------------------------------------------------

// gui includes
#include "gmatwxdefs.hpp"
#include <string.h>
#include <wx/variant.h>
//#include "ViewTextFrame.hpp"
//#include "DocViewFrame.hpp"
//#include "TextEditView.hpp"
//#include "TextDocument.hpp"
//#include "MdiTextEditView.hpp"
//#include "MdiDocViewFrame.hpp"
#include "GmatAppData.hpp"
#include "GmatMainNotebook.hpp"
#include "CelesBodySelectDialog.hpp"
#include "PropagationConfigPanel.hpp"

// base includes
#include "gmatdefs.hpp"
#include "GuiInterpreter.hpp"
#include "Propagator.hpp"
#include "PropSetup.hpp"
#include "ForceModel.hpp"
#include "Planet.hpp"
#include "MessageInterface.hpp"

//------------------------------------------------------------------------------
// event tables and other macros for wxWindows
//------------------------------------------------------------------------------
BEGIN_EVENT_TABLE(PropagationConfigPanel, wxPanel)
    EVT_BUTTON(ID_BUTTON_SCRIPT, PropagationConfigPanel::OnScriptButton)
    EVT_BUTTON(ID_BUTTON_OK, PropagationConfigPanel::OnOKButton)
    EVT_BUTTON(ID_BUTTON_APPLY, PropagationConfigPanel::OnApplyButton)
    EVT_BUTTON(ID_BUTTON_CANCEL, PropagationConfigPanel::OnCancelButton)
    EVT_BUTTON(ID_BUTTON_ADD_BODY, PropagationConfigPanel::OnAddButton)
    EVT_BUTTON(ID_BUTTON_GRAV_SEARCH, PropagationConfigPanel::OnGravSearchButton)
    EVT_BUTTON(ID_BUTTON_SETUP, PropagationConfigPanel::OnSetupButton)
    EVT_BUTTON(ID_BUTTON_MAG_SEARCH, PropagationConfigPanel::OnMagSearchButton)
    EVT_BUTTON(ID_BUTTON_PM_EDIT, PropagationConfigPanel::OnPMEditButton)
    EVT_BUTTON(ID_BUTTON_SRP_EDIT, PropagationConfigPanel::OnSRPEditButton)
    EVT_TEXT(ID_TEXTCTRL_PROP, PropagationConfigPanel::OnIntegratorTextUpdate)
    EVT_TEXT(ID_TEXTCTRL_GRAV1, PropagationConfigPanel::OnGravityTextUpdate)
    EVT_TEXT(ID_TEXTCTRL_GRAV2, PropagationConfigPanel::OnGravityTextUpdate)
    EVT_TEXT(ID_TEXTCTRL_MAGN1, PropagationConfigPanel::OnMagneticTextUpdate)
    EVT_TEXT(ID_TEXTCTRL_MAGN2, PropagationConfigPanel::OnMagneticTextUpdate)
    EVT_COMBOBOX(ID_CB_INTGR, PropagationConfigPanel::OnIntegratorSelection)
    EVT_TEXT(ID_CB_BODY, PropagationConfigPanel::OnBodySelection)
    EVT_COMBOBOX(ID_CB_GRAV, PropagationConfigPanel::OnGravitySelection)
    EVT_COMBOBOX(ID_CB_ATMOS, PropagationConfigPanel::OnAtmosphereSelection)
    EVT_CHECKBOX(ID_CHECKBOX, PropagationConfigPanel::OnSRPCheckBoxChange)
END_EVENT_TABLE()

//------------------------------------------------------------------------------
// PropagationConfigPanel()
//------------------------------------------------------------------------------
/**
 * A constructor.
 */
//------------------------------------------------------------------------------
PropagationConfigPanel::PropagationConfigPanel(wxWindow *parent, const wxString &propName)
    : wxPanel(parent)
{
    propSetupName = std::string(propName.c_str());
    Initialize();
    Setup(this);
    LoadData();
    
    applyButton->Enable(false);
}

void PropagationConfigPanel::Initialize()
{  
    //MessageInterface::ShowMessage("Entering PropagationConfigPanel::Initialize()\n");
    // Default integrator values
    newPropName      = "";
    thePropSetup     = NULL;
    thePropagator    = NULL;
    newProp          = NULL;
    isIntegratorChanged = false;
    integratorString = "RKV 8(9)";
    
    // Default force model values
    numOfAtmosTypes     = 3;
    numOfGraFields      = 4;
    numOfMagFields      = 1;  //waw: TBD
    isForceModelChanged = false;
    numOfForces         = 0;
    theForceModel       = NULL;
    
    theGuiInterpreter = GmatAppData::GetGuiInterpreter(); 

    if (theGuiInterpreter != NULL)
    {        
        thePropSetup = theGuiInterpreter->GetPropSetup(propSetupName);

//          //loj: 3/12/04 debug ================================
      
//          MessageInterface::ShowMessage("PropagationConfigPanel::Initialize() propTypeName = %s\n",
//                                        thePropSetup->GetStringParameter("Type").c_str());
//          MessageInterface::ShowMessage("PropagationConfigPanel::Initialize() fmName = %s\n",
//                                        thePropSetup->GetStringParameter("FM").c_str());
                                    
//          numOfForces = thePropSetup->GetNumForces();
//          MessageInterface::ShowMessage("PropagationConfigPanel::Initialize() numOfForces = %d\n",
//                                        numOfForces);
      
//          ForceModel *fm = thePropSetup->GetForceModel();
//          MessageInterface::ShowMessage("ForceModel name = %s\n", fm->GetName().c_str());
     
//          for (int i=0; i<numOfForces; i++)
//          {
//              MessageInterface::ShowMessage("Force Type = %s, Name = %s\n",
//                                            fm->GetForce(i)->GetTypeName().c_str(),
//                                            fm->GetForce(i)->GetName().c_str());
//          }
//          //===================================================

        // initialize integrator type array
        integratorArray.Add("RKV 8(9)");
        integratorArray.Add("RKN 6(8)");
        integratorArray.Add("RKF 5(6)");
    
        if (thePropSetup != NULL)
        {
            thePropagator = thePropSetup->GetPropagator();            
            theForceModel = thePropSetup->GetForceModel();
            numOfForces   = thePropSetup->GetNumForces();

            PhysicalModel *force;
            
            for (Integer i = 0; i < numOfForces; i++)
            {
                force = theForceModel->GetForce(i);
                //MessageInterface::ShowMessage("forcetype=%s\n", force->GetTypeName().c_str());
                //loj: 3/30/04 added check for PointMassForce
                if (force->GetTypeName() == "PointMassForce")
                {
                    thePMForces.push_back((PointMassForce *)force);
                
                    thePlanets.push_back(thePMForces[i]->GetBody());
                    primaryBodiesArray.Add(thePlanets[i]->GetName().c_str());
                    primaryBodiesGravityArray.Add(thePMForces[i]->GetTypeName().c_str());
                
                    degreeID = thePlanets[i]->GetParameterID("Degree");
                    orderID = thePlanets[i]->GetParameterID("Order");
                    degreeArray.Add(wxVariant((long)thePlanets[i]->GetIntegerParameter(degreeID)));
                    orderArray.Add(wxVariant((long)thePlanets[i]->GetIntegerParameter(orderID)));
                }
            }

            primaryBodyString = primaryBodiesArray.Item(0).c_str();
            savedBodiesArray = primaryBodiesArray;
                
            if (primaryBodiesGravityArray[0].CmpNoCase("PointMassForce") == 0)
                gravityFieldString  = wxT("Point Mass");
            else
                gravityFieldString  = wxT("None");
                
            numOfBodies = (Integer)primaryBodiesArray.GetCount();
        }
        else
        {
            MessageInterface::ShowMessage("PropagationConfigPanel():Initialize() thePropSetup is NULL\n");
        }
    }
    
    //MessageInterface::ShowMessage("Exiting PropagationConfigPanel::Initialize()\n");
}

void PropagationConfigPanel::Setup(wxWindow *parent)
{              
    // wxStaticText
    integratorStaticText =
        new wxStaticText( parent, ID_TEXT, wxT("Integrator Type"),
                          wxDefaultPosition, wxSize(250,30),
                          wxST_NO_AUTORESIZE);
    setting1StaticText =
        new wxStaticText( parent, ID_TEXT, wxT("Step Size: "),
                          wxDefaultPosition, wxSize(250,30),
                          wxST_NO_AUTORESIZE );
    setting2StaticText =
        new wxStaticText( parent, ID_TEXT, wxT("Max Int Error: "),
                          wxDefaultPosition, wxSize(250,30),
                          wxST_NO_AUTORESIZE );
    setting3StaticText =
        new wxStaticText( parent, ID_TEXT, wxT("Min Step Size: "),
                          wxDefaultPosition, wxSize(250,30),
                          wxST_NO_AUTORESIZE );
    setting4StaticText =
        new wxStaticText( parent, ID_TEXT, wxT("Max Step Size: "),
                          wxDefaultPosition, wxSize(250,30),
                          wxST_NO_AUTORESIZE );
    setting5StaticText =
        new wxStaticText( parent, ID_TEXT, wxT("Max failed steps: "),
                          wxDefaultPosition, wxSize(250,30),
                          wxST_NO_AUTORESIZE );
                           
    item42 = new wxStaticText( parent, ID_TEXT, wxT("Type"), wxDefaultPosition, wxDefaultSize, 0 );
    item47 = new wxStaticText( parent, ID_TEXT, wxT("Type"), wxDefaultPosition, wxDefaultSize, 0 );
    item38 = new wxStaticText( parent, ID_TEXT, wxT("Degree"), wxDefaultPosition, wxDefaultSize, 0 );
    item40 = new wxStaticText( parent, ID_TEXT, wxT("Order"), wxDefaultPosition, wxDefaultSize, 0 );
    item54 = new wxStaticText( parent, ID_TEXT, wxT("Order"), wxDefaultPosition, wxDefaultSize, 0 );
    item56 = new wxStaticText( parent, ID_TEXT, wxT("Type"), wxDefaultPosition, wxDefaultSize, 0 );
    item52 = new wxStaticText( parent, ID_TEXT, wxT("Degree"), wxDefaultPosition, wxDefaultSize, 0 );

    // wxTextCtrl
    setting1TextCtrl = new wxTextCtrl( parent, ID_TEXTCTRL_PROP, wxT(""), wxDefaultPosition, wxSize(100,-1), 0 );
    setting2TextCtrl = new wxTextCtrl( parent, ID_TEXTCTRL_PROP, wxT(""), wxDefaultPosition, wxSize(100,-1), 0 );
    setting3TextCtrl = new wxTextCtrl( parent, ID_TEXTCTRL_PROP, wxT(""), wxDefaultPosition, wxSize(100,-1), 0 );
    setting4TextCtrl = new wxTextCtrl( parent, ID_TEXTCTRL_PROP, wxT(""), wxDefaultPosition, wxSize(100,-1), 0 );
    setting5TextCtrl = new wxTextCtrl( parent, ID_TEXTCTRL_PROP, wxT(""), wxDefaultPosition, wxSize(100,-1), 0 );
    bodyTextCtrl = new wxTextCtrl( parent, ID_TEXTCTRL, wxT(""), wxDefaultPosition, wxSize(150,-1), wxTE_READONLY );
    gravityDegreeTextCtrl = new wxTextCtrl( parent, ID_TEXTCTRL_GRAV1, wxT(""), wxDefaultPosition, wxSize(50,-1), 0 );
    gravityOrderTextCtrl = new wxTextCtrl( parent, ID_TEXTCTRL_GRAV2, wxT(""), wxDefaultPosition, wxSize(50,-1), 0 );
    pmEditTextCtrl = new wxTextCtrl( parent, ID_TEXTCTRL, wxT(""), wxDefaultPosition, wxSize(550,-1), wxTE_READONLY );
    magneticDegreeTextCtrl = new wxTextCtrl( parent, ID_TEXTCTRL_MAGN1, wxT(""), wxDefaultPosition, wxSize(50,-1), 0 );
    magneticOrderTextCtrl = new wxTextCtrl( parent, ID_TEXTCTRL_MAGN2, wxT(""), wxDefaultPosition, wxSize(50,-1), 0 );

    // wxButton
    scriptButton = new wxButton( parent, ID_BUTTON_SCRIPT, wxT("Create Script"), wxDefaultPosition, wxDefaultSize, 0 );
    bodyButton = new wxButton( parent, ID_BUTTON_ADD_BODY, wxT("Add Body"), wxDefaultPosition, wxDefaultSize, 0 );
    searchGravityButton = new wxButton( parent, ID_BUTTON_GRAV_SEARCH, wxT("Search"), wxDefaultPosition, wxDefaultSize, 0 );
    setupButton = new wxButton( parent, ID_BUTTON_SETUP, wxT("Setup"), wxDefaultPosition, wxDefaultSize, 0 );
    editMassButton = new wxButton( parent, ID_BUTTON_PM_EDIT, wxT("Edit"), wxDefaultPosition, wxDefaultSize, 0 );
    okButton = new wxButton( parent, ID_BUTTON_OK, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
    applyButton = new wxButton( parent, ID_BUTTON_APPLY, wxT("Apply"), wxDefaultPosition, wxDefaultSize, 0 );
    cancelButton = new wxButton( parent, ID_BUTTON_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    helpButton = new wxButton( parent, ID_BUTTON_HELP, wxT("Help"), wxDefaultPosition, wxDefaultSize, 0 );
    searchMagneticButton = new wxButton( parent, ID_BUTTON_MAG_SEARCH, wxT("Search"), wxDefaultPosition, wxDefaultSize, 0 );
    editPressureButton = new wxButton( parent, ID_BUTTON_SRP_EDIT, wxT("Edit"), wxDefaultPosition, wxDefaultSize, 0 );
   
    // wxString
    wxString strArray1[] = 
    {
        integratorArray[0],
        integratorArray[1],
        integratorArray[2]
    };
    
    wxString strArray2[numOfBodies];
    if ( !primaryBodiesArray.IsEmpty() )
        for (Integer i = 0; i < (Integer)primaryBodiesArray.GetCount(); i++)
            strArray2[i] = primaryBodiesArray[i].c_str();
    
    wxString strArray3[] = 
    {
        wxT("None"),
        wxT("Point Mass"),
        wxT("JGM-2"),
        wxT("JGM-3")
    };
    wxString strArray4[] = 
    {
        wxT("None"),
        wxT("Exponential"),
        wxT("MISISE-90")
    };
    wxString strArray5[] = 
    {
        wxT("None")
    };
    
    // wxComboBox
    integratorComboBox =
        new wxComboBox( parent, ID_CB_INTGR, wxT(integratorString),
                        wxDefaultPosition, wxSize(100,-1), IntegratorCount,
                        //wxDefaultPosition, wxSize(-1,-1), IntegratorCount,
                        strArray1, wxCB_DROPDOWN|wxCB_READONLY );
    bodyComboBox =
        new wxComboBox( parent, ID_CB_BODY, wxT(primaryBodyString),
                        wxDefaultPosition,  wxSize(100,-1), numOfBodies,
                        strArray2, wxCB_DROPDOWN|wxCB_READONLY );
    gravityTypeComboBox =
        new wxComboBox( parent, ID_CB_GRAV, wxT(gravityFieldString),
                        wxDefaultPosition, wxSize(300,-1), numOfGraFields,
                        strArray3, wxCB_DROPDOWN|wxCB_READONLY );
    atmosComboBox =
        new wxComboBox( parent, ID_CB_ATMOS, wxT(strArray4[0]),
                        wxDefaultPosition, wxSize(150,-1), numOfAtmosTypes,
                        strArray4, wxCB_DROPDOWN|wxCB_READONLY );
    magneticTypeComboBox =
        new wxComboBox( parent, ID_CB_MAG, wxT(strArray5[0]),
                        wxDefaultPosition, wxSize(300,-1), numOfMagFields,
                        strArray5, wxCB_DROPDOWN|wxCB_READONLY );
      
    // wxCheckBox
    srpCheckBox = new wxCheckBox( parent, ID_CHECKBOX, wxT("Use"),
                                  wxDefaultPosition, wxDefaultSize, 0 );
   
    // wx*Sizers      
    wxBoxSizer *boxSizer1 = new wxBoxSizer( wxVERTICAL );
    wxBoxSizer *boxSizer2 = new wxBoxSizer( wxHORIZONTAL );
    wxBoxSizer *boxSizer3 = new wxBoxSizer( wxHORIZONTAL );
    wxBoxSizer *boxSizer4 = new wxBoxSizer( wxHORIZONTAL );
    
    wxFlexGridSizer *flexGridSizer1 = new wxFlexGridSizer( 5, 0, 0 );
    wxFlexGridSizer *flexGridSizer2 = new wxFlexGridSizer( 2, 0, 0 );
    wxFlexGridSizer *flexGridSizer3 = new wxFlexGridSizer( 2, 0, 2 );
        
    wxStaticBox *staticBox1 = new wxStaticBox( parent, -1, wxT("Numerical Integrator") );
    wxStaticBoxSizer *staticBoxSizer1 = new wxStaticBoxSizer( staticBox1, wxVERTICAL );
    wxStaticBox *staticBox2 = new wxStaticBox( parent, -1, wxT("Environment Model") );
    wxStaticBoxSizer *staticBoxSizer2 = new wxStaticBoxSizer( staticBox2, wxVERTICAL );
    wxStaticBox *staticBox3 = new wxStaticBox( parent, -1, wxT("Primary Bodies") );
    wxStaticBoxSizer *staticBoxSizer3 = new wxStaticBoxSizer( staticBox3, wxVERTICAL );
    wxStaticBox *item37 = new wxStaticBox( parent, -1, wxT("Gravity Field") );
    wxStaticBoxSizer *item36 = new wxStaticBoxSizer( item37, wxHORIZONTAL );
    wxStaticBox *item46 = new wxStaticBox( parent, -1, wxT("Atmospheric Model") );
    wxStaticBoxSizer *item45 = new wxStaticBoxSizer( item46, wxHORIZONTAL );
    wxStaticBox *item51 = new wxStaticBox( parent, -1, wxT("Magnetic Field") );
    wxStaticBoxSizer *item50 = new wxStaticBoxSizer( item51, wxHORIZONTAL );
    wxStaticBox *staticBox7 = new wxStaticBox( parent, -1, wxT("Point Masses") );
    wxStaticBoxSizer *staticBoxSizer7 = new wxStaticBoxSizer( staticBox7, wxVERTICAL );
    wxStaticBox *item65 = new wxStaticBox( parent, -1, wxT("Solar Radiation Pressure") );
    wxStaticBoxSizer *item64 = new wxStaticBoxSizer( item65, wxHORIZONTAL );
    
    // Add to wx*Sizers    
    flexGridSizer1->Add( 200, 20, 0, wxALIGN_CENTRE|wxALL, 5 );
    flexGridSizer1->Add( 200, 20, 0, wxALIGN_CENTRE|wxALL, 5 );
    flexGridSizer1->Add( 200, 20, 0, wxALIGN_CENTRE|wxALL, 5 );
    flexGridSizer1->Add( 200, 20, 0, wxALIGN_CENTRE|wxALL, 5 );
    flexGridSizer1->Add( scriptButton, 0, wxALIGN_RIGHT|wxALL, 5 );    
    
    flexGridSizer2->Add( integratorStaticText, 0, wxGROW|wxALIGN_LEFT|wxALL, 5 );
    flexGridSizer2->Add( integratorComboBox, 0, wxGROW|wxALIGN_LEFT|wxALL, 5 );
    flexGridSizer2->Add( setting1StaticText, 0, wxGROW|wxALIGN_LEFT|wxALL, 5 );
    flexGridSizer2->Add( setting1TextCtrl, 0, wxGROW|wxALIGN_LEFT|wxALL, 5 );
    flexGridSizer2->Add( setting2StaticText, 0, wxGROW|wxALIGN_LEFT|wxALL, 5 );
    flexGridSizer2->Add( setting2TextCtrl, 0, wxGROW|wxALIGN_LEFT|wxALL, 5 );
    flexGridSizer2->Add( setting3StaticText, 0, wxGROW|wxALIGN_LEFT|wxALL, 5 );
    flexGridSizer2->Add( setting3TextCtrl, 0, wxGROW|wxALIGN_LEFT|wxALL, 5 );
    flexGridSizer2->Add( setting4StaticText, 0, wxGROW|wxALIGN_LEFT|wxALL, 5 );
    flexGridSizer2->Add( setting4TextCtrl, 0, wxGROW|wxALIGN_LEFT|wxALL, 5 );
    flexGridSizer2->Add( setting5StaticText, 0, wxGROW|wxALIGN_LEFT|wxALL, 5 );
    flexGridSizer2->Add( setting5TextCtrl, 0, wxGROW|wxALIGN_LEFT|wxALL, 5 );

    boxSizer3->Add( bodyComboBox, 0, wxGROW|wxALIGN_CENTRE|wxALL, 5 );
    boxSizer3->Add( bodyTextCtrl, 0, wxGROW|wxALIGN_CENTRE|wxALL, 5 );
    boxSizer3->Add( bodyButton, 0, wxGROW|wxALIGN_CENTRE|wxALL, 5 );
    
    item36->Add( item42, 0, wxALIGN_CENTRE|wxALL, 5 );
    item36->Add( gravityTypeComboBox, 0, wxALIGN_CENTRE|wxALL, 5 ); 
    item36->Add( item38, 0, wxALIGN_CENTRE|wxALL, 5 );
    item36->Add( gravityDegreeTextCtrl, 0, wxALIGN_CENTRE|wxALL, 5 );
    item36->Add( item40, 0, wxALIGN_CENTRE|wxALL, 5 );
    item36->Add( gravityOrderTextCtrl, 0, wxALIGN_CENTRE|wxALL, 5 );
    item36->Add( searchGravityButton, 0, wxALIGN_CENTRE|wxALL, 5 );

    item45->Add( item47, 0, wxALIGN_CENTRE|wxALL, 5 );   
    item45->Add( atmosComboBox, 0, wxALIGN_CENTRE|wxALL, 5 );
    item45->Add( setupButton, 0, wxALIGN_CENTRE|wxALL, 5 );   
    
    item50->Add( magneticTypeComboBox, 0, wxALIGN_CENTRE|wxALL, 5 );
    item50->Add( item52, 0, wxALIGN_CENTRE|wxALL, 5 );
    item50->Add( magneticDegreeTextCtrl, 0, wxALIGN_CENTRE|wxALL, 5 );
    item50->Add( item54, 0, wxALIGN_CENTRE|wxALL, 5 );
    item50->Add( magneticOrderTextCtrl, 0, wxALIGN_CENTRE|wxALL, 5 );
    item50->Add( item56, 0, wxALIGN_CENTRE|wxALL, 5 ); 
    item50->Add( searchMagneticButton, 0, wxALIGN_CENTRE|wxALL, 5 );
    
    flexGridSizer3->Add( pmEditTextCtrl, 0, wxALIGN_CENTRE|wxALL, 5 );
    flexGridSizer3->Add( editMassButton, 0, wxALIGN_CENTRE|wxALL, 5 );
    
    item64->Add( 20, 20, 0, wxALIGN_CENTRE|wxALL, 5 );
    item64->Add( srpCheckBox, 0, wxALIGN_CENTRE|wxALL, 5 );
    item64->Add( 20, 20, 0, wxALIGN_CENTRE|wxALL, 5 );
    item64->Add( editPressureButton, 0, wxALIGN_CENTRE|wxALL, 5 );
    
    staticBoxSizer3->Add( boxSizer3, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
    staticBoxSizer3->Add( item36, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
    staticBoxSizer3->Add( item45, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
    staticBoxSizer3->Add( item50, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
    
    staticBoxSizer7->Add( flexGridSizer3, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
    
    staticBoxSizer1->Add( flexGridSizer2, 0, wxALIGN_CENTRE|wxALL, 5 );
    
    staticBoxSizer2->Add( staticBoxSizer3, 0, wxALIGN_CENTRE|wxALL, 5 );
    staticBoxSizer2->Add( staticBoxSizer7, 0, wxALIGN_CENTRE|wxALL, 5 );
    staticBoxSizer2->Add( item64, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
    
    boxSizer2->Add( staticBoxSizer1, 0, wxGROW|wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
    boxSizer2->Add( staticBoxSizer2, 0, wxGROW|wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
           
    boxSizer4->Add( okButton, 0, wxALIGN_CENTRE|wxALL, 5 );
    boxSizer4->Add( applyButton, 0, wxALIGN_CENTRE|wxALL, 5 );
    boxSizer4->Add( cancelButton, 0, wxALIGN_CENTRE|wxALL, 5 );
    boxSizer4->Add( helpButton, 0, wxALIGN_CENTRE|wxALL, 5 );
    
    boxSizer1->Add( flexGridSizer1, 0, wxALIGN_CENTRE|wxALL, 5 );
    boxSizer1->Add( boxSizer2, 0, wxALIGN_CENTRE|wxALL, 5 );
    boxSizer1->Add( boxSizer4, 0, wxALIGN_CENTRE|wxALL, 5 );
    
    // waw: future implementation
    helpButton->Enable(false);
    
    parent->SetAutoLayout(true);
    parent->SetSizer( boxSizer1 );
    boxSizer1->Fit( parent );
    boxSizer1->SetSizeHints( parent );
}

void PropagationConfigPanel::LoadData()
{
    //MessageInterface::ShowMessage("Entering PropagationConfigPanel::LoadData()\n");
  
    std::string propType = thePropagator->GetTypeName();
        
    int typeId;
    
    if (propType == "RungeKutta89")
        typeId = RKV89_ID;    
    else if (propType == "DormandElMikkawyPrince68")
        typeId = RKN68_ID;    
    else if (propType == "RungeKuttaFehlberg56")
        typeId = RKF56_ID;    

    integratorComboBox->SetSelection(typeId);
    integratorString = integratorArray[typeId];
    //MessageInterface::ShowMessage("Calling DisplayIntegratorData() from PropagationConfigPanel::LoadData()\n");    
    DisplayIntegratorData(false);
    DisplayForceData();
}

void PropagationConfigPanel::SaveData()
{   
    //---------------------------------
    // Saving the integrator/propagator
    //---------------------------------
    if (isIntegratorChanged)
    {
        newProp->SetRealParameter("StepSize", atof(setting1TextCtrl->GetValue()) );
        newProp->SetRealParameter("ErrorThreshold", atof(setting2TextCtrl->GetValue()) );
        newProp->SetRealParameter("MinStep", atof(setting3TextCtrl->GetValue()) );
        newProp->SetRealParameter("MaxStep", atof(setting4TextCtrl->GetValue()) );
        newProp->SetIntegerParameter("MaxStepAttempts", atoi(setting5TextCtrl->GetValue()) );
        
        thePropSetup->SetPropagator(newProp);
        //MessageInterface::ShowMessage("PropagationConfigPanel():SetData() newPropType = %s name = %s\n",
        //    newProp->GetTypeName().c_str(), newProp->GetName().c_str());
    }
    
    //------------------------
    // Saving the force model
    //------------------------
    // the primary body data  
    for (Integer i = 0; i < primaryBodiesArray.GetCount(); i++)
    {       
        if (!primaryBodiesArray.IsEmpty())
        {
            for (Integer j = 0; j < savedBodiesArray.GetCount(); j++)
                if (primaryBodiesArray[i].CmpNoCase(savedBodiesArray[j].c_str()) == 0)
                    primaryBodiesArray[i] = wxT("NULL");
        }
    }
    for (Integer i = 0; i < primaryBodiesArray.GetCount(); i++)
    { 
        if ( (primaryBodiesArray[i].CmpNoCase("NULL") != 0)&&(primaryBodiesGravityArray[i].CmpNoCase("PointMassForce") == 0) )
        {
            thePMForces[i] = (PointMassForce *)theGuiInterpreter->CreatePhysicalModel("PointMassForce", "");
            thePMForces[i]->SetBody(thePlanets[i]);
            theForceModel->AddForce(thePMForces[i]);       
        }
    }
    // the point mass body data
    Planet *body;
    PointMassForce *pm;
    if (!pointmassBodiesArray.IsEmpty())
    {
        for (Integer i = 0; i < pointmassBodiesArray.GetCount(); i++)
        {
            body = new Planet(pointmassBodiesArray[i].c_str());
            pm = (PointMassForce *)theGuiInterpreter->CreatePhysicalModel("PointMassForce", "");
            pm->SetBody(body);
            theForceModel->AddForce(pm);
        }    
    }
    
    if (theForceModel != NULL)
        thePropSetup->SetForceModel(theForceModel);
         
//    if (atmosComboBox->GetStringSelection().Cmp("None") != 0)
//    {
//         ;  
//          = atmosComboBox->GetStringSelection();
//    
//    }
//    if (magneticTypeComboBox->GetStringSelection().Cmp("None") != 0)
//    {
//        ;  
//         = magneticTypeComboBox->GetStringSelection();
//         = magneticDegreeTextCtrl->GetValue();
//         = magneticOrderTextCtrl->GetValue();
//    }
//    
//     = useSRP;
}

void PropagationConfigPanel::DisplayIntegratorData(bool integratorChanged)
{               
    //MessageInterface::ShowMessage("Entering PropagationConfigPanel::DisplayIntegratorData()\n");
        
    if (integratorChanged)
    {
        if (integratorString.IsSameAs(integratorArray[RKV89_ID]))
        {
            newPropName = propSetupName + "RKV89";
        
            newProp = theGuiInterpreter->GetPropagator(newPropName);
            if (newProp == NULL)
            {
                //MessageInterface::ShowMessage("PropConfigPanel::DisplayIntegratorData() "
                //                              "Creating RungeKutta89\n");
                newProp = theGuiInterpreter->CreatePropagator("RungeKutta89", newPropName);
            }
        }
        else if (integratorString.IsSameAs(integratorArray[RKN68_ID]))
        {
            newPropName = propSetupName + "RKN68";
            newProp = theGuiInterpreter->GetPropagator(newPropName);
        
            if (newProp == NULL)
            {
                //MessageInterface::ShowMessage("PropConfigPanel::DisplayIntegratorData() "
                //                              "Creating DormandElMikkawyPrince68\n");
                newProp = theGuiInterpreter->CreatePropagator("DormandElMikkawyPrince68", newPropName);
            }
        }
        else if (integratorString.IsSameAs(integratorArray[RKF56_ID]))
        {   
            newPropName = propSetupName + "RKF56";
            newProp = theGuiInterpreter->GetPropagator(newPropName);
        
            if (newProp == NULL)
            {
                //MessageInterface::ShowMessage("PropConfigPanel::DisplayIntegratorData() "
                //                              "Creating RungeKuttaFehlberg56\n");
                newProp = theGuiInterpreter->CreatePropagator("RungeKuttaFehlberg56", newPropName);
            }            
        }
    }
    else
    {
        newProp = thePropSetup->GetPropagator();
    }
    
    // fill in data   
    setting1TextCtrl->SetValue(wxVariant(newProp->GetRealParameter("StepSize")));
    setting2TextCtrl->SetValue(wxVariant(newProp->GetRealParameter("ErrorThreshold")));
    setting3TextCtrl->SetValue(wxVariant(newProp->GetRealParameter("MinStep")));
    setting4TextCtrl->SetValue(wxVariant(newProp->GetRealParameter("MaxStep")));
    setting5TextCtrl->SetValue(wxVariant((long)newProp->GetIntegerParameter("MaxStepAttempts")));
}

void PropagationConfigPanel::DisplayForceData()
{    
    DisplayPrimaryBodyData();
    DisplayGravityFieldData();
    DisplayAtmosphereModelData();
    DisplayMagneticFieldData();
    DisplaySRPData();
}

void PropagationConfigPanel::DisplayPrimaryBodyData()
{
    Integer bodyIndex;
    for (Integer i = 0; i < primaryBodiesArray.GetCount(); i++)
    {
        bodyTextCtrl->AppendText(primaryBodiesArray.Item(i) + " ");
        if ( primaryBodiesArray[i].CmpNoCase(primaryBodyString.c_str()) == 0 )
            bodyIndex = i;
    }
    bodyComboBox->SetSelection(bodyIndex);
}

void PropagationConfigPanel::DisplayGravityFieldData()
{            
    primaryBodyString = bodyComboBox->GetStringSelection();
    Integer index = bodyComboBox->GetSelection();

    for (Integer i = 0; i < primaryBodiesArray.GetCount(); i++)
    {
        if ( primaryBodiesArray[i].CmpNoCase(primaryBodyString) == 0 )
        {
            if ( primaryBodiesGravityArray[i].CmpNoCase("PointMassForce") == 0 )
            {
                gravityTypeComboBox->SetValue("Point Mass");
                gravityDegreeTextCtrl->SetValue("0");
                gravityOrderTextCtrl->SetValue("0");
                gravityDegreeTextCtrl->Enable(false);
                gravityOrderTextCtrl->Enable(false);
            }
            else
            {
                gravityTypeComboBox->SetValue("None");
                gravityDegreeTextCtrl->Enable(true);
                gravityOrderTextCtrl->Enable(true);
                gravityDegreeTextCtrl->SetValue(degreeArray.Item(i));
                gravityOrderTextCtrl->SetValue(orderArray.Item(i));
            }
        }
    }       
}

void PropagationConfigPanel::DisplayAtmosphereModelData()
{
    // To display Earth specific combo choice
    int x = atmosComboBox->FindString("None");
    int y = atmosComboBox->FindString("Exponential");
    int z = atmosComboBox->FindString("MISISE-90");
    
    // waw:  Venus & Mars for future implementation
    
    if ( primaryBodyString.Cmp(SolarSystem::EARTH_NAME.c_str()) == 0 )
    {                   
        if (x == -1)
            atmosComboBox->Append("None");
            
        if (y == -1)
            atmosComboBox->Append("Exponential");

        if (z == -1)
            atmosComboBox->Append("MISISE-90");    
        
        atmosComboBox->SetSelection(0); // TBD
        setupButton->Enable(true); 
    }
    else
    {
        atmosComboBox->Clear();
        atmosComboBox->Append("None");
        atmosComboBox->SetSelection(0);
        
        setupButton->Enable(false);
    }
}

void PropagationConfigPanel::DisplayMagneticFieldData()
{
    // waw: Future build implementation
}

void PropagationConfigPanel::DisplaySRPData()
{
    useSRP = false;  // waw: TBD
    srpCheckBox->SetValue(useSRP);
}

void PropagationConfigPanel::OnIntegratorSelection()
{
    if (!integratorString.IsSameAs(integratorComboBox->GetStringSelection()))
    {
        isIntegratorChanged = true;
        integratorString = integratorComboBox->GetStringSelection();
        DisplayIntegratorData(true);
        applyButton->Enable(true);
    }
}

void PropagationConfigPanel::OnBodySelection()
{
    if (!primaryBodyString.IsSameAs(bodyComboBox->GetStringSelection()))
    {
        primaryBodyString = bodyComboBox->GetStringSelection();
        DisplayGravityFieldData();
        DisplayAtmosphereModelData();
        DisplayMagneticFieldData();
        applyButton->Enable(true);
    }
}

void PropagationConfigPanel::OnGravitySelection()
{
    wxString gravityTypeString = gravityTypeComboBox->GetStringSelection();
    primaryBodyString = bodyComboBox->GetStringSelection();

    for (Integer i = 0; i < primaryBodiesArray.GetCount(); i++)
    {
        if ( primaryBodiesArray[i].CmpNoCase(primaryBodyString) == 0 )
        {     
            if (gravityTypeString.CmpNoCase("Point Mass") == 0)
            {
                primaryBodiesGravityArray[i] = "PointMassForce";
                editMassButton->Enable(true);
                gravityDegreeTextCtrl->Enable(false);
                gravityOrderTextCtrl->Enable(false);
            }
            else //waw:future implementation
            {
                primaryBodiesGravityArray[i] = "NULL";
                editMassButton->Enable(false);
                gravityDegreeTextCtrl->Enable(true);
                gravityOrderTextCtrl->Enable(true);
            }
            isForceModelChanged = true;
            applyButton->Enable(true);
        }
    }
}

void PropagationConfigPanel::OnAtmosphereSelection()
{
    applyButton->Enable(true);
    isForceModelChanged = true;
}


//------------------------------------------------------------------------------
// wxMenuBar* CreateScriptWindowMenu(const std::string &docType)
//------------------------------------------------------------------------------
//wxMenuBar* PropagationConfigPanel::CreateScriptWindowMenu(const std::string &docType)
//{
//    // Make a menubar
//    wxMenu *fileMenu = new wxMenu;
//    wxMenu *editMenu = (wxMenu *) NULL;
//  
//    fileMenu->Append(wxID_NEW, _T("&New..."));
//    fileMenu->Append(wxID_OPEN, _T("&Open..."));
//
//    if (docType == "sdi")
//    {
//        fileMenu->Append(wxID_CLOSE, _T("&Close"));
//        fileMenu->Append(wxID_SAVE, _T("&Save"));
//        fileMenu->Append(wxID_SAVEAS, _T("Save &As..."));
//        fileMenu->AppendSeparator();
//        fileMenu->Append(wxID_PRINT, _T("&Print..."));
//        fileMenu->Append(wxID_PRINT_SETUP, _T("Print &Setup..."));
//        fileMenu->Append(wxID_PREVIEW, _T("Print Pre&view"));
//   
//        editMenu = new wxMenu;
//        editMenu->Append(wxID_UNDO, _T("&Undo"));
//        editMenu->Append(wxID_REDO, _T("&Redo"));
//        editMenu->AppendSeparator();
//        //editMenu->Append(DOCVIEW_CUT, _T("&Cut last segment"));
//  
//        docMainFrame->editMenu = editMenu;
//        fileMenu->AppendSeparator();
//    }
//    
//    fileMenu->Append(wxID_EXIT, _T("E&xit"));
//    
//    // A nice touch: a history of files visited. Use this menu.
//    mDocManager->FileHistoryUseMenu(fileMenu);
//    
//    //wxMenu *help_menu = new wxMenu;
//    //help_menu->Append(DOCVIEW_ABOUT, _T("&About"));
//    
//    wxMenuBar *menuBar = new wxMenuBar;
//    
//    menuBar->Append(fileMenu, _T("&File"));
//    
//    if (editMenu)
//        menuBar->Append(editMenu, _T("&Edit"));
//    
//    //menuBar->Append(help_menu, _T("&Help"));
//
//    return menuBar;
//}
//
//void PropagationConfigPanel::CreateScript()
//{
//    //not MAC mode
//    //----------------------------------------------------------------
//    // NOTE:  it is temporary until it is fixed for Mac
//    // #if !defined __WXMAC__
//    //----------------------------------------------------------------
//  #if 1 
//    // Create a document manager
//    mDocManager = new wxDocManager;
//
//    // Create a template relating text documents to their views
//    mDocTemplate = 
//        new wxDocTemplate(mDocManager, _T("Text"), _T("*.script"),
//                          _T(""), _T("script"), _T("Text Doc"), _T("Text View"),
//                          CLASSINFO(TextDocument), CLASSINFO(MdiTextEditView));
//  
//    // Create the main frame window    
//    mdiDocMainFrame =
//        new MdiDocViewFrame(mDocManager, mdiDocMainFrame, _T("Script Window (MDI)"),
//                            wxPoint(0, 0), wxSize(600, 500),
//                            (wxDEFAULT_FRAME_STYLE | wxNO_FULL_REPAINT_ON_RESIZE));
//    
//    // Give it an icon (this is ignored in MDI mode: uses resources)
//    mdiDocMainFrame->SetIcon(wxIcon(_T("doc")));
//  
//    // Make a menubar
//    wxMenuBar *menuBar = CreateScriptWindowMenu("mdi");
//       
//    // Associate the menu bar with the frame
//    mdiDocMainFrame->SetMenuBar(menuBar);
//    
//    mdiDocMainFrame->Centre(wxBOTH);
//    mdiDocMainFrame->Show(TRUE);
//    //----------------------------------------------------------------
//  #else
//    // Create a document manager
//    mDocManager = new wxDocManager;
//
//    // Create a template relating text documents to their views
//    mDocTemplate = 
//        new wxDocTemplate(mDocManager, _T("Text"), _T("*.script"),
//                          _T(""), _T("script"), _T("Text Doc"), _T("Text View"),
//                          CLASSINFO(TextDocument), CLASSINFO(TextEditView));
//    
//    // Create the main frame window    
//    docMainFrame =
//        new DocViewFrame(mDocManager, this, -1, _T("Script Window"),
//                         wxPoint(0, 0), wxSize(600, 500), wxDEFAULT_FRAME_STYLE);
//      
//    // Make a menubar
//    wxMenuBar *menuBar = CreateScriptWindowMenu("sdi");
//      
//    // Associate the menu bar with the frame
//    docMainFrame->SetMenuBar(menuBar);
//    
//    docMainFrame->Centre(wxBOTH);
//    docMainFrame->Show(TRUE);
//  #endif
//}

// wxButton Events
void PropagationConfigPanel::OnScriptButton()
{
//    CreateScript();
//    applyButton->Enable(true);
}

void PropagationConfigPanel::OnOKButton()
{
    if (applyButton->IsEnabled())
    {
        SaveData();     
    }
    GmatMainNotebook *gmatMainNotebook = GmatAppData::GetMainNotebook();
    gmatMainNotebook->ClosePage();
}

void PropagationConfigPanel::OnApplyButton()
{
    SaveData();
    applyButton->Enable(false);
}

void PropagationConfigPanel::OnCancelButton()
{
    GmatMainNotebook *gmatMainNotebook = GmatAppData::GetMainNotebook();
    gmatMainNotebook->ClosePage();
}

void OnHelpButton()
{
    // waw: Future build implementation
}

void PropagationConfigPanel::OnAddButton()
{   
    CelesBodySelectDialog bodyDlg(this, primaryBodiesArray);
    bodyDlg.ShowModal();
        
    if (bodyDlg.IsBodySelected())
    {        
        Planet *body;
        wxArrayString &names = bodyDlg.GetBodyNames();
        for(Integer i=0; i < names.GetCount(); i++)
        {
            primaryBodiesArray.Add(names[i]);
            body = new Planet(names[i].c_str());
            thePlanets.push_back(body);
            primaryBodiesGravityArray.Add("PointMassForce");
            bodyTextCtrl->AppendText(names[i] + " ");
            bodyComboBox->Append(names[i]);
        }       
        applyButton->Enable(true);
    }
    
    bool pmChanged = false;
    for (Integer i = 0; i < primaryBodiesArray.GetCount(); i++)
    {
        for (Integer j = 0; j < pointmassBodiesArray.GetCount(); j++)
        {
            if (primaryBodiesArray[i].CmpNoCase(pointmassBodiesArray[j]) == 0)
            {
                wxString body = pointmassBodiesArray.Item(j);
                pointmassBodiesArray.Remove(body);
                pmChanged = true;
            }
        
        }
    }
    
    if (pmChanged)
    {
        pmEditTextCtrl->Clear();
        
        if (!pointmassBodiesArray.IsEmpty())
        {
            for (Integer i = 0; i < pointmassBodiesArray.GetCount(); i++)
            {
                pmEditTextCtrl->AppendText(pointmassBodiesArray.Item(i));
                pmEditTextCtrl->AppendText(" ");
            }
        }
            
    }
}

void PropagationConfigPanel::OnGravSearchButton()
{
    wxFileDialog dialog(this, _T("Choose a file"), _T(""), _T(""), _T("*.*"));
    
    if (dialog.ShowModal() == wxID_OK)
    {
        wxString filename;
        
        filename = dialog.GetPath().c_str();
        
        gravityTypeComboBox->Append(filename); 
    }
        
    applyButton->Enable(true);
}

void PropagationConfigPanel::OnSetupButton()
{
    applyButton->Enable(true);
}

void PropagationConfigPanel::OnMagSearchButton()
{
    wxFileDialog dialog(this, _T("Choose a file"), _T(""), _T(""), _T("*.*"));
    
    if (dialog.ShowModal() == wxID_OK)
    {
        wxString filename;
        
        filename = dialog.GetPath().c_str();
        
        magneticTypeComboBox->Append(filename); 
    }
    
    applyButton->Enable(true);
}

void PropagationConfigPanel::OnPMEditButton()
{
    CelesBodySelectDialog bodyDlg(this, primaryBodiesArray);
    bodyDlg.ShowModal();
    
    if (bodyDlg.IsBodySelected())
    {        
        Planet *body;
        wxArrayString &names = bodyDlg.GetBodyNames();
        
        for (Integer i=0; i < names.GetCount(); i++)
        {
            pmEditTextCtrl->AppendText(names[i] + " ");
            pointmassBodiesArray.Add(names[i]);
        }
        applyButton->Enable(true);
    }
}   

void PropagationConfigPanel::OnSRPEditButton()
{
    applyButton->Enable(true);
}

// wxTextCtrl Events
void PropagationConfigPanel::OnIntegratorTextUpdate()
{
    isIntegratorChanged = true;
    
    wxString set1 = setting1TextCtrl->GetValue();
    wxString set2 = setting2TextCtrl->GetValue();
    wxString set3 = setting3TextCtrl->GetValue();
    wxString set4 = setting4TextCtrl->GetValue();
    wxString set5 = setting5TextCtrl->GetValue();

    applyButton->Enable(true);
}

void PropagationConfigPanel::OnGravityTextUpdate()
{
    wxString deg = gravityDegreeTextCtrl->GetValue();
    wxString ord = gravityOrderTextCtrl->GetValue();
    
    if (!deg.IsNumber())
    {
        gravityDegreeTextCtrl->Clear();
        return;
    }
    else if (!ord.IsNumber())
    {
        gravityOrderTextCtrl->Clear();
        return;
    }
    else
    {
        for (Integer i = 0; i < primaryBodiesArray.GetCount(); i++)
        {
            if ( primaryBodiesArray[i].CmpNoCase(primaryBodyString) == 0 )
            {
                if ( primaryBodiesGravityArray[i].CmpNoCase("PointMassForce") == 0 )
                {
                    degreeArray[i] = "NULL";
                    orderArray[i] = "NULL";
                }
                else
                {
                    degreeArray[i] = deg;
                    orderArray[i] = ord;
                }
            }
        }      
    }
}

void PropagationConfigPanel::OnMagneticTextUpdate()
{
    applyButton->Enable(true);
}

// wxCheckBox Events
void PropagationConfigPanel::OnSRPCheckBoxChange()
{
    useSRP = srpCheckBox->GetValue();
    applyButton->Enable(true);
}
