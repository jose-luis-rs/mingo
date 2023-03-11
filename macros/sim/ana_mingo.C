int getplane(int d){

if (d==4)return 2;
else if (d==6)return 3;
else if (d==7)return 4;

return NAN;
}

void ana_mingo(const Float_t nev = -1){

    TStopwatch timer;
    timer.Start();
    
    TGeoManager::SetVerboseLevel(0);

    TFile* file = TFile::Open("sim0.8.root");
    
    double energy = 800;//MeV
    
    //Ascii output file
    std::ofstream out;
    out.open("output.txt");
    
    out << "----- FILE HEADER ----------------------------------" << endl;
    out << "Event_Number	Ini_E[MeV]	Ini_X[cm]	Ini_Y[cm]	Ini_Z[cm]	Ini_Theta	Ini_Phi	Number_of_hits" << endl;
    out << "Plane_h	X_h[cm]	Y_h[cm]	Z_h[cm]	Time_h[ns]	 Being 'h' each hit of the event" << endl;
    out << "----- END OF HEADER ----------------------------------" << endl;

    // READING TREE
    TTree* tree = (TTree*)file->Get("evt");
    
    // TREE ENTRIES--------------------------------------------------------------------
    Int_t nevents = tree->GetEntries();
    if (nev > -1)
       nevents = nev;
    std::cout << "Events: " << nevents << std::endl;

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
        
        std::stringstream sprint;
        sprint.precision(4);
                
        sprint << ie+1<< "	"<<energy<<" 	0	0	0	0	0	"<< mingopointsPerEvent << endl;
        
        
        if(mingopointsPerEvent>0)
        {
            auto Data = new R3BAlpidePoint*[mingopointsPerEvent];
            for (Int_t j = 0; j < mingopointsPerEvent; j++)
            {
                Data[j] = (R3BAlpidePoint*)(mingopoint->At(j));
                
                auto planeid = getplane(Data[j]->GetDetectorID());
                
                //cout<<planeid<<endl;
                
                sprint << planeid<< "	"<<Data[j]->GetXIn()<< "	"<<Data[j]->GetYIn() << "	"<<Data[j]->GetZIn()<< "	"<<Data[j]->GetTime() << endl;
                
            }
            if (Data)
                delete[] Data;
                
            out << sprint.str();
            
            sprint.str().clear();
            
                
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
