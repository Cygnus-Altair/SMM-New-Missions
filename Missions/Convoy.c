class ConvoyMission extends SurvivorMissions
{
	int MissionCutoffTime;
	//Mission related entities 
	Car Heli; // vehicle reward, for this mission Heli is actually a hmmwv or dingo.
	bool createIfNoneExists = true
	Car TempHeli
	Object Turret;
	ItemBase MissionObject;
	ItemBase HeliKey;
	Object Foxhole1;
	Object Foxhole2;
	vector Foxholepos;
	ExpansionMarkerData m_ExpansionServerMarker;
	ExpansionMarkerModule m_ExpansionMarkerModule;
	string MarkerName;
	string MarkerIcon;
	#ifdef EXPANSIONMOD
		#ifdef ENFUSION_AI_PROJECT
		//eAIFactionCivilian civilian2 = new eAIFactionCivilian();
		#endif
		#endif
	//Object Boxcar;
	int HordeDensity = 30;					//infected per wave 
	//Mission parameters
	int MsgDlyFinish = 30;					//seconds, message delay time after player has finished mission
	
	//Mission containers
	ref array<vector> ContainerSpawns = new array<vector>;
	ref array<vector> InfectedSpawns = new array<vector>;
	ref array<vector> FireSpawns = new array<vector>;
	ref array<string> InfectedTypes = new array<string>;
	//ref array<string> VehicleTypes = new array<string>;
	ref array<string> ammoTypes = new array<string>;
	ref array<string> foodTypes = new array<string>;	
	ref array<string> WreckedVehicle = new array<string>;
	ref array<vector> WreckVector = new array<vector>;
	ref array<vector> scoutlocation = new array<vector>;
	ref array<ref Param3<string,vector,vector>> Wreckage = new array<ref Param3<string,vector,vector>>;
	ref array<ref Param3<string,string,string>> Vehicles = new array<ref Param3<string,string,string>>;
	//we need an array where we hold those dead infected
    ref array<Object> DeadInfected = new array<Object>;
//	ref array<string> missionbots = new array<string>;
//	ref array<string> helperbot = new array<string>;
//	ref array<vector> patrol_100 = {"-44 0 -44","-44 0 0","-44 0 44","0 0 44","44 0 34","44 0 0","44 0 -44","0 0 -44"};
	eAIGroup deletethis = null;
	//Mission variables 
	string SurvivorName;	
	string SelectedVehicle;
	string VehicleType;
	string VehicleColor1;
	string VehicleColor2;
	vector scoutpos;

	
	bool SentrySpawned = false;
	bool IsExtended() return false;
	bool secondwavespawned = false;
	
	void ConvoyMission()
	{
		CF_Modules<ExpansionMarkerModule>.Get(m_ExpansionMarkerModule);
        MarkerName = "Defend Convoy";
        MarkerIcon = "Vehicle Crash";
        CreateExpansionServerMarker();
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
        m_MissionMessage1 = "A Convoy has been overrun by infected "+ m_MissionLocationDir +" of "+ m_MissionLocation + ". The remaining survivors are locked out of their vehicle.";
        m_MissionMessage2 = "They just finished burying the dead in their foxhole, but now they see more waves of incoming.";
        m_MissionMessage3 = "Help them fight off the infected. This will give them time to find the key to unlock their remaining vehicle.";		
		
		//Engine fire spawns 
		FireSpawns.Insert("0.5 -1.0 -0.1");		//engine

		//Container spawnpoints
		ContainerSpawns.Insert("-3 1 4");
		ContainerSpawns.Insert("-4 1 3");
		
		WreckVector.Insert("5 .70 -11.2");		WreckVector.Insert("-.05 .7 -10.2");	
		WreckVector.Insert("0.5 0 30");			WreckVector.Insert("5.5 0 29");
		WreckVector.Insert("5.5 0 18");			WreckVector.Insert("0 0 19");
	//	WreckVector.Insert("0 0.25 -32");		//WreckVector.Insert("5 0.25 -35"); //commented out since moved to scoutpos.
		WreckVector.Insert("0 0 0");			WreckVector.Insert("5 0 0");
		
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
		
		scoutlocation.Insert("5 0 -30");
		scoutlocation.Insert("0 0 -29");
		
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
		InfectedTypes.Insert("ZmbF_BlueCollarFat_Green");	InfectedTypes.Insert("ZmbF_ShortSkirt_beige");
		InfectedTypes.Insert("ZmbM_PolicemanSpecForce");	InfectedTypes.Insert("ZmbF_DoctorSkinny");
		InfectedTypes.Insert("ZmbF_VillagerOld_Red");
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
		InfectedTypes.Insert("ZmbM_Jacket_magenta");		InfectedTypes.Insert("ZmbM_Jacket_black");
		InfectedTypes.Insert("ZmbM_Jacket_stripes");
		
		//you can change this to whatever you want the vehicle to be.  beware of the naming scheme you might need to change code a bit.
		//these two vehicle types have a standard naming convention for door parts, which lets you select the color you want easily.
		Vehicles.Insert( new Param3<string,string,string>("rag_hummer", "", "_Armored"));
		Vehicles.Insert( new Param3<string,string,string>("rag_hummer", "_gold", "_Armored"));
		Vehicles.Insert( new Param3<string,string,string>("rag_hummer", "_camo", "_Armored"));
		Vehicles.Insert( new Param3<string,string,string>("rag_hummer", "_tan", "_Armored"));
		Vehicles.Insert( new Param3<string,string,string>("rag_hummer", "_red", "_Armored"));
		Vehicles.Insert( new Param3<string,string,string>("rag_hummer", "_purple", "_Armored"));
		Vehicles.Insert( new Param3<string,string,string>("rag_hummer", "_winter", "_Armored"));
		Vehicles.Insert( new Param3<string,string,string>("rag_hummer", "_anarchy", "_Armored"));
	//	Vehicles.Insert( new Param3<string,string,string>("Dingo", "", ""));
	//	Vehicles.Insert( new Param3<string,string,string>("Dingo", "_Don", "_DARKGREEN"));
	//	Vehicles.Insert( new Param3<string,string,string>("Dingo", "_Don", "_WTR"));
	//	Vehicles.Insert( new Param3<string,string,string>("Dingo", "_Don", "_CDF"));
		//Vehicles.Insert( new Param3<string,string,string>("Dingo", "_Don", "_CP"));
		//Vehicles.Insert( new Param3<string,string,string>("Dingo", "_Don", "_UN"));
	//	Vehicles.Insert( new Param3<string,string,string>("Dingo", "_Don", "_CZ_Woodland"));
		//Vehicles.Insert( new Param3<string,string,string>("Dingo", "_Don", "_CZ_DESERT"));
		//Vehicles.Insert( new Param3<string,string,string>("Dingo", "_Don", "_GER_DESERT"));
		//Vehicles.Insert( new Param3<string,string,string>("ZIL_131", "", ""));
		Vehicles.Insert( new Param3<string,string,string>("rag_mtvr", "", ""));
		//Get random vehicle 
		int	RandomVehicle = Math.RandomIntInclusive(0,7);	//!change randomization limit after adding new vehicles!
		Param3<string,string,string> VehiclesDef = Vehicles.Get(RandomVehicle);	
		VehicleType = VehiclesDef.param1;
		VehicleColor1 = VehiclesDef.param2;
		VehicleColor2 = VehiclesDef.param3;
		SelectedVehicle = VehicleType+VehicleColor1+VehicleColor2;
	//	string RandomWreck = WreckVehicle.GetRandomElement();
		WreckedVehicle.Insert( SelectedVehicle );		WreckedVehicle.Insert("bldr_wreck_bmp2");
		WreckedVehicle.Insert("bldr_wreck_hmmwv");		WreckedVehicle.Insert("bldr_wreck_ural"); 
		WreckedVehicle.Insert("Land_Wreck_Uaz");		WreckedVehicle.Insert("Land_wreck_truck01_aban2_blue");
		WreckedVehicle.Insert("Land_wreck_truck01_aban2_green");	WreckedVehicle.Insert("Land_wreck_truck01_aban1_blue");  //comment this vehicle out if you dont have SV mod.
				
		string RandomWreck1 = WreckedVehicle.GetRandomElement(); 
		//ensuring a scout vehicle will be picked (personal preference)
		while(RandomWreck1 != "bldr_wreck_hmmwv" && RandomWreck1 != "Land_Wreck_Uaz" ){ RandomWreck1 = WreckedVehicle.GetRandomElement();}
		WreckedVehicle.RemoveItem(RandomWreck1);  //removing wreck from array so it wont be chosen twice.
		
		string RandomWreck2 = WreckedVehicle.GetRandomElement();
		WreckedVehicle.RemoveItem(RandomWreck2);

		string RandomWreck3 = WreckedVehicle.GetRandomElement();
		WreckedVehicle.RemoveItem(RandomWreck3);

		string RandomWreck4 = WreckedVehicle.GetRandomElement();
		WreckedVehicle.RemoveItem(RandomWreck4);

		string RandomWreck5 = WreckedVehicle.GetRandomElement();
		WreckedVehicle.RemoveItem(RandomWreck5);
		string RandomWreck6 = WreckedVehicle.GetRandomElement();
		while(RandomWreck1 != SelectedVehicle && RandomWreck2 != SelectedVehicle && RandomWreck3 != SelectedVehicle && RandomWreck4 != SelectedVehicle && RandomWreck5 != SelectedVehicle && RandomWreck6 != SelectedVehicle){ RandomWreck6 = WreckedVehicle.GetRandomElement();}
		WreckedVehicle.RemoveItem(RandomWreck6);
		
	//	vector Wpos1 = WreckVector.GetRandomElement();
	//	WreckVector.RemoveItem(Wpos1);
		vector Wpos2 = WreckVector.GetRandomElement();
		WreckVector.RemoveItem(Wpos2);
		vector Wpos3 = WreckVector.GetRandomElement();
		WreckVector.RemoveItem(Wpos3);
		vector Wpos4 = WreckVector.GetRandomElement();
		WreckVector.RemoveItem(Wpos4);
		vector Wpos5 = WreckVector.GetRandomElement();
		WreckVector.RemoveItem(Wpos5);
		vector Wpos6 = WreckVector.GetRandomElement();
		WreckVector.RemoveItem(Wpos6);
		
		scoutpos = scoutlocation.GetRandomElement();  //using this instead of Wpos1 to ensure that a scout type vehicle will always be up front.
		
		Wreckage.Insert( new Param3<string,vector,vector>(RandomWreck1, scoutpos, "0 0 0"));
		Wreckage.Insert( new Param3<string,vector,vector>(RandomWreck2, Wpos2, "0 0 0"));
		Wreckage.Insert( new Param3<string,vector,vector>(RandomWreck3, Wpos3, "-1 0 0"));
		Wreckage.Insert( new Param3<string,vector,vector>(RandomWreck4, Wpos4, "0 0 0"));
		Wreckage.Insert( new Param3<string,vector,vector>(RandomWreck5, Wpos5, "0 0 0"));
		Wreckage.Insert( new Param3<string,vector,vector>(RandomWreck6, Wpos6, "0 0 0"));

		Wreckage.Insert( new Param3<string,vector,vector>("Land_Dead_MassGrave", "-5 0 8", "0 0 0"));

    MissionCutoffTime = MissionSettings.RestartCycleTime - (m_MissionTimeout + MissionSettings.DelayTime);
    
    if ( GetGame().GetTime() * 0.001 > MissionCutoffTime )
    MissionSettings.DelayTime = 3600;
	}
	#ifdef EXPANSIONMODNAVIGATION
    void CreateExpansionServerMarker()
    {

        if (!CF_Modules<ExpansionMarkerModule>.Get(m_ExpansionMarkerModule))
        return;

        m_ExpansionServerMarker = m_ExpansionMarkerModule.CreateServerMarker(MarkerName, MarkerIcon, m_MissionPosition, ARGB(255,50,50,255), true);
    }

    void RemoveExpansionServerMarker()
    {
        if ( !m_ExpansionServerMarker )
            return;
        
        m_ExpansionMarkerModule.RemoveServerMarker( m_ExpansionServerMarker.GetUID() );
    }
    #endif
	void ~ConvoyMission()
	{
		RemoveExpansionServerMarker();
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
		
		Foxhole1 = GetGame().CreateObject("bldr_wreck_decal_small2",m_MissionPosition);
		Foxhole1.SetOrientation( PlaneOrientation.ToVector() );
		Foxhole1.PlaceOnSurface();
		m_MissionObjects.Insert(Foxhole1);

		
		
		
		for ( int i=0; i < Wreckage.Count(); i++ )
		{
			Param3<string,vector,vector> WreckageDef = Wreckage.Get(i);
			string WreckageType = WreckageDef.param1;
			vector WreckagePos = Foxhole1.ModelToWorld( WreckageDef.param2 );
			vector WreckageOri = WreckageDef.param3;
			vector WreckageDir = Foxhole1.GetDirection();
			if (WreckageType == "bldr_wreck_hmmwv") vector HMMWVpos = WreckagePos;
			Print("Selected Vehicle is: " + SelectedVehicle );
			if (WreckageType == SelectedVehicle){
			Heli = GetGame().CreateObject(SelectedVehicle, WreckagePos);
				if (VehicleType == "rag_hummer")
				{
					Heli.GetInventory().CreateAttachment("rag_hummer_wheel");
					Heli.GetInventory().CreateAttachment("rag_hummer_wheel");
					Heli.GetInventory().CreateAttachment("rag_hummer_wheel");
					Heli.GetInventory().CreateAttachment("rag_hummer_wheel");
					Heli.GetInventory().CreateAttachment("SparkPlug");
					Heli.GetInventory().CreateAttachment("CarBattery");
					Heli.GetInventory().CreateAttachment("CarRadiator");
					Heli.GetInventory().CreateAttachment("HeadlightH7");
					Heli.GetInventory().CreateAttachment("HeadlightH7");
					Heli.GetInventory().CreateAttachment("CanisterGasoline");
					Heli.GetInventory().CreateAttachment("rag_hummer_Door" + VehicleColor1 + "_1");
					Heli.GetInventory().CreateAttachment("rag_hummer_Door" + VehicleColor1 + "_2");
					Heli.GetInventory().CreateAttachment("rag_hummer_Door" + VehicleColor1 + "_3");
					Heli.GetInventory().CreateAttachment("rag_hummer_Door" + VehicleColor1 + "_4");
					Heli.GetInventory().CreateAttachment("rag_hummer_Hood" + VehicleColor1);
					Heli.GetInventory().CreateAttachment("rag_hummer_Trunk" + VehicleColor1);
					
				}
				if (VehicleType == "rag_mtvr")
				{
					Heli.GetInventory().CreateAttachment("rag_mtvr_wheel");
					Heli.GetInventory().CreateAttachment("rag_mtvr_wheel");
					Heli.GetInventory().CreateAttachment("rag_mtvr_wheel");
					Heli.GetInventory().CreateAttachment("rag_mtvr_wheel");
					Heli.GetInventory().CreateAttachment("rag_mtvr_wheel");
					Heli.GetInventory().CreateAttachment("rag_mtvr_wheel");
					Heli.GetInventory().CreateAttachment("rag_mtvr_wheel");
					Heli.GetInventory().CreateAttachment("SparkPlug");
					Heli.GetInventory().CreateAttachment("TruckBattery");
					Heli.GetInventory().CreateAttachment("CarRadiator");
					Heli.GetInventory().CreateAttachment("HeadlightH7");
					Heli.GetInventory().CreateAttachment("HeadlightH7");
					Heli.GetInventory().CreateAttachment("CanisterGasoline");
					Heli.GetInventory().CreateAttachment("rag_mtvr_door_1_1");
					Heli.GetInventory().CreateAttachment("rag_mtvr_door_1_2");
					Heli.GetInventory().CreateAttachment("rag_mtvr_hood");
					Heli.GetInventory().CreateAttachment("rag_mtvr_cargo1door");
				
				}
				if (VehicleType == "ZIL_131")
				{
					Heli.GetInventory().CreateAttachment("ZIL_131_Wheel");
					Heli.GetInventory().CreateAttachment("ZIL_131_Wheel");
					Heli.GetInventory().CreateAttachment("ZIL_131_Wheel");
					Heli.GetInventory().CreateAttachment("ZIL_131_Wheel");
					Heli.GetInventory().CreateAttachment("ZIL_131_Wheel");
					Heli.GetInventory().CreateAttachment("ZIL_131_Wheel");
					Heli.GetInventory().CreateAttachment("ZIL_131_Wheel");
					Heli.GetInventory().CreateAttachment("SparkPlug");
					Heli.GetInventory().CreateAttachment("CarBattery");
					Heli.GetInventory().CreateAttachment("CarRadiator");
					Heli.GetInventory().CreateAttachment("HeadlightH7");
					Heli.GetInventory().CreateAttachment("HeadlightH7");
					Heli.GetInventory().CreateAttachment("CanisterGasoline");
					Heli.GetInventory().CreateAttachment("ZIL_131_doors_driver");
					Heli.GetInventory().CreateAttachment("ZIL_131_doors_codriver");
					Heli.GetInventory().CreateAttachment("ZIL_131_doors_trunk");
				
				}
				if (VehicleType == "Dingo")
				{
					Heli.GetInventory().CreateAttachment("DingoWheel");
					Heli.GetInventory().CreateAttachment("DingoWheel");
					Heli.GetInventory().CreateAttachment("DingoWheel");
					Heli.GetInventory().CreateAttachment("DingoWheel");
					Heli.GetInventory().CreateAttachment("DingoWheel");
					Heli.GetInventory().CreateAttachment("SparkPlug");
					Heli.GetInventory().CreateAttachment("CarRadiator");
					Heli.GetInventory().CreateAttachment("CarBattery");
					Heli.GetInventory().CreateAttachment("HeadlightH7");
					Heli.GetInventory().CreateAttachment("HeadlightH7");
					Heli.GetInventory().CreateInInventory("CanisterGasoline");
					Heli.GetInventory().CreateAttachment("DingoDoors_Driver" + VehicleColor1 + VehicleColor2);
					Heli.GetInventory().CreateAttachment("DingoDoors_CoDriver" + VehicleColor1 + VehicleColor2);
					Heli.GetInventory().CreateAttachment("DingoDoors_Cargo1" + VehicleColor1 + VehicleColor2);
					Heli.GetInventory().CreateAttachment("DingoDoors_Cargo2" + VehicleColor1 + VehicleColor2);
					Heli.GetInventory().CreateAttachment("DingoHood" + VehicleColor1 + VehicleColor2);
					Heli.GetInventory().CreateAttachment("DingoTrunk" + VehicleColor1 + VehicleColor2);
				}
				Heli.SetOrientation( PlaneOrientation.ToVector() );
				Heli.PlaceOnSurface();
				Print("[SMM] There are "+ m_MissionObjects.Count() +" mission objects before Heli insertion");
				m_MissionObjects.InsertAt( Heli, 0 );
				Print("[SMM] There are "+ m_MissionObjects.Count() +" mission objects after Heli insertion");
	
				Print ("SMM: Mission object count is:" + m_MissionObjects.Count());
				EntityAI.Cast( Heli ).SetLifetime( (m_MissionTimeout - m_MissionTime) + 30 );
				HeliKey = Heli.GetInventory().CreateInInventory("MCK_CarKey_Green");
			
			}
			
			if (WreckageType != SelectedVehicle){
			Object PlankBarricade = GetGame().CreateObject( WreckageType, WreckagePos, false, false, true );
			PlankBarricade.SetPosition( WreckagePos );
			PlankBarricade.SetDirection( WreckageDir );
			PlankBarricade.SetOrientation( PlankBarricade.GetOrientation() + WreckageOri );
			PlankBarricade.PlaceOnSurface();
			if (WreckageType == "bldr_wreck_bmp2"){
			Object Smoke1 = GetGame().CreateObject("SmokePoint_1", PlankBarricade.ModelToWorld( "0.75 0 -0.5" ), false, false, true);
			Object interior1 = GetGame().CreateObject("vbldr_truck_01_engine_prx", PlankBarricade.ModelToWorld( "0.15 -0.25 2.5" ), false, false, true);
			interior1.SetOrientation( PlankBarricade.GetOrientation() + "90 10 0" );
			m_MissionObjects.Insert( Smoke1 ); 
			m_MissionObjects.Insert( interior1 ); }
			if (WreckageType == "bldr_wreck_hmmwv")
			{
				Object Smoke2 = GetGame().CreateObject("SmokePoint_2", PlankBarricade.ModelToWorld( "0 -0.75 -1.5" ), false, false, true);
				m_MissionObjects.Insert( Smoke2 );
			}
			if (WreckageType == "Land_Dead_MassGrave"){	Foxhole2 = PlankBarricade;	}
		//	if (WreckageType == "SmokePoint_1"){
		//		PlankBarricade.SetPosition( BMPpos + WreckageDef.param2 );
		//	}
			
			EntityAI.Cast( PlankBarricade ).SetLifetime( (m_MissionTimeout - m_MissionTime) + 300 );
			m_MissionObjects.Insert( PlankBarricade );
				}

			
		//	vector FlatCar = "1 1 0";
		//	vector FlatCarH = PlankBarricade.ModelToWorld(FlatCar);
		//	if (WreckageType == "Nitrix_Train_Wagon_Flat") PlankBarricade.SetPosition(FlatCarH);
			
				
		}
		Print("[SMM] There are "+ m_MissionObjects.Count() +" mission objects after PlankBarricade");
		//GetGame().ObjectDelete( Foxhole1 );
		PlaneOriConversion = m_MissionDescription[3].ToInt();
	//	if ( PlaneOriConversion < 0 ) PlaneOriConversion += 180;
		PlaneOrientation = PlaneOriConversion.ToString() +" 0 0";
		Heli.SetOrientation( PlaneOrientation.ToVector() );
		Heli.PlaceOnSurface();
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
		//	carScript.ResetLifetime();
		}

		m_MissionPosition = Foxhole2.ModelToWorld("0 0 0");
							
		Print("[SMM] Survivor Mission "+ m_selectedMission +" :: "+ m_MissionName +" ...mission deployed!");
	}
	
		
	void SpawnRewards()
	{
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
		int selectedLoadout;
		
		for (int k=0; k < ContainerSpawns.Count(); k++)
		{	
		//	MissionObject = ItemBase.Cast( GetGame().CreateObject( "WoodenCrate", Heli.ModelToWorld( ContainerSpawns.Get(k))));
			//above line commented out since I replace MissionObject below with Heli, to make reward spawn in car.
			//Get random loadout 
			selectedLoadout = Math.RandomIntInclusive(0,9);	//!change randomization limit after adding new loadouts!	
		
			//Spawn selected loadout items in mission object
			EntityAI weapon;
					
			if (selectedLoadout == 0)
			{
				Heli.GetInventory().CreateInInventory("AmmoBox");
				Heli.GetInventory().CreateInInventory(RandomAmmo1);
				Heli.GetInventory().CreateInInventory(RandomAmmo2);
				Heli.GetInventory().CreateInInventory(RandomAmmo3);
				Heli.GetInventory().CreateInInventory(RandomAmmo4);
				Heli.GetInventory().CreateInInventory(RandomAmmo5);
				Heli.GetInventory().CreateInInventory(RandomAmmo6);
				Heli.GetInventory().CreateInInventory(RandomAmmo7);
				Heli.GetInventory().CreateInInventory(RandomAmmo8);

			}
			if (selectedLoadout == 1)
			{
				Heli.GetInventory().CreateInInventory(RandomFood1);
				Heli.GetInventory().CreateInInventory(RandomFood2);
				Heli.GetInventory().CreateInInventory(RandomFood3);
				Heli.GetInventory().CreateInInventory(RandomFood4);
				Heli.GetInventory().CreateInInventory(RandomFood5);
				Heli.GetInventory().CreateInInventory(RandomFood6);
				Heli.GetInventory().CreateInInventory(RandomFood7);
				Heli.GetInventory().CreateInInventory(RandomFood8);
				Heli.GetInventory().CreateInInventory("CanOpener");
			}
			if (selectedLoadout == 2)
			{
				Heli.GetInventory().CreateInInventory("AmmoBox");
				Heli.GetInventory().CreateInInventory(RandomAmmo1);
				Heli.GetInventory().CreateInInventory(RandomAmmo2);
				Heli.GetInventory().CreateInInventory(RandomAmmo3);
				Heli.GetInventory().CreateInInventory(RandomAmmo4);
				Heli.GetInventory().CreateInInventory(RandomAmmo5);
				Heli.GetInventory().CreateInInventory(RandomAmmo6);
				Heli.GetInventory().CreateInInventory(RandomAmmo7);
				Heli.GetInventory().CreateInInventory(RandomAmmo8);
			}
			if (selectedLoadout == 3)
			{
				Heli.GetInventory().CreateInInventory("SalineBagIV");
				Heli.GetInventory().CreateInInventory("SalineBagIV");
				Heli.GetInventory().CreateInInventory("SamSplint");
				Heli.GetInventory().CreateInInventory("SamSplint");
				Heli.GetInventory().CreateInInventory("BandageDressing");
				Heli.GetInventory().CreateInInventory("BandageDressing");
				Heli.GetInventory().CreateInInventory("Morphine");
				Heli.GetInventory().CreateInInventory("Morphine");
				Heli.GetInventory().CreateInInventory("Epinephrine");
				Heli.GetInventory().CreateInInventory("Epinephrine");
				Heli.GetInventory().CreateInInventory("TetracyclineAntibiotics");
				Heli.GetInventory().CreateInInventory("TetracyclineAntibiotics");
				Heli.GetInventory().CreateInInventory("PainkillerTablets");
				Heli.GetInventory().CreateInInventory("PainkillerTablets");
				Heli.GetInventory().CreateInInventory("IodineTincture");
				Heli.GetInventory().CreateInInventory("IodineTincture");
			}
			if (selectedLoadout == 4)
			{
				Heli.GetInventory().CreateInInventory("FlashGrenade");
				Heli.GetInventory().CreateInInventory("FlashGrenade");
				Heli.GetInventory().CreateInInventory("FlashGrenade");
				Heli.GetInventory().CreateInInventory("M18SmokeGrenade_Green");
				Heli.GetInventory().CreateInInventory("M18SmokeGrenade_Green");
				Heli.GetInventory().CreateInInventory("M18SmokeGrenade_Purple");
				Heli.GetInventory().CreateInInventory("M18SmokeGrenade_Purple");
				Heli.GetInventory().CreateInInventory("M18SmokeGrenade_Red");
				Heli.GetInventory().CreateInInventory("M18SmokeGrenade_Red");
				Heli.GetInventory().CreateInInventory("M67Grenade");
				Heli.GetInventory().CreateInInventory("M67Grenade");
				Heli.GetInventory().CreateInInventory("RGD5Grenade");
				Heli.GetInventory().CreateInInventory("RGD5Grenade");

			}
			if (selectedLoadout == 5)
			{
				Heli.GetInventory().CreateInInventory("FlashGrenade");
				Heli.GetInventory().CreateInInventory("FlashGrenade");
				Heli.GetInventory().CreateInInventory("FlashGrenade");
				Heli.GetInventory().CreateInInventory("M18SmokeGrenade_Green");
				Heli.GetInventory().CreateInInventory("M18SmokeGrenade_Green");
				Heli.GetInventory().CreateInInventory("M18SmokeGrenade_Purple");
				Heli.GetInventory().CreateInInventory("M18SmokeGrenade_Purple");
				Heli.GetInventory().CreateInInventory("M18SmokeGrenade_Red");
				Heli.GetInventory().CreateInInventory("M18SmokeGrenade_Red");
				Heli.GetInventory().CreateInInventory("M67Grenade");
				Heli.GetInventory().CreateInInventory("M67Grenade");
				Heli.GetInventory().CreateInInventory("RGD5Grenade");
				Heli.GetInventory().CreateInInventory("RGD5Grenade");
			}
			if (selectedLoadout == 6)
			{
					Heli.GetInventory().CreateInInventory("NailBox");
					Heli.GetInventory().CreateInInventory("NailBox");
					Heli.GetInventory().CreateInInventory("MetalWire");
					Heli.GetInventory().CreateInInventory("Hammer");
					Heli.GetInventory().CreateInInventory("Pliers");
					Heli.GetInventory().CreateInInventory("Hatchet");
					Heli.GetInventory().CreateInInventory("Hacksaw");
					Heli.GetInventory().CreateInInventory("Whetstone");
					Heli.GetInventory().CreateInInventory("Battery9V");
					Heli.GetInventory().CreateInInventory("CodeLock");
			}
			if (selectedLoadout == 7)
			{
				Heli.GetInventory().CreateInInventory("NVGoggles");
				Heli.GetInventory().CreateInInventory("NVGoggles");
				Heli.GetInventory().CreateInInventory("PersonalRadio");
				Heli.GetInventory().CreateInInventory("PersonalRadio");
				Heli.GetInventory().CreateInInventory("PersonalRadio");
				Heli.GetInventory().CreateInInventory("Battery9V");
				Heli.GetInventory().CreateInInventory("Battery9V");
				Heli.GetInventory().CreateInInventory("Battery9V");
				Heli.GetInventory().CreateInInventory("M4_Suppressor");
				Heli.GetInventory().CreateInInventory("M4_Suppressor");
				Heli.GetInventory().CreateInInventory("PistolSuppressor");
				Heli.GetInventory().CreateInInventory("PistolSuppressor");
				Heli.GetInventory().CreateInInventory("AK_Suppressor");
				Heli.GetInventory().CreateInInventory("AK_Suppressor");
				Heli.GetInventory().CreateInInventory("UniversalLight");
				Heli.GetInventory().CreateInInventory("UniversalLight");
			}
			if (selectedLoadout == 8)
			{
				Heli.GetInventory().CreateInInventory("NVGoggles");
				Heli.GetInventory().CreateInInventory("NVGoggles");
				Heli.GetInventory().CreateInInventory("PersonalRadio");
				Heli.GetInventory().CreateInInventory("PersonalRadio");
				Heli.GetInventory().CreateInInventory("PersonalRadio");
				Heli.GetInventory().CreateInInventory("Battery9V");
				Heli.GetInventory().CreateInInventory("Battery9V");
				Heli.GetInventory().CreateInInventory("Battery9V");
				Heli.GetInventory().CreateInInventory("M4_Suppressor");
				Heli.GetInventory().CreateInInventory("M4_Suppressor");
				Heli.GetInventory().CreateInInventory("PistolSuppressor");
				Heli.GetInventory().CreateInInventory("PistolSuppressor");
				Heli.GetInventory().CreateInInventory("AK_Suppressor");
				Heli.GetInventory().CreateInInventory("AK_Suppressor");
				Heli.GetInventory().CreateInInventory("UniversalLight");
				Heli.GetInventory().CreateInInventory("UniversalLight");
			}
			if (selectedLoadout == 9)
			{
				Heli.GetInventory().CreateInInventory("LandMineTrap");
				Heli.GetInventory().CreateInInventory("LandMineTrap");
				Heli.GetInventory().CreateInInventory("PersonalRadio");
				Heli.GetInventory().CreateInInventory("Battery9V");
				Heli.GetInventory().CreateInInventory("LandMineTrap");
				Heli.GetInventory().CreateInInventory("Hacksaw");
				Heli.GetInventory().CreateInInventory("NVGoggles");
				Heli.GetInventory().CreateInInventory("Hacksaw");

			}	
		int card = Math.RandomIntInclusive(0,9);
		int coin = Math.RandomIntInclusive(0,3);
		if ( card <= 6 && coin ==1 ) Heli.GetInventory().CreateInInventory("RedemptionKeyCard_01" );
		if ( card > 6 && card < 9 && coin ==1 ) Heli.GetInventory().CreateInInventory("RedemptionKeyCard_02" );
		if ( card >= 9 && coin ==1 ) Heli.GetInventory().CreateInInventory("RedemptionKeyCard_03" );		
			Print("[SMM] Mission rewards spawned in reward container"+k+". Randomly selected loadout was "+selectedLoadout+"." );
				
			//Insert mission container into mission objects list
		//	m_MissionObjects.Insert( MissionObject );
		}	
	}

		#ifdef ENFUSION_AI_PROJECT
		#ifdef EXPANSIONMODAI
	eAIGroup m_Group = null;

