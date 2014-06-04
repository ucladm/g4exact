#include "EventAction.hh"
#include "Analysis.hh"
#include "TpcLArHit.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

EventAction::EventAction():
  G4UserEventAction(),
  _tpcHCID(-1)
{
  // set printing per each event
  G4RunManager::GetRunManager()->SetPrintProgress(1); 
}


EventAction::~EventAction()
{ }

void EventAction::BeginOfEventAction(const G4Event*)
{
  if (_tpcHCID==-1) {
    G4SDManager* sdMan = G4SDManager::GetSDMpointer();
    _tpcHCID = sdMan->GetCollectionID("TpcLArSD/TpcLArColl");
  }
}

void EventAction::EndOfEventAction(const G4Event* event)
{
  G4HCofThisEvent* hce = event->GetHCofThisEvent();
  if (!hce) {
    G4ExceptionDescription msg;
    msg << "No hits collection of this event found.\n"; 
    G4Exception("EventAction::EndOfEventAction()",
                "Code001", JustWarning, msg);
    return;
  }

  // Get hits collection
  TpcLArHitsCollection* tpcHC
    = static_cast<TpcLArHitsCollection*>(hce->GetHC(_tpcHCID));

  if ( !tpcHC ) {
    G4ExceptionDescription msg;
    msg << "Some of hits collections of this event not found.\n"; 
    G4Exception("EventAction::EndOfEventAction()",
                "Code001", JustWarning, msg);
    return;
  }


  // Fill histos and ntuple

  // Get analysis manager
  G4AnalysisManager* anMan = G4AnalysisManager::Instance();


  //G4int n_hit = tpcHC->entries();
  //anMan->FillH1(0, n_hit);

  TpcLArHit* hit = (*tpcHC)[0];
  G4double edep = hit->GetEdep();
  anMan->FillH1(0, edep);
  anMan->FillNtupleDColumn(0, edep);
  anMan->AddNtupleRow();
}
