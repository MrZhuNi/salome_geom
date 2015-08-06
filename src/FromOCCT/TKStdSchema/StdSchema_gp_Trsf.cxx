#ifndef _StdSchema_gp_Trsf_HeaderFile
#include <StdSchema_gp_Trsf.hxx>
#endif
#ifndef _gp_Trsf_HeaderFile
#include <gp_Trsf.hxx>
#endif
#include <StdSchema_gp_Trsf.ixx>
#ifndef _Storage_Schema_HeaderFile
#include <Storage_Schema.hxx>
#endif
#ifndef _Storage_stCONSTclCOM_HeaderFile
#include <Storage_stCONSTclCOM.hxx>
#endif

void StdSchema_gp_Trsf::SWrite(const gp_Trsf& pp, Storage_BaseDriver& f, const Handle(Storage_Schema)& theSchema)
{
  (void)pp; (void)theSchema; // dummy to avoid compiler warning on unused arg
  f.BeginWriteObjectData();
  f.PutReal(pp.ScaleFactor());
  f.PutInteger(pp.Form());
    StdSchema_gp_Mat::SWrite(pp.HVectorialPart(),f,theSchema);
    StdSchema_gp_XYZ::SWrite(pp.TranslationPart(),f,theSchema);

  f.EndWriteObjectData();
}

void StdSchema_gp_Trsf::SRead(gp_Trsf& pp, Storage_BaseDriver& f, const Handle(Storage_Schema)& theSchema)
{
  (void)pp; (void)theSchema; // dummy to avoid compiler warning on unused arg
  f.BeginReadObjectData();

    Standard_Real gp_Trsfscale;
    f.GetReal(gp_Trsfscale);
    pp.SetScaleFactor(gp_Trsfscale);

    gp_TrsfForm gp_Trsfshape;
    Standard_Integer i_CSFDB_gp_Trsfshape;
    f.GetInteger(i_CSFDB_gp_Trsfshape);
    gp_Trsfshape = (gp_TrsfForm)i_CSFDB_gp_Trsfshape;
    pp.SetForm(gp_Trsfshape);

    StdSchema_gp_Mat::SRead((gp_Mat&)pp.HVectorialPart(),f,theSchema);

    StdSchema_gp_XYZ::SRead((gp_XYZ&)pp.TranslationPart(),f,theSchema);

  f.EndReadObjectData();
}
