#include "construction.hh"
#include "G4VisAttributes.hh"
#include "G4Text.hh"
#include "G4Colour.hh"
#include "G4VVisManager.hh"


MyDetectorConstruction::MyDetectorConstruction()
{
	//fMessenger = new G4GenericMessenger(this, "/detector/", "DetectorConstruction");
	
	//fMessenger->DeclareProperty("nCols", nCols, "Number of columns");
	//fMessenger->DeclareProperty("nRows", nCols, "Number of rows");
	
}

MyDetectorConstruction::~MyDetectorConstruction()
{}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
	G4NistManager *nist = G4NistManager::Instance();
	
	G4Material *BGO = new G4Material("BGO", 7.1300*g/cm3, 3);
	BGO->AddElement(nist->FindOrBuildElement("Bi"),1);
	BGO->AddElement(nist->FindOrBuildElement("Ge"),1);
	BGO->AddElement(nist->FindOrBuildElement("O"),1);
	/*
	G4MaterialPropertiesTable* scint_mpt = new G4MaterialPropertiesTable();
  	std::vector<G4double> scint_En={1.*CLHEP::eV, 2.5*CLHEP::eV, 3.0*CLHEP::eV, 4.0*CLHEP::eV,5.*CLHEP::eV};
  	std::vector<G4double> scint_In={0.1, 0.5, 0.9, 0.1,0.0};
 	std::vector<G4double> scint_RI={2.15, 2.15, 2.15, 2.15,2.15};
  	std::vector<G4double> scint_AL={1.118*CLHEP::cm, 1.118*CLHEP::cm, 1.118*CLHEP::cm, 1.118*CLHEP::cm, 1.118*CLHEP::cm};
  	G4bool spl=true;
	G4PhysicsFreeVector* pip= new G4PhysicsFreeVector(scint_En,scint_In,spl);
  	std::cout <<" PIP SPLIN "<<pip->GetSpline()<<std::endl;
  	std::cout <<" SPLINE ACTIVATED "<<std::endl;
  
  	//  auto pippo=scint_mpt->AddProperty("SCINTILLATIONCOMPONENT1",scint_En,scint_In,nwk,spl);
  	scint_mpt->AddProperty("SCINTILLATIONCOMPONENT1",pip);
	scint_mpt->AddProperty("RINDEX",scint_En,scint_RI,4);
  	scint_mpt->AddProperty("ABSLENGTH",scint_En,scint_AL,4);
  	//  scint_mpt->AddConstProperty("SCINTILLATIONYIELD", 8./CLHEP::keV);
  	scint_mpt->AddConstProperty("SCINTILLATIONYIELD", 0./CLHEP::MeV);
  	scint_mpt->AddConstProperty("RESOLUTIONSCALE", 1.);
  	scint_mpt->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 300*CLHEP::ns);
  	BGO->SetMaterialPropertiesTable(scint_mpt);
  	std::cout <<"++++++++++++++++BGO+++++++++"<<std::endl;
  	std::cout <<BGO;
  	std::cout <<std::endl;
  	std::cout <<"++++++++++++++++BGO+PROP+++++"<<std::endl;
  	scint_mpt->DumpTable();
  	std::cout <<"++++++++++++++++BGO+END+++++"<<std::endl; 
	*/
	

	G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR");
	
	G4Box *solidWorld = new G4Box("solidWorld", 0.15*m, 0.15*m, 0.15*m);
	G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
	G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);
	
	G4Box *solidRadiator = new G4Box("solidRadiator", 0.01*m, 0.01*m, 0.1*m);
	
	G4LogicalVolume *logicRadiator = new G4LogicalVolume(solidRadiator, BGO, "logicalRadiator");
	
	fScoringVolume = logicRadiator;
	
	
	G4VPhysicalVolume *physRadiator = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicRadiator, "physRadiator", logicWorld, false, 0, true);
	
	G4VisAttributes* visAttributes = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0, 0.5)); // Blue color
	// Optionally, set other visual attributes
	visAttributes->SetVisibility(true);  // Make sure the volume is visible
	visAttributes->SetForceSolid(true);  // Force the volume to be drawn as a solid

	// Attach the visualization attributes to the logical volume
	logicRadiator->SetVisAttributes(visAttributes);
	// Draw text on the scene
	// Set up visualization attributes with the desired color
	G4VisAttributes visAttributesColor(G4Colour(1.0, 1.0, 0.0));  // Blue color

	// Apply the visualization attributes to the text
	G4Text text("BGO Volume", G4Point3D(0.07*m, 0.07*m, 0));  // Position at (0,0,50)
	text.SetVisAttributes(visAttributes);
	text.SetScreenSize(100);  // Text size

	G4VVisManager* visManager = G4VVisManager::GetConcreteInstance();
	if (visManager) {
    	visManager->Draw(text);
	}
	/*
	G4Box *solidDetector = new G4Box("solidDetector", 0.005*m, 0.005*m, 0.01*m);
	
	logicDetector = new G4LogicalVolume(solidDetector, worldMat, "logicDetector");
	
	for(G4int i=0; i<100; i++)
	{
		for(G4int j=0; j<100; j++)
		{
			G4VPhysicalVolume *physDetector = new G4PVPlacement(0, G4ThreeVector(-0.5*m + (i+0.5)*m/100, -0.5*m + (j+0.5)*m/100, (0.5-0.01)*m), logicDetector, "physDetector", logicWorld, false, j+i*100, true);
		}
	}
	*/
	
	return physWorld;
}

/*
void MyDetectorConstruction::ConstructSDandField()
{
	MySensitiveDetector *sensDet = new MySensitiveDetector("SensitiveDetector");
	
	logicDetector -> SetSensitiveDetector(sensDet);
}
*/




