//  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
//
//  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com

#include <Standard_Stream.hxx>

#include <GEOMImpl_SmoothingSurfaceDriver.hxx>
#include <GEOMImpl_ISmoothingSurface.hxx>
#include <GEOMImpl_Types.hxx>
#include <GEOM_Function.hxx>

#include <TFunction_Logbook.hxx>
#include <StdFail_NotDone.hxx>

//@@ include required header files here @@//
#include <TopoDS_Vertex.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Compound.hxx>
#include <TopoDS.hxx>

#include <TColgp_SequenceOfPnt.hxx>
#include <TColgp_Array2OfPnt.hxx>
#include <TColgp_SequenceOfXY.hxx>
#include <TColgp_SequenceOfXYZ.hxx>

#include <BRepAdaptor_HSurface.hxx>

#include <BRep_Builder.hxx>
#include <BRepGProp.hxx>
#include <BRep_Tool.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>

#include <GeomPlate_Surface.hxx>
#include <GeomPlate_BuildPlateSurface.hxx>
#include <GeomPlate_PointConstraint.hxx>
#include <GeomPlate_MakeApprox.hxx>
#include <GeomPlate_PlateG0Criterion.hxx>

#include <Geom_BSplineSurface.hxx>
#include <Geom_Surface.hxx>

#include <GProp_GProps.hxx>
#include <Bnd_Box.hxx>
#include <BRepBndLib.hxx>
#include <gp_Pnt.hxx>

#include <GC_MakePlane.hxx>
//=======================================================================
//function : GetID
//purpose  :
//=======================================================================
const Standard_GUID& GEOMImpl_SmoothingSurfaceDriver::GetID()
{
  static Standard_GUID aGUID("1C3A0F3F-729D-4E83-8232-78E74FC5637C");
  return aGUID;
}

//=======================================================================
//function : GEOMImpl_SmoothingSurfaceDriver
//purpose  :
//=======================================================================
GEOMImpl_SmoothingSurfaceDriver::GEOMImpl_SmoothingSurfaceDriver()
{
}

