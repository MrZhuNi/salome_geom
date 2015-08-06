#ifndef _ShapeSchema_gp_Circ2d_HeaderFile
#include <ShapeSchema_gp_Circ2d.hxx>
#endif
#ifndef _gp_Circ2d_HeaderFile
#include <gp_Circ2d.hxx>
#endif
#include <ShapeSchema_gp_Circ2d.ixx>
#ifndef _Storage_Schema_HeaderFile
#include <Storage_Schema.hxx>
#endif
#ifndef _Storage_stCONSTclCOM_HeaderFile
#include <Storage_stCONSTclCOM.hxx>
#endif

void ShapeSchema_gp_Circ2d::SWrite(const gp_Circ2d& pp, Storage_BaseDriver& f, const Handle(Storage_Schema)& theSchema)
{
  (void)pp; (void)theSchema; // dummy to avoid compiler warning on unused arg
  f.BeginWriteObjectData();
    ShapeSchema_gp_Ax22d::SWrite(pp.Axis(),f,theSchema);
  f.PutReal(pp.Radius());

  f.EndWriteObjectData();
}

void ShapeSchema_gp_Circ2d::SRead(gp_Circ2d& pp, Storage_BaseDriver& f, const Handle(Storage_Schema)& theSchema)
{
  (void)pp; (void)theSchema; // dummy to avoid compiler warning on unused arg
  f.BeginReadObjectData();

    ShapeSchema_gp_Ax22d::SRead((gp_Ax22d&)pp.Axis(),f,theSchema);

    Standard_Real gp_Circ2dradius;
    f.GetReal(gp_Circ2dradius);
    pp.SetRadius(gp_Circ2dradius);

  f.EndReadObjectData();
}
