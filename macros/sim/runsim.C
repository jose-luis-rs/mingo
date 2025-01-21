/******************************************************************************
 *   Copyright (C) 2019 GSI Helmholtzzentrum für Schwerionenforschung GmbH    *
 *   Copyright (C) 2019-2023 Members of R3B Collaboration                     *
 *                                                                            *
 *             This software is distributed under the terms of the            *
 *                 GNU General Public Licence (GPL) version 3,                *
 *                    copied verbatim in the file "LICENSE".                  *
 *                                                                            *
 * In applying this license GSI does not waive the privileges and immunities  *
 * granted to it by virtue of its status as an Intergovernmental Organization *
 * or submit itself to any jurisdiction.                                      *
 ******************************************************************************/

#include <TStopwatch.h>
#include <TString.h>
#include <TSystem.h>
#include <memory>

void runsim(int nbevents = 1)
{
    // Timer
    TStopwatch timer;
    timer.Start();

    // Logging
    auto logger = FairLogger::GetLogger();
    logger->SetLogVerbosityLevel("low");
    logger->SetLogScreenLevel("info");
    logger->SetColoredLog(true);

    // System paths
    const TString workDirectory = getenv("VMCWORKDIR");
    gSystem->Setenv("GEOMPATH", workDirectory + "/geometry");
    gSystem->Setenv("CONFIG_DIR", workDirectory + "/gconfig");

    // Output files
    const TString simufile = "sim.root";
    const TString parfile = "par.root";

    // Basic simulation setup
    auto run = new FairRunSim();
    run->SetName("TGeant4");
    run->SetStoreTraj(0);
    run->SetMaterials("media_r3b.geo");
    run->SetSink(new FairRootFileSink(simufile));
    
    // -----   Runtime data base   --------------------------------------------
    FairRuntimeDb *rtdb = run->GetRuntimeDb();
    
    // Geometry: Cave
    auto cave = new R3BCave("CAVE");
    cave->SetGeometryFileName("r3b_cave_vacuum.geo");
    run->AddModule(cave);

    // Geometry:
    //run->AddModule(new Mingo("mingo_v2023.sinPb.geo.root",{0.,0.,0.}));// without Pb
    //run->AddModule(new Mingo("mingo_v2023.1.geo.root",{0.,0.,0.}));// configuration 1
    run->AddModule(new Mingo("mingo_v2023.3.geo.root",{0.,0.,0.}));// configuration 2
    //run->AddModule(new Mingo("mingo_v2023.07.81.104.geo.root",{0.,0.,0.}));// Setup 81,81,104 Pb
    
    
    // -----   Create PrimaryGenerator   --------------------------------------
    FairPrimaryGenerator *primGen = new FairPrimaryGenerator();

    // Primary particle generator
    Int_t pdgId = 11;       // electron beam
    Int_t mult = 1;
    Double32_t theta1 = 0.; // polar angle range
    Double32_t theta2 = 0.;
    Double32_t momentum = 0.3;
    auto boxGen = new FairBoxGenerator(pdgId, mult);
    boxGen->SetThetaRange(theta1, theta2);
    boxGen->SetPRange(momentum, momentum);
    boxGen->SetPhiRange(0., 360.);
    boxGen->SetXYZ(0., 0., -1.);
    primGen->AddGenerator(boxGen);
    
    run->SetGenerator(primGen);

    // Init
    run->Init();
    
    // -----   Runtime database   ---------------------------------------------
    Bool_t kParameterMerged = kTRUE;
    FairParRootFileIo *parOut = new FairParRootFileIo(kParameterMerged);
    parOut->open(parfile.Data());
    rtdb->setOutput(parOut);
    rtdb->saveOutput();
    rtdb->print();

    // Simulate
    run->Run(nbevents);

    // Report
    timer.Stop();
    std::cout << "Real time: " << timer.RealTime()/60. << " min, CPU time: " << timer.CpuTime()/60. << " min" << std::endl;
    std::cout << "Macro finished successfully." << std::endl;
}
