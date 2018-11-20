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

    tPDG.resize(maxTracks, 0);
    tParentPDG.resize(maxTracks, 0);
    tTrackID.resize(maxTracks, 0);
    tKinEnergy.resize(maxTracks, 0);

    trTracks->SetBranchAddress("PDG", &(tPDG[0]));
    trTracks->SetBranchAddress("parentPDG", &(tParentPDG[0]));
    trTracks->SetBranchAddress("trackID", &(tTrackID[0]));
    trTracks->SetBranchAddress("kinEnergy", &(tKinEnergy[0]));

    // Hits variables

    vector<int> hPDG, hTrackID;
    vector<double> hEnergy;

    hPDG.resize(maxHits, 0);
    hTrackID.resize(maxHits, 0);
    hEnergy.resize(maxHits, 0);

    trHits->SetBranchAddress("PDG", &(hPDG[0]));
    trHits->SetBranchAddress("trackID", &(hTrackID[0]));
    trHits->SetBranchAddress("energy", &(hEnergy[0]));

    // Calo variables

    float cTotalEnergy;
    vector<double> cEnergy, cEloss;
    vector<int> cPartnum, cParent, cPDG;

    cEnergy.resize(maxParticles, 0);
    cEloss.resize(maxParticles, 0);
    cPartnum.resize(maxParticles, 0);
    cParent.resize(maxParticles, 0);
    cPDG.resize(maxParticles, 0);

    trCalo->SetBranchAddress("totalEnergy", &cTotalEnergy);
    trCalo->SetBranchAddress("energy", &(cEnergy[0]));
    trCalo->SetBranchAddress("eloss", &(cEloss[0]));
    trCalo->SetBranchAddress("partnum", &(cPartnum[0]));
    trCalo->SetBranchAddress("parent", &(cParent[0]));
    trCalo->SetBranchAddress("pdg", &(cPDG[0]));

    // Define histograms

    TH1F *h1 = new TH1F("h1", "totCaloE",   2000, 0, 1000);
    TH1F *h2 = new TH1F("h2", "elossMu",    2000, 0, 1000);
    TH1F *h3 = new TH1F("h3", "elossNotMu", 2000, 0, 1000);

    // Events loop begins here

    for (int i=0; i<nEvents; i++){

        trTracks->GetEvent(i);
        trHits->GetEvent(i);
        trCalo->GetEvent(i);

        h1->Fill(cTotalEnergy);

        for(int j=0; j<nTracks; j++){
            //do track stuff
            // cout<<"track "<<j<<", PDG "<<tPDG[j]<<endl;
        }

        for(int j=0; j<nHits; j++){
            //do hits stuff
            // cout<<"hit "<<j<<", trackID "<<hTrackID[j]<<endl;
        }

        float eLoss_Mu = 0;
        float eLoss_NotMu = 0;

        for(int j=0; j<nParticles; j++){
            if(cPDG[j] == 13) eLoss_Mu += cEloss[j]; 
                else eLoss_NotMu += cEloss[j];
        }

        h2->Fill(eLoss_Mu);
        h3->Fill(eLoss_NotMu);

    }
    TCanvas *c1 = new TCanvas();
    h1->Draw();
    TCanvas *c2 = new TCanvas();
    h2->Draw();
    TCanvas *c3 = new TCanvas();
    h3->Draw();

    return;

}