//#include "$CurrentDir:\\mpmissions\\dayzOffline.chernarusplus\\eAI\\HeliStealBot.c"
//#include "$CurrentDir:\\mpmissions\\dayzOffline.chernarusplus\\SurvivorMissionModule\\EventsWorldData.c"
class HeliStealMission extends SurvivorMissions
{
	//Mission related entities 
	Car Heli;
	ItemBase MissionObject;
	ItemBase HeliKey;
	
	//Mission parameters
	int MsgDlyFinish = 30;					//seconds, message delay time after player has finished mission
	
	//Mission containers
	ref array<vector> ContainerSpawns = new array<vector>;
	ref array<vector> InfectedSpawns = new array<vector>;
	ref array<vector> FireSpawns = new array<vector>;
	ref array<string> InfectedTypes = new array<string>;
	ref array<string> HeliTypes = new array<string>;
	ref array<string> ammoTypes = new array<string>;
	ref array<string> foodTypes = new array<string>;	
	ref array<string> missionbots = new array<string>;
	ref array<string> helperbot = new array<string>;
	ref array<vector> patrol_100 = {"-44 0 -44","-44 0 0","-44 0 44","0 0 44","44 0 34","44 0 0","44 0 -44","0 0 -44"};
	eAIGroup deletethis = null;
	//Mission variables 
	string SurvivorName;		
	
	bool IsExtended() return false;
	
	void HeliStealMission()
	{
		//Mission mission timeout
		m_MissionTimeout = 2400;			//seconds, mission duration time
		
		//Mission zones
		m_MissionZoneOuterRadius = 500.0;	//meters (!Do not set lower than 200m), mission activation distance
		m_MissionZoneInnerRadius = 8.0;		//meters (!Do not set outside of 1-5m), mission finishing distance to target object
				
		//Mission informant
		m_MissionInformant = "The Information Broker";
	
		//Mission person names
		SurvivorName = "Gerry Lane";
		
		//Set mission messages
        m_MissionMessage1 = "Be advised: A helicopter has landed "+ m_MissionLocationDir +" of "+ m_MissionLocation + " and is offloading cargo.";
        m_MissionMessage2 = "Take out the guards, secure the heli, and exfil.";
        m_MissionMessage3 = "If it is locked, search the pilot for the key.";		
		
		//Engine fire spawns 
		/*FireSpawns.Insert("5.15 -2.18 -4.63");		//engine
		FireSpawns.Insert("5.15 -2.18 -5");			//engine
		FireSpawns.Insert("5.15 0 0");			//fire trace
		FireSpawns.Insert("4.8 0 0.5");			//fire trace
		FireSpawns.Insert("5.3 0 1");			//fire trace
		FireSpawns.Insert("5.15 0 15");			//fire trace
		FireSpawns.Insert("5.15 0 25");			//fire trace
		//FireSpawns.Insert("5.15 0 30");			//fire trace
		//FireSpawns.Insert("5.15 0 45");			//fire trace
		//FireSpawns.Insert("5.15 0 55");			//fire trace
		*/
		//Container spawnpoints
		ContainerSpawns.Insert("2 1 1");
		ContainerSpawns.Insert("3 1 2");
		
		//Heli Spawnpoints spawnpoints
		HeliTypes.Insert("HeliSIB_UH1D_clear");
		HeliTypes.Insert("HeliSIB_UH1D_iron_clear");
		HeliTypes.Insert("HeliSIB_UH1D_navy_clear");
		HeliTypes.Insert("HeliSIB_UH1D_airforce_clear");
		HeliTypes.Insert("HeliSIB_uh1d_FZKilgore_clear");
		HeliTypes.Insert("Helilittle_bird_RF_clear");
		HeliTypes.Insert("Heli_apache_clear");		
		
		//Create Random Ammo selection
		ammoTypes.Insert("AmmoBox_308WinTracer_20Rnd");
		ammoTypes.Insert("AmmoBox_308Win_20Rnd");
		ammoTypes.Insert("AmmoBox_545x39Tracer_20Rnd");
		ammoTypes.Insert("AmmoBox_545x39_20Rnd");	
		ammoTypes.Insert("AmmoBox_556x45Tracer_20Rnd");
		ammoTypes.Insert("AmmoBox_556x45_20Rnd");
		ammoTypes.Insert("AmmoBox_762x39Tracer_20Rnd");
		ammoTypes.Insert("AmmoBox_762x39_20Rnd");	
		ammoTypes.Insert("AmmoBox_762x54Tracer_20Rnd");
		ammoTypes.Insert("AmmoBox_762x54_20Rnd");	

		//Create Random Ammo selection
		foodTypes.Insert("BakedBeansCan");
		foodTypes.Insert("TacticalBaconCan");
		foodTypes.Insert("PeachesCan");
		foodTypes.Insert("SardinesCan");	
		foodTypes.Insert("SodaCan_Cola");
		foodTypes.Insert("SodaCan_Kvass");
		foodTypes.Insert("SodaCan_Pipsi");		
		
		//Infected spawnpoints
		InfectedSpawns.Insert("-10.5186 0 25.0269");
		InfectedSpawns.Insert("24.9775 0 -10.4146");
		InfectedSpawns.Insert("-30.1726 0 -6.2729");
		InfectedSpawns.Insert("-20.9209 0 4.6636");
		InfectedSpawns.Insert("15.0283 0 -10.3276");
		InfectedSpawns.Insert("7.2461 0 30.5884");
		InfectedSpawns.Insert("-20.6855 0 5.9956");
		InfectedSpawns.Insert("40 0 20");
		InfectedSpawns.Insert("-20 0 -40");
		InfectedSpawns.Insert("-50 0 50");
		InfectedSpawns.Insert("35 0 40");
		InfectedSpawns.Insert("-40 0 -20");
		InfectedSpawns.Insert("-30 0 -40");
		
		//Infected types
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
		InfectedTypes.Insert("ZmbM_HikerSkinny_Blue");		InfectedTypes.Insert("ZmbF_JoggerSkinny_Red");
		InfectedTypes.Insert("ZmbM_HikerSkinny_Yellow");	InfectedTypes.Insert("ZmbF_MilkMaidOld_Beige");
		InfectedTypes.Insert("ZmbM_PolicemanFat");			InfectedTypes.Insert("ZmbF_VillagerOld_Green");
		InfectedTypes.Insert("ZmbM_PatrolNormal_Summer");	InfectedTypes.Insert("ZmbF_ShortSkirt_yellow");
		InfectedTypes.Insert("ZmbM_JoggerSkinny_Blue");		InfectedTypes.Insert("ZmbF_NurseFat");
		InfectedTypes.Insert("ZmbM_VillagerOld_White");		InfectedTypes.Insert("ZmbF_PoliceWomanNormal");
		InfectedTypes.Insert("ZmbM_SkaterYoung_Brown");		InfectedTypes.Insert("ZmbF_HikerSkinny_Blue");
		InfectedTypes.Insert("ZmbM_MechanicSkinny_Green");	InfectedTypes.Insert("ZmbF_ParamedicNormal_Green");
		InfectedTypes.Insert("ZmbM_DoctorFat");				InfectedTypes.Insert("ZmbF_JournalistNormal_Red");
		InfectedTypes.Insert("ZmbM_PatientSkinny");			InfectedTypes.Insert("ZmbF_SurvivorNormal_White");
		InfectedTypes.Insert("ZmbM_ClerkFat_Brown");		InfectedTypes.Insert("ZmbF_JoggerSkinny_Brown");
		InfectedTypes.Insert("ZmbM_ClerkFat_White");		InfectedTypes.Insert("ZmbF_MechanicNormal_Grey");
		InfectedTypes.Insert("ZmbM_Jacket_magenta");		InfectedTypes.Insert("ZmbF_BlueCollarFat_Green");
		InfectedTypes.Insert("ZmbM_PolicemanSpecForce");	InfectedTypes.Insert("ZmbF_DoctorSkinny");
		/*
		m_MissionEvents.RemoveOrdered( m_selectedMission );
		m_MissionPositions.RemoveOrdered( m_selectedMission );
		Print("[SMM] Survivor Mission "+ m_selectedMission +" :: "+ m_MissionName +" ...mission removed from spawn list");
		*/
	}
	
