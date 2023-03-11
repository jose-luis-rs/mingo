// ---------------------------------------------------------------
// -----                        Mingo                        -----
// -----          Created 11/03/2023 by J.L. Rodriguez       -----
// ---------------------------------------------------------------

#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairVolume.h"

#include "Mingo.h"
#include "R3BAlpidePoint.h"
#include "R3BLogger.h"
#include "R3BMCStack.h"

#include "TClonesArray.h"
#include "TGeoManager.h"
#include "TVirtualMC.h"

Mingo::Mingo()
    : Mingo("")
{
}

Mingo::Mingo(const TString& geoFile, const TGeoTranslation& trans, const TGeoRotation& rot)
    : Mingo(geoFile, { trans, rot })
{
}

Mingo::Mingo(const TString& geoFile, const TGeoCombiTrans& combi)
    : R3BDetector("Mingo", kTRA, geoFile, combi)
    , fMingoPoint(new TClonesArray("R3BAlpidePoint"))
{
    ResetParameters();
}

Mingo::~Mingo()
{
    R3BLOG(debug1, "");
    if (fMingoPoint)
    {
        fMingoPoint->Delete();
        delete fMingoPoint;
    }
}

void Mingo::Initialize()
{
    FairDetector::Initialize();

    R3BLOG(info, " ");
    R3BLOG(info, "Sens. Vol. (McId) " << TVirtualMC::GetMC()->VolId("Plane"));
}

// -----   Public method ProcessHits  --------------------------------------
Bool_t Mingo::ProcessHits(FairVolume* vol)
{
    if (TVirtualMC::GetMC()->IsTrackEntering())
    {
        gGeoManager->cd(gMC->CurrentVolPath());
        fELoss = 0.;
        fTime = TVirtualMC::GetMC()->TrackTime() * 1.0e09;
        fLength = TVirtualMC::GetMC()->TrackLength();
        TVirtualMC::GetMC()->TrackPosition(fPosIn);
        TVirtualMC::GetMC()->TrackMomentum(fMomIn);
    }

    // Sum energy loss for all steps in the active volume
    fELoss += TVirtualMC::GetMC()->Edep();

    // Set additional parameters at exit of active volume. Create MingoPoint.
    if (TVirtualMC::GetMC()->IsTrackExiting() || TVirtualMC::GetMC()->IsTrackStop() ||
        TVirtualMC::GetMC()->IsTrackDisappeared())
    {
        fTrackID = TVirtualMC::GetMC()->GetStack()->GetCurrentTrackNumber();
        R3BLOG(debug, TVirtualMC::GetMC()->CurrentVolPath());
        
        fBarrelID = vol->getMCid();
        fSensorID = vol->getCopyNo();

        TVirtualMC::GetMC()->TrackPosition(fPosOut);
        TVirtualMC::GetMC()->TrackMomentum(fMomOut);
        if (fELoss == 0.)
        {
           // return kFALSE;
        }

        AddHit(fTrackID,
               fBarrelID,
               fSensorID,
               TVector3(fPosIn.X(), fPosIn.Y(), fPosIn.Z()),
               TVector3(fPosOut.X(), fPosOut.Y(), fPosOut.Z()),
               TVector3(fMomIn.Px(), fMomIn.Py(), fMomIn.Pz()),
               TVector3(fMomOut.Px(), fMomOut.Py(), fMomOut.Pz()),
               fTime,
               fLength,
               fELoss,
               TVirtualMC::GetMC()->TrackPid());

        // Increment number of AlpidePoints for this track
        R3BStack* stack = static_cast<R3BStack*>(TVirtualMC::GetMC()->GetStack());
        stack->AddPoint(kTRA);
        ResetParameters();
    }

    return kTRUE;
}

// -----   Public method EndOfEvent   -----------------------------------------
void Mingo::EndOfEvent()
{
    if (fVerboseLevel)
    {
        Print();
    }
    Reset();
}
// ----------------------------------------------------------------------------

// -----   Public method Register   -------------------------------------------
void Mingo::Register() { FairRootManager::Instance()->Register("MingoPoint", GetName(), fMingoPoint, kTRUE); }
// ----------------------------------------------------------------------------

// -----   Public method GetCollection   --------------------------------------
TClonesArray* Mingo::GetCollection(Int_t iColl) const
{
    if (iColl == 0)
    {
        return fMingoPoint;
    }
    else
    {
        return nullptr;
    }
}
// ----------------------------------------------------------------------------

// -----   Public method Print   ----------------------------------------------
void Mingo::Print(Option_t*) const
{
    Int_t nHits = fMingoPoint->GetEntriesFast();
    R3BLOG(info, nHits << " points registered in this event");
}

// -----   Public method Reset   ----------------------------------------------
void Mingo::Reset()
{
    fMingoPoint->Clear();
    ResetParameters();
}

// -----   Private method AddHit   --------------------------------------------
R3BAlpidePoint* Mingo::AddHit(Int_t trackID,
                                  Int_t detID,
                                  Int_t detCopyID,
                                  TVector3 posIn,
                                  TVector3 posOut,
                                  TVector3 momIn,
                                  TVector3 momOut,
                                  Double_t time,
                                  Double_t length,
                                  Double_t eLoss,
                                  Int_t pdgcode)
{
    TClonesArray& clref = *fMingoPoint;
    Int_t size = clref.GetEntriesFast();
    return new (clref[size])
        R3BAlpidePoint(trackID, detID, detCopyID, posIn, posOut, momIn, momOut, time, length, eLoss, pdgcode);
}

// -----   Public method CheckIfSensitive   -----------------------------------
Bool_t Mingo::CheckIfSensitive(std::string name)
{
    if (TString(name).Contains("Plane"))
    {
        LOG(info) << "Found geometry from ROOT file: " << name;
        return kTRUE;
    }
    return kFALSE;
}

ClassImp(Mingo)
