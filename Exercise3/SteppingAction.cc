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
/// \file SteppingAction.cc
/// \brief Implementation of the BASEVOLUME::SteppingAction class

#include "SteppingAction.hh"
#include "EventAction.hh"
#include "construction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4AnalysisManager.hh"

#include "G4OpticalPhoton.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(EventAction* eventAction)
: fEventAction(eventAction)
{
    // Open the file for writing
    outputFile.open("stepping_data.csv");

    // Check if the file opened successfully
    if (!outputFile.is_open()) {
        G4cerr << "Error: Could not open file stepping_data.csv" << G4endl;
        return;
    }

    // Write the header line to the CSV file
    outputFile << "TrackID,ParentID,ParticleName,StepStartX,StepStartY,StepStartZ,"
               << "StepEndX,StepEndY,StepEndZ,KineticEnergyBefore(keV),"
               << "KineticEnergyAfter(keV),EnergyDeposited(keV),"
               << "StepLength(mm),VolumeName" << std::endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{
    // Close the file if it was opened successfully
    if (outputFile.is_open()) {
        outputFile.close();
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* step)
{

  if (!fScoringVolume) {
    const MyDetectorConstruction* detConstruction
      = static_cast<const MyDetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    fScoringVolume = detConstruction->GetScoringVolume();
  }

  // get volume of the current step
  G4LogicalVolume* volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
  
  std::stringstream&  p=fEventAction->msg().instance();

  G4Track* tra= step->GetTrack();
  const G4DynamicParticle* dynamicParticle = tra->GetDynamicParticle();
  G4ParticleDefinition* particle = dynamicParticle->GetDefinition();

  static G4ParticleDefinition* opticalphoton =
    G4OpticalPhoton::OpticalPhotonDefinition();
  if (particle == opticalphoton){
    p<<" =======>>>> Optical photons ";
  }

  
  p<<" =======>> Particle in this step "<< particle->GetParticleName();
  G4StepPoint* preStepPoint = step->GetPreStepPoint();
  G4StepPoint* postStepPoint = step->GetPostStepPoint();

 
  p <<"\n +++ Step  start: "<<preStepPoint->GetPosition()<<" stop:"<<postStepPoint->GetPosition()<<std::endl;
  if (preStepPoint ->GetStepStatus() == fGeomBoundary) {
    p << "Step starts on geometry boundary: Entered the Scintillator" << std::endl;
    p  <<" Id "<<tra->GetTrackID()<<" Paret ID "<<tra->GetParentID()<<std::endl;
    p <<" Process out "<<postStepPoint->GetProcessDefinedStep()->GetProcessName()<<std::endl;
    p <<" ENTERING "<<preStepPoint->GetMaterial()->GetName()<<" MATERIAL "<<std::endl;
    if (volume == fScoringVolume){
      iPoint = postStepPoint->GetPosition();
    }
  }
  else if (postStepPoint->GetStepStatus() == fGeomBoundary) {
    p << "Step ends on geometry boundary: Entering the Scintillator" << std::endl;
    p <<" Process out "<<postStepPoint->GetProcessDefinedStep()->GetProcessName()<<std::endl;
    G4Material* nextMaterial = postStepPoint->GetMaterial();
    p <<nextMaterial->GetName()<<std::endl;
  }
  else {
    p <<" Stepping"<<std::endl;
    p <<" Id "<<tra->GetTrackID()<<" Parent ID "<<tra->GetParentID()<<std::endl;
    if (tra->GetTrackID() != 1) {
      p <<" Id "<<tra->GetTrackID()<<" Parent ID "<<tra->GetParentID()
	<<" creator process "
	<<tra->GetCreatorProcess()->GetProcessName()
	//		<<" step in Process "<<preStepPoint->GetProcessDefinedStep()->GetProcessName()
	<<" step out Process "<<postStepPoint->GetProcessDefinedStep()->GetProcessName()
	<<std::endl;
      p <<" Particle "<<particle->GetParticleName()
	<<" Energy "<<dynamicParticle->GetKineticEnergy()/CLHEP::keV<<" keV"
	<<" before "<<preStepPoint->GetKineticEnergy()/CLHEP::keV<<" keV"
	<<" after "<<postStepPoint->GetKineticEnergy()/CLHEP::keV<<" keV"
	<<std::endl;
      if (particle->GetParticleName()=="e-" && volume == fScoringVolume) {
	fEventAction->AddIonLen(step->GetStepLength()/CLHEP::mm);	
	fEventAction->nSteps();
	fEventAction->FirstLastPosition(iPoint,postStepPoint->GetPosition());	
	// collect energy deposited in this step
	G4double edepStep = step->GetTotalEnergyDeposit();
	p <<" Deposited Energy in the step "<<edepStep/CLHEP::keV<<" keV in "
	  <<step->GetStepLength()/CLHEP::mm<<" mm"<<std::endl;
      }
    }
  }  

  // Extract the necessary data
    G4int trackID = tra->GetTrackID();
    G4int parentID = tra->GetParentID();
    G4String particleName = particle->GetParticleName();
    G4ThreeVector stepStart = preStepPoint->GetPosition();
    G4ThreeVector stepEnd = postStepPoint->GetPosition();
    G4double kineticEnergyBefore = preStepPoint->GetKineticEnergy();
    G4double kineticEnergyAfter = postStepPoint->GetKineticEnergy();
    G4double energyDeposited = step->GetTotalEnergyDeposit();
    G4double stepLength = step->GetStepLength();

    // Write the data to the CSV file
    if (outputFile.is_open()) {
        outputFile << trackID << ","
                   << parentID << ","
                   << particleName << ","
                   << stepStart.x() << "," << stepStart.y() << "," << stepStart.z() << ","
                   << stepEnd.x() << "," << stepEnd.y() << "," << stepEnd.z() << ","
                   << kineticEnergyBefore / CLHEP::keV << ","
                   << kineticEnergyAfter / CLHEP::keV << ","
                   << energyDeposited / CLHEP::keV << ","
                   << stepLength / CLHEP::mm << ","
                   << volume->GetName() << std::endl;
    } else {
        G4cerr << "Error: Unable to write to stepping_data.csv" << G4endl;
    }

    // check if we are in scoring volume
    if (volume != fScoringVolume) return;


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......