//$Header$
//------------------------------------------------------------------------------
//                              ResourceTree
//------------------------------------------------------------------------------
// GMAT: Goddard Mission Analysis Tool
//
// Author: Allison Greene
// Created: 2003/08/29
//
/**
 * This class provides the resource tree and event handlers.
 */
//------------------------------------------------------------------------------
#include "gmatwxdefs.hpp"
#include "bitmaps/folder.xpm"
#include "bitmaps/openfolder.xpm"
#include "bitmaps/file.xpm"
#include "bitmaps/spacecraft.xpm"
#include "bitmaps/sun.xpm"
#include "bitmaps/mercury.xpm"
#include "bitmaps/venus.xpm"    
#include "bitmaps/earth.xpm"
#include "bitmaps/mars.xpm"
#include "bitmaps/jupiter.xpm"
#include "bitmaps/saturn.xpm"
#include "bitmaps/uranus.xpm"
#include "bitmaps/neptune.xpm"
#include "bitmaps/pluto.xpm"
#include "bitmaps/report.xpm"
#include "bitmaps/network.xpm"
#include "bitmaps/burn.xpm"
#include "bitmaps/moon.xpm"

#include "GuiInterpreter.hpp"
#include "GmatAppData.hpp"
#include "ResourceTree.hpp"
#include "MessageInterface.hpp"
#include "GmatTreeItemData.hpp"
#include "ParameterCreateDialog.hpp"

//------------------------------------------------------------------------------
// event tables and other macros for wxWindows
//------------------------------------------------------------------------------

BEGIN_EVENT_TABLE(ResourceTree, wxTreeCtrl)
   EVT_TREE_ITEM_RIGHT_CLICK(-1, ResourceTree::OnItemRightClick)

   // had to change back to double click because of a segmentation
   // fault
   EVT_TREE_ITEM_ACTIVATED(-1, ResourceTree::OnItemActivated)
   EVT_TREE_BEGIN_LABEL_EDIT(-1, ResourceTree::OnBeginLabelEdit)
   EVT_TREE_END_LABEL_EDIT(-1, ResourceTree::OnEndLabelEdit)
   
   EVT_MENU(POPUP_ADD_SC, ResourceTree::OnAddSpacecraft)
   EVT_MENU(POPUP_ADD_FORMATION, ResourceTree::OnAddFormation)
   EVT_MENU(POPUP_ADD_CONSTELLATION, ResourceTree::OnAddConstellation)
   EVT_MENU(POPUP_ADD_IMPULSIVE_BURN, ResourceTree::OnAddImpulsiveBurn)
   EVT_MENU(POPUP_ADD_PROPAGATOR, ResourceTree::OnAddPropagator)
   EVT_MENU(POPUP_ADD_BODY, ResourceTree::OnAddBody)
   EVT_MENU(POPUP_ADD_DIFF_CORR, ResourceTree::OnAddDiffCorr)
   EVT_MENU(POPUP_ADD_REPORT_FILE, ResourceTree::OnAddReportFile)
   EVT_MENU(POPUP_ADD_XY_PLOT, ResourceTree::OnAddXyPlot)
   EVT_MENU(POPUP_ADD_OPENGL_PLOT, ResourceTree::OnAddOpenGlPlot)
   EVT_MENU(POPUP_ADD_VARIABLE, ResourceTree::OnAddVariable)
   EVT_MENU(POPUP_OPEN, ResourceTree::OnOpen)
   EVT_MENU(POPUP_CLOSE, ResourceTree::OnClose)
   EVT_MENU(POPUP_RENAME, ResourceTree::OnRename)
   EVT_MENU(POPUP_DELETE, ResourceTree::OnDelete)
   
END_EVENT_TABLE()

//------------------------------------------------------------------------------
// ResourceTree(wxFrame *parent)
//------------------------------------------------------------------------------
/**
 * A constructor.
 */
//------------------------------------------------------------------------------
ResourceTree::ResourceTree(wxWindow *parent, const wxWindowID id,
                           const wxPoint &pos, const wxSize &size, long style)
    : wxTreeCtrl(parent, id, pos, size, style)
{
    parent = parent;
    mainNotebook = GmatAppData::GetMainNotebook();
    
    theGuiInterpreter = GmatAppData::GetGuiInterpreter();
    theGuiManager = GuiItemManager::GetInstance();
    
    AddIcons();
    AddDefaultResources();

    mNumSpacecraft = 0;
    mNumPropagator = 0;
    mNumImpulsiveBurn = 0;
    mNumReportFile = 0;
    mNumXyPlot = 0;
    mNumOpenGlPlot = 0;
    mNumDiffCorr = 0;
    mNumVariable = 0;

    theGuiManager->UpdateAll();
}

//------------------------------------------------------------------------------
// void UpdateResource()
//------------------------------------------------------------------------------
void ResourceTree::UpdateResource()
{
    MessageInterface::ShowMessage("ResourceTree::UpdateResource() entered\n");
    
    mNumSpacecraft = 0;
    mNumPropagator = 0;
    mNumImpulsiveBurn = 0;
    mNumReportFile = 0;
    mNumXyPlot = 0;
    mNumOpenGlPlot = 0;
    mNumDiffCorr = 0;
    mNumVariable = 0;

    // ag: collapse, so folder icon is closed
    Collapse(mSpacecraftItem);
    Collapse(mPropagatorItem);
    Collapse(mBurnItem);
    Collapse(mSolverItem);
    Collapse(mSubscriberItem);
    Collapse(mVariableItem);
    
    DeleteChildren(mSpacecraftItem);
    DeleteChildren(mPropagatorItem);
    DeleteChildren(mBurnItem);
    DeleteChildren(mSolverItem);
    DeleteChildren(mSubscriberItem);
    DeleteChildren(mVariableItem);

    AddDefaultSpacecraft(mSpacecraftItem);
    AddDefaultPropagators(mPropagatorItem);
    AddDefaultBurns(mBurnItem);
    AddDefaultSolvers(mSolverItem);
    AddDefaultSubscribers(mSubscriberItem);
    AddDefaultVariables(mVariableItem);
      
    theGuiManager->UpdateAll(); //loj: 3/5/04 added

}

