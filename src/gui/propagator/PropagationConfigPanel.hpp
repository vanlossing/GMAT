//$Header$
//------------------------------------------------------------------------------
//                           PropagationConfigPanel
//------------------------------------------------------------------------------
// GMAT: Goddard Mission Analysis Tool
//
// Author: Waka Waktola
// Created: 2003/08/29
// Copyright: (c) 2003 NASA/GSFC. All rights reserved.
//
/**
 * This class contains the Propagation Configuration window.
 */
//------------------------------------------------------------------------------

#ifndef PropagationConfigPanel_hpp
#define PropagationConfigPanel_hpp

// gui includes
#include "gmatwxdefs.hpp"
//#include "ViewTextFrame.hpp"
//#include "DocViewFrame.hpp"
//#include "TextEditView.hpp"
//#include "TextDocument.hpp"
//#include "MdiTextEditView.hpp"
//#include "MdiDocViewFrame.hpp"
#include "CelesBodySelectDialog.hpp"

// base include
#include "gmatdefs.hpp"
#include "GuiInterpreter.hpp"
#include "Propagator.hpp"
#include "Integrator.hpp"
#include "RungeKutta89.hpp"
#include "PropSetup.hpp"
#include "PhysicalModel.hpp"
#include "ForceModel.hpp"
#include "PointMassForce.hpp"
#include "SolarSystem.hpp"
#include "CelestialBody.hpp"
#include "Planet.hpp"

class PropagationConfigPanel : public wxPanel
{
public:
    // constructor
    PropagationConfigPanel(wxWindow *parent, const wxString &propName);
    
private:             
    wxStaticText *integratorStaticText;
    wxStaticText *setting1StaticText;
    wxStaticText *setting2StaticText;
    wxStaticText *setting3StaticText;
    wxStaticText *setting4StaticText;
    wxStaticText *setting5StaticText;
    wxStaticText *setting6StaticText;
    wxStaticText *setting7StaticText;
    wxStaticText *setting8StaticText;
    wxStaticText *setting9StaticText;
    wxStaticText *item38;
    wxStaticText *item40;
    wxStaticText *item42;
    wxStaticText *item47;
    wxStaticText *item52;
    wxStaticText *item54;
    wxStaticText *item56;
                
    wxTextCtrl *setting1TextCtrl;
    wxTextCtrl *setting2TextCtrl;
    wxTextCtrl *setting3TextCtrl;
    wxTextCtrl *setting4TextCtrl;
    wxTextCtrl *setting5TextCtrl;
    wxTextCtrl *setting6TextCtrl;
    wxTextCtrl *setting7TextCtrl;
    wxTextCtrl *setting8TextCtrl;
    wxTextCtrl *setting9TextCtrl;
    wxTextCtrl *bodyTextCtrl;
    wxTextCtrl *gravityDegreeTextCtrl;
    wxTextCtrl *gravityOrderTextCtrl;
    wxTextCtrl *magneticDegreeTextCtrl;
    wxTextCtrl *magneticOrderTextCtrl;
    wxTextCtrl *pmEditTextCtrl;

    wxComboBox *integratorComboBox;
    wxComboBox *bodyComboBox;
    wxComboBox *atmosComboBox;
    wxComboBox *gravityTypeComboBox;
    wxComboBox *magneticTypeComboBox;

    wxCheckBox *srpCheckBox;

    wxButton *bodyButton;
    wxButton *searchGravityButton;
    wxButton *setupButton;
    wxButton *searchMagneticButton;
    wxButton *editMassButton;
    wxButton *editPressureButton;
    wxButton *okButton;
    wxButton *applyButton;
    wxButton *cancelButton;
    wxButton *helpButton;
    wxButton *scriptButton;
    
    wxString integratorString;
    wxString primaryBodyString;
    wxString gravityFieldString;
    wxString earthDegreeString;
    wxString earthOrderString;
    wxString sunDegreeString;
    wxString sunOrderString;
    wxString moonDegreeString;
    wxString moonOrderString;

    std::string propSetupName;
    std::string newPropName;
    
