#include "TpcLArHit.hh"



G4ThreadLocal G4Allocator<TpcLArHit>* TpcLArHitAllocator;


TpcLArHit::TpcLArHit():
  _edep(0.)
{ }

TpcLArHit::~TpcLArHit()
{ }

TpcLArHit::TpcLArHit(const TpcLArHit& right):
  G4VHit()
{
  _edep = right._edep;
}

const TpcLArHit& TpcLArHit::operator=(const TpcLArHit& right)
{
  _edep = right._edep;
  return *this;
}


