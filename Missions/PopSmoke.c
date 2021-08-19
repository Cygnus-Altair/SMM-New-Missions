class PopSmokeMission extends SurvivorMissions
{
	//Mission related entities
	Car MissionCar;
	Object MissionBuilding;
	ItemBase MissionObject;
	//FireplaceBase MissionCampfire;
	//Grenade_Base BoobyTrap;	
	//	BoobyTrap = Grenade_Base.Cast( GetGame().CreateObject( "M18SmokeGrenade_Green" , m_MissionPosition, false, false, false ));
	//	BoobyTrap.SetPosition( m_MissionPosition );
	//	BoobyTrap.Unpin();
	//4120,,10830 N NWAF
	// 10820,12416 - house NEAF
	//11897.7,140.012,12609.1 - northern end NEAF
	// Land_RoadCone left -11.6,0,7.9
	
	//Mission parameters
	int ReqSmokeAmount = 4;					//pieces, mission granades
	int ExtendedTimout = 1500;				//seconds, mission duration time for extended mission
	int MsgDlyFinish = 60;					//seconds, message delay time after player has finished mission
	
	//Mission containers
	ref array<vector> ExtendedPosList = new array<vector>;
	ref array<vector> Spawnpoints = new array<vector>;
	ref array<vector> PriInfectSpawns = new array<vector>;
	ref array<vector> SecInfectSpawns = new array<vector>;
	ref array<string> InfectedTypes = new array<string>;
	ref array<vector> Flarepoints = new array<vector>;
	ref array<vector> Conepoints = new array<vector>;
	
	//Mission variables 
	string SurvivorName;	
	
	bool IsExtended() return true;
	
	void PopSmokeMission()
	{		
		//Select primary mission
		m_MissionExtended = true;
		
		//Mission timeout
		m_MissionTimeout = 2700;			//seconds, primary mission duration time
		
		//Mission zones
		m_MissionZoneOuterRadius = 150.0;	//meters (!Do not set lower than 200m), mission activation distance
		m_MissionZoneInnerRadius = 4.0;		//meters (!Do not set outside of 1-5m), mission finishing distance to target object
				
		//Mission informant
		m_MissionInformant = "Dr. Legasov";
	
		//Mission person names
		TStringArray SurvivorNames = {"Yuri","Michail","Boris","Valeri","Anatoli","Ivan","Alexej","Dimitrij","Sergej","Nikolai","Vladimir"};
		SurvivorName = SurvivorNames.GetRandomElement();
		
		//Set mission messages for primary mission
		m_MissionMessage1 = SurvivorName +", one of my co-workers has been contacted by a NATO unit attempting to provide support for survivors.";
		m_MissionMessage2 = "This team has been searching the countryside, providing food, medical supplies, and weapons to anyone they find who has not been infected. "+ SurvivorName +" needs four smoke grenades to signal them the next time they fly over.";
		m_MissionMessage3 = "I have several smoke grenades on the table in my green house \n** "+ m_MissionLocation +" **\nI cannot make it there in time.  If you can get there go up the stairs, pick the locks and then grab the grenades.  If we succeed you will get one of the airdropped supply bags.";
		
		
		//Spawnpoints for MissionObjects in middle garage (Do not change order!)
		Spawnpoints.Insert("2.757 -0.529 -1.178");  //on table right (3x motor oil) - not used
		Spawnpoints.Insert("2.757 -0.529 -1.178");  //on table center (bag)
		Spawnpoints.Insert("4.337 -0.211 0.569");	//on dinner table (Grenade pile)
		//Spawnpoints.Insert("3.008301 -0.5299 -1.109");	//on dinner table (Grenade pile)
		
		// spawn point for flares
		Flarepoints.Insert("-4.03 0 -15");
		Flarepoints.Insert("15 0 -4.03");
		Flarepoints.Insert("4.03 0 15");
		Flarepoints.Insert("-15 0 4.03");
	//	Flarepoints.Insert("-11.06 0 7.9");
	//	Flarepoints.Insert("-11.06 0 -7.9");
	//	Flarepoints.Insert("11.06 0 -7.9");
	//	Flarepoints.Insert("11.06 0 7.9");
		
		//Spawnpoint for reward container in Gas Station Building
		Spawnpoints.Insert("0.330 -1.538 0.6");		//beside desk
				
		//Infected spawnpoints for primary mission
		PriInfectSpawns.Insert("-10 0 20");
		PriInfectSpawns.Insert("-12 0 -30");
		PriInfectSpawns.Insert("20 0 -15");
		PriInfectSpawns.Insert("-30 0 -15");
		PriInfectSpawns.Insert("-16 0 -25");
		PriInfectSpawns.Insert("17 0 10");
		PriInfectSpawns.Insert("28 0 -30");
		
		//Infected spawnpoints for secondary mission 
		SecInfectSpawns.Insert("-11 0 -7");
		SecInfectSpawns.Insert("-8 0 8");
		SecInfectSpawns.Insert("8 0 -9");
		SecInfectSpawns.Insert("6 0 -5");
		SecInfectSpawns.Insert("18 0 15");
		
		//Infected types for secondary mission position
		//Male												//Female
		InfectedTypes.Insert("ZmbM_CitizenASkinny_Brown");	InfectedTypes.Insert("ZmbF_JournalistNormal_White");
		InfectedTypes.Insert("ZmbM_priestPopSkinny");		InfectedTypes.Insert("ZmbF_Clerk_Normal_White");
		InfectedTypes.Insert("ZmbM_HermitSkinny_Beige");	InfectedTypes.Insert("ZmbF_CitizenANormal_Blue");
		InfectedTypes.Insert("ZmbM_CitizenBFat_Red");		InfectedTypes.Insert("ZmbF_CitizenBSkinny");
		InfectedTypes.Insert("ZmbM_FishermanOld_Green");	InfectedTypes.Insert("ZmbF_HikerSkinny_Grey");
		InfectedTypes.Insert("ZmbM_HunterOld_Autumn");		InfectedTypes.Insert("ZmbF_SurvivorNormal_Orange");
		InfectedTypes.Insert("ZmbM_CitizenBFat_Blue");		InfectedTypes.Insert("ZmbF_HikerSkinny_Green");
		InfectedTypes.Insert("ZmbM_MotobikerFat_Black");	InfectedTypes.Insert("ZmbF_JoggerSkinny_Green");
		InfectedTypes.Insert("ZmbM_JoggerSkinny_Red");		InfectedTypes.Insert("ZmbF_SkaterYoung_Striped");
		InfectedTypes.Insert("ZmbM_MechanicSkinny_Grey");	InfectedTypes.Insert("ZmbF_BlueCollarFat_Red");
		InfectedTypes.Insert("ZmbM_HandymanNormal_Green");	InfectedTypes.Insert("ZmbF_MechanicNormal_Beige");
		InfectedTypes.Insert("ZmbM_HeavyIndustryWorker");	InfectedTypes.Insert("ZmbF_PatientOld");
		InfectedTypes.Insert("ZmbM_Jacket_black");			InfectedTypes.Insert("ZmbF_ShortSkirt_beige");
		InfectedTypes.Insert("ZmbM_Jacket_stripes");		InfectedTypes.Insert("ZmbF_VillagerOld_Red");
								
		//Search for mission building at primary mission position	
		GetGame().GetObjectsAtPosition( m_MissionPosition , 1.0 , m_ObjectList , m_ObjectCargoList );
		for ( int i = 0 ; i < m_ObjectList.Count(); i++ )
		{ 
			Object FoundObject = m_ObjectList.Get(i);
			if ( FoundObject.GetType() == "Land_House_1W05_Yellow" )
			{	
				MissionBuilding = FoundObject;			 
				Print("[SMM] MissionBuilding is "+ MissionBuilding.GetType() +" at "+ m_MissionDescription[2] +" of "+ m_MissionDescription[1] +" @ "+ MissionBuilding.GetPosition() );
				break;
			}	
		}
		if ( !MissionBuilding ) Print("[SMM] House couldn't be found. Mission rejected!");
		else
		{
			//Close all house doors
			Building house = Building.Cast( MissionBuilding );
			for ( int j=0; j < 2; j++ ) 
			{
				if ( house.IsDoorOpen(j) ) house.CloseDoor(j);
				if ( house.IsDoorLocked(j) ) house.UnlockDoor(j);
			}			
			GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( GetGame().UpdatePathgraphRegionByObject, 1000, false, house );
			//Fireplace
	//	GetGame().ObjectDelete( MissionCampfire );
	//	MissionCampfire = FireplaceBase.Cast( MissionBuilding.CreateAttachment("FireplaceIndoor"));
	//	MissionCampfire = FireplaceBase.Cast( GetGame().CreateObject( "FireplaceIndoor", m_MissionPosition ) );
	//	house.ActionPlaceFireplaceIndoor( MissionCampfire );
	//	MissionCampfire.Synchronize();
	//	MissionCampfire.GetInventory().CreateAttachment("Firewood");
	//	MissionCampfire.GetInventory().CreateAttachment("WoodenStick");
	//	MissionCampfire.GetInventory().CreateAttachment("Rag");
	//	MissionCampfire.GetInventory().CreateAttachment("Firewood");
	//	MissionCampfire.StartFire( true );
	//	m_MissionObjects.Insert( MissionCampfire );
					
		}			
	}
	
	void ~PopSmokeMission()
	{	
		//Despawn all remaining mission objects
		if ( m_MissionObjects )
		{
			Print("[SMM] Despawning "+ m_MissionObjects.Count() +" mission objects from old mission...");		
			for ( int i = 0; i < m_MissionObjects.Count(); i++ )
			{				
				if ( !m_MissionObjects.Get(i))	continue;
				else
				{	
					if ( m_MissionObjects.Get(i).IsTransport() ) 
					{
						//Don't delete just add full damage to MissionCar
						m_MissionObjects.Get(i).SetHealth("","",0);
						continue;
					}
							
					GetGame().ObjectDelete( m_MissionObjects.Get(i) );
				}			
			}
			m_MissionObjects.Clear();
		}	
		
		//Delete mission AI's
		if ( m_MissionAIs )
		{
			if ( m_MissionAIs.Count() > 0 )
			{
				Print("[SMM] Despawning "+ m_MissionAIs.Count() +" mission AI's from old mission...");
				for ( int k = 0; k < m_MissionAIs.Count(); k++ )
				{
					GetGame().ObjectDelete( m_MissionAIs.Get(k) );
				}
				m_MissionAIs.Clear();			
			}
			else Print("[SMM] No mission AI's to despawn.");
		}
		
		//Delete PlayersInZone list & reset container takeaway
		if ( m_PlayersInZone )	m_PlayersInZone.Clear();
		if ( m_ContainerWasTaken ) m_ContainerWasTaken = false;
	}
	
	
	void SpawnRewards()
	{	
		
	//	Grenade_Base BoobyTrap0;	
	//	BoobyTrap0 = Grenade_Base.Cast( GetGame().CreateObject( "M18SmokeGrenade_Green" , m_MissionPosition, false, false, false ));
	//	BoobyTrap0.SetPosition( m_MissionPosition + Flarepoints.Get(0));
	//	BoobyTrap0.Unpin();	
	//
	//	Grenade_Base BoobyTrap1;	
	//	BoobyTrap1 = Grenade_Base.Cast( GetGame().CreateObject( "M18SmokeGrenade_Green" , m_MissionPosition, false, false, false ));
	//	BoobyTrap1.SetPosition( m_MissionPosition + Flarepoints.Get(1));
	//	BoobyTrap1.Unpin();
	//
	//	Grenade_Base BoobyTrap2;	
	//	BoobyTrap2 = Grenade_Base.Cast( GetGame().CreateObject( "M18SmokeGrenade_Green" , m_MissionPosition, false, false, false ));
	//	BoobyTrap2.SetPosition( m_MissionPosition + Flarepoints.Get(2));
	//	BoobyTrap2.Unpin();
	//
	//	Grenade_Base BoobyTrap3;	
	//	BoobyTrap3 = Grenade_Base.Cast( GetGame().CreateObject( "M18SmokeGrenade_Green" , m_MissionPosition, false, false, false ));
	//	BoobyTrap3.SetPosition( m_MissionPosition + Flarepoints.Get(3));
	//	BoobyTrap3.Unpin();		
		
		//new MissionObject 
		MissionObject = ItemBase.Cast( GetGame().CreateObject( "WoodenCrate", m_MissionPosition ));
		
		//Get random loadout 
		int selectedLoadout = Math.RandomIntInclusive( 0, 5);	//!change randomization limit after adding new loadouts!	

		//Spawn selected loadout items in mission object
		EntityAI weapon;
				
		if ( selectedLoadout == 0 )
		{
			weapon = MissionObject.GetInventory().CreateInInventory("MSFC_M4A1_MULTICAM");
				weapon.GetInventory().CreateAttachment("M4_RISHndgrd");
				weapon.GetInventory().CreateAttachment("M4_OEBttstck");
				weapon.GetInventory().CreateAttachment("AD_ACOG");
				weapon.GetInventory().CreateAttachment("M4_Suppressor");
			MissionObject.GetInventory().CreateInInventory("Mag_STANAG_30Rnd");
			MissionObject.GetInventory().CreateInInventory("Mag_STANAG_30Rnd");
			MissionObject.GetInventory().CreateInInventory("M4_T3NRDSOptic_mung");
			MissionObject.GetInventory().CreateInInventory("Ammo_556x45");
			MissionObject.GetInventory().CreateInInventory("Ammo_556x45");
			MissionObject.GetInventory().CreateInInventory("CanOpener");
			MissionObject.GetInventory().CreateInInventory("PeachesCan");
			MissionObject.GetInventory().CreateInInventory("Canteen");
			MissionObject.GetInventory().CreateInInventory("Battery9V");
			MissionObject.GetInventory().CreateInInventory("Goldbar_Base");
		}
		if (selectedLoadout == 1)
		{
			weapon = MissionObject.GetInventory().CreateInInventory("MSFC_SVD_STAR");
				weapon.GetInventory().CreateAttachment("PSO1Optic");
			MissionObject.GetInventory().CreateInInventory("Mag_SVD_10Rnd");
			MissionObject.GetInventory().CreateInInventory("Mag_SVD_10Rnd");
			MissionObject.GetInventory().CreateInInventory("PSO1Optic");
			MissionObject.GetInventory().CreateInInventory("KazuarOptic");
			MissionObject.GetInventory().CreateInInventory("Ammo_762x54");
			MissionObject.GetInventory().CreateInInventory("Ammo_762x54");
			MissionObject.GetInventory().CreateInInventory("Ammo_762x54");
			MissionObject.GetInventory().CreateInInventory("Ammo_762x54");
			MissionObject.GetInventory().CreateInInventory("Ammo_762x54");
			MissionObject.GetInventory().CreateInInventory("CanOpener");
			MissionObject.GetInventory().CreateInInventory("PeachesCan");
			MissionObject.GetInventory().CreateInInventory("Canteen");
			MissionObject.GetInventory().CreateInInventory("Battery9V");
			MissionObject.GetInventory().CreateInInventory("Goldbar_Base");
		}
		if (selectedLoadout == 2)
		{
			weapon = MissionObject.GetInventory().CreateInInventory("MSFC_AKM_MATRIX");
				weapon.GetInventory().CreateAttachment("AK_RailHndgrd");
				weapon.GetInventory().CreateAttachment("AK_PlasticButtstock");
				weapon.GetInventory().CreateAttachment("PSO1Optic");
				weapon.GetInventory().CreateAttachment("AK_Suppressor");
			MissionObject.GetInventory().CreateInInventory("Mag_AKM_30Rnd");
			MissionObject.GetInventory().CreateInInventory("Mag_AKM_30Rnd");
			MissionObject.GetInventory().CreateInInventory("Ammo_762x39");
			MissionObject.GetInventory().CreateInInventory("Ammo_762x39");
			MissionObject.GetInventory().CreateInInventory("Ammo_762x39");
			MissionObject.GetInventory().CreateInInventory("CanOpener");
			MissionObject.GetInventory().CreateInInventory("PeachesCan");
			MissionObject.GetInventory().CreateInInventory("Canteen");
			MissionObject.GetInventory().CreateInInventory("Battery9V");
			MissionObject.GetInventory().CreateInInventory("Goldbar_Base");
		}
		if (selectedLoadout == 3)
		{
			weapon = MissionObject.GetInventory().CreateInInventory("FAL");
				weapon.GetInventory().CreateAttachment("Fal_OeBttstck");
			MissionObject.GetInventory().CreateInInventory("Mag_FAL_20Rnd");
			MissionObject.GetInventory().CreateInInventory("Mag_FAL_20Rnd");
			MissionObject.GetInventory().CreateInInventory("Mag_FAL_20Rnd");
			MissionObject.GetInventory().CreateInInventory("M4_Suppressor");
			MissionObject.GetInventory().CreateInInventory("AD_ACOG");
			MissionObject.GetInventory().CreateInInventory("FNX45");
			MissionObject.GetInventory().CreateInInventory("Mag_FNX45_15Rnd");
			MissionObject.GetInventory().CreateInInventory("Mag_FNX45_15Rnd");
			MissionObject.GetInventory().CreateInInventory("Ammo_45ACP");
			MissionObject.GetInventory().CreateInInventory("FNP45_MRDSOptic");
			MissionObject.GetInventory().CreateInInventory("PistolSuppressor");
			MissionObject.GetInventory().CreateInInventory("Goldbar_Base");
			MissionObject.GetInventory().CreateInInventory("AmmoBox");
			MissionObject.GetInventory().CreateInInventory("Battery9V");
		}	
		if (selectedLoadout == 4)
		{
			weapon = MissionObject.GetInventory().CreateInInventory("MSFC_SKS_HONEYCOMB");
				weapon.GetInventory().CreateAttachment("PUScopeOptic");
			MissionObject.GetInventory().CreateInInventory("Ammo_762x39");
			MissionObject.GetInventory().CreateInInventory("Ammo_762x39");
			weapon = MissionObject.GetInventory().CreateInInventory("FNX45");
				weapon.GetInventory().CreateAttachment("PistolSuppressor");
				EntityAI weaponlight = weapon.GetInventory().CreateAttachment("TLRLight");
					weaponlight.GetInventory().CreateAttachment("Battery9V");
			MissionObject.GetInventory().CreateInInventory("Mag_FNX45_15Rnd");
			MissionObject.GetInventory().CreateInInventory("Ammo_45ACP");
			MissionObject.GetInventory().CreateInInventory("AmmoBox");
			MissionObject.GetInventory().CreateInInventory("Goldbar_Base");
			MissionObject.GetInventory().CreateInInventory("Goldbar_Base");
		}	
		if (selectedLoadout == 5)
		{
			weapon = MissionObject.GetInventory().CreateInInventory("MSFC_Winchester70_ORANGE");
				weapon.GetInventory().CreateAttachment("AD_Pilad");
			MissionObject.GetInventory().CreateInInventory("Ammo_308Win");
			MissionObject.GetInventory().CreateInInventory("FNX45");
			MissionObject.GetInventory().CreateInInventory("Mag_FNX45_15Rnd");
			MissionObject.GetInventory().CreateInInventory("Ammo_45ACP");
			MissionObject.GetInventory().CreateInInventory("AmmoBox");
			MissionObject.GetInventory().CreateInInventory("PistolSuppressor");
			MissionObject.GetInventory().CreateInInventory("TLRLight");
			MissionObject.GetInventory().CreateInInventory("Battery9V");
			MissionObject.GetInventory().CreateInInventory("Goldbar_Base");
		}
		
		Print("[SMM] Mission rewards spawned in reward container. Randomly selected loadout was "+selectedLoadout+"." );			
	}
	
	void SpawnObjects()
	{	
		if ( m_MissionExtended )
		{	
			//Spawn Grenade bag
			MissionObject = ItemBase.Cast( GetGame().CreateObject("CourierBag", MissionBuilding.ModelToWorld( Spawnpoints.Get(1) )));
			m_MissionObjects.InsertAt( MissionObject, 1 );			
		
			
			//Cluster spawn mission smoke grenade 			
			for ( int j=0; j < ReqSmokeAmount; j++ )
			{
				Object smokeg = GetGame().CreateObject("M18SmokeGrenade_Green", MissionBuilding.ModelToWorld( Spawnpoints.Get(2) ));
				vector gpos = { Math.RandomFloatInclusive(-0.3, 0.3), 0, Math.RandomFloatInclusive(-0.3, 0.3) };
				smokeg.SetPosition( MissionBuilding.ModelToWorld( Spawnpoints.Get(2) + gpos ) );
				m_MissionObjects.Insert( smokeg );
			}
			
			Print("[SMM] Survivor Mission "+ m_selectedMission +" :: "+ m_MissionName +" ...primary mission deployed!");
		}
		else
		{
			//Spawn nothing when player is entering secondary mission zone
				
			Print("[SMM] Survivor Mission Extension "+ m_selectedMission +" :: "+ m_MissionName +" ...secondary mission deployed!");
		}
	}
	
	void SpawnAIs()
	{	
		string RandomInfected;
		vector InfectedPos;
		DayZInfected Zed;
				
		if ( !MissionBuilding ) Print("[SMM] Extended Mission : Missionbuilding couldn't be found in "+ m_MissionDescription[3] +"." );		
		else
		{
			if ( m_MissionExtended )
			{
						
				//Spawn some infected at garage
				for ( int j = 0 ; j < PriInfectSpawns.Count() ; j++ )
				{
		    	   	RandomInfected = InfectedTypes.GetRandomElement();
					InfectedPos = MissionBuilding.ModelToWorld( PriInfectSpawns.Get(j) );
					Zed = DayZInfected.Cast( GetGame().CreateObject( RandomInfected, InfectedPos, false, true ));
					m_MissionAIs.Insert( Zed );
				}
			}
			else
			{
				//Spawn some infected at Gas station
				for ( int k = 0 ; k < SecInfectSpawns.Count() ; k++ )
				{
		    	   	RandomInfected = InfectedTypes.GetRandomElement();
					InfectedPos = MissionBuilding.ModelToWorld( SecInfectSpawns.Get(k) );
					Zed = DayZInfected.Cast( GetGame().CreateObject( RandomInfected, InfectedPos, false, true ));
					m_MissionAIs.Insert( Zed );
				}	
//Spawn pilot
		DayZInfected InfectedPilot = DayZInfected.Cast( GetGame().CreateObject( "ZmbM_CommercialPilotOld_Olive" , m_MissionPosition , false , true ));
			InfectedPilot.SetPosition( m_MissionPosition );
		//InfectedPilot.PlaceOnSurface();
			InfectedPilot.GetInventory().CreateAttachment("ZSh3PilotHelmet_Green");
			InfectedPilot.GetInventory().CreateInInventory("M67Grenade");
			InfectedPilot.GetInventory().CreateInInventory("CanOpener");
			InfectedPilot.GetInventory().CreateInInventory("BandageDressing");
			InfectedPilot.GetInventory().CreateInInventory("PeachesCan");
			InfectedPilot.GetInventory().CreateInInventory("Canteen");
			InfectedPilot.GetInventory().CreateAttachment("UKAssVest_Olive");
			InfectedPilot.GetInventory().CreateInInventory("PersonalRadio");
			InfectedPilot.GetInventory().CreateInInventory("Battery9V");
			InfectedPilot.GetInventory().CreateInInventory("Battery9V");
			InfectedPilot.GetInventory().CreateInInventory("Battery9V");
			
		//Pilot is dead
		InfectedPilot.SetHealth("","",0);
		
		m_MissionAIs.Insert( InfectedPilot );				
			}
		}
	}
	
	void ObjDespawn() 
	{	
		//Despawn all mission objects at mission timeout except those retains until next mission
		for ( int i = 0; i < m_MissionObjects.Count(); i++ )
		{
			//Exception: MissionCar will remain but gets damaged
			if ( m_MissionObjects.Get(i) && m_MissionObjects.Get(i).IsTransport() )	
			{
				m_MissionObjects.Get(i).SetHealth("","",0);
				continue;
			}
			
			//Delete Object
			GetGame().ObjectDelete( m_MissionObjects.Get(i) );
			m_MissionObjects.Remove(i);
		}
	}
	
	void ExtendMission()
	{	//When player enters mission target zone at primary mission
		
		//Set messages for secondary mission
		m_MissionMessage1 = "All right, so you have found the grenades in the house. Place the grenades in the bag, then bring the bag to "+ SurvivorName +".";
		m_MissionMessage2 = "After you have them, bring them to the end of the Airport Runway ** "+ m_MissionDescription[3] +" **\nin the middle of the traffic cones, and "+ SurvivorName +" will set them off to mark out an area for the airdrop.";
		m_MissionMessage3 = "Please hurry, the aircraft is coming soon. Be carefull, there might be bandits on the way to "+ m_MissionDescription[3] +". They could have intercepted our little radio talk here.";
		
		//init Messenger for new messages
		m_MsgNum = 1;					//skip msg 0, begin with msg 1
		m_MsgChkTime = m_MissionTime;
		MsgCutoff = false;
		
		//increase mission MissionTimeout for secondary mission
		m_MissionTimeout = m_MissionTime + ExtendedTimout;  
		
		//deployment settings & init for secondary mission		
		m_MissionZoneOuterRadius = 80.0;
		m_MissionZoneInnerRadius = 2.0;
		
		//Get secondary mission position
		if ( EventsWorldData.GetBuildingsAtLoc("none", m_MissionDescription[3], ExtendedPosList ))
		m_MissionPosition = ExtendedPosList.GetRandomElement();
		else Print("[SMM] Can't get secondary MissionPosition in "+ m_MissionDescription[3] +" from EventsWorldData!");
	}
	
	void MissionFinal()
	{	//When player enters last mission target zone

	}
	
	void PlayerChecks( PlayerBase player )
	{
		//Check if MissionObject is brought to MissionPosition 
		if ( MissionObject && MissionObject.ClassName() == "CourierBag" && !m_MissionExtended )
		{		
			float BagDistance = vector.Distance( MissionObject.GetPosition(), m_MissionPosition );
			if ( BagDistance < 3 )
			{
				int CargoCount = MissionObject.GetInventory().CountInventory();
				int LastCount = 0;
				int FoundObjects = 0;
				
				if ( CargoCount != LastCount )
				{
					if ( CargoCount >= ReqSmokeAmount && FoundObjects <= ReqSmokeAmount )
					{	
						CargoBase CargoItems1 = MissionObject.GetInventory().GetCargo();		
						
						for ( int i = 0; i < CargoCount; i++ )
						{
							EntityAI CargoItem = CargoItems1.GetItem(i);
							if ( m_MissionObjects.Find( CargoItem ) > -1 ) FoundObjects++;
							else continue;					
							
							//When requested amount of grenades are present, despawn MissionObject & spawn rewards
							if ( FoundObjects >= ReqSmokeAmount )
							{
								Print("[SMM] Player with SteamID64: "+ player.GetIdentity().GetPlainId() +" has successfully stored the requested amount of "+ ReqSmokeAmount +" MissionObjects in the container.");
								//delete container first
								GetGame().ObjectDelete( MissionObject );
								//spawn smoke
									Grenade_Base BoobyTrap0;	
									BoobyTrap0 = Grenade_Base.Cast( GetGame().CreateObject( "M18SmokeGrenade_Green" , m_MissionPosition, false, false, false ));
									BoobyTrap0.SetPosition( m_MissionPosition + Flarepoints.Get(0));
									BoobyTrap0.Unpin();	
							
									Grenade_Base BoobyTrap1;	
									BoobyTrap1 = Grenade_Base.Cast( GetGame().CreateObject( "M18SmokeGrenade_Green" , m_MissionPosition, false, false, false ));
									BoobyTrap1.SetPosition( m_MissionPosition + Flarepoints.Get(1));
									BoobyTrap1.Unpin();
							
									Grenade_Base BoobyTrap2;	
									BoobyTrap2 = Grenade_Base.Cast( GetGame().CreateObject( "M18SmokeGrenade_Green" , m_MissionPosition, false, false, false ));
									BoobyTrap2.SetPosition( m_MissionPosition + Flarepoints.Get(2));
									BoobyTrap2.Unpin();
							
									Grenade_Base BoobyTrap3;	
									BoobyTrap3 = Grenade_Base.Cast( GetGame().CreateObject( "M18SmokeGrenade_Green" , m_MissionPosition, false, false, false ));
									BoobyTrap3.SetPosition( m_MissionPosition + Flarepoints.Get(3));
									BoobyTrap3.Unpin();		
								//spawn rewards 
								GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( this.SpawnRewards, 10000 );
								//SpawnRewards();
								m_RewardsSpawned = true;
								m_MsgNum = -1;
								m_MsgChkTime = m_MissionTime + MsgDlyFinish;
								break;
							} 
						}
						LastCount = CargoCount;
					}
				} 		
			}
		}		
	}
		
	void UpdateBots( float dt )
	{
		//No bots involved in this mission		
	}
	
	bool DeployMission()
	{	//When first player enters the mission zone (primary/secondary)
		//Get MissionBuilding at secondary mission position
		if ( !m_MissionExtended )
		{			
			//GetGame().GetObjectsAtPosition( m_MissionPosition , 1.0 , m_ObjectList , m_ObjectCargoList );
			//for ( int i=0; i < m_ObjectList.Count(); i++ )
			//{ 
			//	Object FoundObject = m_ObjectList.Get(i);
			//	if ( FoundObject.GetType() == "Land_FuelStation_Build" )
			//	{			 
			//		MissionBuilding = FoundObject;
			//		Print("[SMM] MissionBuilding extended is "+ m_MissionDescription[3] +" Gas Station @ "+ m_MissionPosition );
			//		
			//		//new MissionPosition is rewards spawnpoint
			//		m_MissionPosition = MissionBuilding.ModelToWorld( Spawnpoints.Get(5) );
			//		break;
			//	}	
			//}
		Conepoints.Insert("-4.03 0 -15");
		Conepoints.Insert("15 0 -4.03");
		Conepoints.Insert("4.03 0 15");
		Conepoints.Insert("-15 0 4.03");
		vector ConePosition0 = m_MissionPosition + Conepoints.Get(0);
		vector ConePosition1 = m_MissionPosition + Conepoints.Get(1);
		vector ConePosition2 = m_MissionPosition + Conepoints.Get(2);
		vector ConePosition3 = m_MissionPosition + Conepoints.Get(3);
		ItemBase Cone0;	
		Cone0 = ItemBase.Cast( GetGame().CreateObject( "TrafficCone_Prop" , ConePosition0, false, false, false ));
		//Cone0.SetPosition( m_MissionPosition + Conepoints.Get(0));
		m_MissionObjects.Insert( Cone0 );

		ItemBase Cone1;	
		Cone1 = ItemBase.Cast( GetGame().CreateObject( "TrafficCone_Prop" , ConePosition1, false, false, false ));
		//Cone1.SetPosition( m_MissionPosition + Conepoints.Get(1));
		m_MissionObjects.Insert( Cone1 );

		ItemBase Cone2;	
		Cone2 = ItemBase.Cast( GetGame().CreateObject( "TrafficCone_Prop" , ConePosition2, false, false, false ));
		//Cone2.SetPosition( m_MissionPosition + Conepoints.Get(2));
		m_MissionObjects.Insert( Cone2 );

		ItemBase Cone3;	
		Cone3 = ItemBase.Cast( GetGame().CreateObject( "TrafficCone_Prop" , ConePosition3, false, false, false ));
		//Cone3.SetPosition( m_MissionPosition + Conepoints.Get(3));
		m_MissionObjects.Insert( Cone3 );	
		}
		
		if ( MissionBuilding )
		{
			//Call spawners	
			GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).Call( this.SpawnObjects );
			GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).Call( this.SpawnAIs );
			return true;		
		}
		else 
		{
			Print("[SMM] ERROR : MissionBuilding can't be found!");
			if ( MissionSettings.DebugMode ) Print("[SMM] MissionBuilding is NULL!");
			return false;
		}
	}
}