//------------------------------------------------------------------------------
//  void AddDefaultResources()
//------------------------------------------------------------------------------
void ResourceTree::AddDefaultResources()
{
    wxTreeItemId resource =
        AddRoot(wxT("Resources"), -1, -1,
                new GmatTreeItemData(wxT("Resources"), GmatTree::RESOURCES_FOLDER));

    //----- Spacecraft
    mSpacecraftItem =
        AppendItem(resource, wxT("Spacecraft"), GmatTree::ICON_FOLDER, -1,
                   new GmatTreeItemData(wxT("Spacecraft"),
                                        GmatTree::SPACECRAFT_FOLDER));
    
    SetItemImage(mSpacecraftItem, GmatTree::ICON_OPENFOLDER,
                 wxTreeItemIcon_Expanded);
    
    //----- Formations
    wxTreeItemId formationItem =
        AppendItem(resource, wxT("Formations"), GmatTree::ICON_FOLDER, -1,
                   new GmatTreeItemData(wxT("Formations"),
                                        GmatTree::FORMATIONS_FOLDER));
    
    SetItemImage(formationItem, GmatTree::ICON_OPENFOLDER, 
                 wxTreeItemIcon_Expanded);

    //----- Constellations
    wxTreeItemId constellationItem =
        AppendItem(resource,
                   wxT("Constellations"), GmatTree::ICON_FOLDER, -1,
                   new GmatTreeItemData(wxT("Constellations"),
                                        GmatTree::CONSTELLATIONS_FOLDER));
    
    SetItemImage(constellationItem, GmatTree::ICON_OPENFOLDER, 
                 wxTreeItemIcon_Expanded);

    //----- Burns
    mBurnItem =
        AppendItem(resource, wxT("Burns"), GmatTree::ICON_FOLDER, -1,
                   new GmatTreeItemData(wxT("Burns"),
                                        GmatTree::BURNS_FOLDER));
    
    SetItemImage(mBurnItem, GmatTree::ICON_OPENFOLDER, wxTreeItemIcon_Expanded);

    
    //----- Propagators
    mPropagatorItem =
        AppendItem(resource,
                   wxT("Propagators"), GmatTree::ICON_FOLDER, -1,
                   new GmatTreeItemData(wxT("Propagators"),
                                        GmatTree::PROPAGATORS_FOLDER));
    
    SetItemImage(mPropagatorItem, GmatTree::ICON_OPENFOLDER, 
                 wxTreeItemIcon_Expanded);
    
    //----- Universe
    wxTreeItemId universeItem =
        AppendItem(resource, wxT("Universe"), GmatTree::ICON_FOLDER, -1,
                   new GmatTreeItemData(wxT("Universe"),
                                        GmatTree::UNIVERSE_FOLDER));
    
    SetItemImage(universeItem, GmatTree::ICON_OPENFOLDER, 
                 wxTreeItemIcon_Expanded);

    //----- Solver
    mSolverItem =
        AppendItem(resource, wxT("Solvers"), GmatTree::ICON_FOLDER, -1,
                   new GmatTreeItemData(wxT("Solvers"),
                                        GmatTree::SOLVERS_FOLDER));
    
    SetItemImage(mSolverItem, GmatTree::ICON_OPENFOLDER, 
                 wxTreeItemIcon_Expanded);

    //----- Subscribers
    mSubscriberItem =
        AppendItem(resource, wxT("Plots/Reports"), GmatTree::ICON_FOLDER,
                   -1, new GmatTreeItemData(wxT("Plots/Reports"),
                                            GmatTree::SUBSCRIBERS_FOLDER));
    
    SetItemImage(mSubscriberItem, GmatTree::ICON_OPENFOLDER, 
                 wxTreeItemIcon_Expanded);

    //----- Interfaces
    wxTreeItemId interfaceItem =
        AppendItem(resource, wxT("Interfaces"), GmatTree::ICON_FOLDER, -1,
                   new GmatTreeItemData(wxT("Interfaces"),
                                        GmatTree::INTERFACES_FOLDER));
    
    SetItemImage(interfaceItem, GmatTree::ICON_OPENFOLDER, 
                 wxTreeItemIcon_Expanded);

    //----- Subscripts
    wxTreeItemId subscriptItem =
        AppendItem(resource, wxT("Subscripts"), GmatTree::ICON_FOLDER, -1,
                   new GmatTreeItemData(wxT("Subscripts"),
                                        GmatTree::SUBSCRIPTS_FOLDER));

    SetItemImage(subscriptItem, GmatTree::ICON_OPENFOLDER,
                 wxTreeItemIcon_Expanded);
    
    //----- Vairables
    mVariableItem = 
        AppendItem(resource, wxT("Variables"), GmatTree::ICON_FOLDER, -1,
                   new GmatTreeItemData(wxT("Variables"),
                                        GmatTree::VARIABLES_FOLDER));
    // ag:  Should the GmatTree type of variableItem change?

    SetItemImage(mVariableItem, GmatTree::ICON_OPENFOLDER,
                 wxTreeItemIcon_Expanded);
    
    //----- GroundStations
    AppendItem(resource, wxT("Ground Stations"), GmatTree::ICON_FOLDER, 
               -1, new GmatTreeItemData(wxT("Ground Stations"),
                                        GmatTree::GROUNDSTATIONS_FOLDER));

    AddDefaultBodies(universeItem);
    AddDefaultSpacecraft(mSpacecraftItem);
    AddDefaultFormations(formationItem);
    AddDefaultConstellations(constellationItem);
    AddDefaultPropagators(mPropagatorItem);
    AddDefaultSolvers(mSolverItem);
    AddDefaultSubscribers(mSubscriberItem);
    AddDefaultInterfaces(interfaceItem);
    AddDefaultVariables(mVariableItem);
}

