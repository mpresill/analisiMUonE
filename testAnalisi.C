
void testAnalisi(){

    TFile *f = new TFile("test.root");

    TTree *tTracks  = (TTree*)f->Get("tracks");
    TTree *tHits    = (TTree*)f->Get("hits");
    TTree *tCalo    =  (TTree*)f->Get("calo");

    int eventID, nTracks, nHits, nParticles;
    tTracks->SetBranchAddress("eventID", &eventID);
    tTracks->SetBranchAddress("nTracks", &nTracks);
    tHits->SetBranchAddress("nHits", &nHits);
    tCalo->SetBranchAddress("nParticles", &nParticles);

    int maxTracks = 0;
    int maxHits = 0;
    int maxParticles = 0;

    for (int i=0; i<tTracks->GetEntries(); i++){
        tTracks->GetEvent(i);
        if(nTracks>maxTracks)           maxTracks = nTracks;
        if(nHits>maxHits)               maxHits = nHits;
        if(nParticles>maxParticles)     maxParticles = nParticles;
    }

    vector<int> PDG, parentPDG, trackID;

    PDG.resize(maxTracks);
    parentPDG.resize(maxTracks);
    trackID.resize(maxTracks);

    tTracks->SetBranchAddress("PDG", &(PDG[0]));
    tTracks->SetBranchAddress("parentPDG", &(parentPDG[0]));
    tTracks->SetBranchAddress("trackID", &(trackID[0]));

    for (int i=0; i<tTracks->GetEntries(); i++){
        tTracks->GetEvent(i);
        cout<<"evt "<<eventID<<", nTracks "<<nTracks<<endl<<endl;

        for(int j=0; j<nTracks; j++){
            cout<< PDG[j] <<endl;
        }
    }

    return;

}