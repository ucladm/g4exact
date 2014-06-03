#include "DetectorConstruction.hh"
#include "TpcLArSD.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"
#include "G4VSensitiveDetector.hh"
#include "G4ios.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction():
  G4VUserDetectorConstruction(),
  _tpcLArLogical(0)
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
  G4Material* lAr = nist->FindOrBuildMaterial("G4_lAr");
  G4Material* gAr = nist->FindOrBuildMaterial("G4_Ar"); // Should build custom GAr with higher density
  G4Material* teflon = nist->FindOrBuildMaterial("G4_TEFLON");

  
  ///////////////////////////////////////////////////////////////////
  /////////////////////////// DIMENSIONS ////////////////////////////
  ///////////////////////////////////////////////////////////////////

  // experimental hall
  G4double world_width = 30*cm;
  G4double world_height  = 100*cm;

  // LAr bath
  G4double lar_bath_diam = 20*cm;
  G4double lar_bath_height = 80*cm;

  // TPC
  G4double tpc_diam = 7.6*cm;
  G4double tpc_lAr_height = 7.6*cm;
  G4double tpc_gAr_height = 0.8*cm;

  // Teflon body
  G4double teflon_diam = 9.9*cm;
  G4double teflon_height = 13.5*cm;


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
    = new G4LogicalVolume(LArBathSolid, lAr, "LArBathLogical");
  new G4PVPlacement(0, G4ThreeVector(0,0,-0.5*(world_height-lar_bath_height)),
                    LArBathLogical, "LArBathPhysical", worldLogical,
                    false, 0, checkOverlaps);

  // Teflon body
  G4VSolid* teflonSolid
    = new G4Tubs("teflonSolid",
                 0, 0.5*teflon_diam, 0.5*teflon_height,
                 0*deg, 360*deg);
  G4LogicalVolume* teflonLogical
    = new G4LogicalVolume(teflonSolid, teflon, "teflonLogical");
  new G4PVPlacement(0, G4ThreeVector(),
                    teflonLogical, "teflonPhysical", LArBathLogical,
                    false, 0, checkOverlaps);

  
  // TPC - LAr
  G4VSolid* tpcLArSolid
    = new G4Tubs("tpcLArSolid",
                 0, 0.5*tpc_diam, 0.5*tpc_lAr_height,
                 0*deg, 360*deg);
  _tpcLArLogical
    = new G4LogicalVolume(tpcLArSolid, lAr, "tpcLArLogical");
  new G4PVPlacement(0, G4ThreeVector(0, 0, -0.5*tpc_gAr_height),
                    _tpcLArLogical, "tpcLArPhysical", teflonLogical,
                    false, 0, checkOverlaps);

  
  // TPC - GAr; has same mother as LAr
  G4VSolid* tpcGArSolid
    = new G4Tubs("tpcGArSolid",
                 0, 0.5*tpc_diam, 0.5*tpc_gAr_height,
                 0*deg, 360*deg);
  G4LogicalVolume* tpcGArLogical
    = new G4LogicalVolume(tpcGArSolid, gAr, "tpcGArLogical");
  new G4PVPlacement(0, G4ThreeVector(0, 0, 0.5*tpc_lAr_height),
                    tpcGArLogical, "tpcGArPhysical", teflonLogical,
                    false, 0, checkOverlaps);
  
  
  // must return world physical volume
  return worldPhysical;
  
}


void DetectorConstruction::ConstructSDandField()
{
  
  // sensitive detectors -----------------------------------------------------
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  G4String SDname;
  
  G4VSensitiveDetector* tpcLAr
    = new TpcLArSD(SDname="/TpcLArSD");
  SDman->AddNewDetector(tpcLAr);
  _tpcLArLogical->SetSensitiveDetector(tpcLAr);
  
  
}
