void testAnalisi(){

    TFile *f = new TFile("test.root");

    TTree *trTracks  = (TTree*)f->Get("tracks");
    TTree *trHits    = (TTree*)f->Get("hits");
    TTree *trCalo    =  (TTree*)f->Get("calo");


    // Get n events, max tracks/hits/calo

    int eventID, nTracks, nHits, nParticles;
    trTracks->SetBranchAddress("eventID", &eventID);
    trTracks->SetBranchAddress("nTracks", &nTracks);
    trHits->SetBranchAddress("nHits", &nHits);
    trCalo->SetBranchAddress("nParticles", &nParticles);

    int nEvents = trTracks->GetEntries();
    int maxTracks = 0;
    int maxHits = 0;
    int maxParticles = 0;

    for (int i=0; i<nEvents; i++){
        trTracks->GetEvent(i);
        trHits->GetEvent(i);
        trCalo->GetEvent(i);
        if(nTracks > maxTracks)           maxTracks = nTracks;
        if(nHits > maxHits)               maxHits = nHits;
        if(nParticles > maxParticles)     maxParticles = nParticles;
    }

    // Matching variables

    // Tracks variables

    vector<int> tPDG, tParentPDG, tTrackID;
    vector<double> tKinEnergy;

    tPDG.resize(maxTracks);
    tParentPDG.resize(maxTracks);
    tTrackID.resize(maxTracks);
    tKinEnergy.resize(maxTracks);

    trTracks->SetBranchAddress("tPDG", &(tPDG[0]));
    trTracks->SetBranchAddress("tParentPDG", &(tParentPDG[0]));
    trTracks->SetBranchAddress("tTrackID", &(tTrackID[0]));
    trTracks->SetBranchAddress("tKinEnergy", &(tKinEnergy[0]));

    // Hits variables

    vector<int> hPDG, hTrackID;
    vector<double> hEnergy;

    hPDG.resize(maxHits);
    hTrackID.resize(maxHits);
    hEnergy.resize(maxHits);

    trHits->SetBranchAddress("hPDG", &(hPDG[0]));
    trHits->SetBranchAddress("hTrackID", &(hTrackID[0]));
    trHits->SetBranchAddress("hEnergy", &(hEnergy[0]));

    // Calo variables

    float cTotalEnergy;
    vector<double> cEnergy, cEloss, cPartnum, cParent;

    cEnergy.resize(maxParticles);
    cEloss.resize(maxParticles);
    cPartnum.resize(maxParticles);
    cParent.resize(maxParticles);

    trCalo->SetBranchAddress("cTotalEnergy", &cTotalEnergy);
    trCalo->SetBranchAddress("cEnergy", &(cEnergy[0]));
    trCalo->SetBranchAddress("cEloss", &(cEloss[0]));
    trCalo->SetBranchAddress("cPartnum", &(cPartnum[0]));
    trCalo->SetBranchAddress("cParent", &(cParent[0]));

    // Events loop begins here

    for (int i=0; i<nEvents; i++){

        trTracks->GetEvent(i);
        trHits->GetEvent(i);
        trCalo->GetEvent(i);

        cout<<eventID<<" --- tot calo energy "<<cTotalEnergy<<endl;

        for(int j=0; j<nTracks; j++){
            //do track stuff
            // cout<<"track "<<j<<", PDG "<<tPDG[j]<<endl;
        }

        for(int j=0; j<nHits; j++){
            //do hits stuff
            // cout<<"hit "<<j<<", trackID "<<hTrackID[j]<<endl;
        }


        for(int j=0; j<nParticles; j++){
            //do calo stuff
        }

    }

    return;

}