	void ~HeliStealMission()
	{
		//Despawn all remaining mission objects
		if ( m_MissionObjects )
		{	
			Print("[SMM] Despawning "+ m_MissionObjects.Count() +" mission objects from old mission...");				
			for ( int i = 0; i < m_MissionObjects.Count(); i++ )
			{
				if ( !m_MissionObjects.Get(i) ) continue;
				else GetGame().ObjectDelete( m_MissionObjects.Get(i) );		
			}
			m_MissionObjects.Clear();
		}
		
		//Despawn mission AI's
		#ifdef EAI
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
		eAIGroup g = deletethis;
				for (int r = g.Count() - 1; r > -1; r--) {
					PlayerBase p = g.GetMember(r);
					if (p.IsAI()) {
						g.RemoveMember(r);
						GetGame().ObjectDelete(p);
					}
				}	
		#endif
		//Delete PlayersInZone list & reset container takeaway
		if ( m_PlayersInZone )	m_PlayersInZone.Clear();
		if ( m_ContainerWasTaken ) m_ContainerWasTaken = false;	
	}
	
	//void ExplodeHeli(int count)
	//{
	//	for ( int n=0; n < count; n++)
	//	{
	//		Grenade_Base Explosives = Grenade_Base.Cast( GetGame().CreateObject( "M67Grenade",Heli.ModelToWorld("5.15 -2.28 -4.63")));
	//		Explosives.ActivateImmediate();
	//	}
	//
	//}
	void SpawnObjects()
	{
		
		//Spawn crashed plane crashed C130 
		int PlaneOriConversion;
		string PlaneOrientation;
		
		PlaneOriConversion = m_MissionDescription[3].ToInt() - 180;
		if ( PlaneOriConversion < 0 ) PlaneOriConversion += 360;
		PlaneOrientation = PlaneOriConversion.ToString() +" 0 0";
		
		string RandomHeli = HeliTypes.GetRandomElement();
		string RandomAmmo1 = ammoTypes.GetRandomElement();
		string RandomAmmo2 = ammoTypes.GetRandomElement();
		string RandomAmmo3 = ammoTypes.GetRandomElement();
		string RandomAmmo4 = ammoTypes.GetRandomElement();
		string RandomAmmo5 = ammoTypes.GetRandomElement();
		string RandomAmmo6 = ammoTypes.GetRandomElement();
		string RandomAmmo7 = ammoTypes.GetRandomElement();
		string RandomAmmo8 = ammoTypes.GetRandomElement();
		string RandomFood1 = foodTypes.GetRandomElement();
		string RandomFood2 = foodTypes.GetRandomElement();
		string RandomFood3 = foodTypes.GetRandomElement();
		string RandomFood4 = foodTypes.GetRandomElement();
		string RandomFood5 = foodTypes.GetRandomElement();
		string RandomFood6 = foodTypes.GetRandomElement();
		string RandomFood7 = foodTypes.GetRandomElement();
		string RandomFood8 = foodTypes.GetRandomElement();
		
		
		Heli = Car.Cast(GetGame().CreateObject( RandomHeli, m_MissionPosition ));
		Heli.SetOrientation( PlaneOrientation.ToVector() );
		Heli.PlaceOnSurface();
		if (RandomHeli == "Helilittle_bird_RF_Clear")
		{
			Heli.GetInventory().CreateAttachment("RFlittle_bird_blade1");
			Heli.GetInventory().CreateAttachment("RFlittle_bird_blade2");
			Heli.GetInventory().CreateAttachment("RFlittle_bird_blade3");
			Heli.GetInventory().CreateAttachment("RFlittle_bird_blade4");
			Heli.GetInventory().CreateAttachment("RFlittle_bird_blade5");
			Heli.GetInventory().CreateAttachment("RFlittle_bird_bladem1");
			Heli.GetInventory().CreateAttachment("RFlittle_bird_bladem3");
			Heli.GetInventory().CreateAttachment("CarBattery");
			Heli.GetInventory().CreateAttachment("HeadlightH7");
			Heli.GetInventory().CreateAttachment("HeadlightH7");
		}
		if (RandomHeli == "Heli_apache_clear")
		{
			Heli.GetInventory().CreateAttachment("apache_blade1");
			Heli.GetInventory().CreateAttachment("apache_blade2");
			Heli.GetInventory().CreateAttachment("apache_blade3");
			Heli.GetInventory().CreateAttachment("apache_blade4");
			Heli.GetInventory().CreateAttachment("apache_bladem1");
			Heli.GetInventory().CreateAttachment("apache_bladem2");
			Heli.GetInventory().CreateAttachment("apache_bladem3");
			Heli.GetInventory().CreateAttachment("apache_bladem4");
			Heli.GetInventory().CreateAttachment("apache_Door_1_1");
			Heli.GetInventory().CreateAttachment("apache_Door_2_1");
			Heli.GetInventory().CreateAttachment("CarBattery");
			Heli.GetInventory().CreateAttachment("HeadlightH7");
			Heli.GetInventory().CreateAttachment("HeadlightH7");
		}
		if (RandomHeli == "HeliSIB_uh1d_FZKilgore_clear")
		{
			Heli.GetInventory().CreateAttachment("SIBUH1D_blade1");
			Heli.GetInventory().CreateAttachment("SIBUH1D_blade2");
			Heli.GetInventory().CreateAttachment("SIBUH1D_bladem1");
			Heli.GetInventory().CreateAttachment("SIBUH1D_bladem2");
			Heli.GetInventory().CreateAttachment("SIBUH1D_Door_1_1_FZKilgore");
			Heli.GetInventory().CreateAttachment("SIBUH1D_Door_1_2_FZKilgore");
			Heli.GetInventory().CreateAttachment("SIBUH1D_Door_2_1_FZKilgore");
			Heli.GetInventory().CreateAttachment("SIBUH1D_Door_2_2_FZKilgore");
			Heli.GetInventory().CreateAttachment("CarBattery");
			Heli.GetInventory().CreateAttachment("HeadlightH7");
			Heli.GetInventory().CreateAttachment("HeadlightH7");
		}
		if (RandomHeli == "HeliSIB_UH1D_clear")
		{
			Heli.GetInventory().CreateAttachment("SIBUH1D_blade1");
			Heli.GetInventory().CreateAttachment("SIBUH1D_blade2");
			Heli.GetInventory().CreateAttachment("SIBUH1D_bladem1");
			Heli.GetInventory().CreateAttachment("SIBUH1D_bladem2");
			Heli.GetInventory().CreateAttachment("SIBUH1D_Door_1_1");
			Heli.GetInventory().CreateAttachment("SIBUH1D_Door_1_2");
			Heli.GetInventory().CreateAttachment("SIBUH1D_Door_2_1");
			Heli.GetInventory().CreateAttachment("SIBUH1D_Door_2_2");
			Heli.GetInventory().CreateAttachment("CarBattery");
			Heli.GetInventory().CreateAttachment("HeadlightH7");
			Heli.GetInventory().CreateAttachment("HeadlightH7");
		}
		if (RandomHeli == "HeliSIB_UH1D_iron_clear")
		{
			Heli.GetInventory().CreateAttachment("SIBUH1D_blade1");
			Heli.GetInventory().CreateAttachment("SIBUH1D_blade2");
			Heli.GetInventory().CreateAttachment("SIBUH1D_bladem1");
			Heli.GetInventory().CreateAttachment("SIBUH1D_bladem2");
			Heli.GetInventory().CreateAttachment("SIBUH1D_Door_1_1_iron");
			Heli.GetInventory().CreateAttachment("SIBUH1D_Door_1_2_iron");
			Heli.GetInventory().CreateAttachment("SIBUH1D_Door_2_1_iron");
			Heli.GetInventory().CreateAttachment("SIBUH1D_Door_2_2_iron");
			Heli.GetInventory().CreateAttachment("CarBattery");
			Heli.GetInventory().CreateAttachment("HeadlightH7");
			Heli.GetInventory().CreateAttachment("HeadlightH7");
		}
		if (RandomHeli == "HeliSIB_UH1D_navy_clear")
		{
			Heli.GetInventory().CreateAttachment("SIBUH1D_blade1");
			Heli.GetInventory().CreateAttachment("SIBUH1D_blade2");
			Heli.GetInventory().CreateAttachment("SIBUH1D_bladem1");
			Heli.GetInventory().CreateAttachment("SIBUH1D_bladem2");
			Heli.GetInventory().CreateAttachment("SIBUH1D_Door_1_1_navy");
			Heli.GetInventory().CreateAttachment("SIBUH1D_Door_1_2_navy");
			Heli.GetInventory().CreateAttachment("SIBUH1D_Door_2_1_navy");
			Heli.GetInventory().CreateAttachment("SIBUH1D_Door_2_2_navy");
			Heli.GetInventory().CreateAttachment("CarBattery");
			Heli.GetInventory().CreateAttachment("HeadlightH7");
			Heli.GetInventory().CreateAttachment("HeadlightH7");
		}
		if (RandomHeli == "HeliSIB_UH1D_airforce_clear")
		{
			Heli.GetInventory().CreateAttachment("SIBUH1D_blade1");
			Heli.GetInventory().CreateAttachment("SIBUH1D_blade2");
			Heli.GetInventory().CreateAttachment("SIBUH1D_bladem1");
			Heli.GetInventory().CreateAttachment("SIBUH1D_bladem2");
			Heli.GetInventory().CreateAttachment("SIBUH1D_Door_1_1_airforce");
			Heli.GetInventory().CreateAttachment("SIBUH1D_Door_1_2_airforce");
			Heli.GetInventory().CreateAttachment("SIBUH1D_Door_2_1_airforce");
			Heli.GetInventory().CreateAttachment("SIBUH1D_Door_2_2_airforce");
			Heli.GetInventory().CreateAttachment("CarBattery");
			Heli.GetInventory().CreateAttachment("HeadlightH7");
			Heli.GetInventory().CreateAttachment("HeadlightH7");
		}
		EntityAI.Cast( Heli ).SetLifetime( (m_MissionTimeout - m_MissionTime) + 30 );
		m_MissionObjects.Insert( Heli );

		//Enginefire
		/*
		for (int s=0; s < FireSpawns.Count(); s++)
		{
			FireplaceBase Enginefire;
			
			if ( s > 1 )
			{
				vector posXZ = Heli.ModelToWorld( FireSpawns.Get(s) );
				float x = posXZ[0];
				float z = posXZ[2];
				float y = GetGame().SurfaceY(x,z);
				vector Groundpos = {x,y,z};
				
				Enginefire = FireplaceBase.Cast( GetGame().CreateObject( "Fireplace", Groundpos));
				Enginefire.SetPosition( Groundpos - "0 0.3 0" );
			}
			else 
			Enginefire = FireplaceBase.Cast( GetGame().CreateObject( "Fireplace", Heli.ModelToWorld( FireSpawns.Get(s) )));
			
			Enginefire.Synchronize();
			Enginefire.GetInventory().CreateAttachment("Firewood");
			Enginefire.GetInventory().CreateAttachment("WoodenStick");
			Enginefire.GetInventory().CreateAttachment("Rag");
			Enginefire.StartFire( true );
			m_MissionObjects.Insert( Enginefire );
		}
		
		//Create smoke on destroyed engine
		Object EngineSmoke = GetGame().CreateObject( "Wreck_UH1Y", Heli.ModelToWorld("0 -7 0"),false,false,true);
		m_MissionObjects.Insert( EngineSmoke );
		*/
		//Explosion at Patrol time delayed	
		//ExplodeHeli(3);
		/*
		GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( this.ExplodeHeli, 800, false, 6 );
		GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( this.ExplodeHeli, 1400, false, 5 );
		GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( this.ExplodeHeli, 1800, false, 6 );
		*/
		//Spawn containers 
		int selectedLoadout;
		
		for (int i=0; i < ContainerSpawns.Count(); i++)
		{	
			MissionObject = ItemBase.Cast( GetGame().CreateObject( "WoodenCrate", Heli.ModelToWorld( ContainerSpawns.Get(i))));
			
			//Get random loadout 
			selectedLoadout = Math.RandomIntInclusive(0,9);	//!change randomization limit after adding new loadouts!	
		
			//Spawn selected loadout items in mission object
			EntityAI weapon;
					
			if (selectedLoadout == 0)
			{
				MissionObject.GetInventory().CreateInInventory("AmmoBox");
				MissionObject.GetInventory().CreateInInventory(RandomAmmo1);
				MissionObject.GetInventory().CreateInInventory(RandomAmmo2);
				MissionObject.GetInventory().CreateInInventory(RandomAmmo3);
				MissionObject.GetInventory().CreateInInventory(RandomAmmo4);
				MissionObject.GetInventory().CreateInInventory(RandomAmmo5);
				MissionObject.GetInventory().CreateInInventory(RandomAmmo6);
				MissionObject.GetInventory().CreateInInventory(RandomAmmo7);
				MissionObject.GetInventory().CreateInInventory(RandomAmmo8);

			}
			if (selectedLoadout == 1)
			{
				MissionObject.GetInventory().CreateInInventory(RandomFood1);
				MissionObject.GetInventory().CreateInInventory(RandomFood2);
				MissionObject.GetInventory().CreateInInventory(RandomFood3);
				MissionObject.GetInventory().CreateInInventory(RandomFood4);
				MissionObject.GetInventory().CreateInInventory(RandomFood5);
				MissionObject.GetInventory().CreateInInventory(RandomFood6);
				MissionObject.GetInventory().CreateInInventory(RandomFood7);
				MissionObject.GetInventory().CreateInInventory(RandomFood8);
				MissionObject.GetInventory().CreateInInventory("CanOpener");
			}
			if (selectedLoadout == 2)
			{
				MissionObject.GetInventory().CreateInInventory("AmmoBox");
				MissionObject.GetInventory().CreateInInventory(RandomAmmo1);
				MissionObject.GetInventory().CreateInInventory(RandomAmmo2);
				MissionObject.GetInventory().CreateInInventory(RandomAmmo3);
				MissionObject.GetInventory().CreateInInventory(RandomAmmo4);
				MissionObject.GetInventory().CreateInInventory(RandomAmmo5);
				MissionObject.GetInventory().CreateInInventory(RandomAmmo6);
				MissionObject.GetInventory().CreateInInventory(RandomAmmo7);
				MissionObject.GetInventory().CreateInInventory(RandomAmmo8);
			}
			if (selectedLoadout == 3)
			{
				MissionObject.GetInventory().CreateInInventory("Splint");
				MissionObject.GetInventory().CreateInInventory("Splint");
				MissionObject.GetInventory().CreateInInventory("BandageDressing");
				MissionObject.GetInventory().CreateInInventory("BandageDressing");
				MissionObject.GetInventory().CreateInInventory("Morphine");
				MissionObject.GetInventory().CreateInInventory("Morphine");
				MissionObject.GetInventory().CreateInInventory("Epinephrine");
				MissionObject.GetInventory().CreateInInventory("Epinephrine");
				MissionObject.GetInventory().CreateInInventory("TetracyclineAntibiotics");
				MissionObject.GetInventory().CreateInInventory("TetracyclineAntibiotics");
				MissionObject.GetInventory().CreateInInventory("PainkillerTablets");
				MissionObject.GetInventory().CreateInInventory("PainkillerTablets");
				MissionObject.GetInventory().CreateInInventory("SalineBagIV");
				MissionObject.GetInventory().CreateInInventory("SalineBagIV");
			}
			if (selectedLoadout == 4)
			{
				MissionObject.GetInventory().CreateInInventory("FlashGrenade");
				MissionObject.GetInventory().CreateInInventory("FlashGrenade");
				MissionObject.GetInventory().CreateInInventory("FlashGrenade");
				MissionObject.GetInventory().CreateInInventory("M18SmokeGrenade_Green");
				MissionObject.GetInventory().CreateInInventory("M18SmokeGrenade_Green");
				MissionObject.GetInventory().CreateInInventory("M18SmokeGrenade_Purple");
				MissionObject.GetInventory().CreateInInventory("M18SmokeGrenade_Purple");
				MissionObject.GetInventory().CreateInInventory("M18SmokeGrenade_Red");
				MissionObject.GetInventory().CreateInInventory("M18SmokeGrenade_Red");
				MissionObject.GetInventory().CreateInInventory("M67Grenade");
				MissionObject.GetInventory().CreateInInventory("M67Grenade");
				MissionObject.GetInventory().CreateInInventory("M67Grenade");
				MissionObject.GetInventory().CreateInInventory("RGD5Grenade");
				MissionObject.GetInventory().CreateInInventory("RGD5Grenade");
				MissionObject.GetInventory().CreateInInventory("RGD5Grenade");
			}
			if (selectedLoadout == 5)
			{
				MissionObject.GetInventory().CreateInInventory("FlashGrenade");
				MissionObject.GetInventory().CreateInInventory("FlashGrenade");
				MissionObject.GetInventory().CreateInInventory("FlashGrenade");
				MissionObject.GetInventory().CreateInInventory("M18SmokeGrenade_Green");
				MissionObject.GetInventory().CreateInInventory("M18SmokeGrenade_Green");
				MissionObject.GetInventory().CreateInInventory("M18SmokeGrenade_Purple");
				MissionObject.GetInventory().CreateInInventory("M18SmokeGrenade_Purple");
				MissionObject.GetInventory().CreateInInventory("M18SmokeGrenade_Red");
				MissionObject.GetInventory().CreateInInventory("M18SmokeGrenade_Red");
				MissionObject.GetInventory().CreateInInventory("M67Grenade");
				MissionObject.GetInventory().CreateInInventory("M67Grenade");
				MissionObject.GetInventory().CreateInInventory("M67Grenade");
				MissionObject.GetInventory().CreateInInventory("RGD5Grenade");
				MissionObject.GetInventory().CreateInInventory("RGD5Grenade");
				MissionObject.GetInventory().CreateInInventory("RGD5Grenade");
			}
			if (selectedLoadout == 6)
			{
					MissionObject.GetInventory().CreateInInventory("NailBox");
					MissionObject.GetInventory().CreateInInventory("NailBox");
					MissionObject.GetInventory().CreateInInventory("MetalWire");
					MissionObject.GetInventory().CreateInInventory("Hammer");
					MissionObject.GetInventory().CreateInInventory("Pliers");
					MissionObject.GetInventory().CreateInInventory("Hatchet");
					MissionObject.GetInventory().CreateInInventory("Hacksaw");
					MissionObject.GetInventory().CreateInInventory("Whetstone");
					MissionObject.GetInventory().CreateInInventory("Battery9V");
			}
			if (selectedLoadout == 7)
			{
				MissionObject.GetInventory().CreateInInventory("NVGoggles");
				MissionObject.GetInventory().CreateInInventory("NVGoggles");
				MissionObject.GetInventory().CreateInInventory("PersonalRadio");
				MissionObject.GetInventory().CreateInInventory("PersonalRadio");
				MissionObject.GetInventory().CreateInInventory("PersonalRadio");
				MissionObject.GetInventory().CreateInInventory("Battery9V");
				MissionObject.GetInventory().CreateInInventory("Battery9V");
				MissionObject.GetInventory().CreateInInventory("Battery9V");
				MissionObject.GetInventory().CreateInInventory("M4_Suppressor");
				MissionObject.GetInventory().CreateInInventory("M4_Suppressor");
				MissionObject.GetInventory().CreateInInventory("PistolSuppressor");
				MissionObject.GetInventory().CreateInInventory("PistolSuppressor");
				MissionObject.GetInventory().CreateInInventory("AK_Suppressor");
				MissionObject.GetInventory().CreateInInventory("AK_Suppressor");
				MissionObject.GetInventory().CreateInInventory("UniversalLight");
				MissionObject.GetInventory().CreateInInventory("UniversalLight");
			}
			if (selectedLoadout == 8)
			{
				MissionObject.GetInventory().CreateInInventory("NVGoggles");
				MissionObject.GetInventory().CreateInInventory("NVGoggles");
				MissionObject.GetInventory().CreateInInventory("PersonalRadio");
				MissionObject.GetInventory().CreateInInventory("PersonalRadio");
				MissionObject.GetInventory().CreateInInventory("PersonalRadio");
				MissionObject.GetInventory().CreateInInventory("Battery9V");
				MissionObject.GetInventory().CreateInInventory("Battery9V");
				MissionObject.GetInventory().CreateInInventory("Battery9V");
				MissionObject.GetInventory().CreateInInventory("M4_Suppressor");
				MissionObject.GetInventory().CreateInInventory("M4_Suppressor");
				MissionObject.GetInventory().CreateInInventory("PistolSuppressor");
				MissionObject.GetInventory().CreateInInventory("PistolSuppressor");
				MissionObject.GetInventory().CreateInInventory("AK_Suppressor");
				MissionObject.GetInventory().CreateInInventory("AK_Suppressor");
				MissionObject.GetInventory().CreateInInventory("UniversalLight");
				MissionObject.GetInventory().CreateInInventory("UniversalLight");
			}
			if (selectedLoadout == 9)
			{
				MissionObject.GetInventory().CreateInInventory("LandMineTrap");
				MissionObject.GetInventory().CreateInInventory("LandMineTrap");
				MissionObject.GetInventory().CreateInInventory("PersonalRadio");
				MissionObject.GetInventory().CreateInInventory("Battery9V");
				MissionObject.GetInventory().CreateInInventory("LandMineTrap");
				MissionObject.GetInventory().CreateInInventory("Hacksaw");
				MissionObject.GetInventory().CreateInInventory("NVGoggles");
				MissionObject.GetInventory().CreateInInventory("Hacksaw");

			}	
					
			Print("[SMM] Mission rewards spawned in reward container"+i+". Randomly selected loadout was "+selectedLoadout+"." );
				
			//Insert mission container into mission objects list
			m_MissionObjects.Insert( MissionObject );
		}
							
		Print("[SMM] Survivor Mission "+ m_selectedMission +" :: "+ m_MissionName +" ...mission deployed!");
	}
	
		

// IMPORTANT: If you add an entry to the above list, pick the loadout for it by adding the loadout filename to this list
//		ref array<string> patrol_loadouts = {"SoldierLoadout.json"};

// you may change these quantities
//const int NUMBER_PER_PATROL = 5;	// Number of AI per patrol
//const int MAXR = 800;				// This is the main spawn radius- how close a player needs to be to spawn them in.
//const int MINR = 250;				// If a player is this close to the patrol start point, it is too close for them to spawn
//const int DESPAWNR = 1000;			// If all players are this far away, they despawn.
//const int RESPAWN_SECONDS = 300;	// The amount of seconds before a new patrol will spawn, after they both die

// channel your inner M.C. Hammer - can't touch this
//autoptr array<autoptr eAIDynamicPatrol> patrols = {};
#ifdef EAI
	eAIGroup m_activeGroup = null;
/*void SpawnSentry(vector pos, string loadout = "SoldierLoadout.json") {
    eAIGame game = MissionServer.Cast(GetGame().GetMission()).GetEAIGame();
    eAIBase ai = game.SpawnAI_Sentry(pos, loadout);
    ai.GetGroup().SetFaction(new eAIFactionWest());
	m_MissionAIs.Insert(ai);
//	missionbots.Insert(ai);
}*/
void SpawnPatrol(vector pos, string loadout = "HeliMissionPatrol.json") {
    eAIGame game = MissionServer.Cast(GetGame().GetMission()).GetEAIGame();
    eAIBase ai = game.SpawnAI_Patrol(pos, loadout);
//	m_MissionAIs.Insert(ai);
	m_activeGroup = ai.GetGroup();
	deletethis = ai.GetGroup();
		ai.GetGroup().SetFaction(new eAIFactionWest());
	ai.GetGroup().SetSpeedLimit(1.0);
	foreach (vector v : patrol_100) {m_activeGroup.AddWaypoint(v);}
		int count = 3; // temporary so we don't decrease the actual amount
		while (count > 1) {
			(game.SpawnAI_Helper(ai, loadout)).RequestTransition("Rejoin");
			//m_MissionAIs.Insert(ai);
			count--;
		}
		ai.SetUnlimitedMags(true);
}
	//	return m_activeGroup;
//    ai.GetGroup().SetFaction(new eAIFactionWest());
//	m_MissionAIs.Insert(ai);
//	missionbots.Insert(ai);

/*void InitHeliPatrol() {
ref array<vector> patrol_100 = {"-34 0 -34","-34 0 0","-34 0 34","0 0 34","34 0 34","34 0 0","34 0 -34","0 0 -34"};
        // channel your inner M.C. Hammer - can't touch this
        autoptr array<autoptr eAIDynamicPatrol> patrols = {};
        //Spawn eAI
        for (int i = 0; i < patrol_100.Count(); i++) {
            vector pos = Heli.ModelToWorld() + patrol_100[i];
            float x = pos[0] + Math.RandomFloat(-12, 12);
            float z = pos[2] + Math.RandomFloat(-12, 12);
            float y = GetGame().SurfaceY(x, z);
            patrol_100[i] = (x, y, z);
        }
        autoptr eAIDynamicPatrol pat = new eAIDynamicPatrol(patrol_100[0], patrol_100, "RandCivLoadout.json", 3, 5, 500, 2000, 5000);
        patrols.Insert(pat);
        pat.UpdateTriggers();
		//add eAI to the mission AI array
for ( i=0; i < patrols.Count(); i++)
{
    m_MissionAIs.Insert( Object.Cast( patrols.Get(i) ));
}
}*/
#endif
	void SpawnAIs()
	{	
		
		for (int u = 0; u < patrol_100.Count(); u++) {
            vector pos = m_MissionPosition + patrol_100[u];
            float x = pos[0] + Math.RandomFloat(-12, 12);
            float z = pos[2] + Math.RandomFloat(-12, 12);
            float y = GetGame().SurfaceY(x, z);
            patrol_100[u] = Vector (x, y, z);
        }
		vector gpos = { Math.RandomFloatInclusive(-10.0, -3.0), 0, Math.RandomFloatInclusive(-10.0, -3.0) };
	//	vector gpos = { -40, 0, -40};
	//	vector hpos = { Math.RandomFloatInclusive(-10.0, 10.0), 0, Math.RandomFloatInclusive(-10.0, 10.0) };		
			vector SoldierBotPos1 = Heli.ModelToWorld( gpos );
	//		vector SoldierBotPos2 = Heli.ModelToWorld( hpos );
	//SpawnSentry(SoldierBotPos1);
	//SpawnSentry(SoldierBotPos2);
	SpawnPatrol(SoldierBotPos1);
		//InitHeliPatrol();
		//SpawnSentry(pos, true);
		
		
		//eAIGame game = MissionServer.Cast(GetGame().GetMission()).GetEAIGame();
	//	eAIBase ai = game.SpawnAI_Sentry(soldierbotpos, false);
	//	vector soldierbotpos2 = Heli.ModelToWorld( "5 0 -12" );
	//	m_MissionAIs.Insert( GetGame().CreateObject( "eAI_SurvivorM_Mirek" , soldierbotpos2 , false , true ) );
		
		/*
		if (m_MissionLocation == "Northwest Airfield")
		{
			BotGroupMissionNWAHeli();
		}
		
		if (m_MissionLocation == "Northeast Airfield")
		{

			BotGroupMissionNEAHeli();
		}
		
		if (m_MissionLocation == "Balota Airstrip")
		{
			
			BotGroupMissionBalHeli();
			
		}
		*/
		/*
		//Spawn infected pilot
		vector InfectedPos = Heli.ModelToWorld( "3 0 -9" );
		m_MissionAIs.Insert( GetGame().CreateObject( "ZmbM_CommercialPilotOld_Olive" , InfectedPos , false , true ) );
		DayZInfected InfectedSurvivor = DayZInfected.Cast( m_MissionAIs[0] );
			InfectedSurvivor.GetInventory().CreateAttachment("ZSh3PilotHelmet_Green");
			InfectedSurvivor.GetInventory().CreateAttachment("UKAssVest_Olive");
			InfectedSurvivor.GetInventory().CreateInInventory("Glock19");
			InfectedSurvivor.GetInventory().CreateInInventory("Mag_Glock_15Rnd");
			InfectedSurvivor.GetInventory().CreateInInventory("Battery9V");
			InfectedSurvivor.GetInventory().CreateInInventory("Battery9V");
			InfectedSurvivor.GetInventory().CreateInInventory("Battery9V");
					
			InfectedSurvivor.SetHealth("","",15);
		*/
		
		//Spawn infected
		
		//for ( int j = 0 ; j < InfectedSpawns.Count() ; j++ )
		//{
    	//   	string RandomInfected = InfectedTypes.GetRandomElement();
		//	vector InfectedPos1 = Heli.ModelToWorld( InfectedSpawns.Get(j) );
		//	m_MissionAIs.Insert( GetGame().CreateObject( RandomInfected, InfectedPos1, false, true ) );
		//}	
		
	}
	
