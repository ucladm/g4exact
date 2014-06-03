#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "Randomize.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


PrimaryGeneratorAction::PrimaryGeneratorAction():
  G4VUserPrimaryGeneratorAction(),
  fParticleGun(0)
{
  G4int n_particle = 1;
  fParticleGun = new G4ParticleGun(n_particle);

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle
    = particleTable->FindParticle(particleName="gamma");
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticlePosition(G4ThreeVector(0,0,-10*cm));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0,0,1));
  fParticleGun->SetParticleEnergy(1.*MeV);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
  
  G4double theta = G4UniformRand()*2.*pi;
  G4double u = G4UniformRand()*2.-1;
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(std::sqrt(1.-u*u)*std::cos(theta),
                                                           std::sqrt(1.-u*u)*std::sin(theta),
                                                           u) );
  fParticleGun->GeneratePrimaryVertex(event);
}
