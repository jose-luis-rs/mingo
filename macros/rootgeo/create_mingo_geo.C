//
//    General macro to generate a ROOT mingo geometry
//
//    Author: Jose Luis <j.l.rodriguez.sanchez@udc.es>
//

#include "TGeoManager.h"
#include "TMath.h"
#include <iomanip>
#include <iostream>

void create_mingo_geo(TString geoTag = "v2023.07.81.104") {
  // --------------------------------------------------------------------------
  //
  // Execute macro:  root -l
  //                 .L create_mingo_geo.C
  //                 create_mingo_geo()
  // --------------------------------------------------------------------------

  TGeoManager *gGeoMan = NULL;

  // -------   Load media from media file   -----------------------------------
  auto geoLoad = new FairGeoLoader("TGeo", "FairGeoLoader");
  FairGeoInterface *geoFace = geoLoad->getGeoInterface();
  TString geoPath = gSystem->Getenv("VMCWORKDIR");
  TString medFile = geoPath + "/geometry/media_r3b.geo";
  geoFace->setMediaFile(medFile);
  geoFace->readMedia();
  gGeoMan = gGeoManager;

  // -------   Geometry file name (output)   ----------------------------------
  TString geoFileName = geoPath + "/geometry/mingo_";
  geoFileName = geoFileName + geoTag + ".geo.root";

  // -----------------   Get and create the required media    -----------------
  FairGeoMedia *geoMedia = geoFace->getMedia();
  FairGeoBuilder *geoBuild = geoLoad->getGeoBuilder();

  FairGeoMedium *mVac = geoMedia->getMedium("vacuum");
  if (!mVac)
    Fatal("Main", "FairMedium vacuum not found");
  geoBuild->createMedium(mVac);
  TGeoMedium *pMedVac = gGeoMan->GetMedium("vacuum");
  if (!pMedVac)
    Fatal("Main", "Medium vacuum not found");

  FairGeoMedium *mPb = geoMedia->getMedium("lead");
  if (!mPb)
    Fatal("Main", "FairMedium lead not found");
  geoBuild->createMedium(mPb);
  TGeoMedium *pMedPb = gGeoMan->GetMedium("lead");
  if (!pMedPb)
    Fatal("Main", "Medium lead not found");
    
  FairGeoMedium *mAl = geoMedia->getMedium("aluminium");
  if (!mAl)
    Fatal("Main", "FairMedium aluminium not found");
  geoBuild->createMedium(mAl);
  TGeoMedium *pMedAl = gGeoMan->GetMedium("aluminium");
  if (!pMedAl)
    Fatal("Main", "Medium aluminium not found");

  // --------------   Create geometry and top volume  -------------------------
  gGeoMan = dynamic_cast<TGeoManager*>(gROOT->FindObject("FAIRGeom"));
  gGeoMan->SetName("TARGETgeom");
  TGeoVolume *top = new TGeoVolumeAssembly("TOP");
  gGeoMan->SetTopVolume(top);

  // Positions -----------------------------------------------------------------
  double dx = 0., dy = 0., dz = 0.;
  // Pb layer dimensions
  double WorldSize = 100.; // 100 x 100 cm^2
  double WorldL = 50.; // 50 cm
  double detectorThickness=2.2;
  double layer1Size = 99.0; // 99 x 99 cm^2
  //double layer1L = 1.62; // cm
  double layer1L = 0.81; // cm
  double layer2Size = 99.0; // 99 x 99 cm^2
  double layer2L = 1.04; // cm
  
  // World definition ----------------------------------------------------------
  auto vWorld = new TGeoBBox("WBox",WorldSize,WorldSize,WorldL);
  auto World = new TGeoVolume("MingoWorld", vWorld, pMedVac);
  TGeoRotation *fRefRot = NULL; // unitary rotation
  auto fMatrixref = new TGeoCombiTrans("", dx, dy, dz, fRefRot);
  top->AddNode(World, 1, fMatrixref);
  
  
  // Plane1 --------------------------------------------------------------------
  TGeoVolume *Plane1 = gGeoManager->MakeBox("Plane1", 
                                             pMedAl, 
                                             99. / 2.,
                                             99. / 2., 
                                             detectorThickness / 2.
                                             );
  Plane1->SetVisLeaves(kTRUE);
  Plane1->SetLineColor(2); // red
  auto fMatrix0 = new TGeoCombiTrans("", dx, dy, detectorThickness / 2., fRefRot);
  World->AddNode(Plane1,0,fMatrix0);  

  // Target --------------------------------------------------------------------
  TGeoVolume *pb1 = gGeoManager->MakeBox("Pb_1", 
                                             pMedPb, 
                                             layer1Size / 2.,
                                             layer1Size / 2., 
                                             layer1L / 2.
                                             );
  pb1->SetVisLeaves(kTRUE);
  pb1->SetLineColor(4); // blue
  auto fMatrix1 = new TGeoCombiTrans("", dx, dy, detectorThickness +layer1L / 2., fRefRot);
  World->AddNode(pb1,0,fMatrix1);  
  
  // Plane2 --------------------------------------------------------------------
  TGeoVolume *Plane2 = gGeoManager->MakeBox("Plane2", 
                                             pMedAl, 
                                             99. / 2.,
                                             99. / 2., 
                                             detectorThickness / 2.
                                             );
  Plane2->SetVisLeaves(kTRUE);
  Plane2->SetLineColor(2); // red
  auto fMatrix2 = new TGeoCombiTrans("", dx, dy, 10.+ detectorThickness / 2., fRefRot);
  World->AddNode(Plane2,0,fMatrix2);  
  
  // Layer2 --------------------------------------------------------------------
  TGeoVolume *pb2 = gGeoManager->MakeBox("Pb_2", 
                                             pMedPb, 
                                             layer1Size / 2.,
                                             layer1Size / 2., 
                                             layer1L / 2.
                                             );
  pb2->SetVisLeaves(kTRUE);
  pb2->SetLineColor(4); // blue
  auto fMatrix3 = new TGeoCombiTrans("", dx, dy, 10. + layer1L / 2. + detectorThickness, fRefRot);
  World->AddNode(pb2,0,fMatrix3);

   
  // Plane3 --------------------------------------------------------------------
  TGeoVolume *Plane3 = gGeoManager->MakeBox("Plane3", 
                                             pMedAl, 
                                             99. / 2.,
                                             99. / 2., 
                                             detectorThickness / 2.
                                             );
  Plane3->SetVisLeaves(kTRUE);
  Plane3->SetLineColor(2); // red
  auto fMatrix4 = new TGeoCombiTrans("", dx, dy, 20. + detectorThickness / 2., fRefRot);
  World->AddNode(Plane3,0,fMatrix4);
  
  // Layer3 --------------------------------------------------------------------
  TGeoVolume *pb3 = gGeoManager->MakeBox("Pb_3", 
                                             pMedPb, 
                                             layer2Size / 2.,
                                             layer2Size / 2., 
                                             layer2L / 2.
                                             );
  pb3->SetVisLeaves(kTRUE);
  pb3->SetLineColor(4); // blue
  auto fMatrix5 = new TGeoCombiTrans("", dx, dy, 20. + layer2L / 2. + detectorThickness, fRefRot);
  World->AddNode(pb3,0,fMatrix5);
  

  // Plane3 --------------------------------------------------------------------
  TGeoVolume *Plane4 = gGeoManager->MakeBox("Plane4", 
                                             pMedAl, 
                                             99. / 2.,
                                             99. / 2., 
                                             detectorThickness / 2.
                                             );
  Plane4->SetVisLeaves(kTRUE);
  Plane4->SetLineColor(2); // red
  auto fMatrix6 = new TGeoCombiTrans("", dx, dy, 40.+detectorThickness / 2., fRefRot);
  World->AddNode(Plane4,0,fMatrix6);
  
  

  // ---------------   Check Overlaps   ----------------------------------------
  gGeoMan->CloseGeometry();
  gGeoMan->CheckOverlaps(0.001); // precision in cm
  gGeoMan->PrintOverlaps();
  gGeoMan->Test();

  // ---------------   Save geometry   -----------------------------------------
  auto geoFile = new TFile(geoFileName, "RECREATE");
  top->Write();
  top->Draw();
  geoFile->Close();

  std::cout << "\033[34m Creating geometry:\033[0m "
            << "\033[33m" << geoFileName << " \033[0m" << std::endl;
  std::cout << "Macro finished succesfully." << std::endl;
}