//------------------------------------------------------------------------------
// void AddDefaultBodies(wxTreeItemId itemId)
//------------------------------------------------------------------------------
void ResourceTree::AddDefaultBodies(wxTreeItemId itemId)
{
    AppendItem(itemId, wxT("Sun"), GmatTree::ICON_SUN, -1,
               new GmatTreeItemData(wxT("Sun"), GmatTree::DEFAULT_BODY));
    AppendItem(itemId, wxT("Mercury"), GmatTree::ICON_MERCURY, -1,
               new GmatTreeItemData(wxT("Mercury"), GmatTree::DEFAULT_BODY));
    AppendItem(itemId, wxT("Venus"), GmatTree::ICON_VENUS, -1,
               new GmatTreeItemData(wxT("Venus"), GmatTree::DEFAULT_BODY));

    wxTreeItemId earth = AppendItem(itemId, wxT("Earth"), GmatTree::ICON_EARTH, -1,
                                    new GmatTreeItemData(wxT("Earth"), GmatTree::DEFAULT_BODY));
    AppendItem(earth, wxT("Moon"), GmatTree::ICON_MOON, -1,
               new GmatTreeItemData(wxT("Moon"), GmatTree::DEFAULT_BODY));

    AppendItem(itemId, wxT("Mars"), GmatTree::ICON_MARS, -1,
               new GmatTreeItemData(wxT("Mars"), GmatTree::DEFAULT_BODY));
    AppendItem(itemId, wxT("Jupiter"), GmatTree::ICON_JUPITER, -1,
               new GmatTreeItemData(wxT("Jupiter"), GmatTree::DEFAULT_BODY));
    AppendItem(itemId, wxT("Saturn"), GmatTree::ICON_SATURN, -1,
               new GmatTreeItemData(wxT("Saturn"), GmatTree::DEFAULT_BODY));
    AppendItem(itemId, wxT("Uranus"), GmatTree::ICON_URANUS, -1,
               new GmatTreeItemData(wxT("Uranus"), GmatTree::DEFAULT_BODY));
    AppendItem(itemId, wxT("Neptune"), GmatTree::ICON_NEPTUNE, -1,
               new GmatTreeItemData(wxT("Neptune"), GmatTree::DEFAULT_BODY));
    AppendItem(itemId, wxT("Pluto"), GmatTree::ICON_PLUTO, -1,
               new GmatTreeItemData(wxT("Pluto"), GmatTree::DEFAULT_BODY));
}

//------------------------------------------------------------------------------
// void AddDefaultSpacecraft(wxTreeItemId itemId)
//------------------------------------------------------------------------------
void ResourceTree::AddDefaultSpacecraft(wxTreeItemId itemId)
{
    StringArray itemNames = theGuiInterpreter->GetListOfConfiguredItems(Gmat::SPACECRAFT);
    int size = itemNames.size();
    wxString objName;
    
    for (int i = 0; i<size; i++)
    {
        objName = wxString(itemNames[i].c_str());
        AppendItem(itemId, wxT(objName), GmatTree::ICON_SPACECRAFT, -1,
                         new GmatTreeItemData(wxT(objName), GmatTree::DEFAULT_SPACECRAFT));
    };

    if (size > 0)
        Expand(itemId);
}


//------------------------------------------------------------------------------
// void AddDefaultFormations(wxTreeItemId itemId)
//------------------------------------------------------------------------------
void ResourceTree::AddDefaultFormations(wxTreeItemId itemId)
{
    wxTreeItemId mms =
        AppendItem(itemId, wxT("MMS"), -1, -1,
                   new GmatTreeItemData(wxT("MMS"), GmatTree::DEFAULT_FORMATION_FOLDER));
    SetItemImage(mms, GmatTree::ICON_OPENFOLDER, wxTreeItemIcon_Expanded);
    
    AppendItem(mms, wxT("MMS1"), GmatTree::ICON_SPACECRAFT, -1,
               new GmatTreeItemData(wxT("MMS1"), GmatTree::DEFAULT_FORMATION_SPACECRAFT));
    AppendItem(mms, wxT("MMS2"), GmatTree::ICON_SPACECRAFT, -1,
               new GmatTreeItemData(wxT("MMS2"), GmatTree::DEFAULT_FORMATION_SPACECRAFT));
    AppendItem(mms, wxT("MMS3"), GmatTree::ICON_SPACECRAFT, -1,
               new GmatTreeItemData(wxT("MMS3"), GmatTree::DEFAULT_FORMATION_SPACECRAFT));
    AppendItem(mms, wxT("MMS4"), GmatTree::ICON_SPACECRAFT, -1,
               new GmatTreeItemData(wxT("MMS4"), GmatTree::DEFAULT_FORMATION_SPACECRAFT));
}

//------------------------------------------------------------------------------
// void AddDefaultConstellations(wxTreeItemId itemId)
//------------------------------------------------------------------------------
void ResourceTree::AddDefaultConstellations(wxTreeItemId itemId)
{
    wxTreeItemId gps =
        AppendItem(itemId, wxT("GPS"), -1, -1,
                   new GmatTreeItemData(wxT("GPS"), GmatTree::DEFAULT_CONSTELLATION_FOLDER));
    SetItemImage(gps, GmatTree::ICON_OPENFOLDER, wxTreeItemIcon_Expanded);

    AppendItem(gps, wxT("GPS1"), GmatTree::ICON_SPACECRAFT, -1,
               new GmatTreeItemData(wxT("GPS1"), GmatTree::DEFAULT_CONSTELLATION_SATELLITE));
    AppendItem(gps, wxT("GPS2"), GmatTree::ICON_SPACECRAFT, -1,
               new GmatTreeItemData(wxT("GPS2"), GmatTree::DEFAULT_CONSTELLATION_SATELLITE));
    AppendItem(gps, wxT("GPS3"), GmatTree::ICON_SPACECRAFT, -1,
               new GmatTreeItemData(wxT("GPS3"), GmatTree::DEFAULT_CONSTELLATION_SATELLITE));
    AppendItem(gps, wxT("GPS4"), GmatTree::ICON_SPACECRAFT, -1,
               new GmatTreeItemData(wxT("GPS4"), GmatTree::DEFAULT_CONSTELLATION_SATELLITE));
}

