#ifndef _ShapeSchema_gp_Trsf2d_HeaderFile
#include <ShapeSchema_gp_Trsf2d.hxx>
#endif
#ifndef _gp_Trsf2d_HeaderFile
#include <gp_Trsf2d.hxx>
#endif
#include <ShapeSchema_gp_Trsf2d.ixx>
#ifndef _Storage_Schema_HeaderFile
#include <Storage_Schema.hxx>
#endif
#ifndef _Storage_stCONSTclCOM_HeaderFile
#include <Storage_stCONSTclCOM.hxx>
#endif

void ShapeSchema_gp_Trsf2d::SWrite(const gp_Trsf2d& pp, Storage_BaseDriver& f, const Handle(Storage_Schema)& theSchema)
{
  (void)pp; (void)theSchema; // dummy to avoid compiler warning on unused arg
  f.BeginWriteObjectData();
  f.PutReal(pp.ScaleFactor());
  f.PutInteger(pp.Form());
    ShapeSchema_gp_Mat2d::SWrite(pp.HVectorialPart(),f,theSchema);
    ShapeSchema_gp_XY::SWrite(pp.TranslationPart(),f,theSchema);

  f.EndWriteObjectData();
}

void ShapeSchema_gp_Trsf2d::SRead(gp_Trsf2d& pp, Storage_BaseDriver& f, const Handle(Storage_Schema)& theSchema)
{
  (void)pp; (void)theSchema; // dummy to avoid compiler warning on unused arg
  f.BeginReadObjectData();

    Standard_Real gp_Trsf2dscale;
    f.GetReal(gp_Trsf2dscale);
    pp.SetScaleFactor(gp_Trsf2dscale);

    gp_TrsfForm gp_Trsf2dshape;
    Standard_Integer i_CSFDB_gp_Trsf2dshape;
    f.GetInteger(i_CSFDB_gp_Trsf2dshape);
    gp_Trsf2dshape = (gp_TrsfForm)i_CSFDB_gp_Trsf2dshape;
    //pp._CSFDB_Setgp_Trsf2dshape(gp_Trsf2dshape);

    ShapeSchema_gp_Mat2d::SRead((gp_Mat2d&)pp.HVectorialPart(),f,theSchema);

    ShapeSchema_gp_XY::SRead((gp_XY&)pp.TranslationPart(),f,theSchema);

  f.EndReadObjectData();
}
