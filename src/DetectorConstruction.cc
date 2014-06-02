#include "DetectorConstruction.hh"


#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
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


  ///////////////////////////////////////////////////////////////////
  /////////////////////////// MATERIALS /////////////////////////////
  ///////////////////////////////////////////////////////////////////

  G4Material* air = nist->FindOrBuildMaterial("G4_AIR");
  G4Material* LAr = nist->FindOrBuildMaterial("G4_lAr");
  G4Material* teflon = nist->FindOrBuildMaterial("G4_TEFLON");

  
  ///////////////////////////////////////////////////////////////////
  /////////////////////////// DIMENSIONS ////////////////////////////
  ///////////////////////////////////////////////////////////////////

  // experimental hall
  G4double world_width = 30*cm;
  G4double world_height  = 100*cm;

  // LAr bath
  G4double lar_bath_diam = 20*cm;
  G4double lar_bath_height = 75*cm;

  // TPC
  G4double tpc_id = 7.6*cm;
  G4double tpc_od = 9.9*cm;
  G4double tpc_height = 9.9*cm;
  G4double tpc_top_cap_height = 2.54*cm;
  G4double tpc_bot_cap_height = 2.54*cm;

  ///////////////////////////////////////////////////////////////////
  ///////////////////////////  GEOMETRIES ///////////////////////////
  ///////////////////////////////////////////////////////////////////

  // world volume / experimental hall
  G4VSolid* worldSolid
    = new G4Box("worldBox", 0.5*world_width, 0.5*world_width, 0.5*world_height);
  G4LogicalVolume* worldLogical
    = new G4LogicalVolume(worldSolid, air, "worldLogical");
  G4VPhysicalVolume* worldPhysical
    = new G4PVPlacement(0, G4ThreeVector(),
                        worldLogical, "worldPhysical", 0,
                        false, 0, checkOverlaps);

  // LAr bath
  G4VSolid* LArBathSolid
    = new G4Tubs("LArBathSolid",
                 0., 0.5*lar_bath_diam, 0.5*lar_bath_height,
                 0*deg, 360*deg);
  G4LogicalVolume* LArBathLogical
    = new G4LogicalVolume(LArBathSolid, LAr, "LArBathLogical");
  new G4PVPlacement(0, G4ThreeVector(0,0,-0.5*(world_height-lar_bath_height)),
                    LArBathLogical, "LArBathPhysical", worldLogical,
                    false, 0, checkOverlaps);
  
  // TPC
  // composed of union of tube with end caps
  G4VSolid* tpcTubeSolid
    = new G4Tubs("tpcTubeSolid",
                 0.5*tpc_id, 0.5*tpc_od, 0.5*tpc_height,
                 0*deg, 360*deg);
  G4VSolid* tpcTopSolid
    = new G4Tubs("tpcTopSolid",
                 0., 0.5*tpc_od, 0.5*tpc_top_cap_height,
                 0*deg, 360*deg);
  G4VSolid* tpcBotSolid
    = new G4Tubs("tpcBotSolid",
                 0., 0.5*tpc_od, 0.5*tpc_bot_cap_height,
                 0*deg, 360*deg);
  G4UnionSolid* tpcUnion1Solid
    = new G4UnionSolid("Tube+Top", tpcTubeSolid, tpcTopSolid,
                       0, G4ThreeVector(0, 0, 0.5*(tpc_height+tpc_top_cap_height)) );
  G4UnionSolid* tpcUnion2Solid
    = new G4UnionSolid("Tube+Top+Bot", tpcUnion1Solid, tpcBotSolid,
                       0, G4ThreeVector(0, 0, -0.5*(tpc_height+tpc_bot_cap_height)) );
  G4LogicalVolume* tpcLogical
    = new G4LogicalVolume(tpcUnion2Solid, teflon, "tpcLogical");
  new G4PVPlacement(0, G4ThreeVector(),
                    tpcLogical, "tpcPhysical", LArBathLogical,
                    false, 0, checkOverlaps);
  
  // must return world physical volume
  return worldPhysical;
  
}