    wxArrayString primaryBodiesArray;
    wxArrayString savedBodiesArray;
    wxArrayString pointmassBodiesArray;    
    wxArrayString primaryBodiesGravityArray;
    
//    wxDocManager *mDocManager;
//    wxDocTemplate *mDocTemplate;
//    ViewTextFrame *mTextFrame;
    
    Integer numOfIntegrators;
    Integer numOfBodies;
    Integer numOfAtmosTypes;
    Integer numOfForces;
    Integer numOfMagFields;
    Integer numOfGraFields;
    Integer orderID;
    Integer degreeID;
    
    bool useSRP;
    bool isForceModelChanged;
    bool isIntegratorChanged;
    
    GuiInterpreter *theGuiInterpreter;
    Propagator     *thePropagator;
    Propagator     *newProp;
    Integrator     *theIntegrator;
    PropSetup      *thePropSetup;
    PhysicalModel  *thePhysicalModel;
    ForceModel     *theForceModel;
    PointMassForce *thePointMass;
    SolarSystem    *theSolarSystem;
    CelestialBody  *theEarth;  
    CelestialBody  *theSun;    
    CelestialBody  *theMoon; 
    std::vector<PointMassForce *> thePMForces;
    std::vector<Planet *> thePlanets;
   
    // Layout & data handling methods
    void Initialize();
    void Setup(wxWindow *parent);
    void LoadData();
    void SaveData();
    void DisplayIntegratorData();
    void DisplayPrimaryBodyData();
    void DisplayForceData(); //loj: 2/11/04 added
    void DisplayGravityFieldData();
    void DisplayAtmosphereModelData();
    void DisplayMagneticFieldData();
    void DisplayPointMassData();
    void DisplaySRPData();
    ForceModel* UpdateForceModel();
//    void CreateScript();
//    wxMenuBar* CreateScriptWindowMenu(const std::string &docType);
    
    // Text control event method
    void OnIntegratorTextUpdate();
    void OnGravityTextUpdate();
    void OnMagneticTextUpdate();
    
    // Checkbox event method
    void OnSRPCheckBoxChange();
    
    // Combobox event method
    void OnIntegratorSelection();
    void OnBodySelection();
    void OnGravitySelection();
    void OnAtmosphereSelection();
    
    // Button event methods
    void OnScriptButton();
    void OnOKButton();
    void OnApplyButton();
    void OnCancelButton();
    void OnHelpButton();
    void OnAddButton();
    void OnGravSearchButton();
    void OnSetupButton();
    void OnMagSearchButton();
    void OnPMEditButton();
    void OnSRPEditButton();

    // any class wishing to process wxWindows events must use this macro
    DECLARE_EVENT_TABLE();
    
    // IDs for the controls and the menu commands
    enum
    {     
        ID_TEXT = 42000,
        ID_TEXTCTRL,
        ID_TEXTCTRL_PROP,
        ID_TEXTCTRL_INTG1,
        ID_TEXTCTRL_INTG2,
        ID_TEXTCTRL_INTG3,
        ID_TEXTCTRL_INTG4,
        ID_TEXTCTRL_INTG5,
        ID_TEXTCTRL_INTG6,
        ID_TEXTCTRL_INTG7,
        ID_TEXTCTRL_INTG8,
        ID_TEXTCTRL_INTG9,
        ID_TEXTCTRL_GRAV1,
        ID_TEXTCTRL_GRAV2,
        ID_TEXTCTRL_MAGN1,
        ID_TEXTCTRL_MAGN2,
        ID_CHECKBOX,
        ID_CB_INTGR,
        ID_CB_BODY,
        ID_CB_GRAV,
        ID_CB_ATMOS,
        ID_CB_MAG,
        ID_BUTTON_SCRIPT,
        ID_BUTTON_OK,
        ID_BUTTON_APPLY,
        ID_BUTTON_CANCEL,
        ID_BUTTON_HELP,
        ID_BUTTON_ADD_BODY,
        ID_BUTTON_GRAV_SEARCH,
        ID_BUTTON_SETUP,
        ID_BUTTON_MAG_SEARCH,
        ID_BUTTON_PM_EDIT,
        ID_BUTTON_SRP_EDIT 
    };
};

#endif // PropagationConfigPanel_hpp