//------------------------------------------------------------------------------
// void AddDefaultPropagators(wxTreeItemId itemId)
//------------------------------------------------------------------------------
void ResourceTree::AddDefaultPropagators(wxTreeItemId itemId)
{
    StringArray itemNames = theGuiInterpreter->GetListOfConfiguredItems(Gmat::PROP_SETUP);
    int size = itemNames.size();
    wxString objName;
    
    for (int i = 0; i<size; i++)
    {
        objName = wxString(itemNames[i].c_str());
        AppendItem(itemId, wxT(objName), GmatTree::ICON_FILE, -1,
                   new GmatTreeItemData(wxT(objName), GmatTree::DEFAULT_PROPAGATOR));
    };

    if (size > 0)
        Expand(itemId);
}

//------------------------------------------------------------------------------
// void AddDefaultBurns(wxTreeItemId itemId)
//------------------------------------------------------------------------------
void ResourceTree::AddDefaultBurns(wxTreeItemId itemId)
{
    //MessageInterface::ShowMessage("ResourceTree::AddDefaultBurns() entered\n");
    StringArray itemNames = theGuiInterpreter->GetListOfConfiguredItems(Gmat::BURN);
    int size = itemNames.size();
    wxString objName;
    wxString objTypeName;
    
    for (int i = 0; i<size; i++)
    {
        Burn *burn = theGuiInterpreter->GetBurn(itemNames[i]);
        objName = wxString(itemNames[i].c_str());
        objTypeName = wxString(burn->GetTypeName().c_str());

        if (objTypeName == "ImpulsiveBurn")
        {
            //MessageInterface::ShowMessage("ResourceTree::AddDefaultBurns() objTypeName = ImpulsiveBurn\n");
            AppendItem(itemId, wxT(objName), GmatTree::ICON_BURN, -1,
                       new GmatTreeItemData(wxT(objName),
                                            GmatTree::DEFAULT_IMPULSIVE_BURN));
        }
    };

    if (size > 0)
        Expand(itemId);
}

//------------------------------------------------------------------------------
// void AddDefaultSolvers(wxTreeItemId itemId)
//------------------------------------------------------------------------------
void ResourceTree::AddDefaultSolvers(wxTreeItemId itemId)
{
    StringArray itemNames = theGuiInterpreter->GetListOfConfiguredItems(Gmat::SOLVER);
    int size = itemNames.size();
    wxString objName;
    wxString objTypeName;
    
    for (int i = 0; i<size; i++)
    {
        Solver *solver = theGuiInterpreter->GetSolver(itemNames[i]);
        objName = wxString(itemNames[i].c_str());
        objTypeName = wxString(solver->GetTypeName().c_str());
        
        if (objTypeName == "DifferentialCorrector")
        {
            AppendItem(itemId, wxT(objName), GmatTree::ICON_FILE, -1,
                       new GmatTreeItemData(wxT(objName), GmatTree::DEFAULT_DIFF_CORR));
        }
    };

    if (size > 0)
        Expand(itemId);
}

//------------------------------------------------------------------------------
// void AddDefaultSubscribers(wxTreeItemId itemId)
//------------------------------------------------------------------------------
void ResourceTree::AddDefaultSubscribers(wxTreeItemId itemId)
{
    StringArray itemNames = theGuiInterpreter->GetListOfConfiguredItems(Gmat::SUBSCRIBER);
    int size = itemNames.size();
    wxString objName;
    wxString objTypeName;
    
    for (int i = 0; i<size; i++)
    {
        Subscriber *sub = theGuiInterpreter->GetSubscriber(itemNames[i]);
        objName = wxString(itemNames[i].c_str());
        objTypeName = wxString(sub->GetTypeName().c_str());

        if (objTypeName == "ReportFile")
            AppendItem(itemId, wxT(objName), GmatTree::ICON_REPORT, -1,
                       new GmatTreeItemData(wxT(objName),
                                            GmatTree::DEFAULT_REPORT_FILE));
        else if (objTypeName == "XyPlot")
            AppendItem(itemId, wxT(objName), GmatTree::ICON_REPORT, -1,
                       new GmatTreeItemData(wxT(objName),
                                            GmatTree::DEFAULT_XY_PLOT));
        else if (objTypeName == "OpenGlPlot")
            AppendItem(itemId, wxT(objName), GmatTree::ICON_REPORT, -1,
                       new GmatTreeItemData(wxT(objName),
                                            GmatTree::DEFAULT_OPENGL_PLOT));
    };

    if (size > 0)
        Expand(itemId);
}

//------------------------------------------------------------------------------
// void AddDefaultInterfaces(wxTreeItemId itemId)
//------------------------------------------------------------------------------
void ResourceTree::AddDefaultInterfaces(wxTreeItemId itemId)
{
    AppendItem(itemId, wxT("TCP/IP"), GmatTree::ICON_NETWORK, -1,
               new GmatTreeItemData(wxT("TCP/IP"), GmatTree::DEFAULT_INTERFACE));
    AppendItem(itemId, wxT("Mex"), GmatTree::ICON_FILE, -1,
               new GmatTreeItemData(wxT("Mex"), GmatTree::DEFAULT_INTERFACE));
    
}

//------------------------------------------------------------------------------
// void AddDefaultVariables(wxTreeItemId itemId)
//------------------------------------------------------------------------------
void ResourceTree::AddDefaultVariables(wxTreeItemId itemId)
{
    StringArray itemNames = theGuiInterpreter->GetListOfConfiguredItems(Gmat::PARAMETER);
    int size = itemNames.size();
    wxString objName;
    Parameter *param;
    
    for (int i = 0; i<size; i++)
    {
        objName = wxString(itemNames[i].c_str());
        param = theGuiInterpreter->GetParameter(itemNames[i]);
        
        AppendItem(itemId, wxT(objName), GmatTree::ICON_FILE, -1,
                   new GmatTreeItemData(wxT(objName),
                                        GmatTree::DEFAULT_VARIABLE));
    };

    theGuiManager->UpdateParameter();
    
    if (size > 0)
        Expand(itemId);
}
//------------------------------------------------------------------------------
// void OnItemRightClick(wxTreeEvent& event)
//------------------------------------------------------------------------------
void ResourceTree::OnItemRightClick(wxTreeEvent& event)
{
    ShowMenu(event.GetItem(), event.GetPoint());
}