	//		vector SoldierBotPos1 = Heli.ModelToWorld( "3 0 -9" );
	//		vector SoldierBotPos2 = Heli.ModelToWorld( "-3 0 9" );
	//	vector SoldierBotPos2 = Heli.ModelToWorld( "3 0 -9" );
		#ifdef EAI
		//ref array<vector> patrol_100  = {SoldierBotPos1, SoldierBotPos2};
	// If you add another patrol array, add it to this list
	//	ref array<array<vector>> patrol_list = {patrol_100};
//	void InitHeliPatrol() {
//	for (int i = 0; i < patrol_list.Count(); i++) {
//		string loadout = "SoldierLoadout.json"; // default
		//if (i < patrol_loadouts.Count()) loadout = patrol_loadouts[i];
//		autoptr eAIDynamicPatrol botpat = new eAIDynamicPatrol(patrol_list[i][0], patrol_list[i], loadout, 5, 50, 800, 1200, 300);
//		patrols.Insert(botpat);
//		botpat.UpdateTriggers();
//	}
//}
#endif

	void ObjDespawn() 
	{	
	
		//Despawn all mission objects at mission timeout except those retains until next mission
		for ( int i = 0; i < m_MissionObjects.Count(); i++ )
		{
			if ( m_MissionObjects.Get(i))
			{
				//Exception: Barricades will remain
				//if ( m_MissionObjects.Get(i).GetType() == "Helipolice_SIB4" ) continue;
				//if ( m_MissionObjects.Get(i).GetType() == "Helipolice_swat_SIB5" ) continue;
				//if ( m_MissionObjects.Get(i).GetType() == "HeliSIB_UH1D" ) continue;
				//if ( m_MissionObjects.Get(i).GetType() == "Heliblack_hawk_SIB" ) continue;
				//if ( m_MissionObjects.Get(i).GetType() == "WoodenCrate" ) continue;
				
			
				//Delete Object
				GetGame().ObjectDelete( m_MissionObjects.Get(i) );
			}		
		}
		
	}
	
