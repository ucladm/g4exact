#include "DetectorConstruction.hh"
#include "TpcLArSD.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4IntersectionSolid.hh"
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

  // Inner Cylinder
  G4double InnerCylinder_height = 0.5*teflon_height-(0.5*tpc_lAr_height+0.5*tpc_gAr_height)-0.1*cm;
  G4double InnerCylinder_z= -0.5*teflon_height+0.5*InnerCylinder_height;
  // Inch
  G4double inch = 2.54*cm;
  
  // Mini-Box length
  G4double miniLength= teflon_height-tpc_lAr_height-tpc_gAr_height-InnerCylinder_height-0.2*inch;
  G4double minidistance_z= 0.5*(teflon_height-miniLength);

  ///////////////////////////////////////////////////////////////////
  ///////////////////////////  GEOMETRIES ///////////////////////////
  ///////////////////////////////////////////////////////////////////

   //world volume / experimental hall
  G4VSolid* worldSolid
    = new G4Box("worldBox", 0.5*world_width, 0.5*world_width, 0.5*world_height);
  G4LogicalVolume* worldLogical
    = new G4LogicalVolume(worldSolid, air, "worldLogical");
  G4VPhysicalVolume* worldPhysical
    = new G4PVPlacement(0, G4ThreeVector(),
                        worldLogical, "worldPhysical",0,
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
	
 //TPC- Inner cylinder
  G4VSolid* InnerCylinder
   	= new G4Tubs("InnerCylinder",
                 0, 0.45*tpc_diam, 0.5*InnerCylinder_height,
                 0*deg, 360*deg);
  G4LogicalVolume* InnerCylinderLogical
    = new G4LogicalVolume(InnerCylinder, lAr, "InnerCylinderLogical");
   new G4PVPlacement(0, G4ThreeVector(0, 0,InnerCylinder_z),
        InnerCylinderLogical, "InnerCylinderPhysical", teflonLogical,
        false, 0,checkOverlaps);

//Box 1
G4VSolid* Box1
    = new G4Box("Box1", 0.5*inch, 0.5*inch,0.5*miniLength);
  G4LogicalVolume* Box1Logical
    = new G4LogicalVolume(Box1, lAr, "Box1Logical");
    new G4PVPlacement(0, G4ThreeVector(0,0,minidistance_z),
                        Box1Logical, "Box1Physical",teflonLogical,
                        false, 0, checkOverlaps);
//Box 2
G4VSolid* Box2
    = new G4Box("Box2", 0.5*inch, 0.5*inch, 0.5*miniLength);
  G4LogicalVolume* Box2Logical
    = new G4LogicalVolume(Box2, lAr, "Box2Logical");
    new G4PVPlacement(0, G4ThreeVector(1.2*inch,0,minidistance_z),
                        Box2Logical, "Box2Physical",teflonLogical,
                        false, 0, checkOverlaps);
//Box 3
G4VSolid* Box3
    = new G4Box("Box3", 0.5*inch, 0.5*inch, 0.5*miniLength);
  G4LogicalVolume* Box3Logical
    = new G4LogicalVolume(Box3, lAr, "Box3Logical");
    new G4PVPlacement(0, G4ThreeVector(-1.2*inch,0,minidistance_z),
                        Box3Logical, "Box3Physical",teflonLogical,
                        false, 0, checkOverlaps);
//Box 4
G4VSolid* Box4
    = new G4Box("Box4", 0.5*inch, 0.5*inch, 0.5*miniLength);
  G4LogicalVolume* Box4Logical
    = new G4LogicalVolume(Box4, lAr, "Box4Logical");
    new G4PVPlacement(0, G4ThreeVector(0.6*inch,1.2*inch,minidistance_z),
                        Box4Logical, "Box4Physical",teflonLogical,
                        false, 0, checkOverlaps);
//Box 5
G4VSolid* Box5
    = new G4Box("Box5", 0.5*inch, 0.5*inch, 0.5*miniLength);
  G4LogicalVolume* Box5Logical
    = new G4LogicalVolume(Box5, lAr, "Box5Logical");
    new G4PVPlacement(0, G4ThreeVector(-0.6*inch,1.2*inch,minidistance_z),
                        Box5Logical, "Box5Physical",teflonLogical,
                        false, 0, checkOverlaps);

//Box 6
G4VSolid* Box6
    = new G4Box("Box6", 0.5*inch,0.5*inch, 0.5*miniLength);
  G4LogicalVolume* Box6Logical
    = new G4LogicalVolume(Box6, lAr, "Box6Logical");
    new G4PVPlacement(0, G4ThreeVector(0.6*inch,-1.2*inch,minidistance_z),
                        Box6Logical, "Box6Physical",teflonLogical,
                        false, 0, checkOverlaps);
//Box 7
G4VSolid* Box7
    = new G4Box("Box7", 0.5*inch, 0.5*inch, 0.5*miniLength);
  G4LogicalVolume* Box7Logical
    = new G4LogicalVolume(Box7, lAr, "Box7Logical");
    new G4PVPlacement(0, G4ThreeVector(-0.6*inch,-1.2*inch,minidistance_z),
                        Box7Logical, "Box7Physical",teflonLogical,
                        false, 0, checkOverlaps);



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
