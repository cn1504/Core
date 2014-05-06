#include "Assets.h"

namespace Core
{
	namespace Assets
	{
		std::unordered_map<std::string, Mesh*> Meshes;
		std::unordered_map<std::string, Material*> Materials;
		

		void CreateStandardMaterials()
		{
			Material* Al = new Material("Aluminum");
			Al->DiffuseColor = glm::vec3(0.588, 0.579, 0.579);
			Al->DiffuseIntensity = 0.844f;
			Al->SpecularColor = glm::vec3(1.0, 1.0, 1.0);
			Al->SpecularHardness = 5.0f;
			Al->SpecularIntensity = 0.5f;
			Al->Density = 2.7e3f;
			Al->YoungsModulus = 7.0e10f;
			Al->BulkModulus = 7.5e10f;
			Al->ShearModulus = 2.5e10f;
			Al->YieldStrength = 18e6f;
			Al->UltimateStrength = 45e6f;

			Material* brass = new Material("Brass");
			brass->DiffuseColor = glm::vec3(0.726, 0.582, 0.172);
			brass->DiffuseIntensity = 0.843f;
			brass->SpecularColor = glm::vec3(0.895, 0.895, 0.787);
			brass->SpecularHardness = 5.0f;
			brass->SpecularIntensity = 0.5f;
			brass->IndexOfRefraction = 5.575f;
			brass->Density = 8.6e3f;
			brass->YoungsModulus = 9.0e10f;
			brass->BulkModulus = 6.0e10f;
			brass->ShearModulus = 3.5e10f;
			brass->YieldStrength = 200.0e6f;
			brass->UltimateStrength = 550.0e6f;
			
			Material* Cu = new Material("Copper");
			Cu->DiffuseColor = glm::vec3(1.0, 0.51, 0.5);
			Cu->DiffuseIntensity = 0.8f;
			Cu->SpecularColor = glm::vec3(1.0, 1.0, 0.8);
			Cu->SpecularIntensity = 0.5f;
			Cu->SpecularHardness = 5.0f;
			Cu->Density = 8.92e3f;
			Cu->YoungsModulus = 11.0e10f;
			Cu->BulkModulus = 14.0e10f;
			Cu->ShearModulus = 4.4e10f;
			Cu->YieldStrength = 70e6f;
			Cu->UltimateStrength = 220e6f;
			
			Material* glass = new Material("Glass");
			glass->DiffuseColor = glm::vec3(1.0, 1.0, 1.0);
			glass->DiffuseIntensity = 0.5f;
			glass->SpecularColor = glm::vec3(1.0, 1.0, 1.0);
			glass->SpecularIntensity = 0.5f;
			glass->SpecularHardness = 5.0f;
			glass->Density = 2.53e3f;
			glass->YoungsModulus = 6.0e10f;
			glass->BulkModulus = 5.0e10f;
			glass->ShearModulus = 2.5e10f;
			glass->YieldStrength = 33.0e6f;
			glass->UltimateStrength = 33.0e6f;
			
			Material* Fe = new Material("Iron");
			Fe->DiffuseColor = glm::vec3(0.805, 0.804, 0.917);
			Fe->DiffuseIntensity = 0.203f;
			Fe->SpecularColor = glm::vec3(0.97, 0.97, 1.0);
			Fe->SpecularHardness = 5.0f;
			Fe->SpecularIntensity = 0.109f;
			Fe->Density = 7.8e3f;
			Fe->YoungsModulus = 21.0e10f;
			Fe->BulkModulus = 17.0e10f;
			Fe->ShearModulus = 7.8e10f;
			Fe->YieldStrength = 90.0e6f;
			Fe->UltimateStrength = 350e6f;

			Material* Pb = new Material("Lead");
			Pb->DiffuseColor = glm::vec3(0.435, 0.38, 0.38);
			Pb->DiffuseIntensity = 0.887f;
			Pb->SpecularColor = glm::vec3(0.59, 0.567, 0.575);
			Pb->SpecularIntensity = 0.384f;
			Pb->SpecularHardness = 5.0f;
			Pb->Density = 11.34e3f;
			Pb->YoungsModulus = 1.6e10f;
			Pb->BulkModulus = 4.1e10f;
			Pb->ShearModulus = 0.6e10f;
			Pb->YieldStrength = 1.0e6f;
			Pb->UltimateStrength = 12.0e6f;

			Material* Ni = new Material("Nickel");
			Ni->DiffuseColor = glm::vec3(0.667, 0.667, 0.667);
			Ni->DiffuseIntensity = 1.0f;
			Ni->SpecularColor = glm::vec3(1.0, 0.997, 0.778);
			Ni->SpecularHardness = 5.0f;
			Ni->SpecularIntensity = 1.0f;
			Ni->Density = 8.9e3f;
			Ni->YoungsModulus = 21.0e10f;
			Ni->BulkModulus = 17.0e10f;
			Ni->ShearModulus = 7.8e10f;
			Ni->YieldStrength = 0.0f;
			Ni->UltimateStrength = 165.0e6f;

			Material* steel = new Material("Steel");
			steel->DiffuseColor = glm::vec3(0.3, 0.3, 0.3);
			steel->DiffuseIntensity = 0.7f;
			steel->SpecularColor = glm::vec3(1.0, 1.0, 1.0);
			steel->SpecularHardness = 5.0f;
			steel->SpecularIntensity = 1.0f;
			steel->Density = 7.8e3f;
			steel->YoungsModulus = 20.0e10f;
			steel->BulkModulus = 16.0e10f;
			steel->ShearModulus = 7.5e10f;
			steel->YieldStrength = 951.0e6f;
			steel->UltimateStrength = 1110.0e6f;

			Material* Ag = new Material("Silver");
			Ag->DiffuseColor = glm::vec3(0.667, 0.667, 0.667);
			Ag->DiffuseIntensity = 1.0f;
			Ag->SpecularColor = glm::vec3(1.0, 1.0, 1.0);
			Ag->SpecularHardness = 5.0f;
			Ag->SpecularIntensity = 1.0f;
			Ag->Density = 10.5e3f;
			Ag->YoungsModulus = 83.0e9f;
			Ag->ShearModulus = 30.0e9f;
			Ag->BulkModulus = 100.0e9f;
			Ag->YieldStrength = 124.0e6f;
			Ag->UltimateStrength = 172.0e6f;

			Material* Au = new Material("Gold");
			Au->DiffuseColor = glm::vec3(1.0, 0.87, 0.006);
			Au->DiffuseIntensity = 0.8f;
			Au->SpecularColor = glm::vec3(1.0, 0.997, 0.778);
			Au->SpecularIntensity = 0.993f;
			Au->SpecularHardness = 5.0f;
			Au->Density = 19.3e3f;
			Au->YoungsModulus = 79.0e9f;
			Au->ShearModulus = 27.0e9f;
			Au->BulkModulus = 180.0e9f;
			Au->YieldStrength = 220.0e6f;
			Au->UltimateStrength = 205.0e6f;

			Material* Pt = new Material("Platnum");
			Pt->DiffuseColor = glm::vec3(0.667, 0.667, 0.667);
			Pt->DiffuseIntensity = 0.7f;
			Pt->SpecularColor = glm::vec3(1.0, 1.0, 1.0);
			Pt->SpecularHardness = 5.0f;
			Pt->SpecularIntensity = 1.0f;
			Pt->Density = 21.45e3f;
			Pt->YoungsModulus = 168.0e9f;
			Pt->ShearModulus = 61.0e9f;
			Pt->BulkModulus = 230.0e9f;
			Pt->YieldStrength = 185.0e6f;
			Pt->UltimateStrength = 250.0e6f;

			Material* silk = new Material("Silk");
			silk->DiffuseColor = glm::vec3(1.0, 1.0, 1.0);
			silk->DiffuseIntensity = 1.0f;
			silk->SpecularColor = glm::vec3(1.0, 1.0, 1.0);
			silk->SpecularHardness = 30.0f;
			silk->SpecularIntensity = 0.0f;
			silk->Density = 1.3e3f;
			silk->YieldStrength = 1652.0e6f;
			silk->UltimateStrength = 1000.0e6f;

			Material* marble = new Material("Marble");
			marble->DiffuseColor = glm::vec3(1.0, 0.854, 0.925);
			marble->DiffuseIntensity = 1.0f;
			marble->SpecularColor = glm::vec3(1.0, 0.837, 0.787);
			marble->SpecularIntensity = 0.817f;
			marble->SpecularHardness = 5.0f;
			marble->Density = 2.76e3f;
			marble->YieldStrength = 15.0e6f;
			marble->UltimateStrength = 15.0e6f;

			Material* concrete = new Material("Concrete");
			concrete->DiffuseColor = glm::vec3(0.6, 0.6, 0.6);
			concrete->DiffuseIntensity = 0.8f;
			concrete->SpecularColor = glm::vec3(1.0, 1.0, 1.0);
			concrete->SpecularHardness = 5.0f;
			concrete->SpecularIntensity = 0.05f;
			concrete->Density = 2.7e3f;
			concrete->YieldStrength = 3.0e6f;
			concrete->UltimateStrength = 3.0e6f;
			
			Material* asphalt = new Material("Asphalt");
			asphalt->DiffuseColor = glm::vec3(0.2, 0.2, 0.2);
			asphalt->DiffuseIntensity = 0.469f;
			asphalt->SpecularColor = glm::vec3(1.0, 1.0, 1.0);
			asphalt->SpecularHardness = 5.0f;
			asphalt->SpecularIntensity = 0.410f;

			Material* chalk = new Material("Chalk");
			chalk->DiffuseColor = glm::vec3(0.946, 0.935, 0.888);
			chalk->DiffuseIntensity = 0.94f;
			chalk->SpecularColor = glm::vec3(1.0, 1.0, 1.0);
			chalk->SpecularHardness = 5.0f;
			chalk->SpecularIntensity = 0.093f;
			chalk->Density = 2.5e3f;
			chalk->UltimateStrength = 1.0e6f;

			Material* carbonFiber = new Material("CarbonFiber");
			carbonFiber->Density = 1.75e3f;
			carbonFiber->YieldStrength = 1600.0e6f;
			carbonFiber->UltimateStrength = 1600.0e6f;

			Material* humanHair = new Material("HumanHair");
			humanHair->UltimateStrength = 380.0f;

			Material* humanSkin = new Material("HumanSkin");
			humanSkin->DiffuseColor = glm::vec3(1.0, 0.745, 0.665);
			humanSkin->DiffuseIntensity = 0.9f;
			humanSkin->SpecularColor = glm::vec3(1.0, 1.0, 1.0);
			humanSkin->SpecularHardness = 5.0f;
			humanSkin->SpecularIntensity = 0.1f;
			humanSkin->Density = 2.2e3f;
			humanSkin->YieldStrength = 15.0e6f;
			humanSkin->UltimateStrength = 20.0e6f;

			Material* cork = new Material("Cork");
			cork->DiffuseColor = glm::vec3(0.937, 0.753, 0.573);
			cork->DiffuseIntensity = 1.0f;
			cork->SpecularColor = glm::vec3(1.0, 1.0, 1.0);
			cork->SpecularHardness = 5.0f;
			cork->SpecularIntensity = 0.209f;
			cork->Density = 0.24e3f;
			
			Material* bamboo = new Material("Bamboo");
			bamboo->Density = 0.4e3f;
			bamboo->YieldStrength = 425.0e6f;
			bamboo->UltimateStrength = 425.0e6f;

			Material* wood = new Material("Wood");
			wood->DiffuseColor = glm::vec3(0.489, 0.367, 0.291);
			wood->DiffuseIntensity = 0.8f;
			wood->SpecularColor = glm::vec3(1.0, 1.0, 1.0);
			wood->SpecularHardness = 5.0f;
			wood->SpecularIntensity = 0.0f;
			wood->Density = 0.4e3f;
			wood->UltimateStrength = 40.0e6f;

			Material* ice = new Material("Ice");
			ice->Density = 0.9167e3f;

			Material* water = new Material("Water");
			water->Density = 1.0e3f;

			Material* blood = new Material("Blood");
			blood->Density = 1.025e3f;

			Material* bone = new Material("Bone");
			bone->DiffuseColor = glm::vec3(0.8, 0.74, 0.6);
			bone->DiffuseIntensity = 0.8f;
			bone->SpecularColor = glm::vec3(0.8, 0.74, 0.6);
			bone->SpecularIntensity = 0.1f;
			bone->SpecularHardness = 5.0f;
			bone->Density = 1.6e3f;
			bone->YieldStrength = 112.0e6f;
			bone->UltimateStrength = 130.0e6f;

			Material* leather = new Material("Leather");
			leather->DiffuseColor = glm::vec3(0.51, 0.4, 0.278);
			leather->DiffuseIntensity = 0.8f;
			leather->SpecularColor = glm::vec3(0.763, 0.665, 0.58);
			leather->SpecularHardness = 5.0f;
			leather->SpecularIntensity = 0.7f;
			leather->Density = 0.86e3f;
			leather->YieldStrength = 22.0e6f;
			leather->UltimateStrength = 22.0e6f;

			Material* nylon = new Material("Nylon");
			nylon->Density = 1.15e3f;
			nylon->YieldStrength = 45.0e6f;
			nylon->UltimateStrength = 75.0e6f;

			Material* rubber = new Material("Rubber");
			rubber->Density = 1.1e3f;
			rubber->YieldStrength = 16.0e6f;
			rubber->UltimateStrength = 16.0e6f;

			Material* sapphire = new Material("Sapphire");
			sapphire->Density = 4.0e3f;
			sapphire->YieldStrength = 400.0e6f;
			sapphire->UltimateStrength = 1900.0e6f;

			Material* diamond = new Material("Diamond");
			diamond->Density = 3.5e3f;
			diamond->YieldStrength = 1600.0e6f;
			diamond->UltimateStrength = 2800.0e6f;

			Material* plastic = new Material("Plastic");
			plastic->Density = 0.91e3f;
			plastic->YieldStrength = 28.0e6f;
			plastic->UltimateStrength = 60.0e6f;

			Material* Ti = new Material("Titanium");
			Ti->Density = 4.506e3f;
			Ti->YoungsModulus = 11.6e10f;
			Ti->ShearModulus = 44.0e9f;
			Ti->BulkModulus = 110.0e9f;
			Ti->YieldStrength = 160.0e6f;
			Ti->UltimateStrength = 308e6f;

			Material* W = new Material("Tungsten");
			W->Density = 19.25e3f;
			W->YoungsModulus = 411.0e9f;
			W->ShearModulus = 161.0e9f;
			W->BulkModulus = 310.0e9f;
			W->YieldStrength = 941.0e6f;
			W->UltimateStrength = 1510.0e6f;

			Material* Sn = new Material("Tin");
			Sn->Density = 7.31e3f;
			Sn->YoungsModulus = 50.0e9f;
			Sn->ShearModulus = 18.0e9f;
			Sn->BulkModulus = 58.0e9f;
			Sn->YieldStrength = 11.5e6f;
			Sn->UltimateStrength = 110.0e6f;

			Material* Co = new Material("Cobalt");
			Co->Density = 8.9e3f;
			Co->YoungsModulus = 209.0e9f;
			Co->ShearModulus = 75.0e9f;
			Co->BulkModulus = 180.0e9f;
			Co->YieldStrength = 750.0e3f;
			Co->UltimateStrength = 1050.0e3f;

			if (Settings::Misc::VerboseLogging)
				Debug::Log("Material Count: " + std::to_string(Materials.size()));

		}


		void Clear()
		{
			for (auto c : Meshes)
			{
				delete c.second;
			}

			for (auto c : Materials)
			{
				delete c.second;
			}
		}
	}
}