//$Header$
//------------------------------------------------------------------------------
//                              XyPlotCurve
//------------------------------------------------------------------------------
// GMAT: Goddard Mission Analysis Tool
//
// ** Legal **
//
// Author: Linda Jun
// Created: 2004/01/20
/**
 * Declares XyPlotCurve for XY plot.
 */
//------------------------------------------------------------------------------
#include "XyPlotCurve.hpp"
#include "LinearInterpolator.hpp"
#include "MessageInterface.hpp"
#include <algorithm>              //for min_element, max_element

#define DEBUG_XY_PLOT_CURVE_ADD 0
#define DEBUG_XY_PLOT_CURVE_GET 0
#define DEBUG_XY_PLOT_CURVE_MAX 0

//------------------------------------------------------------------------------
// XyPlotCurve(int offsetY, double startY, double endY, double defaultY)
//------------------------------------------------------------------------------
XyPlotCurve::XyPlotCurve(int offsetY, double startY, double endY,
                         const wxString &curveTitle)
   : wxPlotCurve(offsetY, startY, endY, curveTitle)
{
   mInterp = new LinearInterpolator(); //loj: should I allow user to use his own?
   mXdata.clear();
   mYdata.clear();
}

//------------------------------------------------------------------------------
// ~XyPlotCurve()
//------------------------------------------------------------------------------
XyPlotCurve::~XyPlotCurve()
{
   delete mInterp;
}

//------------------------------------------------------------------------------
// double GetFirstX()
//------------------------------------------------------------------------------
double XyPlotCurve::GetFirstX()
{
   return mFirstX;
}

//------------------------------------------------------------------------------
// void SetFirstX(double x)
//------------------------------------------------------------------------------
void XyPlotCurve::SetFirstX(double x)
{
   mFirstX = x;
}

//------------------------------------------------------------------------------
// void AddData(double x, double y)
//------------------------------------------------------------------------------
void XyPlotCurve::AddData(double x, double y)
{
   //---------------------------------------------
   //Notes:
   // loj: for build2 x has to be time
   //      save a1mjd time as (21545xxx) for build 2,
   //      X has to be integer
   //---------------------------------------------
   wxInt32 intTime = (wxInt32)(x * 1000); 
    
   mXdata.push_back(intTime);
   mYdata.push_back(y);

#if DEBUG_XY_PLOT_CURVE_ADD
   MessageInterface::ShowMessage
      ("XyPlotCurve::AddData() size = %d, x = %d, y = %f\n",
       mXdata.size(), mXdata.back(), mYdata.back());
#endif
}

//------------------------------------------------------------------------------
// void SetInterpolator(Interpolator *interp)
//------------------------------------------------------------------------------
void XyPlotCurve::SetInterpolator(Interpolator *interp)
{
   if (mInterp == NULL)
      mInterp = interp;
}

//------------------------------------
// methods inherited from wxPlotCurve
//------------------------------------

//------------------------------------------------------------------------------
// virtual wxInt32 GetStartX()
//------------------------------------------------------------------------------
wxInt32 XyPlotCurve::GetStartX()
{
   if (mXdata.size() == 0)
      return 0;
   else
      return mXdata.front();
}

//------------------------------------------------------------------------------
// virtual wxInt32 GetEndX()
//------------------------------------------------------------------------------
wxInt32 XyPlotCurve::GetEndX()
{
   if (mXdata.size() == 0)
      return 0;
   else
      return mXdata.back();
}

//------------------------------------------------------------------------------
// virtual double GetY(wxInt32 x)
//------------------------------------------------------------------------------
double XyPlotCurve::GetY(wxInt32 x)
{
   double yVal = 0.0; // should be undefined?
   unsigned int i;
    
   for (i=0; i<mXdata.size(); i++)
   {
      if (mXdata[i] == x)
      {
         yVal = mYdata[i];
         break;
      }
      else if (mXdata[i] > x)
      {
         // interpolate
         mInterp->Clear();
         mInterp->AddPoint(mXdata[i-1], &mYdata[i-1]);
         mInterp->AddPoint(mXdata[i], &mYdata[i]);
         mInterp->Interpolate(x, &yVal);
         break;
      }
   }
    
#if DEBUG_XY_PLOT_CURVE_GET
   MessageInterface::ShowMessage
      ("XyPlotCurve::GetY() size = %d, x = %d, y = %f\n",
       mXdata.size(), x, yVal);
#endif
   
   return yVal;
}

//loj: 3/10/04 added
//------------------------------------------------------------------------------
// virtual void ClearData()
//------------------------------------------------------------------------------
void XyPlotCurve::ClearData()
{
   mXdata.clear();
   mYdata.clear();
}

//loj: 7/23/04 added
//------------------------------------------------------------------------------
// virtual double GetYMin()
//------------------------------------------------------------------------------
double XyPlotCurve::GetYMin()
{
   std::vector<double>::iterator pos;
   pos = min_element (mYdata.begin(), mYdata.end());

   return *pos;
}

//loj: 7/23/04 added
//------------------------------------------------------------------------------
// virtual double GetYMax()
//------------------------------------------------------------------------------
double XyPlotCurve::GetYMax()
{
   std::vector<double>::iterator pos;
   pos = max_element (mYdata.begin(), mYdata.end());

#if DEBUG_XY_PLOT_CURVE_MAX
   MessageInterface::ShowMessage
      ("XyPlotCurve::GetYMax() size = %d, max=%f\n",
       mYdata.size(), *pos);
#endif
   
   return *pos;
}

