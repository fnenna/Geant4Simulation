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
// \brief Implementation of the TrackingAction class
//
//

#include "TrackingAction.hh"
//#include "TrackInformation.hh"

#include "G4Track.hh"
#include "G4TrackingManager.hh"
#include <fstream>
#include <iostream>
#include <iomanip>



  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  TrackingAction::TrackingAction(EventAction* evac )
    : G4UserTrackingAction(), fEventAction(evac)
  {
    // Open the CSV file for writing (create if it doesn't exist)
    outputFile.open("tracking_data.csv");
    // Check if the file was successfully opened
    if (!outputFile.is_open()) {
        std::cerr << "Error: Could not open or create tracking_data.csv" << std::endl;
        // Handle the error (exit, throw exception, etc.)
    } else {
        // Write the header to the CSV file
        outputFile << "TrackID,ParentID,ParticleName,ParticleID,InitProcName,InitVol,InitKEne,"
                      "InitPosX,InitPosY,InitPosZ,InitTime,FinProcName,FinKEne,"
                      "FinPosX,FinPosY,FinPosZ,FinVol,FinTime\n";
    } }

    TrackingAction::~TrackingAction() {
    // Close the CSV file when the TrackingAction is destroyed
    if (outputFile.is_open()) {
        outputFile.close();
    }
    }
  
  void TrackingAction::PreUserTrackingAction(const G4Track* tra)
  {

    const G4DynamicParticle* dynamicParticle = tra->GetDynamicParticle();
    G4ParticleDefinition* particle = dynamicParticle->GetDefinition();
    _p.clear();
    if (fEventAction->mapFinalParticles().find(tra->GetTrackID())==fEventAction->mapFinalParticles().end()){
      _p.setTrackId(tra->GetTrackID());
      _p.setParentTrackId(tra->GetParentID());
      _p.setParticleName(particle->GetParticleName());
      _p.setParticleId(particle->GetPDGEncoding());
      if (tra->GetTrackID()!=1){
	_p.setInitProcName(tra->GetCreatorProcess()->GetProcessName());
	//	_p.setInitVol("Word");
      } else {
	_p.setInitProcName("Generator");
	//	
      }
      _p.setInitVol(tra->GetVolume()->GetName());
      _p.setInitKEne(dynamicParticle->GetKineticEnergy());
      _p.setInitPos(tra->GetPosition());
      _p.setInitTime(tra->GetGlobalTime()/CLHEP::ps);
      //      std::cout<<" PPPPPPP "<<_p<<std::endl;
      fEventAction->insertFinalParticle(_p);
    }
  
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  void TrackingAction::PostUserTrackingAction(const G4Track* tra)
  {
    const G4DynamicParticle* dynamicParticle = tra->GetDynamicParticle();
    //    G4ParticleDefinition* particle = dynamicParticle->GetDefinition();
    
    const G4Step* step=tra->GetStep();
    G4StepPoint* postStepPoint = step->GetPostStepPoint();
    G4String procout = postStepPoint->GetProcessDefinedStep()->GetProcessName();
    if (tra->GetTrackID() == 1){
      G4cout <<" PostUserTrackingAction "<<tra->GetTrackID()<<" proc "<<procout<<" time "<<tra->GetGlobalTime()/CLHEP::ps<<" Energy "<<dynamicParticle->GetKineticEnergy()<<" position "<<tra->GetPosition()<<G4endl;
    }
    _p=fEventAction->mapFinalParticles()[tra->GetTrackID()];
    _p.setFinProcName(procout);
    _p.setFinKEne(dynamicParticle->GetKineticEnergy());
    _p.setFinPos(tra->GetPosition());
    _p.setFinVol(tra->GetVolume()->GetName());
    _p.setFinTime(tra->GetGlobalTime()/CLHEP::ps);
    fEventAction->insertFinalParticle(_p);

    // Write the particle data to the CSV file
    if (outputFile.is_open()) {
        outputFile << _p.getTrackId() << ","
                   << _p.getParentTrackId() << ","
                   << _p.getParticleName() << ","
                   << _p.getParticleId() << ","
                   << _p.getInitProcName() << ","
                   << _p.getInitVol() << ","
                   << _p.getInitKEne() << ","
                   << _p.getInitPos().x() << ","
                   << _p.getInitPos().y() << ","
                   << _p.getInitPos().z() << ","
                   << _p.getInitTime() << ","
                   << _p.getFinProcName() << ","
                   << _p.getFinKEne() << ","
                   << _p.getFinPos().x() << ","
                   << _p.getFinPos().y() << ","
                   << _p.getFinPos().z() << ","
                   << _p.getFinVol() << ","
                   << _p.getFinTime()
                   << "\n";
          outputFile.flush(); // Ensure the data is written


    }
  }