//------------------------------------------------------------------------------
// void ShowMenu(wxTreeItemId itemId, const wxPoint& pt)
//------------------------------------------------------------------------------
void ResourceTree::ShowMenu(wxTreeItemId itemId, const wxPoint& pt)
{
    GmatTreeItemData *treeItem = (GmatTreeItemData *)GetItemData(itemId);
    wxString title = treeItem->GetDesc();
    int dataType = treeItem->GetDataType();
    
#if wxUSE_MENUS
    wxMenu menu;
    
    if (strcmp(title, wxT("Spacecraft")) == 0)
        menu.Append(POPUP_ADD_SC, wxT("Add Spacecraft..."));
    else if (strcmp(title, wxT("Formations")) == 0)
        menu.Append(POPUP_ADD_FORMATION, wxT("Add Formation..."));
    else if (strcmp(title, wxT("Constellations")) == 0)
        menu.Append(POPUP_ADD_CONSTELLATION, wxT("Add Constellation..."));
    else if (strcmp(title, wxT("Burns")) == 0)
        menu.Append(POPUP_ADD_BURN, wxT("Add"), CreatePopupMenu(Gmat::BURN));
    else if (strcmp(title, wxT("Propagators")) == 0)
        menu.Append(POPUP_ADD_PROPAGATOR, wxT("Add Propagator..."));
    else if (strcmp(title, wxT("Solvers")) == 0)
        menu.Append(POPUP_ADD_SOLVER, wxT("Add"), CreatePopupMenu(Gmat::SOLVER));
    else if (strcmp(title, wxT("Universe")) == 0)
        menu.Append(POPUP_ADD_BODY, wxT("Add Body..."));
    else if (strcmp(title, wxT("Plots/Reports")) == 0)
        menu.Append(POPUP_ADD_SUBSCRIBER, _T("Add"), CreatePopupMenu(Gmat::SUBSCRIBER));
    else if (strcmp(title, wxT("Variables")) == 0)
        menu.Append(POPUP_ADD_VARIABLE, wxT("Add Variable..."));
    else if ((dataType == GmatTree::DEFAULT_FORMATION_FOLDER)     ||
             (dataType == GmatTree::CREATED_FORMATION_FOLDER)     ||
             (dataType == GmatTree::DEFAULT_CONSTELLATION_FOLDER) ||
             (dataType == GmatTree::CREATED_CONSTELLATION_FOLDER))
    {
        menu.Append(POPUP_ADD_SC, wxT("Add Spacecraft..."));
        menu.AppendSeparator();
        menu.Append(POPUP_OPEN, wxT("Open"));
        menu.Append(POPUP_CLOSE, wxT("Close"));
        menu.AppendSeparator();
        menu.Append(POPUP_RENAME, wxT("Rename"));
        menu.Append(POPUP_DELETE, wxT("Delete"));
    }
    else if (dataType == GmatTree::DEFAULT_PROPAGATOR)
    {
        menu.Append(POPUP_OPEN, wxT("Open"));
        menu.Append(POPUP_CLOSE, wxT("Close"));
    }
    else
    {
        menu.Append(POPUP_OPEN, wxT("Open"));
        menu.Append(POPUP_CLOSE, wxT("Close"));
        menu.AppendSeparator();
        menu.Append(POPUP_RENAME, wxT("Rename"));
        menu.Append(POPUP_DELETE, wxT("Delete"));
    }

    //loj: 2/13/04 I just thought about this for the future build
//      switch (dataType)
//      {
//      case GmatTree::SPACECRAFT_FOLDER:
//          menu.Append(POPUP_CLEAR_SC, wxT("Clear"));
//      case GmatTree::BURNS_FOLDER:
//          menu.Append(POPUP_CLEAR_BURN, wxT("Clear"));
//      case GmatTree::PROPAGATORS_FOLDER:
//           menu.Append(POPUP_CLEAR_PROPAGATOR, wxT("Clear"));
//      case GmatTree::SOLVERS_FOLDER:
//          menu.Append(POPUP_CLEAR_SOLVER, wxT("Clear"));
//      case GmatTree::SUBSCRIBERS_FOLDER:
//          menu.Append(POPUP_CLEAR_SUBSCRIBER, wxT("Clear"));
//          break;
//      default:
//          break;
//      }
    
    PopupMenu(&menu, pt);
#endif // wxUSE_MENUS
}

//------------------------------------------------------------------------------
// void OnItemActivated(wxTreeEvent &event)
//------------------------------------------------------------------------------
void ResourceTree::OnItemActivated(wxTreeEvent &event)
{
    // get some info about this item
    wxTreeItemId itemId = event.GetItem();
    GmatTreeItemData *item = (GmatTreeItemData *)GetItemData(itemId);

    mainNotebook->CreatePage(item);
}

// ag:  Removed - can use GmatAppData::Set/Get MainNotebook
////------------------------------------------------------------------------------
//// void SetMainNotebook (GmatMainNotebook *mainNotebook)
////------------------------------------------------------------------------------
//void ResourceTree::SetMainNotebook (GmatMainNotebook *mainNotebook)
//{
//  mainNotebook = mainNotebook;
//}
//
////------------------------------------------------------------------------------
//// GmatMainNotebook *GetMainNotebook()
////------------------------------------------------------------------------------
//GmatMainNotebook *ResourceTree::GetMainNotebook()
//{
//   return mainNotebook;
//}

//------------------------------------------------------------------------------
// void OnOpen(wxCommandEvent &event)
//------------------------------------------------------------------------------
void ResourceTree::OnOpen(wxCommandEvent &event)
{
    // Get info from selected item
    GmatTreeItemData *item = (GmatTreeItemData *) GetItemData(GetSelection());
    mainNotebook->CreatePage(item);
}

//------------------------------------------------------------------------------
// void OnClose(wxCommandEvent &event)
//------------------------------------------------------------------------------
void ResourceTree::OnClose(wxCommandEvent &event)
{
    // make item most current, then close it
    GmatTreeItemData *item = (GmatTreeItemData *) GetItemData(GetSelection());
    mainNotebook->CreatePage(item);
    mainNotebook->ClosePage();
}

