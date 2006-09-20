#ifndef NONLINEARCONSTRAINTPANEL_HPP_
#define NONLINEARCONSTRAINTPANEL_HPP_

#include "gmatwxdefs.hpp"
#include "GmatPanel.hpp"

#include "gmatdefs.hpp"
#include "GuiInterpreter.hpp"
//#include "NonLinearConstraint.hpp"
#include "Solver.hpp"
#include "Parameter.hpp"

class NonLinearConstraintPanel : public GmatPanel
{
public:
   NonLinearConstraintPanel(wxWindow *parent, GmatCommand *cmd);
   ~NonLinearConstraintPanel(); 
    
private:             

   wxString  solverName;
   wxString lhs, rhs;
   Parameter *lhsParam;
   Parameter *rhsParam;

   wxTextCtrl *mLHSTextCtrl;
   wxTextCtrl *mRHSTextCtrl;

   wxButton *mLeftChooseButton;
   wxButton *mRightChooseButton;

   wxComboBox *mSolverComboBox;
   wxComboBox *mComparisonComboBox;
   
   wxArrayString mObjectTypeList;
//   NonLinearConstraint *mNonLinearConstraintCommand;
   
   // methods inherited from GmatPanel
   virtual void Create();
   virtual void LoadData();
   virtual void SaveData();

   void ShowGoalSetup();
   
   // event handling method
   void OnTextChange(wxCommandEvent& event);    
   void OnSolverSelection(wxCommandEvent &event);
   void OnButtonClick(wxCommandEvent& event);
   
   // any class wishing to process wxWindows events must use this macro
   DECLARE_EVENT_TABLE();
  
   // IDs for the controls and the menu commands
   enum
   {     
      ID_TEXT = 53000,
      ID_TEXTCTRL,
      ID_BUTTON,
      ID_COMBO,
      ID_GRID,
   };
};

#endif /*NONLINEARCONSTRAINTPANEL_HPP_*/
