// -------------------------------------------------------------------------
// -----                      MingoPoint source file                   -----
// -------------------------------------------------------------------------

#include "MingoPoint.h"

// -----   Default constructor   -------------------------------------------
MingoPoint::MingoPoint()
    : FairMCPoint()
{
    fSensorID = 0;
    fPid = 0;
    fX_out = fY_out = fZ_out = fEloss = 0.;
    fPx_out = fPy_out = fPz_out = 0.;
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
MingoPoint::MingoPoint(Int_t trackID,
                               Int_t detID,
                               Int_t sensorID,
                               TVector3 posIn,
                               TVector3 posOut,
                               TVector3 momIn,
                               TVector3 momOut,
                               Double_t tof,
                               Double_t length,
                               Double_t eLoss,
                               Int_t PId)
    : FairMCPoint(trackID, detID, posIn, momIn, tof, length, eLoss)
{
    fSensorID = sensorID;
    fPid = PId;
    fX_out = posOut.X();
    fY_out = posOut.Y();
    fZ_out = posOut.Z();
    fEloss = eLoss;
    fPx_out = momOut.Px();
    fPy_out = momOut.Py();
    fPz_out = momOut.Pz();
}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
MingoPoint::~MingoPoint() {}
// -------------------------------------------------------------------------

// -----   Point x coordinate from linear extrapolation   ------------------
Double_t MingoPoint::GetX(Double_t z) const
{
    if ((fZ_out - z) * (fZ - z) >= 0.)
        return (fX_out + fX) / 2.;
    Double_t dz = fZ_out - fZ;
    return (fX + (z - fZ) / dz * (fX_out - fX));
}
// -------------------------------------------------------------------------

// -----   Point y coordinate from linear extrapolation   ------------------
Double_t MingoPoint::GetY(Double_t z) const
{
    if ((fZ_out - z) * (fZ - z) >= 0.)
        return (fY_out + fY) / 2.;
    Double_t dz = fZ_out - fZ;
    return (fY + (z - fZ) / dz * (fY_out - fY));
}
// -------------------------------------------------------------------------

ClassImp(MingoPoint);
