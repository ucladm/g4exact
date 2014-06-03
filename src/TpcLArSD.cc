#include "TpcLArSD.hh"
#include "TpcLArHit.hh"

#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"


TpcLArSD::TpcLArSD(G4String name):
  G4VSensitiveDetector(name), _hitsCollection(0), _hcid(-1)
{
  collectionName.insert("TpcLArColl");
}

TpcLArSD::~TpcLArSD()
{ }

void TpcLArSD::Initialize(G4HCofThisEvent* hce)
{
  _hitsCollection
    = new TpcLArHitsCollection(SensitiveDetectorName, collectionName[0]);
  if (_hcid<0)
    _hcid = G4SDManager::GetSDMpointer()->GetCollectionID(_hitsCollection);
  hce->AddHitsCollection(_hcid, _hitsCollection);

  // fill tpc with zero energy deposition
  TpcLArHit* hit = new TpcLArHit();
  _hitsCollection->insert(hit);
}

G4bool TpcLArSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
  G4double edep = step->GetTotalEnergyDeposit();
  if (edep==0.) return true;

  G4TouchableHistory* touchable
    = (G4TouchableHistory*)(step->GetPreStepPoint()->GetTouchable());
  G4VPhysicalVolume* physical = touchable->GetVolume();
  G4int copyNo = physical->GetCopyNo();

  TpcLArHit* hit = (*_hitsCollection)[copyNo];
  // add energy deposition
  hit->AddEdep(edep);

  return true;
}
