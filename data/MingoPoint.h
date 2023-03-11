// -----------------------------------------------------------------
// -----              MingoPoint header file                   -----
// -----          Created 11/03/2023 by J.L. Rodriguez         -----
// -----------------------------------------------------------------

#ifndef MingoPoint_H
#define MingoPoint_H 1

#include "FairMCPoint.h"

#include "TObject.h"
#include "TVector3.h"
#include <stdint.h>

class MingoPoint : public FairMCPoint
{
  public:
    /** Default constructor **/
    MingoPoint();

    /** Constructor with arguments
     *@param trackID  Index of MCTrack
     *@param barID    Barrel ID
     *@param sensorID Sensor ID
     *@param posIn    Coordinates at entrance to active volume [cm]
     *@param posOut   Coordinates at exit of active volume [cm]
     *@param momIn    Momentum of track at entrance [GeV]
     *@param momOut   Momentum of track at exit [GeV]
     *@param tof      Time since event start [ns]
     *@param length   Track length since creation [cm]
     *@param eLoss    Energy deposit [GeV]
     *@param pid      Particle ID
     **/
    MingoPoint(Int_t trackID,
                   Int_t barID,
                   Int_t sensorID,
                   TVector3 posIn,
                   TVector3 posOut,
                   TVector3 momIn,
                   TVector3 momOut,
                   Double_t tof,
                   Double_t length,
                   Double_t eLoss,
                   Int_t pid);

    /** Destructor **/
    virtual ~MingoPoint();

    /** Accessors **/
    Int_t GetBarrelID() const { return fDetectorID; }
    Int_t GetSensorID() const { return fSensorID; }
    Double_t GetXIn() const { return fX; }
    Double_t GetYIn() const { return fY; }
    Int_t GetPid() const { return fPid; }
    Double_t GetZIn() const { return fZ; }
    Double_t GetXOut() const { return fX_out; }
    Double_t GetYOut() const { return fY_out; }
    Double_t GetZOut() const { return fZ_out; }
    Double_t GetPxOut() const { return fPx_out; }
    Double_t GetPyOut() const { return fPy_out; }
    Double_t GetPzOut() const { return fPz_out; }
    inline Double_t GetTheta() const
    {
        TVector3 v;
        v.SetXYZ(fX, fY, fZ);
        return v.Theta();
    }
    inline Double_t GetPhi() const
    {
        TVector3 v;
        v.SetXYZ(fX, fY, fZ);
        return v.Phi();
    }

    void PositionIn(TVector3& pos) { pos.SetXYZ(fX, fY, fZ); }
    void PositionOut(TVector3& pos) { pos.SetXYZ(fX_out, fY_out, fZ_out); }
    void MomentumOut(TVector3& mom) { mom.SetXYZ(fPx_out, fPy_out, fPz_out); }

    /** Point coordinates at given z from linear extrapolation **/
    Double_t GetX(Double_t z) const;
    Double_t GetY(Double_t z) const;

    /** Modifiers **/
    void SetPositionOut(TVector3 pos);
    void SetMomentumOut(TVector3 mom);
    void SetBarrelID(Int_t id) { fDetectorID = id; };
    void SetSensorID(Int_t id) { fSensorID = id; };

  private:
    Int_t fSensorID, fPid;
    Double32_t fX_out, fY_out, fZ_out, fEloss;
    Double32_t fPx_out, fPy_out, fPz_out;

    ClassDef(MingoPoint, 1)
};

inline void MingoPoint::SetPositionOut(TVector3 pos)
{
    fX_out = pos.X();
    fY_out = pos.Y();
    fZ_out = pos.Z();
}

inline void MingoPoint::SetMomentumOut(TVector3 mom)
{
    fPx_out = mom.Px();
    fPy_out = mom.Py();
    fPz_out = mom.Pz();
}

#endif /* MingoPoint_H */