//=======================================================================
//function : Execute
//purpose  :
//=======================================================================
Standard_Integer GEOMImpl_SmoothingSurfaceDriver::Execute(TFunction_Logbook& log) const
{
  if (Label().IsNull()) return 0;
  Handle(GEOM_Function) aFunction = GEOM_Function::GetFunction(Label());
  if (aFunction.IsNull()) return 0;

  Standard_Integer aType = aFunction->GetType();
  cout << "Type : " << aType << endl;

  if (aType != SMOOTHINGSURFACE_POINTS_ISCLOSED) return 0;
  cout << "Youhou : " << aType << endl;

  GEOMImpl_ISmoothingSurface aData (aFunction);
  
  bool isClosed = aData.GetisClosed();
  int nbPoints = aData.GetLength();

  TopoDS_Shape aShape;
  TopoDS_Face aInitShape;
  
  GeomPlate_BuildPlateSurface aBuilder(3,10);

  // ** Creation of compound
  BRep_Builder aB;
  TopoDS_Compound aComp;
  aB.MakeCompound(aComp);
  for (int ind=1;ind<=nbPoints;ind++)
  {
    Handle(GEOM_Function) aPoint = aData.GetPoint(ind);
    TopoDS_Shape aShapePnt = aPoint->GetValue();
    aB.Add(aComp,aShapePnt);
  }

  Bnd_Box BoundingBox;
  Standard_Real aXmin, aYmin, aZmin, aXmax, aYmax, aZmax;
  BRepBndLib::Add(aComp, BoundingBox);
  BoundingBox.Get(aXmin, aYmin, aZmin, aXmax, aYmax, aZmax);

  cout << "test isClosed" << endl;
  if (isClosed) 
  {   
    // Initial surface : Sphere
    // ** Retrieve center of mass
    GProp_GProps aSystem;
    BRepGProp::SurfaceProperties(aComp, aSystem);
    gp_Pnt aCenterMass = aSystem.CentreOfMass();

    // ** Computation of radius
    Standard_Real aRMax = aXmax-aXmin;
    if (aRMax < aYmax-aYmin )
      aRMax = aYmax-aYmin;
    if (aRMax < aZmax-aZmin )
      aRMax = aZmax-aZmin;
    // ** Creation of sphere
    aInitShape = BRepPrimAPI_MakeSphere(aCenterMass, aRMax).Face();
  cout << "Closed " << endl;
  }
  else 
  {
    // Initial surface : Plane
    Standard_Real aX = aXmax-aXmin;
    Standard_Real aY = aYmax-aYmin;
    Standard_Real aZ = aZmax-aZmin;
    gp_Pnt aPnt1;
    gp_Pnt aPnt2;
    gp_Pnt aPnt3;
    if (aX > aY)
    {
      if (aZ > aY)
      {
        // Y Plane
        cout << "cas 1 " << endl;
        aPnt1 = gp_Pnt(aXmin, aYmin+aY/2, aZmin);
        aPnt2 = gp_Pnt(aXmin, aYmin+aY/2, aZmax);
        aPnt3 = gp_Pnt(aXmax, aYmin+aY/2, aZmin);
      }
      else
      {
        // Z Plane
        cout << "cas 2 " << endl;
        aPnt1 = gp_Pnt(aXmin, aYmin, aZmin+aZ/2);
        aPnt2 = gp_Pnt(aXmin, aYmax, aZmin+aZ/2);
        aPnt3 = gp_Pnt(aXmax, aYmin, aZmin+aZ/2);
      }
    }
    else 
    {
      // aY > aX
      if (aZ > aX)
      {
        // X Plane
        cout << "cas 3 " << endl;
        aPnt1 = gp_Pnt(aXmin+aX/2, aYmin, aZmin);
        aPnt2 = gp_Pnt(aXmin+aX/2, aYmin, aZmax);
        aPnt3 = gp_Pnt(aXmin+aX/2, aYmax, aZmin);
      }
      else
      {
        // Z Plane
        cout << "cas 4 " << endl;
        aPnt1 = gp_Pnt(aXmin, aYmin, aZmin+aZ/2);
        aPnt2 = gp_Pnt(aXmin, aYmax, aZmin+aZ/2);
        aPnt3 = gp_Pnt(aXmax, aYmin, aZmin+aZ/2);
      }
    }
    GC_MakePlane aMakePlane (aPnt1, aPnt2, aPnt3);
  cout << "Not closed " << endl;
    double aSize = (aX+aY+aZ)/2;
#if OCC_VERSION_LARGE > 0x06050100 // for OCC-6.5.2 and higher version
    aInitShape = BRepBuilderAPI_MakeFace(aMakePlane, -aSize, aSize, -aSize, aSize, Precision::Confusion()).Face();
#else
    aInitShape = BRepBuilderAPI_MakeFace(aMakePlane, -aSize, aSize, -aSize, aSize).Face();
#endif
  }

  // ** Initialization of surface
  Handle(BRepAdaptor_HSurface) HSI = new BRepAdaptor_HSurface();
  HSI->ChangeSurface().Initialize(aInitShape);
  aBuilder.LoadInitSurface( BRep_Tool::Surface(HSI->ChangeSurface().Face()));

  cout << "Init surface" << endl;
  for (int i=1; i<=nbPoints ; i++)
  {
    Handle(GEOM_Function) aPoint = aData.GetPoint(i);
    TopoDS_Shape aShapePnt = aPoint->GetValue();
    TopoDS_Vertex aVertexPnt = TopoDS::Vertex(aShapePnt);
    gp_Pnt aPnt = BRep_Tool::Pnt( aVertexPnt ); 
    Handle(GeomPlate_PointConstraint) PCont= new GeomPlate_PointConstraint(aPnt,0);
    aBuilder.Add(PCont);
  }
  cout << "avant Perform surface" << endl;
  aBuilder.Perform();
  cout << "Perform surface" << endl;
  
  // A ce niveau : surface algo
  Handle(GeomPlate_Surface) gpPlate = aBuilder.Surface();
  
  Standard_Integer nbcarreau=9;
  Standard_Integer degmax=8;
  Standard_Real seuil;
  /*TColgp_SequenceOfXY S2d;
  TColgp_SequenceOfXYZ S3d;
  S2d.Clear();
  S3d.Clear();
  aBuilder.Disc2dContour(4,S2d);
  aBuilder.Disc3dContour(4,0,S3d);*/
  seuil = Max(0.0001,10*aBuilder.G0Error());
  //GeomPlate_PlateG0Criterion critere (S2d,S3d,seuil);
  GeomPlate_MakeApprox Mapp(gpPlate,0.0001,nbcarreau,degmax,seuil);
  cout << "Approx surface" << endl;

  Handle (Geom_Surface) Surf (Mapp.Surface());

  Standard_Real Umin, Umax, Vmin, Vmax;
  
  aBuilder.Surface()->Bounds( Umin, Umax, Vmin, Vmax);
  
  BRepBuilderAPI_MakeFace MF(Surf,Umin, Umax, Vmin, Vmax);
  aShape = MF.Shape();
   cout << "RES surface" << endl;
 
  if (aShape.IsNull()) return 0;

  aFunction->SetValue(aShape);

  log.SetTouched(Label());

  return 1;
}

//=======================================================================
//function :  GEOMImpl_SmoothingSurfaceDriver_Type_
//purpose  :
//=======================================================================
Standard_EXPORT Handle_Standard_Type& GEOMImpl_SmoothingSurfaceDriver_Type_()
{
  static Handle_Standard_Type aType1 = STANDARD_TYPE(TFunction_Driver);
  if ( aType1.IsNull()) aType1 = STANDARD_TYPE(TFunction_Driver);
  static Handle_Standard_Type aType2 = STANDARD_TYPE(MMgt_TShared);
  if ( aType2.IsNull()) aType2 = STANDARD_TYPE(MMgt_TShared);
  static Handle_Standard_Type aType3 = STANDARD_TYPE(Standard_Transient);
  if ( aType3.IsNull()) aType3 = STANDARD_TYPE(Standard_Transient);

  static Handle_Standard_Transient _Ancestors[]= {aType1,aType2,aType3,NULL};
  static Handle_Standard_Type _aType = new Standard_Type("GEOMImpl_SmoothingSurfaceDriver",
                                                         sizeof(GEOMImpl_SmoothingSurfaceDriver),
                                                         1,
                                                         (Standard_Address)_Ancestors,
                                                         (Standard_Address)NULL);
  return _aType;
}

//=======================================================================
//function : DownCast
//purpose  :
//=======================================================================
const Handle(GEOMImpl_SmoothingSurfaceDriver) Handle(GEOMImpl_SmoothingSurfaceDriver)::DownCast(const Handle(Standard_Transient)& AnObject)
{
  Handle(GEOMImpl_SmoothingSurfaceDriver) _anOtherObject;

  if (!AnObject.IsNull()) {
     if (AnObject->IsKind(STANDARD_TYPE(GEOMImpl_SmoothingSurfaceDriver))) {
       _anOtherObject = Handle(GEOMImpl_SmoothingSurfaceDriver)((Handle(GEOMImpl_SmoothingSurfaceDriver)&)AnObject);
     }
  }

  return _anOtherObject;
}
