int getplane(int d){

if (d==3)return 1;
else if (d==5)return 2;
else if (d==7)return 3;
else if (d==9)return 4;

return NAN;
}

void ana_mingo(const Float_t nev = -1){

    TStopwatch timer;
    timer.Start();
    
    TGeoManager::SetVerboseLevel(0);

    TFile* file = TFile::Open("sim.root");
    
    double energy = 300;//MeV
    
    // READING TREE
    TTree* tree = (TTree*)file->Get("evt");
    
    // TREE ENTRIES--------------------------------------------------------------------
    Int_t nevents = tree->GetEntries();
    if (nev > -1)
       nevents = nev;
    std::cout << "Events: " << nevents << std::endl;
    
    //Ascii output file
    std::ofstream out;
    out.open("output.txt");
    
    std::stringstream sprint;
    sprint <<"HEADER\n";
    sprint <<"CASE\n";
    sprint <<"     Particle []: gamma (0), electron (1), muon (2), neutron (3), proton (4)\n";
    sprint <<"     Number of events []:\n";
    sprint <<"     Emin [MeV]:\n";
    sprint <<"     Emax [MeV]:\n";
    sprint <<"     E distribution: constant (0), gaussian (1), exponential (2)\n";
    sprint <<"     Theta min [deg]:\n";
    sprint <<"     Theta max [deg]:\n";
    sprint <<"     Detector plane - X dimension [mm]:\n";
    sprint <<"     Detector plane - Y dimension [mm]:\n";
    sprint <<"     Detector plane - Z dimension [mm]:\n";
    sprint <<"ACTIVE PLANES\n";
    sprint <<"     Plane 1 - Z coordinate [mm]: 0 by definition\n";
    sprint <<"     Plane 2 - Z coordinate [mm]:\n";
    sprint <<"     Plane 3 - Z coordinate [mm]:\n";
    sprint <<"     Plane 4 - Z coordinate [mm]:\n";
    sprint <<"PASSIVE PLANES\n";
    sprint <<"     Plane 1 - Z coordinate [mm]: Measured downwards from first active plane\n";
    sprint <<"     Plane 2 - Z coordinate [mm]: Measured downwards from first active plane\n";
    sprint <<"     Plane 3 - Z coordinate [mm]: Measured downwards from first active plane\n";
    sprint <<"     Plane 4 - Z coordinate [mm]: Measured downwards from first active plane\n";
    sprint <<"     Plane 1 - Thickness [mm]:\n";
    sprint <<"     Plane 2 - Thickness [mm]:\n";
    sprint <<"     Plane 3 - Thickness [mm]:\n";
    sprint <<"     Plane 4 - Thickness [mm]:\n";
    sprint <<"     Plane 1 - Material []: Pb (0), Fe (1), W (2), Polyethylene (3)\n";
    sprint <<"     Plane 2 - Material []: Pb (0), Fe (1), W (2), Polyethylene (3)\n";
    sprint <<"     Plane 3 - Material []: Pb (0), Fe (1), W (2), Polyethylene (3)\n";
    sprint <<"     Plane 4 - Material []: Pb (0), Fe (1), W (2), Polyethylene (3)\n";
    sprint <<"EVENT\n";
    sprint <<"     Event number []:\n";
    sprint <<"     Initial energy [MeV]:\n";
    sprint <<"     Initial X [mm]: Measured from center of plane, positive to the right\n";
    sprint <<"     Initial Y [mm]: Measured from center of plane, right handed frame\n";
    sprint <<"     Initial Z [mm]: 0 by definition\n";
    sprint <<"     Initial theta [deg]:\n";
    sprint <<"     Initial phi [deg]:\n";
    sprint <<"     Number of hits []:\n";
    sprint <<"HIT\n";
    sprint <<"     Plane number []: First is 1, last is 4\n";
    sprint <<"     X [mm]: Measured from center of plane, positive to the right\n";
    sprint <<"     Y [mm]: Measured from center of plane, right handed frame\n";
    sprint <<"     Z [mm]: Measured downwards from first active plane\n";
    sprint <<"     Time since first impact [ns]:\n";
    sprint <<"DATA\n";
    sprint <<"1	"<<nevents<<"	"<<energy<<"	"<<energy<<"	0	0	0	999	999	22\n";
    sprint <<"0	100	200	400\n";
    sprint <<"null	22	null	222	null	10.4	null	16.2	null	0	null	0\n";
    out << sprint.str();
    sprint.str().clear();
    
    /*
    
    configuration 1
    sprint <<"1	"<<nevents<<"	"<<energy<<"	"<<energy<<"	0	0	0	999	999	22\n";
    sprint <<"0	100	200	400\n";
    sprint <<"null	22	null	222	null	16.2	null	10.4	null	0	null	0\n";

    
    configuration 3
    sprint <<"1	"<<nevents<<"	"<<energy<<"	"<<energy<<"	0	0	0	999	999	22\n";
    sprint <<"0	100	200	400\n";
    sprint <<"null	22	122	222	null	8.1	8.1	10.4	null	0	0	0\n";
    
HEADER
    CASE
        Particle []: gamma (0), electron (1), muon (2), neutron (3), proton (4)
        Number of events []:
        Emin [MeV]:
        Emax [MeV]:
        E distribution: constant (0), gaussian (1), exponential (2)
        Theta min [deg]:
        Theta max [deg]:
        Detector plane - X dimension [mm]:
        Detector plane - Y dimension [mm]:
        Detector plane - Z dimension [mm]:
    ACTIVE PLANES
        Plane 1 - Z coordinate [mm]: 0 by definition
        Plane 2 - Z coordinate [mm]:
        Plane 3 - Z coordinate [mm]:
        Plane 4 - Z coordinate [mm]:
    PASSIVE PLANES
        Plane 1 - Z coordinate [mm]: Measured downwards from first active plane
        Plane 2 - Z coordinate [mm]: Measured downwards from first active plane
        Plane 3 - Z coordinate [mm]: Measured downwards from first active plane
        Plane 4 - Z coordinate [mm]: Measured downwards from first active plane
        Plane 1 - Thickness [mm]:
        Plane 2 - Thickness [mm]:
        Plane 3 - Thickness [mm]:
        Plane 4 - Thickness [mm]:
        Plane 1 - Material []: Pb (0), Fe (1), W (2), Polyethylene (3)
        Plane 2 - Material []: Pb (0), Fe (1), W (2), Polyethylene (3)
        Plane 3 - Material []: Pb (0), Fe (1), W (2), Polyethylene (3)
        Plane 4 - Material []: Pb (0), Fe (1), W (2), Polyethylene (3)
   EVENT
        Event number []:
        Initial energy [MeV]:
        Initial X [mm]: Measured from center of plane, positive to the right
        Initial Y [mm]: Measured from center of plane, right handed frame
        Initial Z [mm]: 0 by definition
        Initial theta [deg]:
        Initial phi [deg]:
        Number of hits []:
    HIT
        Plane number []: First is 1, last is 4
        X [mm]: Measured from center of plane, positive to the right
        Y [mm]: Measured from center of plane, right handed frame
        Z [mm]: Measured downwards from first active plane
        Time since first impact [ns]:
DATA    
    
    */
    
    
    //out << "----- FILE HEADER ----------------------------------" << endl;
    //out << "Event_Number	Ini_E[MeV]	Ini_X[cm]	Ini_Y[cm]	Ini_Z[cm]	Ini_Theta	Ini_Phi	Number_of_hits" << endl;
    //out << "Plane_h	X_h[cm]	Y_h[cm]	Z_h[cm]	Time_h[ns]	 Being 'h' each hit of the event" << endl;
    //out << "----- END OF HEADER ----------------------------------" << endl;

 

    // PointData
    TClonesArray* mingopoint = new TClonesArray("R3BAlpidePoint");
    TBranch* branchTraData = tree->GetBranch("MingoPoint");
    branchTraData->SetAddress(&mingopoint); 
    

    
    // LOOP IN THE EVENTS--------------------------------------------------------------
    for (Int_t ie = 0; ie < nevents; ie++)
    {
        // Print current event in the analysis  -------------
        printf("Processed: %d of %d (%.2f of 100) \r", ie + 1, nevents, 100. * (ie + 1.0) / nevents);
        fflush(stdout);
 
        mingopoint->Clear();
        tree->GetEntry(ie); 
        
        Int_t mingopointsPerEvent = mingopoint->GetEntriesFast();
        int mul=0;
        if(mingopointsPerEvent>0)
        {
            auto Data = new R3BAlpidePoint*[mingopointsPerEvent];
            for (Int_t j = 0; j < mingopointsPerEvent; j++)
            {
                Data[j] = (R3BAlpidePoint*)(mingopoint->At(j));

                if(Data[j]->GetPid()!=22 && Data[j]->GetDetectorID()!=3)mul++;
  
            }
            if (Data)
                delete[] Data;          
        }
        
        
        std::stringstream sprintd;
        sprintd.precision(4);
                
        sprintd << ie+1<< "	"<<energy<<" 	0	0	0	0	0	"<< mul+1 << endl;
        sprintd << 1 << "	"<<0<< "	"<<0<< "	"<<0<< "	"<<0<< endl;

        if(mingopointsPerEvent>0)
        {
            auto Data = new R3BAlpidePoint*[mingopointsPerEvent];
            for (Int_t j = 0; j < mingopointsPerEvent; j++)
            {
                Data[j] = (R3BAlpidePoint*)(mingopoint->At(j));
                
                auto planeid = getplane(Data[j]->GetDetectorID());
                
                //cout<<planeid<<endl;
                if(Data[j]->GetPid()!=22 && Data[j]->GetDetectorID()!=3)
                sprintd << planeid<< "	"<<Data[j]->GetXIn()*10.<< "	"<<Data[j]->GetYIn()*10. << "	"<<Data[j]->GetZIn()*10.<< "	"<<Data[j]->GetTime() << endl;
                
            }
            if (Data)
                delete[] Data;
                
            out << sprintd.str();
            
            sprintd.str().clear();
            
                
        }
    
    }

    out.close();
    timer.Stop();

    Double_t rtime = timer.RealTime();
    Double_t ctime = timer.CpuTime();
    Float_t cpuUsage = ctime / rtime;
    std::cout << "CPU used: " << cpuUsage << std::endl;

    std::cout << std::endl;
    std::cout << "Real time " << rtime / 60. << " min, CPU time " << ctime / 60. << "min" << std::endl << std::endl;
    std::cout << "Macro finished successfully." << std::endl; 
}
