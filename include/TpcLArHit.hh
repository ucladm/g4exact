#ifndef TpcLArHit_h
#define TpcLArHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"

class G4AttDef;
class G4AttValue;

// LArTPC hit
// records energy deposit

class TpcLArHit : public G4VHit
{
public:
  TpcLArHit();
  TpcLArHit(const TpcLArHit& right);
  virtual ~TpcLArHit();

  const TpcLArHit& operator=(const TpcLArHit& right);
  //int operator==(const TpcLArHit& right) const;

  inline void *operator new(size_t);
  inline void operator delete(void *aHit);

  //virtual void Draw();
  
  void SetEdep(G4double de) { _edep = de; }
  void AddEdep(G4double de) { _edep += de; }
  G4double GetEdep() const { return _edep; }


private:
  G4double _edep;
  
};


typedef G4THitsCollection<TpcLArHit> TpcLArHitsCollection;

extern G4ThreadLocal G4Allocator<TpcLArHit>* TpcLArHitAllocator;

inline void* TpcLArHit::operator new(size_t)
{
  if (!TpcLArHitAllocator)
    TpcLArHitAllocator = new G4Allocator<TpcLArHit>;
  return (void*) TpcLArHitAllocator->MallocSingle();
}

inline void TpcLArHit::operator delete(void* aHit)
{
  TpcLArHitAllocator->FreeSingle((TpcLArHit*) aHit);
}


#endif
