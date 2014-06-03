#include "RunAction.hh"
#include "Analysis.hh"

#include "G4Run.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"



RunAction::RunAction():
  G4UserRunAction()
{
  // Create the analysis manager
  G4AnalysisManager* anMan = G4AnalysisManager::Instance();
  G4cout << "Using "<<anMan->GetType() << G4endl;

  // Set default settings
  anMan->SetVerboseLevel(1);
  anMan->SetFileName("tpcsim");

  // Create histos
  anMan->CreateH1("Edeps_hist", "TPC Edeps", 100, 0., 2.*MeV); //id=0

  // Create ntuple
  anMan->CreateNtuple("TPC","TPCtree");
  anMan->CreateNtupleDColumn("Edep"); //id=0
  anMan->FinishNtuple();

}

RunAction::~RunAction()
{
  delete G4AnalysisManager::Instance();
}


void RunAction::BeginOfRunAction(const G4Run*)
{
  //inform the runManager to save random number seed
  //G4RunManager::GetRunManager()->SetRandomNumberStore(true);
  
  // Get analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  // Open an output file 
  // The default file name is set in RunAction::RunAction(),
  // it can be overwritten in a macro
  analysisManager->OpenFile();
}

void RunAction::EndOfRunAction(const G4Run*)
{
  // save histograms & ntuple
  //
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile(); 
}
