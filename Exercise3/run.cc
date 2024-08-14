#include "run.hh"

MyRunAction::MyRunAction()
{}

MyRunAction::~MyRunAction()
{}

void MyRunAction::BeginOfRunAction(const G4Run*)
{
	G4RootAnalysisManager *man = G4RootAnalysisManager::Instance();
	
	man -> OpenFile("output.root");
	
	man ->CreateNtuple("Hits", "Hits");
	man->CreateNtupleIColumn("fEvent");
	man->CreateNtupleDColumn("fX");
	man->CreateNtupleDColumn("fY");
	man->CreateNtupleDColumn("fZ");
	man->FinishNtuple(0);
	
	man ->CreateNtuple("Scoring", "Scoring");
	man ->CreateNtupleDColumn("fEdep");
	man->FinishNtuple(1);
}

void MyRunAction::EndOfRunAction(const G4Run*)
{
	G4RootAnalysisManager *man = G4RootAnalysisManager::Instance();
	
	man -> Write();
	man -> CloseFile();
}
