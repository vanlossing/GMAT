//$Header$
//------------------------------------------------------------------------------
//                              MissionTree
//------------------------------------------------------------------------------
// GMAT: Goddard Mission Analysis Tool
//
// ** Legal **
//
// Author: Allison Greene
// Created: 2003/09/02
/**
 * This class provides the tree for missions.
 */
//------------------------------------------------------------------------------
#ifndef MissionTree_hpp
#define MissionTree_hpp

#include "wx/treectrl.h"
#include "wx/image.h"
#include "wx/imaglist.h"

#include "GmatMainNotebook.hpp"
#include "DecoratedTree.hpp"

#include "GuiInterpreter.hpp"

class MissionTree : public DecoratedTree
{
public:
    // constructors
    MissionTree(wxWindow *parent, const wxWindowID id,
                const wxPoint& pos, const wxSize& size,
                long style);
    //   void SetMainNotebook (GmatMainNotebook *mainNotebook);
    //   GmatMainNotebook *GetMainNotebook();

    void UpdateMission();
    
protected:

private:
    GuiInterpreter *theGuiInterpreter;
    GmatMainNotebook *mainNotebook;
    wxWindow *parent;
    
    wxTreeItemId mMissionSeqTopItem;
    wxTreeItemId mMissionSeqSubItem;
    
    bool before;
    int numManeuver;
    int mNumMissionSeq;
    int mNumPropagate;
    int mNumManeuver;
    int mNumTarget;

    void UpdateCommand();
    void AddDefaultMission();
    void AddDefaultMissionSeq(wxTreeItemId universe);
    void AddIcons();

    // event handlers
    void OnItemRightClick(wxTreeEvent& event);
    void OnItemActivated(wxTreeEvent &event);
    void ShowMenu(wxTreeItemId id, const wxPoint& pt);
    void OnBefore();
    void OnAfter();
    
    void OnAddMissionSeq(wxCommandEvent &event);
    
    void OnAddPropagate(wxCommandEvent &event);
    void OnAddManeuver(wxCommandEvent &event);
    void OnAddTarget(wxCommandEvent &event);
    
    void OnInsertPropagate(wxCommandEvent &event);
    void OnInsertManeuver(wxCommandEvent &event);
    void OnInsertTarget(wxCommandEvent &event);
    
    void OnViewVariables();
    void OnViewGoals();

    void OnRun();   
 
    wxMenu* CreatePopupMenu();
    wxMenu* CreateInsertPopupMenu();
    wxMenu* CreateControlLogicPopupMenu();
    
    DECLARE_EVENT_TABLE();
       
    enum
    {
        POPUP_SWAP_BEFORE = 25000,
        POPUP_SWAP_AFTER,
        POPUP_CUT,
        POPUP_COPY,
        POPUP_PASTE,
        POPUP_DELETE,
        POPUP_RENAME,

        POPUP_ADD_MISSION_SEQ,
        POPUP_ADD_COMMAND,
        POPUP_ADD_PROPAGATE,
        POPUP_ADD_MANEUVER,
        POPUP_ADD_TARGET,
        
        POPUP_INSERT_COMMAND,
        POPUP_INSERT_PROPAGATE,
        POPUP_INSERT_MANEUVER,
        POPUP_INSERT_TARGET,

        POPUP_VIEW_VARIABLES,
        POPUP_VIEW_GOALS, 

        POPUP_RUN,

        POPUP_CONTROL_LOGIC,
        POPUP_WHILE_CONTROL,
        POPUP_FOR_CONTROL,
        POPUP_DO_CONTROL,
    };
};

#endif // MissionTree_hpp
