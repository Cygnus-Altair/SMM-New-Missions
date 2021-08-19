class TrainWreckMission extends SurvivorMissions
{
	//Mission related entities 
	Object Heli;
	Object Turret;
	ItemBase MissionObject;
	ItemBase HeliKey;
	//Object Boxcar;
	
	//Mission parameters
	int MsgDlyFinish = 30;					//seconds, message delay time after player has finished mission
	
	//Mission containers
	ref array<vector> ContainerSpawns = new array<vector>;
	ref array<vector> InfectedSpawns = new array<vector>;
	ref array<vector> FireSpawns = new array<vector>;
	ref array<string> InfectedTypes = new array<string>;
	ref array<string> EngineTypes = new array<string>;
	ref array<string> ammoTypes = new array<string>;
	ref array<string> foodTypes = new array<string>;	
	ref array<ref Param3<string,vector,vector>> Wreckage = new array<ref Param3<string,vector,vector>>;
//	ref array<string> missionbots = new array<string>;
//	ref array<string> helperbot = new array<string>;
//	ref array<vector> patrol_100 = {"-44 0 -44","-44 0 0","-44 0 44","0 0 44","44 0 34","44 0 0","44 0 -44","0 0 -44"};
	eAIGroup deletethis = null;
	//Mission variables 
	string SurvivorName;		
		
	bool SentrySpawned = false;
	bool IsExtended() return false;
	
	void TrainWreckMission()
	{
		//Mission mission timeout
		m_MissionTimeout = 2400;			//seconds, mission duration time
		
		//Mission zones
		m_MissionZoneOuterRadius = 500.0;	//meters (!Do not set lower than 200m), mission activation distance
		m_MissionZoneInnerRadius = 5.0;		//meters (!Do not set outside of 1-5m), mission finishing distance to target object
				
		//Mission informant
		m_MissionInformant = "The Information Broker";
	
		//Mission person names
		SurvivorName = "Gerry Lane";
		
		//Set mission messages
        m_MissionMessage1 = "Be advised: A train has crashed "+ m_MissionLocationDir +" of "+ m_MissionLocation + " attracting a horde of infected.";
        m_MissionMessage2 = "From what I can tell, all the survivors except one have fled with all they can carry.";
        m_MissionMessage3 = "Go to the wreck and help this last survivor deal with the horde and salvage what remains.";		
		
		//Engine fire spawns 
		FireSpawns.Insert("0.5 -1.0 -0.1");		//engine
		/*FireSpawns.Insert("5.15 -2.18 -5");			//engine
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
		ContainerSpawns.Insert("-16 1 4");
		ContainerSpawns.Insert("-15 1 4");
		
		
			
		
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
		
		EngineTypes.Insert("Land_Train_742_Red");		EngineTypes.Insert("Land_Train_742_Blue");
		string RandomEngine = EngineTypes.GetRandomElement();
		Wreckage.Insert( new Param3<string,vector,vector>(RandomEngine, "3 1.3 6.5", "30 0 0"));
		Wreckage.Insert( new Param3<string,vector,vector>("Land_Train_Wagon_Box", "4.5 .70 16.2", "-38 0 0"));
		Wreckage.Insert( new Param3<string,vector,vector>("vbldr_train_wagon_tanker", "3.26 .70 26", "30 0 0"));
	//	Wreckage.Insert( new Param3<string,vector,vector>("vbldr_train_wagon_tanker", ".58 1 25", "0 0 0"));
		Wreckage.Insert( new Param3<string,vector,vector>("Nitrix_Train_Wagon_Flat", "-3 0.25 31", "-105 0 -95"));
	//	Wreckage.Insert( new Param3<string,vector,vector>("bldr_wreck_t72_turret", "0.25 1.4 1.5", "-5 0 0"));
		Wreckage.Insert( new Param3<string,vector,vector>("SmokePoint_1", "0 0 0", "0 0 0"));
		Wreckage.Insert( new Param3<string,vector,vector>("SmokePoint_2", "3 1.5 6.5", "0 0 0"));
	}
	
	void ~TrainWreckMission()
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
	//	#ifdef EAI
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
	#ifdef EAI
		
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
	

	void SpawnObjects()
	{
		
		//Spawn crashed plane crashed C130 
		int PlaneOriConversion;
		string PlaneOrientation;
		
		PlaneOriConversion = m_MissionDescription[3].ToInt() - 180;
		if ( PlaneOriConversion < 0 ) PlaneOriConversion += 360;
		PlaneOrientation = PlaneOriConversion.ToString() +" 0 0";
		

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
		
		
	//	vector trainvector = "2 -0.5 -15";
		Heli = GetGame().CreateObject("bldr_wreck_t72_chassis", m_MissionPosition);
		Heli.SetOrientation( PlaneOrientation.ToVector() );
		Heli.PlaceOnSurface();
		
		EntityAI.Cast( Heli ).SetLifetime( (m_MissionTimeout - m_MissionTime) + 30 );
		m_MissionObjects.Insert( Heli );
		
		for ( int i=0; i < Wreckage.Count(); i++ )
		{
			Param3<string,vector,vector> WreckageDef = Wreckage.Get(i);
			string WreckageType = WreckageDef.param1;
			vector WreckagePos = Heli.ModelToWorld( WreckageDef.param2 );
			vector WreckageOri = WreckageDef.param3;
			vector WreckageDir = Heli.GetDirection();
			
			
			Object PlankBarricade = GetGame().CreateObject( WreckageType, WreckagePos, false, false, true );
			PlankBarricade.SetPosition( WreckagePos );
			PlankBarricade.SetDirection( WreckageDir );
			PlankBarricade.SetOrientation( PlankBarricade.GetOrientation() + WreckageOri );

			vector FlatCar = "1 1 0";
			vector FlatCarH = PlankBarricade.ModelToWorld(FlatCar);
			if (WreckageType == "Nitrix_Train_Wagon_Flat") PlankBarricade.SetPosition(FlatCarH);
			if (WreckageType == "Land_Train_Wagon_Box") m_MissionPosition = WreckagePos;
			EntityAI.Cast( PlankBarricade ).SetLifetime( (m_MissionTimeout - m_MissionTime) + 300 );
			m_MissionObjects.Insert( PlankBarricade );
		}
		PlaneOriConversion = m_MissionDescription[3].ToInt() - 90;
		if ( PlaneOriConversion < 0 ) PlaneOriConversion += 360;
		PlaneOrientation = PlaneOriConversion.ToString() +" 0 0";
		Heli.SetOrientation( PlaneOrientation.ToVector() );
		Heli.PlaceOnSurface();
		vector TurretHeight = "2.5 1.3 0";
		vector TurretAngle = "95 0 0";
		vector TurretDir = Heli.GetDirection();
		vector TurretPos = Heli.ModelToWorld(TurretHeight);
		Turret = GetGame().CreateObject( "bldr_wreck_t72_turret", TurretPos);
		Turret.SetDirection(TurretDir);
		Turret.SetOrientation( Turret.GetOrientation() + TurretAngle );
		m_MissionObjects.Insert( Turret );
		//Enginefire
		
		for (int s=0; s < FireSpawns.Count(); s++)
		{
			FireplaceBase Enginefire;
			Enginefire = FireplaceBase.Cast( GetGame().CreateObject( "Fireplace", Heli.ModelToWorld( FireSpawns.Get(0) )));
			Enginefire.Synchronize();
			Enginefire.GetInventory().CreateAttachment("Firewood");
			Enginefire.GetInventory().CreateAttachment("WoodenStick");
			Enginefire.GetInventory().CreateAttachment("Rag");
			Enginefire.StartFire( true );
			m_MissionObjects.Insert( Enginefire );
		}

		int selectedLoadout;
		
		for (int k=0; k < ContainerSpawns.Count(); k++)
		{	
			MissionObject = ItemBase.Cast( GetGame().CreateObject( "WoodenCrate", Heli.ModelToWorld( ContainerSpawns.Get(k))));
			
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
					MissionObject.GetInventory().CreateInInventory("CodeLock");
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
	
		


#ifdef EAI

	
void SpawnSentry(PlayerBase playername, string loadout = "HeliMissionPatrol.json") {
    eAIGame game = MissionServer.Cast(GetGame().GetMission()).GetEAIGame();
     (game.SpawnAI_Helper( playername, loadout )).RequestTransition("Rejoin");
	 	deletethis = game.GetGroupByLeader(playername);
		eAIGroup gg = deletethis;
				for (int rr = gg.Count() - 1; rr > -1; rr--) {
					PlayerBase pp = gg.GetMember(rr);
					if (pp.IsAI()) {
			Weapon_Base weapon = Weapon_Base.Cast(pp.GetItemInHands());
			weapon.GetInventory().CreateAttachment("M4_OEBttstck");
			weapon.GetInventory().CreateAttachment("M4_PlasticHndgrd");
			weapon.GetInventory().CreateAttachment("M4_CarryHandleOptic");
					}
				}	

	}
#endif
	void SpawnAIs()
	{	

		
		for ( int q = 0 ; q < InfectedSpawns.Count() ; q++ )
		{
    	   	string RandomInfected = InfectedTypes.GetRandomElement();
			vector InfectedPos1 = Heli.ModelToWorld( InfectedSpawns.Get(q) );
			m_MissionAIs.Insert( GetGame().CreateObject( RandomInfected, InfectedPos1, false, true ) );
		}	
		
	}
	


	void ObjDespawn() 
	{	

	}
	
	void MissionFinal()
	{	
		

		m_MissionObjects.Remove(0);
		m_RewardsSpawned = true;
		m_MsgNum = -1;
		m_MsgChkTime = m_MissionTime + MsgDlyFinish;

	}
	
	void PlayerChecks( PlayerBase player )
	{
		if ( !SentrySpawned ) //is the same as SentrySpawned == false!
		{
      //check the "player" and spawn it... or whatever u want to do
    //  then
		SpawnSentry(player,"HumanLoadout.json");
		SentrySpawned = true;
		}
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

