//  GEOM OBJECT : interactive object for Geometry entities visualization
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
//  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
//
//
//
//  File   : GEOM_Actor.h
//  Author : Christophe ATTANASIO
//  Module : GEOM
//  $Header$

#ifndef GEOM_ACTOR_H
#define GEOM_ACTOR_H

#include "SALOME_Actor.h"

//OpenCASCADE
#include <TopoDS_Shape.hxx>
#include <TopoDS.hxx>


class TopoDS_Shape;

#ifdef _WIN_32
#define VTKOCC_EXPORT __declspec (dllexport)
#else
#define VTKOCC_EXPORT
#endif
class VTKOCC_EXPORT GEOM_Actor : public SALOME_Actor {


 public:
  vtkTypeMacro(GEOM_Actor,SALOME_Actor);

  static GEOM_Actor* New();

  // Description:
  // This causes the actor to be rendered. It, in turn, will render the actor's
  // property and then mapper.
  virtual void Render(vtkRenderer *, vtkMapper *);

  // Description:
  // Release any graphics resources that are being consumed by this actor.
  // The parameter window could be used to determine which graphic
  // resources to release.
  void ReleaseGraphicsResources(vtkWindow *);

  const TopoDS_Shape& getTopo();
  void setInputShape(const TopoDS_Shape& ashape,double adef1,int imode);

  double getDeflection();
  void setDeflection(double adefl);

  // SubShape
  void SubShapeOn();
  void SubShapeOff();
  
  // Display Mode
  void setDisplayMode(int);

  // Highlight
  void highlight(Standard_Boolean highlight);
  bool hasHighlight();

  void ShallowCopy(vtkProp *prop);

  // Properties
  void SetHighlightProperty(vtkProperty* Prop);
  void SetWireframeProperty(vtkProperty* Prop);
  void SetShadingProperty(vtkProperty* Prop);

  // Opacity
  void SetOpacity(float opa);
  float GetOpacity();

  // Color
  void SetColor(float r,float g,float b);
  void GetColor(float& r,float& g,float& b);
  
 protected:

  GEOM_Actor();
  ~GEOM_Actor();
  GEOM_Actor(const GEOM_Actor&) {};
  void operator=(const GEOM_Actor&) {};

  void CreateWireframeMapper();
  void CreateShadingMapper();
  void CreateMapper(int theMode);
  void GetMatrix(vtkCamera* theCam, vtkMatrix4x4 *result);


 private:

  bool subshape;

  TopoDS_Shape myShape;
  double deflection;

  vtkMapper* ShadingMapper;
  vtkMapper* WireframeMapper;

  vtkProperty* ShadingProperty;
  vtkProperty* WireframeProperty;
  vtkProperty* HighlightProperty;

};
#endif //GEOM_ACTOR_H
