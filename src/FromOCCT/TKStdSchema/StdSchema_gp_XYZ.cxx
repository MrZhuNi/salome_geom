#ifndef _StdSchema_gp_XYZ_HeaderFile
#include <StdSchema_gp_XYZ.hxx>
#endif
#ifndef _gp_XYZ_HeaderFile
#include <gp_XYZ.hxx>
#endif
#include <StdSchema_gp_XYZ.ixx>
#ifndef _Storage_Schema_HeaderFile
#include <Storage_Schema.hxx>
#endif
#ifndef _Storage_stCONSTclCOM_HeaderFile
#include <Storage_stCONSTclCOM.hxx>
#endif

void StdSchema_gp_XYZ::SWrite(const gp_XYZ& pp, Storage_BaseDriver& f, const Handle(Storage_Schema)& theSchema)
{
  (void)pp; (void)theSchema; // dummy to avoid compiler warning on unused arg
  f.BeginWriteObjectData();
  f.PutReal(pp.X());
  f.PutReal(pp.Y());
  f.PutReal(pp.Z());

  f.EndWriteObjectData();
}

void StdSchema_gp_XYZ::SRead(gp_XYZ& pp, Storage_BaseDriver& f, const Handle(Storage_Schema)& theSchema)
{
  (void)pp; (void)theSchema; // dummy to avoid compiler warning on unused arg
  f.BeginReadObjectData();

    Standard_Real gp_XYZx;
    f.GetReal(gp_XYZx);
    pp.SetX(gp_XYZx);

    Standard_Real gp_XYZy;
    f.GetReal(gp_XYZy);
    pp.SetY(gp_XYZy);

    Standard_Real gp_XYZz;
    f.GetReal(gp_XYZz);
    pp.SetZ(gp_XYZz);

  f.EndReadObjectData();
}