//------------------------------------------------------------------------------
// void OnRename(wxCommandEvent &event)
//------------------------------------------------------------------------------
void ResourceTree::OnRename(wxCommandEvent &event)
{
    //MessageInterface::ShowMessage("ResourceTree::OnRename() entered\n");


    wxTreeItemId item = GetSelection();
    GmatTreeItemData *selItem = (GmatTreeItemData *) GetItemData(item);
    wxString oldName = selItem->GetDesc();
    int dataType = selItem->GetDataType();


    wxString newName = oldName;
    newName = wxGetTextFromUser(wxT("New name: "), wxT("Input Text"),
                                newName, this);
    if ( !newName.IsEmpty())
    {
        Gmat::ObjectType objType;
        
        switch (dataType)
        {
        case GmatTree::DEFAULT_SPACECRAFT:
        case GmatTree::CREATED_SPACECRAFT:
            objType = Gmat::SPACECRAFT;
            break;
        case GmatTree::DEFAULT_IMPULSIVE_BURN:
        case GmatTree::CREATED_IMPULSIVE_BURN:
            objType = Gmat::BURN;
            break;
        case GmatTree::DEFAULT_DIFF_CORR:
        case GmatTree::CREATED_DIFF_CORR:
            objType = Gmat::SOLVER;
            break;
        case GmatTree::DEFAULT_REPORT_FILE:
        case GmatTree::CREATED_REPORT_FILE:
        case GmatTree::DEFAULT_XY_PLOT:
        case GmatTree::CREATED_XY_PLOT:
        case GmatTree::DEFAULT_OPENGL_PLOT:
        case GmatTree::CREATED_OPENGL_PLOT:
            objType = Gmat::SUBSCRIBER;
            break;
        default:
            objType = Gmat::UNKNOWN_OBJECT;
            MessageInterface::ShowMessage("ResourceTree::OnRename() unknown object.\n");
            break;
        }


        // update item only if successful
        if (theGuiInterpreter->
            RenameConfiguredItem(objType, oldName.c_str(), newName.c_str()))
        {
            SetItemText(item, newName);
            GmatTreeItemData *selItem = (GmatTreeItemData *) GetItemData(item);
            selItem->SetDesc(newName);
            theGuiManager->UpdateSpacecraft();
        }
    }


    //loj: It looks better to change name using EditLabel, but How do I get new name?
    //loj:(void) EditLabel(item);


}

//------------------------------------------------------------------------------
// void OnDelete(wxCommandEvent &event)
//------------------------------------------------------------------------------
void ResourceTree::OnDelete(wxCommandEvent &event)
{
    event.Skip();
}

//------------------------------------------------------------------------------
// void OnBeginLabelEdit(wxTreeEvent &event)
//------------------------------------------------------------------------------
void ResourceTree::OnBeginLabelEdit(wxTreeEvent &event)
{
  
    GmatTreeItemData *selItem = (GmatTreeItemData *)
        GetItemData(event.GetItem());
                               
    int dataType = selItem->GetDataType();
    bool isDefaultFolder = ((dataType == GmatTree::RESOURCES_FOLDER)      ||
                            (dataType == GmatTree::SPACECRAFT_FOLDER)     ||
                            (dataType == GmatTree::FORMATIONS_FOLDER)     ||
                            (dataType == GmatTree::CONSTELLATIONS_FOLDER) ||
                            (dataType == GmatTree::BURNS_FOLDER)          ||
                            (dataType == GmatTree::PROPAGATORS_FOLDER)    ||
                            (dataType == GmatTree::UNIVERSE_FOLDER)       ||
                            (dataType == GmatTree::SOLVERS_FOLDER)        ||
                            (dataType == GmatTree::SUBSCRIBERS_FOLDER)    ||
                            (dataType == GmatTree::INTERFACES_FOLDER)     ||
                            (dataType == GmatTree::SUBSCRIPTS_FOLDER)     ||
                            (dataType == GmatTree::VARIABLES_FOLDER)      ||
                            (dataType == GmatTree::GROUNDSTATIONS_FOLDER));
                         
    bool isDefaultItem = ((dataType == GmatTree::DEFAULT_PROPAGATOR)  ||
                          (dataType == GmatTree::DEFAULT_BODY)        ||
                          (dataType == GmatTree::DEFAULT_DIFF_CORR)   ||
                          (dataType == GmatTree::DEFAULT_REPORT_FILE) ||
                          (dataType == GmatTree::DEFAULT_XY_PLOT)     ||
                          (dataType == GmatTree::DEFAULT_OPENGL_PLOT) ||
                          (dataType == GmatTree::DEFAULT_INTERFACE));

    //kind of redundant because OpenPage returns false for some
    //of the default folders
    if ( (!mainNotebook->OpenPage(selItem)) ||
         (isDefaultFolder)                  ||
         (isDefaultItem))
    {
        event.Veto();
    }
}

//------------------------------------------------------------------------------
// void OnEndLabelEdit(wxTreeEvent &event)
//------------------------------------------------------------------------------
void ResourceTree::OnEndLabelEdit(wxTreeEvent &event)
{
    wxString newLabel = event.GetLabel();
  
    // check to see if label is a single word
    if (newLabel.IsWord())
    {
        GmatTreeItemData *selItem = (GmatTreeItemData *)
            GetItemData(GetSelection());

        wxString oldLabel = selItem->GetDesc();
        int itemType = selItem->GetDataType();

        selItem->SetDesc(newLabel);
                  
        // if label refers to an object reset the object name
        if ((itemType == GmatTree::DEFAULT_SPACECRAFT)  ||
            (itemType == GmatTree::CREATED_SPACECRAFT))
        {
            const std::string stdOldLabel = oldLabel.c_str();
            const std::string stdNewLabel = newLabel.c_str();
     
            Spacecraft *theSpacecraft =
                theGuiInterpreter->GetSpacecraft(stdOldLabel);

            theSpacecraft->SetName(stdNewLabel);

            // if (resetName)
            //     selItem->SetDesc(label);
            //  else
            //     selItem->SetDesc(oldLabel);
      
        }
     
 
    }
    else
    {
        event.Veto();
    }
}

