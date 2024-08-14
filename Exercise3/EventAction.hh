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
/// \file EventAction.hh
/// \brief Definition of the SINGLESCINT::EventAction class

#ifndef EVENTACTION_HH
#define EVENTACTION_HH

#include "G4UserEventAction.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"
#include "Dumper.hh"
#include "FinalParticle.hh"
#include <vector>

/// Event action class
///

class RunAction;

class EventAction : public G4UserEventAction
{
public:
  EventAction(RunAction* runAction);
  ~EventAction() override;
  
  void BeginOfEventAction(const G4Event* event) override;
  void EndOfEventAction(const G4Event* event) override;
  
  void AddEdep(G4double edep) { fEdep += edep; }
  
  void AddIonLen(G4double ionlen);
  void FirstLastPosition(const G4ThreeVector& iPoint, const G4ThreeVector& lPoint);
  void nSteps();
  Marcello::Dumper& msg(){return mdp;}
  void insertFinalParticle(Marcello::FinalParticle particle);
  std::map<G4int, Marcello::FinalParticle>&  mapFinalParticles(){return fPartMap;}
  
  
private:
  G4double   fEdep = 0.;
  unsigned int    nEvent=0;
  G4double  fIonL = 0.;
  G4int  nIntera=0;
  G4ThreeVector f1Point,f2Point;
  
private:
  std::map<G4int, Marcello::FinalParticle> fPartMap;
  Marcello::Dumper mdp;
  
private:
  G4int fHistNP;
  G4int fHistLA;
  G4int fHistEA;
  G4int fHistPD;
  G4int fHistzD;
  G4int fHistrD;
  G4int fHistPr;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