void SpawnSentry(vector pos, string loadout = "ConvoyLoadout.json") {
	DayZExpansion game = GetDayZGame().GetExpansionGame();
	#ifdef ENFUSION_AI_PROJECT
		eAIFactionCivilian civilian = new eAIFactionCivilian();
		#ifdef EXPANSIONMODAI
	//eAIBase ai = game.SpawnAI_Patrol(pos, loadout);
	eAIBase ai;
		if (Class.CastTo(ai, GetGame().CreateObject(GetRandomAI(), pos)))
			ExpansionHumanLoadout.Apply(ai, loadout, true);
	
	#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "Spawn");
		#endif
#endif
#endif
	//	eAIBase ai = SpawnAI(pos);
	
		#ifdef ENFUSION_AI_PROJECT
		#ifdef EXPANSIONMODAI
		m_Group = ai.GetGroup();
		m_Group.SetFaction( civilian );
		m_Group.SetWaypointBehaviour(eAIWaypointBehavior.HALT);
		string aigroupfaction = ai.GetGroup().GetFaction().GetName();
		Print ("faction for ai is: "+ aigroupfaction);
		m_Group.SetFormationState(eAIGroupFormationState.NONE);
	//	foreach (vector v : m_Waypoints) m_Group.AddWaypoint(v);

	//	int count = 1 - 1;
	//	while (count != 0)
	//	{
	//		ai = SpawnAI(pos);
	//		ai.SetGroup(m_Group);
	//		count--;
	//	}

	m_MissionAIs.Insert(ai);
	#endif
#endif
}
	
 //   DayZExpansion game = GetDayZGame().GetExpansionGame();