//------------------------------------------------------------------------------
// void AddIcons()
//------------------------------------------------------------------------------
void ResourceTree::AddIcons()
{
    int size = 16;
  
    wxImageList *images = new wxImageList ( size, size, true );
  
    wxBusyCursor wait;
    wxIcon icons[18];

    icons[0] = wxIcon ( folder_xpm );
    icons[1] = wxIcon ( file_xpm );
    icons[2] = wxIcon ( openfolder_xpm );
    icons[3] = wxIcon ( spacecrapt_xpm );
    icons[4] = wxIcon ( sun_xpm );
    icons[5] = wxIcon ( mercury_xpm );
    icons[6] = wxIcon ( venus_xpm );
    icons[7] = wxIcon ( earth_xpm );
    icons[8] = wxIcon ( mars_xpm );
    icons[9] = wxIcon ( jupiter_xpm );
    icons[10] = wxIcon ( saturn_xpm );
    icons[11] = wxIcon ( uranus_xpm );
    icons[12] = wxIcon ( neptune_xpm );
    icons[13] = wxIcon ( pluto_xpm );
    icons[14] = wxIcon ( report_xpm );
    icons[15] = wxIcon ( network_xpm );
    icons[16] = wxIcon ( burn_xpm );
    icons[17] = wxIcon ( moon_xpm );
 
    int sizeOrig = icons[0].GetWidth();
    for ( size_t i = 0; i < WXSIZEOF(icons); i++ )
    {
        if ( size == sizeOrig )
        {
            images->Add(icons[i]);
        }
        else
        {
            images->Add(wxBitmap(wxBitmap(icons[i]).ConvertToImage().Rescale(size, size)));
        }
    }

    AssignImageList(images);

}

//---------------------------------
// Add items to tree
//---------------------------------

//------------------------------------------------------------------------------
// void OnAddBody(wxCommandEvent &event)
//------------------------------------------------------------------------------
void ResourceTree::OnAddBody(wxCommandEvent &event)
{
    wxTreeItemId item = GetSelection();
    AppendItem(item, wxT("New Body"), GmatTree::ICON_EARTH, -1,
               new GmatTreeItemData(wxT("New Body"), GmatTree::CREATED_BODY));
}

//------------------------------------------------------------------------------
// void OnAddSpacecraft(wxCommandEvent &event)
//------------------------------------------------------------------------------
void ResourceTree::OnAddSpacecraft(wxCommandEvent &event)
{
    wxTreeItemId item = GetSelection();
  
    wxString withName;
    withName.Printf("Spacecraft%d", ++mNumSpacecraft);
  
    const std::string stdWithName = withName.c_str();
  
    Spacecraft *theSpacecraft = theGuiInterpreter->CreateSpacecraft("Spacecraft", 
                                                                    stdWithName);

    //  theSpacecraft->SetName("Big Daddy");
  
    wxString newName = wxT(theSpacecraft->GetName().c_str());
  
    AppendItem(item, newName, GmatTree::ICON_SPACECRAFT, -1,
               new GmatTreeItemData(newName, GmatTree::CREATED_SPACECRAFT));

    theGuiManager->UpdateSpacecraft();
  
    Expand(item);
}


//------------------------------------------------------------------------------
// void OnAddFormation(wxCommandEvent &event)
//------------------------------------------------------------------------------
void ResourceTree::OnAddFormation(wxCommandEvent &event)
{
    wxTreeItemId item = GetSelection();
    wxTreeItemId formation =
        AppendItem(item, wxT("New Formation"), -1, -1,
                   new GmatTreeItemData(wxT("New Formation"),
                                        GmatTree::CREATED_FORMATION_FOLDER));

    SetItemImage(formation, GmatTree::ICON_OPENFOLDER, wxTreeItemIcon_Expanded);

    AppendItem(formation, wxT("MMS1"), GmatTree::ICON_SPACECRAFT, -1,
               new GmatTreeItemData(wxT("MMS1"), GmatTree::CREATED_FORMATION_SPACECRAFT));
    AppendItem(formation, wxT("MMS2"), GmatTree::ICON_SPACECRAFT, -1,
               new GmatTreeItemData(wxT("MMS2"), GmatTree::CREATED_FORMATION_SPACECRAFT));
}

//------------------------------------------------------------------------------
// void OnAddConstellation(wxCommandEvent &event)
//------------------------------------------------------------------------------
void ResourceTree::OnAddConstellation(wxCommandEvent &event)
{
    wxTreeItemId item = GetSelection();
    wxTreeItemId constellation =
        AppendItem(item, wxT("New Constellation"), -1, -1,
                   new GmatTreeItemData(wxT("New Constellation"),
                                        GmatTree::CREATED_CONSTELLATION_FOLDER));

    SetItemImage(constellation, GmatTree::ICON_OPENFOLDER, wxTreeItemIcon_Expanded);

    AppendItem(constellation, wxT("GPS1"), GmatTree::ICON_SPACECRAFT, -1,
               new GmatTreeItemData(wxT("GPS1"), GmatTree::CREATED_CONSTELLATION_SATELLITE));
    AppendItem(constellation, wxT("GPS2"), GmatTree::ICON_SPACECRAFT, -1,
               new GmatTreeItemData(wxT("GPS2"), GmatTree::CREATED_CONSTELLATION_SATELLITE));
}

//------------------------------------------------------------------------------
// void OnAddPropagator(wxCommandEvent &event)
//------------------------------------------------------------------------------
void ResourceTree::OnAddPropagator(wxCommandEvent &event)
{
    MessageInterface::ShowMessage("ResourceTree::OnAddPropagator() entered\n");
    wxTreeItemId item = GetSelection();
  
    wxString name;
    name.Printf("Propagator%d", ++mNumPropagator);

    PropSetup *propSetup =
        theGuiInterpreter->CreateDefaultPropSetup(std::string(name.c_str()));

    if (propSetup != NULL)
    {
        AppendItem(item, name, GmatTree::ICON_FILE, -1,
                   new GmatTreeItemData(name, GmatTree::CREATED_PROPAGATOR));

        Expand(item);
    }
    else
    {
        MessageInterface::ShowMessage("ResourceTree::OnAddPropagator() propSetup is NULL\n");
    }
}

