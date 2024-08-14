//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file EventAction.cc
/// \brief Implementation of the BASEVOLUME::EventAction class

#include "EventAction.hh"
#include "RunAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  EventAction::EventAction(RunAction*)
    : nEvent(0)
  {
    auto analysisManager = G4AnalysisManager::Instance();
    fHistNP=analysisManager->GetH1Id("nPhotons");
    fHistLA=analysisManager->GetH1Id("emittedL");
    fHistEA=analysisManager->GetH1Id("emittedE");
    fHistPD=analysisManager->GetH1Id("photVsDist");
    fHistzD=analysisManager->GetH1Id("zDistance");
    fHistrD=analysisManager->GetH1Id("rDistance");
    fHistPr=analysisManager->GetH1Id("firstInter");
  }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  EventAction::~EventAction()
  {}
  
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  void EventAction::BeginOfEventAction(const G4Event* )
  {
    fPartMap.clear();
    fEdep = 0.;
    fIonL = 0; 
    nIntera=0;
    nEvent++;
    if (nEvent% 100 == 0) 
      std::cout <<"-> This is the primary event "<<nEvent<<std::endl;
    /*
      const G4PrimaryVertex* primaryVertex = event->GetPrimaryVertex(0);
      const G4PrimaryParticle* primaryParticle = primaryVertex->GetPrimary(0);
      G4int primaryInteractionType = primaryParticle->GetPDGcode();
    */
    mdp.reset();
    mdp.insert("Begin of Event Action");
    mdp.insert("-> This is the primary event ",nEvent);
  }
  
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  void EventAction::EndOfEventAction(const G4Event* )
  {
    mdp.insert("END of Event Action");
    
    auto analysisManager = G4AnalysisManager::Instance();
    mdp.insert(" Number of Interaction of the electron ",nIntera);
    /*
    if (nIntera == 1) {
      analysisManager->FillH1(fHistIL1, 1000.*fIonL);
      analysisManager->FillH1(fHistzD1,1000.*fPoint.z()/CLHEP::mm);
      analysisManager->FillH1(fHistrD1,1000.*ray/CLHEP::mm);
      
    } else if (nIntera==2){
      analysisManager->FillH1(fHistIL2, 1000.*fIonL);
      analysisManager->FillH1(fHistzD2,1000.*fPoint.z()/CLHEP::mm);
      analysisManager->FillH1(fHistrD2,1000.*ray/CLHEP::mm);
    } else {
      analysisManager->FillH1(fHistILt, 1000.*fIonL);
      analysisManager->FillH1(fHistzDt,1000.*fPoint.z()/CLHEP::mm);
      analysisManager->FillH1(fHistrDt,1000.*ray/CLHEP::mm);
    }
    analysisManager->FillH1(fHistIL, 1000.*fIonL);
    analysisManager->FillH1(fHistzD,1000.*fPoint.z()/CLHEP::mm);
    analysisManager->FillH1(fHistrD,1000.*ray/CLHEP::mm);
    */
    if (f1Point.z()/CLHEP::mm<0.) {
      mdp.insert(" +==++++ NEGATIVE primary interaction ");
      mdp.dump();
    }else if (f1Point.z()/CLHEP::mm> 100.){
      mdp.insert(" +==++++ Out of range primary interaction ");
      mdp.dump();
    } else if (1000.*fIonL>100000.) {
      mdp.insert(" +==++++ Total ionization > 100000 um ");
      mdp.dump();    
    }
    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    std::cout <<" Number of final particles in the event "<<fPartMap.size()<<std::endl;
    G4int nphot=0;
    for (auto i=fPartMap.begin(); i!=fPartMap.end();i++){
      if (i->second.ParticleName()!="opticalphoton")
	std::cout <<" EVT++ : "<<i->second<<std::endl;
      if (i->first == 2) {
	f1Point=i->second.InitPos();
      }
      if (i->second.ParticleName()=="opticalphoton" && i->second.InitVol()=="Scintillator"){
	nphot++;
	G4double lambda = 1239.8/(i->second.InitKEne()/CLHEP::eV);
	analysisManager->FillH1(fHistLA,lambda);
	analysisManager->FillH1(fHistEA,i->second.InitKEne()/CLHEP::eV);
	f2Point=i->second.InitPos();
	G4ThreeVector fPoint=f2Point-f1Point;
	G4double ray = sqrt(fPoint.x()*fPoint.x()+fPoint.y()*fPoint.y());
	G4double dze = fPoint.z();
	analysisManager->FillH1(fHistzD,dze/CLHEP::mm);
	analysisManager->FillH1(fHistrD,ray/CLHEP::mm);
      }
    }
    analysisManager->FillH1(fHistNP,nphot);
    analysisManager->FillH1(fHistPr,f1Point.z()/CLHEP::mm);
    
  }
  void EventAction::AddIonLen(G4double ionlen){
    fIonL += ionlen;
  }
  
  void EventAction::FirstLastPosition(const G4ThreeVector& iPoint, const G4ThreeVector& lPoint){
    f1Point=iPoint;
    f2Point=lPoint;
  }
  void EventAction::nSteps(){
    nIntera++;
  }
  void EventAction::insertFinalParticle(Marcello::FinalParticle particle){
    fPartMap[particle.TrackId()]=particle;
  }

