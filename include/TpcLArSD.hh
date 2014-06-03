#ifndef TpcLArSD_h
#define TpcLArSD_h 1

#include "G4VSensitiveDetector.hh"
#include "TpcLArHit.hh"

class G4step;
class G4HCofThisEvent;
class G4TouchableHistory;

class TpcLArSD : public G4VSensitiveDetector
{
public:
  TpcLArSD(G4String name);
  virtual ~TpcLArSD();
  virtual void Initialize(G4HCofThisEvent* hce);
  virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist);

private:
  TpcLArHitsCollection* _hitsCollection;
  G4int _hcid;

};




#endif