//------------------------------------------------------------------------------
// void OnAddImpulsiveBurn(wxCommandEvent &event)
//------------------------------------------------------------------------------
void ResourceTree::OnAddImpulsiveBurn(wxCommandEvent &event)
{
    wxTreeItemId item = GetSelection();
  
    wxString name;
    name.Printf("ImpulsiveBurn%d", ++mNumImpulsiveBurn);
  
    Burn* burn =
        theGuiInterpreter->CreateBurn("ImpulsiveBurn", std::string(name.c_str()));

    if (burn != NULL)
    {
        AppendItem(item, name, GmatTree::ICON_BURN, -1,
                   new GmatTreeItemData(name, GmatTree::CREATED_IMPULSIVE_BURN));

        Expand(item);
    }
    else
    {
        MessageInterface::ShowMessage("ResourceTree::OnAddImpulsiveBurn() burn is NULL\n");
    }
}

//------------------------------------------------------------------------------
// void OnAddDiffCorr(wxCommandEvent &event)
//------------------------------------------------------------------------------
void ResourceTree::OnAddDiffCorr(wxCommandEvent &event)
{
    wxTreeItemId item = GetSelection();
  
    wxString name;
    name.Printf("DC%d", ++mNumDiffCorr);

    Solver *solver =
        theGuiInterpreter->CreateSolver("DifferentialCorrector", std::string(name.c_str()));
   
    AppendItem(item, name, GmatTree::ICON_FILE, -1,
               new GmatTreeItemData(name, GmatTree::CREATED_DIFF_CORR));

    Expand(item);
}

//------------------------------------------------------------------------------
// void OnAddReportFile(wxCommandEvent &event)
//------------------------------------------------------------------------------
void ResourceTree::OnAddReportFile(wxCommandEvent &event)
{
    wxTreeItemId item = GetSelection();
  
    wxString name;
    name.Printf("ReportFile%d", ++mNumReportFile);

    Subscriber* subs =
        theGuiInterpreter->CreateSubscriber("ReportFile", std::string(name.c_str()));
   
    AppendItem(item, name, GmatTree::ICON_REPORT, -1,
               new GmatTreeItemData(name, GmatTree::CREATED_REPORT_FILE));

    Expand(item);
}

//------------------------------------------------------------------------------
// void OnAddXyPlot(wxCommandEvent &event)
//------------------------------------------------------------------------------
void ResourceTree::OnAddXyPlot(wxCommandEvent &event)
{
    wxTreeItemId item = GetSelection();
  
    wxString name;
    name.Printf("XYPlot%d", ++mNumXyPlot);

    Subscriber* subs =
        theGuiInterpreter->CreateSubscriber("XyPlot", std::string(name.c_str()));
   
    AppendItem(item, name, GmatTree::ICON_REPORT, -1,
               new GmatTreeItemData(name, GmatTree::CREATED_XY_PLOT));

    Expand(item);
}

//------------------------------------------------------------------------------
// void OnAddOpenGlPlot(wxCommandEvent &event)
//------------------------------------------------------------------------------
void ResourceTree::OnAddOpenGlPlot(wxCommandEvent &event)
{
    wxTreeItemId item = GetSelection();
  
    wxString name;
    name.Printf("OpenGlPlot%d", ++mNumOpenGlPlot);

    Subscriber* subs =
        theGuiInterpreter->CreateSubscriber("OpenGlPlot", std::string(name.c_str()));
   
    AppendItem(item, name, GmatTree::ICON_REPORT, -1,
               new GmatTreeItemData(name, GmatTree::CREATED_OPENGL_PLOT));

    Expand(item);
}

//------------------------------------------------------------------------------
// void OnAddVariable(wxCommandEvent &event)
//------------------------------------------------------------------------------
void ResourceTree::OnAddVariable(wxCommandEvent &event)
{
    wxTreeItemId item = GetSelection();
    
    // show dialog to create user parameter
    ParameterCreateDialog paramDlg(this);
    paramDlg.ShowModal();

    if (paramDlg.IsParamCreated())
    {
        wxString name = paramDlg.GetParamName();
        AppendItem(item, name, GmatTree::ICON_FILE, -1,
                   new GmatTreeItemData(name, GmatTree::CREATED_VARIABLE));
        
        Expand(item);
    }
}

//---------------------------------
// Crete popup menu
//---------------------------------

//------------------------------------------------------------------------------
// wxMenu* CreatePopupMenu(Gmat::ObjectType type)
//------------------------------------------------------------------------------
wxMenu* ResourceTree::CreatePopupMenu(Gmat::ObjectType type)
{
    unsigned int i;
    wxMenu *menu = new wxMenu;
    StringArray items = theGuiInterpreter->GetListOfFactoryItems(type);

    switch (type)
    {
    case Gmat::BURN:
        for (i=0; i<items.size(); i++)
        {        
            if (items[i] == "ImpulsiveBurn")
            {
                menu->Append(POPUP_ADD_IMPULSIVE_BURN, wxT("ImpulsiveBurn"));
            }
        }
        break;
    case Gmat::SOLVER:
        for (i=0; i<items.size(); i++)
        {        
            if (items[i] == "DifferentialCorrector")
            {
                menu->Append(POPUP_ADD_DIFF_CORR, wxT("DifferentialCorrector"));
            }
        }
        break;
    case Gmat::SUBSCRIBER:
        for (i=0; i<items.size(); i++)
        {        
            if (items[i] == "ReportFile")
            {
                menu->Append(POPUP_ADD_REPORT_FILE, wxT("ReportFile"));
            }
            else if (items[i] == "XyPlot")
            {
                menu->Append(POPUP_ADD_XY_PLOT, wxT("XYPlot"));
            }
            else if (items[i] == "OpenGlPlot")
            {
                menu->Append(POPUP_ADD_OPENGL_PLOT, wxT("OpenGLPlot"));
            }
        }
        break;
    default:
        break;
    }
    
    return menu;
}