//    eAIBase ai = SpawnAI_Sentry(pos, loadout);
//	m_activeGroup = ai.GetGroup();
//	deletethis = ai.GetGroup();
//	ai.GetGroup().SetFaction(new eAIFactionCivilian());

//		}
//	ai.QuickReloadWeapon(weapon);
	//ai.SetUnlimitedMags(true);


	void SpawnAIs()
	{	
		
	for ( int jx2 = 0; jx2 < 2; jx2++ ) //this spawns 2 sentries, increase "< 2" for more.
		{	
		int Dice1x = Math.RandomIntInclusive( 0, 9);
		int Dice2x = Math.RandomIntInclusive( 0, 9);
			//calc new spawn position
			float xx = Math.RandomFloatInclusive( 2 , 4 );
			float yx = Math.RandomFloatInclusive( 3 , 6 );	

			if ( Dice1x > 4 ) xx *= -1.0;  
			if ( Dice2x < 5 ) yx *= -1.0;	

			vector SoldierBotVec = { xx, 0.06, yx };
	vector	SoldierBotPos = Foxhole2.ModelToWorld( SoldierBotVec );

		SpawnSentry(SoldierBotPos);

		
		}
	}
	


	void ObjDespawn() 
	{	

	}
	void SecondWave()
	{
		int Dice1 = Math.RandomIntInclusive( 0, 9);  //move the dice to inside the for loop for the horde to spawn in circle around convoy
		int Dice2 = Math.RandomIntInclusive( 0, 9);
	for ( int j2 = 0; j2 < HordeDensity; j2++ )
		{
			//calc new spawn position
			float x = Math.RandomFloatInclusive( 20 , 65 );
			float y = Math.RandomFloatInclusive( 20 , 65 );	

			if ( Dice1 > 4 ) x *= -1.0;  
			if ( Dice2 < 5 ) y *= -1.0;
	
			vector NewPosVector = { x, 0, y };
			
			//Spawn infected
			m_MissionAIs.Insert( GetGame().CreateObject( InfectedTypes.GetRandomElement(), m_MissionPosition + NewPosVector, false, true ));	
		}
	}
	void MissionFinal()
	{	
		//Spawn horde
		int Dice1 = Math.RandomIntInclusive( 0, 9);  // Dice are outside the For loop so infected all spawn in same quadrant.
		int Dice2 = Math.RandomIntInclusive( 0, 9);
		for ( int j = 0; j < HordeDensity; j++ )
		{
			//calc new spawn position
			float x = Math.RandomFloatInclusive( 10 , 70 );
			float y = Math.RandomFloatInclusive( 10 , 70 );	

			if ( Dice1 > 4 ) x *= -1.0;  
			if ( Dice2 < 5 ) y *= -1.0;  

			vector NewPosVector = { x, 0, y };
			
			//Spawn infected
			EntityAI horde = ( GetGame().CreateObject( InfectedTypes.GetRandomElement(), m_MissionPosition + NewPosVector, false, true ));	
			m_MissionAIs.Insert(horde);
		}
	}

	void PlayerChecks( PlayerBase player )
	{

        for ( int ix=0; ix < m_MissionAIs.Count(); ix++)
        {
            Object MissionAI = m_MissionAIs.Get(ix);
            if ( MissionAI && !MissionAI.IsAlive() )
            {
                if ( DeadInfected.Find( MissionAI ) > -1 )
                continue;
                else
                {
                    DeadInfected.Insert( MissionAI );
                }
            }
		}	
		 //now we check if the array has the wanted amount of dead infected( I use 5 less than spawn in case a few get stuck in a rock)
		if ( DeadInfected.Count() >= 25 && !secondwavespawned)
        {

            Print("[SMM] The players in the mission zone successfully killed the first requested amount of infected.");
           SecondWave(); 
		   secondwavespawned = true;
		   string message = "That just about takes care of that...\nWatch Out!\nAnother Wave is incoming!";
			string icon = "set:dayz_inventory image:walkietalkie";            
			NotificationSystem.SendNotificationToPlayerIdentityExtended( player.GetIdentity() , 10.0, m_MissionInformant + ":", message, icon );
		}
		 //now we check if the array has the wanted amount of dead infected ( I use 5 less than spawn in case a few get stuck in a rock)
        if ( DeadInfected.Count() >= 55 && !m_RewardsSpawned)
        {
			
            SpawnRewards();
			m_MissionObjects.Remove(0); //this removes vehicle from items to be deleted
			EntityAI.Cast( Heli ).SetLifetime( 3888000 ); //this sets vehicle lifetime to 45 days rather than 40 min
            Print("[SMM] The players in the mission zone successfully killed the requested amount of infected.");
            m_RewardsSpawned = true;
            m_MsgNum = -1;
            m_MsgChkTime = m_MissionTime + MsgDlyFinish;
			string messageF = "The vehicle is now unlocked!\nTake care of the remaining infected and then evac!";
			//string icon = "set:dayz_inventory image:walkietalkie";            
			NotificationSystem.SendNotificationToPlayerIdentityExtended( player.GetIdentity() , 10.0, m_MissionInformant + ":", messageF, icon );
			RemoveExpansionServerMarker();
			Heli.Fill(CarFluid.FUEL, 50);  //this puts in about a third of a tank of gas.
			Heli.Fill(CarFluid.OIL, 100);
			Heli.Fill(CarFluid.BRAKE, 100);
			Heli.Fill(CarFluid.COOLANT, 100);
			CarScript carScript = CarScript.Cast( Heli );

				if( carScript )
				{
					carScript.m_IsCKLocked = false;
					carScript.SynchronizeValues();
					carScript.ResetLifetime();
				}
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

