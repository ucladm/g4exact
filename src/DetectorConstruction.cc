#include "DetectorConstruction.hh"


#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction():
  G4VUserDetectorConstruction()
{ }


DetectorConstruction::~DetectorConstruction()
{ }


G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();

  G4bool checkOverlaps = true;
  
  G4double world_xy = 30*cm;
  G4double world_z  = 100*cm;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");


  ///////////////////////////////////////////////////////////////////
  ///////////////////////////  GEOMETRIES ///////////////////////////
  ///////////////////////////////////////////////////////////////////

  // world volume / experimental hall
  G4VSolid* worldSolid
    = new G4Box("worldBox", 0.5*world_xy, 0.5*world_xy, 0.5*world_z);
  G4LogicalVolume* worldLogical
    = new G4LogicalVolume(worldSolid, world_mat, "worldLogical");
  G4VPhysicalVolume* worldPhysical
    = new G4PVPlacement(0, G4ThreeVector(), worldLogical, "worldPhysical", 0, false, 0, checkOverlaps);
  

  // must return world physical volume
  return worldPhysical;
  
}
