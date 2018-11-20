bool verbose = true;
bool preselection = true;


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
    int maxTracks = 200;
    int maxHits = 1000;
    int maxParticles = 5000;

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
    vector<double> hEnergy, hKinEnergy;

    hPDG.resize(maxHits, 0);
    hTrackID.resize(maxHits, 0);
    hEnergy.resize(maxHits, 0);
    hKinEnergy.resize(maxHits, 0);

    trHits->SetBranchAddress("PDG", &(hPDG[0]));
    trHits->SetBranchAddress("trackID", &(hTrackID[0]));
    trHits->SetBranchAddress("energy", &(hEnergy[0]));
    trHits->SetBranchAddress("kinenergy", &(hKinEnergy[0]));

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

    TH1F *htot = new TH1F("htot", "totCaloE", 500, 0, 500);

    TH1F *hCaloMu = new TH1F("hCaloMu", "eLossCaloMu", 500, 0, 500);
    TH1F *hCaloNotMu = new TH1F("hCaloNotMu", "eCaloNotMu", 500, 0, 500);

    TH1F *hHitsMu = new TH1F("hHitsMu", "eHitsMu", 500, 0, 1.5);
    TH1F *hHitsNotMu = new TH1F("hHitsNotMu", "eHitsNotMu", 500, 0, 1.5);
    TH1F *hHitsEle = new TH1F("hHitsEle", "eHitsEle", 500, 0, 1.5);

    // Events loop begins here

    for (int i=0; i<nEvents; i++){

        trTracks->GetEvent(i);
        trHits->GetEvent(i);
        trCalo->GetEvent(i);

        // Event Selection

        if(preselection){

            if(nTracks<2) continue;
            bool skipEvt = true;
            for (int j=0; j<nTracks; ++j) {
                if (tPDG[j]==11 and tParentPDG[j]==13 and tKinEnergy[j]>1000) skipEvt = false;       
            }
            if(skipEvt) continue;

        }

        if(verbose) cout<<endl<<" ----- "<<i<<endl;

        if(verbose) cout<<" - TRACKS"<<endl;

        for(int j=0; j<nTracks; ++j) if(verbose) cout<<tPDG[j]<<" "<<tKinEnergy[j]<<endl;

        if(verbose) cout<<endl<<" - HITS"<<endl;

        float ehits_Mu = 0;
        float ehits_NotMu = 0;
        float ehits_Ele = 0;

        for(int j=0; j<nHits; ++j){
            if(hPDG[j] == 13) ehits_Mu += hEnergy[j]; 
            else{
                ehits_NotMu += hEnergy[j];
            }

            if(hPDG[j] == 11) ehits_Ele += hEnergy[j]; 

            if(verbose) cout<<hPDG[j]<<" "<<hEnergy[j]<<endl;
        }

        if(verbose) cout<<endl<<" - CALO"<<endl;

        hHitsMu->Fill(ehits_Mu);
        hHitsNotMu->Fill(ehits_NotMu);
        hHitsEle->Fill(ehits_Ele);

        float eCalo_Mu = 0;
        float eCalo_NotMu = 0;
        int muCaloIndex = 0;

        for(int j=0; j<nParticles; ++j){if(cPDG[j] == 13) muCaloIndex = cPartnum[j];}

        for(int j=0; j<nParticles; ++j){
            if(cPDG[j] == 13 || cParent[j] == muCaloIndex) 
                eCalo_Mu += cEloss[j];
            else eCalo_NotMu += cEloss[j];

            if(verbose) cout<<cPDG[j]<<" "<<cEloss[j]<<endl;
        }

        htot->Fill(cTotalEnergy);
        hCaloMu->Fill(eCalo_Mu);
        hCaloNotMu->Fill(eCalo_NotMu);

    }


//    TCanvas *c1 = new TCanvas();
//    htot->Draw();
    TCanvas *c2 = new TCanvas();
    hCaloMu->Draw();
    TCanvas *c3 = new TCanvas();
    hCaloNotMu->Draw();

    TCanvas *c4 = new TCanvas();
    hHitsMu->Draw();
    TCanvas *c5 = new TCanvas();
    hHitsNotMu->Draw();
    TCanvas *c6 = new TCanvas();
    hHitsEle->Draw();

    return;

}