	void MissionFinal()
	{	//When player enters last mission target zone	
		/*
		//Respawn some infected 
		for ( int m = 0 ; m < InfectedSpawns.Count() ; m++)
		{
    	   	string RandomInfected = InfectedTypes.GetRandomElement();
			vector InfectedPos = Heli.ModelToWorld( InfectedSpawns.Get(m) );
			m_MissionAIs.Insert( GetGame().CreateObject( RandomInfected, InfectedPos, false, true ));			
		}
		*/
		//Finish mission
		EntityAI.Cast( Heli ).SetLifetime( 3888000 );

		m_MissionObjects.Remove(0);
		m_RewardsSpawned = true;
		m_MsgNum = -1;
		m_MsgChkTime = m_MissionTime + MsgDlyFinish;
		//Spawn infected pilot
		vector InfectedPos = Heli.ModelToWorld( "-9 0 9" );
		m_MissionAIs.Insert( GetGame().CreateObject( "ZmbM_CommercialPilotOld_Olive" , InfectedPos , false , true ) );
		DayZInfected InfectedSurvivor = DayZInfected.Cast( m_MissionAIs[0] );
			InfectedSurvivor.GetInventory().CreateAttachment("ZSh3PilotHelmet_Green");
			InfectedSurvivor.GetInventory().CreateAttachment("UKAssVest_Olive");
			InfectedSurvivor.GetInventory().CreateInInventory("Glock19");
			InfectedSurvivor.GetInventory().CreateInInventory("Mag_Glock_15Rnd");
			InfectedSurvivor.GetInventory().CreateInInventory("Battery9V");
			InfectedSurvivor.GetInventory().CreateInInventory("Battery9V");
		HeliKey = InfectedSurvivor.GetInventory().CreateInInventory("MCK_CarKey_White");
			
		//
		CarScript carScript = CarScript.Cast( Heli );

		if( carScript )
		{
			MCK_CarKey_Base carKey = MCK_CarKey_Base.Cast(HeliKey);            
            if(carKey)
            {
                int mck_id = carKey.GenerateNewID();
				carKey.SetNewMCKId(mck_id);                

                carScript.m_CarKeyId = mck_id;
                carScript.m_HasCKAssigned = true;
				carScript.SynchronizeValues(); 
		//		m_MCKLogger.LogMCKActivity("Heli Mission has assigned new owner and key (ID: " + mck_id + " ) to vehicle " + carScript.GetDisplayName() + " (" + carScript.m_CarScriptId + ")");
            }
			carScript.m_IsCKLocked = true;
			carScript.SynchronizeValues();
			carScript.ResetLifetime();
		}
	}
	
	void PlayerChecks( PlayerBase player )
	{
		//nothing to check
	}
		
	void UpdateBots(float dt)
	{
		//no bots involved in this mission		
	}
	
	bool DeployMission()
	{	//When first player enters the mission zone (primary/secondary)
		if ( m_MissionPosition && m_MissionPosition != "0 0 0" )
		{
			//Call spawners	
			GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).Call( this.SpawnObjects );
			GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).Call( this.SpawnAIs );
			return true;		
		}
		else 
		{
			Print("[SMM] ERROR : Mission position was rejected or doesn't exist. MissionPosition is NULL!");
			return false;
		}
	}
}

