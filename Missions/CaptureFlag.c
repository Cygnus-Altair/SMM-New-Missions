class CaptureFlagMission extends SurvivorMissions
{		
	//Mission related entities 
	Car Heli;
	ItemBase MissionObject;
	Object FlagBase, Table1, Table2;
	BarrelHoles_ColorBase BarrelFire;
	Flag_Base Flag;
	string KeyColor;
	ItemBase HeliKey;
	vector HeliVector;
	vector bpos;
	eAIGroup m_Group;
	string loadout = "CaptureFlagLoadout.json";
	
	 int MissionCutoffTime;
	
	//Mission parameters
	int MsgDlyFinish = 60;					//seconds, message delay time after player has finished mission
	int HordeDensity = 60;					//infected, spawned in circle area
	int CaptureTime = 1200;					//seconds, time to capture the flag ..full minutes (60,120,...)
	string DownFlag = "FZFlag_Nod";		//class name of flag to lower
	string UpFlag = "FZFlag_GDI";		//class name of flag to raise
	
	//mission vars
	vector PoleHighPos, PoleLowPos;		
	float flag_speed, delta_height;
	int FlagElevationChkTime;
	int FireplaceSpendChkTime;
	int FlagCaptureProgress;
	int TeamPlayerCount;
	bool CaptureFinished = false;
	
	//Mission containers
	ref array<ref Param3<string,vector,vector>> BuildingSpawns = new array<ref Param3<string,vector,vector>>;
	ref array<vector> Spawnpoints = new array<vector>;
	ref array<vector> FireSpawns = new array<vector>;
	ref array<vector> InfectedSpawns = new array<vector>;
	ref array<string> InfectedTypes = new array<string>;
	ref array<string> HeliTypes = new array<string>;
	ref array<string> KeyColors = new array<string>;
	ref array<vector> patrol_101 = {"-50 0 50","-40 0 40","-20 0 -0","-10 0 10","-5 0 5","-2 0 2"};
	eAIGroup deletethis = null
	
	bool IsExtended() return true;
	
	void CaptureFlagMission()
	{
		//Select primary mission
		m_MissionExtended = true;
		
		//Mission mission timeout
		m_MissionTimeout = 2700;			//seconds, mission duration time
		
		//Mission zones
		m_MissionZoneOuterRadius = 700.0;	//meters (!Do not set lower than 200m), mission activation distance
		m_MissionZoneInnerRadius = 100.0;	//meters (!Do not set outside of base radius), mission finishing distance to target object
		
		//Mission informant
		m_MissionInformant = "Mr. The Plague";
		
		//Translate mission location name and direction
/*
		if ( MissionSettings.Opt_TranslateEWData )
		{
			m_MissionLocation = EventsWorldData.TransLoc( m_MissionLocation );
			m_MissionLocationDir = EventsWorldData.TransDir( m_MissionLocationDir );
		}
*/				
		//Set mission messages
		m_MissionMessage1 = "One of our defensive positions was occupied by Bandits yesterday.  The resulting firefight drew a horde of zombies, and their pilot ran off.";
		m_MissionMessage2 = "Recapture the position and hold it for at least"+ ( CaptureTime / 60 ).ToString() +" minutes to ambush them when they return from searching for the pilot.";
		m_MissionMessage3 = "The defensive position is at the ** "+ m_MissionLocationDir +" side of "+ m_MissionLocation +" **\nBe careful, there are a lot of infected and the bandits could return at any time!";
		
		//Rewards spawns
		Spawnpoints.Insert("-0.422 0.453 0");	//table 1
		Spawnpoints.Insert("0.486 0.453 0");	//table 1
		Spawnpoints.Insert("0 0.453 0");		//table 2
		
		//Fireplace spawns
		FireSpawns.Insert("0 0 10");
		FireSpawns.Insert("0 0 -10");

				//Infected spawnpoints
		InfectedSpawns.Insert("-10.5186 0 25.0269");
		InfectedSpawns.Insert("24.9775 0 -10.4146");
		InfectedSpawns.Insert("-30.1726 0 -6.2729");
		InfectedSpawns.Insert("-20.9209 0 4.6636");
		InfectedSpawns.Insert("15.0283 0 -10.3276");
		InfectedSpawns.Insert("7.2461 0 30.5884");
		InfectedSpawns.Insert("-20.6855 0 5.9956");
		
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

		//Spawn surrounding flag base buildings
		BuildingSpawns.Insert( new Param3<string,vector,vector>("Land_Roadblock_WoodenCrate", "-1.5 0 4", "65 0 90"));
		BuildingSpawns.Insert( new Param3<string,vector,vector>("Land_Roadblock_WoodenCrate", "1.5 0 4", "135 0 0"));
		BuildingSpawns.Insert( new Param3<string,vector,vector>("Land_Roadblock_Table", "-1.5 0 -3", "15 0 0"));
		BuildingSpawns.Insert( new Param3<string,vector,vector>("Land_Roadblock_Table", "1.5 0 -3", "0 0 0"));
		BuildingSpawns.Insert( new Param3<string,vector,vector>("BarrelHoles_Blue", "1.8 0 1.8", "0 0 0"));
		BuildingSpawns.Insert( new Param3<string,vector,vector>("bldr_mil_fortified_nest_watchtower", "9 0 9", "135 0 0"));
		BuildingSpawns.Insert( new Param3<string,vector,vector>("bldr_mil_fortified_nest_watchtower", "-9 0 -9", "315 0 0"));		
		BuildingSpawns.Insert( new Param3<string,vector,vector>("Land_Wreck_Ikarus", "13 0 -2.8", "-160 0 0"));
		BuildingSpawns.Insert( new Param3<string,vector,vector>("Land_Wreck_Uaz", "-12 0 2.8", "15 0 0"));
		BuildingSpawns.Insert( new Param3<string,vector,vector>("Land_wreck_truck01_aban1_orange", "0 0 13.5", "90 0 0"));
		
		//Heli Spawnpoints spawnpoints
		HeliTypes.Insert("RFFSHeli_Ka26_KZTiger");
		HeliTypes.Insert("RFFSHeli_Ka26_KZRed");
		HeliTypes.Insert("RFFSHeli_Ka26_KZERDL");
		HeliTypes.Insert("RFFSHeli_Ka26_KZNavy");
		HeliTypes.Insert("RFFSHeli_Ka26_KZAlpine");
		HeliTypes.Insert("RFFSHeli_Bo105m");
		HeliTypes.Insert("RFFSHeli_Bo105m_blackcamo");
		HeliTypes.Insert("RFFSHeli_LittleBird");
		HeliTypes.Insert("RFFSHeli_LittleBird_Camo");
		HeliTypes.Insert("RFFSHeli_LittleBird_Desert");
		HeliTypes.Insert("RFFSHeli_Ka26");
		HeliTypes.Insert("RFFSHeli_Mi2");
		HeliTypes.Insert("RFFSHeli_Mi2_Military");
		HeliTypes.Insert("RFFSHeli_Mi2_Hornet");
		HeliTypes.Insert("RFFSHeli_Bell429");
		HeliTypes.Insert("RFFSHeli_Bell429_Police");
		HeliTypes.Insert("RFFSHeli_Bell429_Medic");
		HeliTypes.Insert("RFFSHeli_Bell429_ZS");
		HeliTypes.Insert("RFFSHeli_Bell429_Uganda");
		HeliTypes.Insert("RFFSHeli_Apache_Desert");
		HeliTypes.Insert("RFFSHeli_Apache_KZTiger");
		HeliTypes.Insert("RFFSHeli_Apache_KZNavy");
		HeliTypes.Insert("RFFSHeli_Apache_KZERDL");
		HeliTypes.Insert("RFFSHeli_Apache");
		HeliTypes.Insert("RFFSHeli_Apache_Winter");
		HeliTypes.Insert("RFFSHeli_R22");
		HeliTypes.Insert("RFFSHeli_R22_Red");	
		HeliTypes.Insert("RFFSHeli_R22_Black");
		HeliTypes.Insert("RFFSHeli_EC135");
		HeliTypes.Insert("RFFSHeli_EC135_BlackCamo");
		
		KeyColors.Insert("MCK_CarKey_Base");		KeyColors.Insert("MCK_CarKey_Blue"); 
		KeyColors.Insert("MCK_CarKey_White");		KeyColors.Insert("MCK_CarKey_Green");
		KeyColors.Insert("MCK_CarKey_Red");			KeyColors.Insert("MCK_CarKey_Yellow"); 
				
		KeyColor = KeyColors.GetRandomElement(); 
		
		//Clean up flag base position		
		GetGame().GetObjectsAtPosition( m_MissionPosition, 18.0 , m_ObjectList, m_ObjectCargoList );
		for ( int b=0; b < m_ObjectList.Count(); b++) 
		{
			Object FoundObject = m_ObjectList.Get(b);
			if ( FoundObject.IsItemBase() )
			{
				Print("[SMM] BeforeSpawnCleanUp :: Object deleted: "+ FoundObject );  
				GetGame().ObjectDelete( FoundObject );
			}
		}
		
		//Activate EntityChecks
		GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( EntityChecks, 0, true );
		MissionCutoffTime = MissionSettings.RestartCycleTime - (m_MissionTimeout + MissionSettings.DelayTime + CaptureTime );
    
    if ( GetGame().GetTime() * 0.001 > MissionCutoffTime )
    MissionSettings.DelayTime = 3600;
	}
	
	void ~CaptureFlagMission()
	{		
		//Despawn all remaining mission objects
		if ( m_MissionObjects )
		{	
			Print("[SMM] Despawning "+ m_MissionObjects.Count() +" mission objects from old mission...");				
			for ( int i = 0; i < m_MissionObjects.Count(); i++ )
			{ 
				if ( !m_MissionObjects.Get(i) ) continue;
				else
				{
					//Delete Object serverside
					GetGame().ObjectDelete( m_MissionObjects.Get(i) );
				}		
			}
			m_MissionObjects.Clear();
		}
		
		//Despawn mission AI's
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
		//Delete PlayersInZone list & reset container takeaway
		if ( m_PlayersInZone )	m_PlayersInZone.Clear();
		if ( m_ContainerWasTaken ) m_ContainerWasTaken = false;	
	}	
	
	void SpawnObjects()
	{		
		string RandomHeli = HeliTypes.GetRandomElement();
		//spawn flagpole (flagpole is the reference for all mission building & object spawnpoints!) 
		FlagBase = GetGame().CreateObject( "bldr_misc_flagpole", m_MissionPosition );
		FlagBase.SetPosition( m_MissionPosition + "0 3.5 0" );
		FlagBase.SetOrientation( Vector( Math.RandomFloatInclusive(0,359.9), 0 , 0));
		m_MissionObjects.Insert( FlagBase );											//Object 0

		//get flag height limits and offset height positions min max
		vector temp = FlagBase.ModelToWorld("0.07 0.0 0.0");
		temp[1] = GetGame().SurfaceY(temp[0], temp[2]);
		PoleHighPos = temp + "0 6.85 0";
		PoleLowPos = temp + "0 2.8 0";
		
		//flag hight and speed calculations
		delta_height = PoleHighPos[1] - PoleLowPos[1];						//height diff min max ...[m]
		flag_speed = -( delta_height / (( CaptureTime / 2 ) * 10 ) );		//vertical speed of flag movement, v = s / t ...[m/100ms]
		
		//spawn flag
		Flag = Flag_Base.Cast( GetGame().CreateObject( DownFlag, PoleHighPos ));
		Flag.SetPosition( PoleHighPos );
		Flag.SetOrientation( FlagBase.GetOrientation() + "180 0 0");
		Flag.ShowSelection("unfolded");
		Flag.HideSelection("folded");
		Flag.SetTakeable(false);
		Flag.SetLifetime(7200);
		m_MissionObjects.Insert( Flag );												//Object 1
		
		vector HeliPos = FlagBase.ModelToWorld("5 0 -16");
		vector HeliDir = FlagBase.GetDirection();
		Heli = Car.Cast(GetGame().CreateObject( RandomHeli, HeliPos ));
		Heli.SetDirection( HeliDir );
		Heli.SetOrientation( Heli.GetOrientation() + "-10 0 0" );
		Heli.PlaceOnSurface();
		Heli.GetInventory().CreateAttachment("RFFSHeli_aviation_battery");
		Heli.GetInventory().CreateAttachment("RFFSHeli_igniter_plug");
		Heli.GetInventory().CreateAttachment("RFFSHeli_hydraulic_hoses");
		Heli.GetInventory().CreateAttachment("RFFSHeli_wiring_harness");
		Heli.GetInventory().CreateAttachment("HeadlightH7");
		
	//	if (RandomHeli == "Helilittle_bird_RF_clear")
	//	{
	//		Heli.GetInventory().CreateAttachment("RFlittle_bird_blade1");
	//		Heli.GetInventory().CreateAttachment("RFlittle_bird_blade2");
	//		Heli.GetInventory().CreateAttachment("RFlittle_bird_blade3");
	//		Heli.GetInventory().CreateAttachment("RFlittle_bird_blade4");
	//		Heli.GetInventory().CreateAttachment("RFlittle_bird_blade5");
	//		Heli.GetInventory().CreateAttachment("RFlittle_bird_bladem1");
	//		Heli.GetInventory().CreateAttachment("RFlittle_bird_bladem3");
	//		Heli.GetInventory().CreateAttachment("CarBattery");
	//		Heli.GetInventory().CreateAttachment("HeadlightH7");
	//		Heli.GetInventory().CreateAttachment("HeadlightH7");
	//	}
	//	if (RandomHeli == "Police_Gold_clear")
	//	{
	//		Heli.GetInventory().CreateAttachment("Police_Gold_Door_1_1");
	//		Heli.GetInventory().CreateAttachment("Police_Gold_Door_1_2");
	//		Heli.GetInventory().CreateAttachment("Police_Gold_Door_2_1");
	//		Heli.GetInventory().CreateAttachment("Police_Gold_Door_2_2");
	//		Heli.GetInventory().CreateAttachment("CarBattery");
	//		Heli.GetInventory().CreateAttachment("HeadlightH7");
	//		Heli.GetInventory().CreateAttachment("HeadlightH7");
	//	}
	//	if (RandomHeli == "Heli_apache_clear")
	//	{
	//		Heli.GetInventory().CreateAttachment("apache_blade1");
	//		Heli.GetInventory().CreateAttachment("apache_blade2");
	//		Heli.GetInventory().CreateAttachment("apache_blade3");
	//		Heli.GetInventory().CreateAttachment("apache_blade4");
	//		Heli.GetInventory().CreateAttachment("apache_bladem1");
	//		Heli.GetInventory().CreateAttachment("apache_bladem2");
	//		Heli.GetInventory().CreateAttachment("apache_bladem3");
	//		Heli.GetInventory().CreateAttachment("apache_bladem4");
	//		Heli.GetInventory().CreateAttachment("apache_Door_1_1");
	//		Heli.GetInventory().CreateAttachment("apache_Door_2_1");
	//		Heli.GetInventory().CreateAttachment("CarBattery");
	//		Heli.GetInventory().CreateAttachment("HeadlightH7");
	//		Heli.GetInventory().CreateAttachment("HeadlightH7");
	//	}
	//	if (RandomHeli == "HeliSIB_uh1d_FZKilgore_clear")
	//	{
	//		Heli.GetInventory().CreateAttachment("SIBUH1D_blade1");
	//		Heli.GetInventory().CreateAttachment("SIBUH1D_blade2");
	//		Heli.GetInventory().CreateAttachment("SIBUH1D_bladem1");
	//		Heli.GetInventory().CreateAttachment("SIBUH1D_bladem2");
	//		Heli.GetInventory().CreateAttachment("SIBUH1D_Door_1_1_FZKilgore");
	//		Heli.GetInventory().CreateAttachment("SIBUH1D_Door_1_2_FZKilgore");
	//		Heli.GetInventory().CreateAttachment("SIBUH1D_Door_2_1_FZKilgore");
	//		Heli.GetInventory().CreateAttachment("SIBUH1D_Door_2_2_FZKilgore");
	//		Heli.GetInventory().CreateAttachment("CarBattery");
	//		Heli.GetInventory().CreateAttachment("HeadlightH7");
	//		Heli.GetInventory().CreateAttachment("HeadlightH7");
	//	}
	//	if (RandomHeli == "HeliSIB_UH1D_clear")
	//	{
	//		Heli.GetInventory().CreateAttachment("SIBUH1D_blade1");
	//		Heli.GetInventory().CreateAttachment("SIBUH1D_blade2");
	//		Heli.GetInventory().CreateAttachment("SIBUH1D_bladem1");
	//		Heli.GetInventory().CreateAttachment("SIBUH1D_bladem2");
	//		Heli.GetInventory().CreateAttachment("SIBUH1D_Door_1_1");
	//		Heli.GetInventory().CreateAttachment("SIBUH1D_Door_1_2");
	//		Heli.GetInventory().CreateAttachment("SIBUH1D_Door_2_1");
	//		Heli.GetInventory().CreateAttachment("SIBUH1D_Door_2_2");
	//		Heli.GetInventory().CreateAttachment("CarBattery");
	//		Heli.GetInventory().CreateAttachment("HeadlightH7");
	//		Heli.GetInventory().CreateAttachment("HeadlightH7");
	//	}
	//	if (RandomHeli == "HeliSIB_UH1D_iron_clear")
	//	{
	//		Heli.GetInventory().CreateAttachment("SIBUH1D_blade1");
	//		Heli.GetInventory().CreateAttachment("SIBUH1D_blade2");
	//		Heli.GetInventory().CreateAttachment("SIBUH1D_bladem1");
	//		Heli.GetInventory().CreateAttachment("SIBUH1D_bladem2");
	//		Heli.GetInventory().CreateAttachment("SIBUH1D_Door_1_1_iron");
	//		Heli.GetInventory().CreateAttachment("SIBUH1D_Door_1_2_iron");
	//		Heli.GetInventory().CreateAttachment("SIBUH1D_Door_2_1_iron");
	//		Heli.GetInventory().CreateAttachment("SIBUH1D_Door_2_2_iron");
	//		Heli.GetInventory().CreateAttachment("CarBattery");
	//		Heli.GetInventory().CreateAttachment("HeadlightH7");
	//		Heli.GetInventory().CreateAttachment("HeadlightH7");
	//	}
	//	if (RandomHeli == "HeliSIB_UH1D_navy_clear")
	//	{
	//		Heli.GetInventory().CreateAttachment("SIBUH1D_blade1");
	//		Heli.GetInventory().CreateAttachment("SIBUH1D_blade2");
	//		Heli.GetInventory().CreateAttachment("SIBUH1D_bladem1");
	//		Heli.GetInventory().CreateAttachment("SIBUH1D_bladem2");
	//		Heli.GetInventory().CreateAttachment("SIBUH1D_Door_1_1_navy");
	//		Heli.GetInventory().CreateAttachment("SIBUH1D_Door_1_2_navy");
	//		Heli.GetInventory().CreateAttachment("SIBUH1D_Door_2_1_navy");
	//		Heli.GetInventory().CreateAttachment("SIBUH1D_Door_2_2_navy");
	//		Heli.GetInventory().CreateAttachment("CarBattery");
	//		Heli.GetInventory().CreateAttachment("HeadlightH7");
	//		Heli.GetInventory().CreateAttachment("HeadlightH7");
	//	}
	//	if (RandomHeli == "HeliSIB_UH1D_airforce_clear")
	//	{
	//		Heli.GetInventory().CreateAttachment("SIBUH1D_blade1");
	//		Heli.GetInventory().CreateAttachment("SIBUH1D_blade2");
	//		Heli.GetInventory().CreateAttachment("SIBUH1D_bladem1");
	//		Heli.GetInventory().CreateAttachment("SIBUH1D_bladem2");
	//		Heli.GetInventory().CreateAttachment("SIBUH1D_Door_1_1_airforce");
	//		Heli.GetInventory().CreateAttachment("SIBUH1D_Door_1_2_airforce");
	//		Heli.GetInventory().CreateAttachment("SIBUH1D_Door_2_1_airforce");
	//		Heli.GetInventory().CreateAttachment("SIBUH1D_Door_2_2_airforce");
	//		Heli.GetInventory().CreateAttachment("CarBattery");
	//		Heli.GetInventory().CreateAttachment("HeadlightH7");
	//		Heli.GetInventory().CreateAttachment("HeadlightH7");
	//	}
	//	if (RandomHeli == "Heliblack_hawk_SIB_clear")
	//	{
	//		Heli.GetInventory().CreateAttachment("SIBblack_hawk_blade1");
	//		Heli.GetInventory().CreateAttachment("SIBblack_hawk_blade2");
	//		Heli.GetInventory().CreateAttachment("SIBblack_hawk_blade3");
	//		Heli.GetInventory().CreateAttachment("SIBblack_hawk_blade4");
	//		Heli.GetInventory().CreateAttachment("SIBblack_hawk_bladem1");
	//		Heli.GetInventory().CreateAttachment("SIBblack_hawk_bladem2");
	//		Heli.GetInventory().CreateAttachment("SIBblack_hawk_bladem3");
	//		Heli.GetInventory().CreateAttachment("SIBblack_hawk_bladem4");
	//		Heli.GetInventory().CreateAttachment("SIBblack_hawk_Door_1_1");
	//		Heli.GetInventory().CreateAttachment("SIBblack_hawk_Door_1_2");
	//		Heli.GetInventory().CreateAttachment("SIBblack_hawk_Door_2_1");
	//		Heli.GetInventory().CreateAttachment("SIBblack_hawk_Door_2_2");
	//		Heli.GetInventory().CreateAttachment("CarBattery");
	//		Heli.GetInventory().CreateAttachment("HeadlightH7");
	//		Heli.GetInventory().CreateAttachment("HeadlightH7");
	//	}
	//	if (RandomHeli == "Heliblack_hawk_SIB_camo_clear")
	//	{
	//		Heli.GetInventory().CreateAttachment("SIBblack_hawk_blade1");
	//		Heli.GetInventory().CreateAttachment("SIBblack_hawk_blade2");
	//		Heli.GetInventory().CreateAttachment("SIBblack_hawk_blade3");
	//		Heli.GetInventory().CreateAttachment("SIBblack_hawk_blade4");
	//		Heli.GetInventory().CreateAttachment("SIBblack_hawk_bladem1");
	//		Heli.GetInventory().CreateAttachment("SIBblack_hawk_bladem2");
	//		Heli.GetInventory().CreateAttachment("SIBblack_hawk_bladem3");
	//		Heli.GetInventory().CreateAttachment("SIBblack_hawk_bladem4");
	//		Heli.GetInventory().CreateAttachment("SIBblack_hawk_Door_1_1_camo");
	//		Heli.GetInventory().CreateAttachment("SIBblack_hawk_Door_1_2_camo");
	//		Heli.GetInventory().CreateAttachment("SIBblack_hawk_Door_2_1_camo");
	//		Heli.GetInventory().CreateAttachment("SIBblack_hawk_Door_2_2_camo");
	//		Heli.GetInventory().CreateAttachment("CarBattery");
	//		Heli.GetInventory().CreateAttachment("HeadlightH7");
	//		Heli.GetInventory().CreateAttachment("HeadlightH7");
	//	}
	//	if (RandomHeli == "Helichinook_RF_clear")
	//	{
	//		Heli.GetInventory().CreateAttachment("RFchinook_blade1");
	//		Heli.GetInventory().CreateAttachment("RFchinook_blade2");
	//		Heli.GetInventory().CreateAttachment("RFchinook_blade3");
	//		Heli.GetInventory().CreateAttachment("RFchinook_rear_blade1");
	//		Heli.GetInventory().CreateAttachment("RFchinook_rear_blade2");
	//		Heli.GetInventory().CreateAttachment("RFchinook_rear_blade3");
	//		Heli.GetInventory().CreateAttachment("RFchinook_Door_1_1_camo");
	//		Heli.GetInventory().CreateAttachment("RFchinook_Door_1_2_camo");
	//		Heli.GetInventory().CreateAttachment("RFchinook_Door_2_2");
	//		Heli.GetInventory().CreateAttachment("CarBattery");
	//		Heli.GetInventory().CreateAttachment("HeadlightH7");
	//		Heli.GetInventory().CreateAttachment("HeadlightH7");
	//	}
	//	if (RandomHeli == "Helichinook_RF_bloody")
	//	{
	//		Heli.GetInventory().CreateAttachment("RFchinook_blade1");
	//		Heli.GetInventory().CreateAttachment("RFchinook_blade2");
	//		Heli.GetInventory().CreateAttachment("RFchinook_blade3");
	//		Heli.GetInventory().CreateAttachment("RFchinook_rear_blade1");
	//		Heli.GetInventory().CreateAttachment("RFchinook_rear_blade2");
	//		Heli.GetInventory().CreateAttachment("RFchinook_rear_blade3");
	//		Heli.GetInventory().CreateAttachment("RFchinook_Door_1_1_bloody");
	//		Heli.GetInventory().CreateAttachment("RFchinook_Door_1_2_bloody");
	//		Heli.GetInventory().CreateAttachment("RFchinook_Door_2_2");
	//		Heli.GetInventory().CreateAttachment("CarBattery");
	//		Heli.GetInventory().CreateAttachment("HeadlightH7");
	//		Heli.GetInventory().CreateAttachment("HeadlightH7");
	//	}
		
		
		EntityAI.Cast( Heli ).SetLifetime( (m_MissionTimeout - m_MissionTime) + 30 );
		m_MissionObjects.Insert( Heli );
		Heli.Fill(CarFluid.FUEL, 50);  //this puts in about a third of a tank of gas.
			Heli.Fill(CarFluid.OIL, 100);
			Heli.Fill(CarFluid.BRAKE, 100);
			Heli.Fill(CarFluid.COOLANT, 100);
			HeliKey = Heli.GetInventory().CreateInInventory(KeyColor);
		CarScript carScript = CarScript.Cast( Heli );
		carScript.m_IsCKLocked = true;
		
		//Spawn additional flag base buildings and objects
		for ( int i=0; i < BuildingSpawns.Count(); i++ )
		{
			Param3<string,vector,vector> BuildingDef = BuildingSpawns.Get(i);
			string BuildingType = BuildingDef.param1;
			vector BuildingPos = FlagBase.ModelToWorld( BuildingDef.param2 );
			vector BuildingOri = BuildingDef.param3;
			vector BuildingDir = FlagBase.GetDirection();
			
			BuildingPos[1] = GetGame().SurfaceY( BuildingPos[0], BuildingPos[2] );
			Object HQextra = GetGame().CreateObject( BuildingType, BuildingPos );
			HQextra.SetDirection( BuildingDir );
			HQextra.SetOrientation( HQextra.GetOrientation() + BuildingOri );
			HQextra.PlaceOnSurface();
			m_MissionObjects.Insert( HQextra );											//Object 2 to 8
		}
		Table1 = m_MissionObjects[5];
		Table2 = m_MissionObjects[6];
		//check darkness for location light support and spawn fireplaces
		int year, month, day, hour, minute;
		
		GetGame().GetWorld().GetDate( year, month, day, hour, minute );		
		
		Mission currentMission = GetGame().GetMission();

		if ( currentMission )
		{			
			WorldData worldData = currentMission.GetWorldData();
			if ( worldData )
			{
				float ApproxSunrise = worldData.GetApproxSunriseTime( month + ( day / 32.0 ) );
				float ApproxSunset = worldData.GetApproxSunsetTime( month + ( day / 32.0 ) );
				
				int SunRiseHour = Math.Round( ApproxSunrise + 0.25 );
				int SunSetHour = Math.Round( ApproxSunset - 0.25 );
				
				if ( MissionSettings.DebugMode )
				Print("[SMM] Test Hour is "+ hour +", SunRiseHour: "+ SunRiseHour +", SonSetHour: "+ SunSetHour );
				
				if ( hour >= SunSetHour || hour <= SunRiseHour ) 
				{
					EntityAI wood, stones;
					
					for ( int k=0; k < FireSpawns.Count(); k++)
					{
						vector CampfirePosition = FlagBase.ModelToWorld( FireSpawns.Get(k) );
						
						//fireplace
						FireplaceBase Campfire = FireplaceBase.Cast( GetGame().CreateObject( "Fireplace", CampfirePosition ) );
						Campfire.PlaceOnSurface();
						Campfire.Synchronize();
						wood = Campfire.GetInventory().CreateAttachment("Firewood");
						ItemBase.Cast( wood ).SetQuantity(6);
						Campfire.GetInventory().CreateAttachment("WoodenStick");
						Campfire.GetInventory().CreateAttachment("Rag");
						stones = Campfire.GetInventory().CreateAttachment("Stone");
						ItemBase.Cast( stones ).SetQuantity(16);
						Campfire.StartFire( true );
						m_MissionObjects.Insert( Campfire );
						
						//Cluttercutter fireplace
						Object cc_fireplace = GetGame().CreateObject( "ClutterCutterFireplace" , Campfire.GetPosition() );
						cc_fireplace.SetOrientation( Campfire.GetOrientation() );
						GetGame().RemoteObjectCreate( cc_fireplace );
						m_MissionObjects.Insert( cc_fireplace );
					}
				}
			}
		}
		
		//flip flagpole after spawning all objects
		//FlagBase.SetOrientation( FlagBase.GetOrientation() + "0 180 0");
		
		Print("[SMM] Survivor Mission "+ m_selectedMission +" :: "+ m_MissionName +" ...mission deployed!");
	}
	
	void SpawnRewards()
	{
		
		for (int i=0; i < TeamPlayerCount; i++)
		{
			vector spawnpos;
			
			if ( i < 2 )
			spawnpos = Table1.ModelToWorld( Spawnpoints.Get(i) );
			else spawnpos = Table2.ModelToWorld( Spawnpoints.Get(i) );
			
			//Mission object is seachest
			MissionObject = ItemBase.Cast( GetGame().CreateObject("SeaChest", spawnpos) ); 
			
			//Get random loadout 			
			int selectedLoadout = Math.RandomIntInclusive(0,11);	//!Change randomization limit after adding new loadouts!
			
			//Spawn selected loadout items in mission object
			EntityAI weapon;
									
			if (selectedLoadout == 0)
			{
				weapon = MissionObject.GetInventory().CreateInInventory("M4A1");
					weapon.GetInventory().CreateAttachment("M4_RISHndgrd_Green");
					weapon.GetInventory().CreateAttachment("M4_MPBttstck");
					weapon.GetInventory().CreateAttachment("M4_Suppressor");
					weapon.GetInventory().CreateAttachment("ACOGOptic");
					//weapon.GetInventory().CreateAttachment("M4_T3NRDSOptic");
				MissionObject.GetInventory().CreateInInventory("Mag_STANAG_30Rnd");
				MissionObject.GetInventory().CreateInInventory("AmmoBox_556x45_20Rnd");
				MissionObject.GetInventory().CreateInInventory("AmmoBox_556x45_20Rnd");
				MissionObject.GetInventory().CreateInInventory("HuntingKnife");
				MissionObject.GetInventory().CreateInInventory("BakedBeansCan");
				MissionObject.GetInventory().CreateInInventory("Canteen");
				MissionObject.GetInventory().CreateInInventory("Battery9V");
			}
			if (selectedLoadout == 1)
			{
				weapon = MissionObject.GetInventory().CreateInInventory("SVD");
					weapon.GetInventory().CreateAttachment("PSO11Optic");
					//weapon.GetInventory().CreateAttachment("PSO1Optic");
					//weapon.GetInventory().CreateAttachment("KazuarOptic");
				MissionObject.GetInventory().CreateInInventory("Mag_SVD_10Rnd");
				MissionObject.GetInventory().CreateInInventory("Mag_SVD_10Rnd");
				MissionObject.GetInventory().CreateInInventory("AmmoBox_762x54_20Rnd");
				MissionObject.GetInventory().CreateInInventory("CanOpener");
				MissionObject.GetInventory().CreateInInventory("PeachesCan");
				MissionObject.GetInventory().CreateInInventory("WaterBottle"); 
				MissionObject.GetInventory().CreateInInventory("Battery9V");
			}
			if (selectedLoadout == 2)
			{
				weapon = MissionObject.GetInventory().CreateInInventory("AKM");
					weapon.GetInventory().CreateAttachment("AK_RailHndgrd");
					weapon.GetInventory().CreateAttachment("AK_Suppressor");
					weapon.GetInventory().CreateAttachment("AK_PlasticBttstck");
					weapon.GetInventory().CreateAttachment("PSO1Optic");
					//weapon.GetInventory().CreateAttachment("KobraOptic");
				MissionObject.GetInventory().CreateInInventory("Mag_AKM_30Rnd");
				MissionObject.GetInventory().CreateInInventory("AmmoBox_762x39_20Rnd");
				MissionObject.GetInventory().CreateInInventory("AmmoBox_762x39_20Rnd");
				MissionObject.GetInventory().CreateInInventory("KitchenKnife");
				MissionObject.GetInventory().CreateInInventory("SardinesCan");
				MissionObject.GetInventory().CreateInInventory("Canteen");
				MissionObject.GetInventory().CreateInInventory("Battery9V");
			}
			if (selectedLoadout == 3)
			{
				weapon = MissionObject.GetInventory().CreateInInventory("FAL");
					weapon.GetInventory().CreateAttachment("Fal_OeBttstck");
					weapon.GetInventory().CreateAttachment("M4_Suppressor");
					weapon.GetInventory().CreateAttachment("M68Optic");
					//weapon.GetInventory().CreateAttachment("ReflexOptic");
				MissionObject.GetInventory().CreateInInventory("Mag_FAL_20Rnd");
				MissionObject.GetInventory().CreateInInventory("Mag_FAL_20Rnd");
				MissionObject.GetInventory().CreateInInventory("WaterBottle");
				MissionObject.GetInventory().CreateInInventory("MilitaryBelt");	
				MissionObject.GetInventory().CreateInInventory("M18SmokeGrenade_Yellow");	
				MissionObject.GetInventory().CreateInInventory("TacticalBaconCan");
				MissionObject.GetInventory().CreateInInventory("SteakKnife");
			}	
			if (selectedLoadout == 4)
			{
				weapon = MissionObject.GetInventory().CreateInInventory("SKS");
					weapon.GetInventory().CreateAttachment("PUScopeOptic");
					weapon.GetInventory().CreateAttachment("SKS_Bayonet");
				MissionObject.GetInventory().CreateInInventory("AmmoBox_762x39_20Rnd");
				MissionObject.GetInventory().CreateInInventory("AmmoBox_762x39_20Rnd");
				weapon = MissionObject.GetInventory().CreateInInventory("FNX45");
					weapon.GetInventory().CreateAttachment("PistolSuppressor");
					weapon.GetInventory().CreateAttachment("FNP45_MRDSOptic");
					weapon.GetInventory().CreateAttachment("TLRLight");
				MissionObject.GetInventory().CreateInInventory("Mag_FNX45_15Rnd");
				MissionObject.GetInventory().CreateInInventory("AmmoBox_45ACP_25Rnd");
				MissionObject.GetInventory().CreateInInventory("WaterBottle");			
				MissionObject.GetInventory().CreateInInventory("Battery9V");
			}
			if (selectedLoadout == 5)
			{			
				weapon = MissionObject.GetInventory().CreateInInventory("Winchester70");
					weapon.GetInventory().CreateAttachment("HuntingOptic");		
				MissionObject.GetInventory().CreateInInventory("Ammo_308Win");
				weapon = MissionObject.GetInventory().CreateInInventory("CZ75");
					weapon.GetInventory().CreateAttachment("PistolSuppressor");
					weapon.GetInventory().CreateAttachment("FNP45_MRDSOptic");
				MissionObject.GetInventory().CreateInInventory("Mag_CZ75_15Rnd");
				MissionObject.GetInventory().CreateInInventory("AmmoBox_45ACP_25Rnd");
				weapon = MissionObject.GetInventory().CreateInInventory("NVGHeadstrap");
					weapon.GetInventory().CreateInInventory("NVGoggles");
				MissionObject.GetInventory().CreateInInventory("Battery9V");
				MissionObject.GetInventory().CreateInInventory("PsilocybeMushroom");
				MissionObject.GetInventory().CreateInInventory("WaterBottle");	
				MissionObject.GetInventory().CreateInInventory("SpaghettiCan");
				MissionObject.GetInventory().CreateInInventory("StoneKnife");
				MissionObject.GetInventory().CreateInInventory("M67Grenade");					
			}
			if (selectedLoadout == 6)
			{			
				weapon = MissionObject.GetInventory().CreateInInventory("UMP45");
					weapon.GetInventory().CreateAttachment("M68Optic");
					weapon.GetInventory().CreateAttachment("PistolSuppressor");			
				MissionObject.GetInventory().CreateInInventory("Mag_UMP_25Rnd");
				MissionObject.GetInventory().CreateInInventory("Mag_UMP_25Rnd");
				MissionObject.GetInventory().CreateInInventory("AmmoBox_45ACP_25Rnd");
				MissionObject.GetInventory().CreateInInventory("Battery9V");
				MissionObject.GetInventory().CreateInInventory("Cannabis");
				MissionObject.GetInventory().CreateInInventory("CannabisSeedsPack");
				MissionObject.GetInventory().CreateInInventory("WaterBottle");	
				MissionObject.GetInventory().CreateInInventory("TunaCan");
				MissionObject.GetInventory().CreateInInventory("RDG5Grenade");					
			}	
			if (selectedLoadout == 7)
			{			
				weapon = MissionObject.GetInventory().CreateInInventory("VSS");
					weapon.GetInventory().CreateAttachment("PSO11Optic");
					//weapon.GetInventory().CreateAttachment("PistolSuppressor");			
				MissionObject.GetInventory().CreateInInventory("Mag_VSS_10Rnd");
				MissionObject.GetInventory().CreateInInventory("Mag_VSS_10Rnd");
				MissionObject.GetInventory().CreateInInventory("AmmoBox_9x39_20Rnd");
				MissionObject.GetInventory().CreateInInventory("Battery9V");
				MissionObject.GetInventory().CreateInInventory("GardenLime");
				MissionObject.GetInventory().CreateInInventory("CannabisSeedsPack");
				MissionObject.GetInventory().CreateInInventory("WaterBottle");	
				MissionObject.GetInventory().CreateInInventory("SardinesCan");
				MissionObject.GetInventory().CreateInInventory("M18SmokeGrenade_Green");					
			}
			if (selectedLoadout == 8)
			{			
				weapon = MissionObject.GetInventory().CreateInInventory("MP5K");
					weapon.GetInventory().CreateAttachment("MP5_RailHndgrd");
					weapon.GetInventory().CreateAttachment("MP5k_StockBttstck");
					weapon.GetInventory().CreateAttachment("M68Optic");
					weapon.GetInventory().CreateAttachment("PistolSuppressor");			
				MissionObject.GetInventory().CreateInInventory("Mag_MP5_30Rnd");
				MissionObject.GetInventory().CreateInInventory("Mag_MP5_30Rnd");
				MissionObject.GetInventory().CreateInInventory("AmmoBox_9x19_25rnd");
				MissionObject.GetInventory().CreateInInventory("GP5GasMask");
				MissionObject.GetInventory().CreateInInventory("NBCGlovesGray");
				MissionObject.GetInventory().CreateInInventory("WaterBottle");	
				MissionObject.GetInventory().CreateInInventory("SpaghettiCan");
				MissionObject.GetInventory().CreateInInventory("M18SmokeGrenade_Red");
				MissionObject.GetInventory().CreateInInventory("Battery9V");
				MissionObject.GetInventory().CreateInInventory("Battery9V");
				MissionObject.GetInventory().CreateInInventory("Battery9V");				
			}
			if (selectedLoadout == 9)
			{			
				weapon = MissionObject.GetInventory().CreateInInventory("AK74");
					weapon.GetInventory().CreateAttachment("AK_RailHndgrd");
					weapon.GetInventory().CreateAttachment("AK74_WoodBttstck");	
					weapon.GetInventory().CreateAttachment("KashtanOptic");
					weapon.GetInventory().CreateAttachment("'AK_Suppressor");			
				MissionObject.GetInventory().CreateInInventory("Mag_AK74_30Rnd");
				MissionObject.GetInventory().CreateInInventory("Mag_AK74_30Rnd");
				MissionObject.GetInventory().CreateInInventory("Headtorch_Grey");
				MissionObject.GetInventory().CreateInInventory("NBCBootsGray");
				MissionObject.GetInventory().CreateInInventory("Canteen");	
				MissionObject.GetInventory().CreateInInventory("TacticalBaconCan");
				MissionObject.GetInventory().CreateInInventory("Tomato");
				MissionObject.GetInventory().CreateInInventory("Battery9V");
				MissionObject.GetInventory().CreateInInventory("Battery9V");				
			}
			if (selectedLoadout == 10)
			{			
				weapon = MissionObject.GetInventory().CreateInInventory("AKS74U");
					weapon.GetInventory().CreateAttachment("AKS74U_Bttstck");			
				MissionObject.GetInventory().CreateInInventory("Mag_AK74_30Rnd");
				MissionObject.GetInventory().CreateInInventory("Mag_AK74_30Rnd");
				MissionObject.GetInventory().CreateInInventory("M67Grenade");
				MissionObject.GetInventory().CreateInInventory("M67Grenade");
				MissionObject.GetInventory().CreateInInventory("Matchbox");
				MissionObject.GetInventory().CreateInInventory("Canteen");	
				MissionObject.GetInventory().CreateInInventory("PortableGasStove");
				MissionObject.GetInventory().CreateInInventory("SmallGasCanister");
				MissionObject.GetInventory().CreateInInventory("Battery9V");
				MissionObject.GetInventory().CreateInInventory("Battery9V");
				MissionObject.GetInventory().CreateInInventory("Battery9V");			
			}
			if (selectedLoadout == 11)
			{			
				weapon = MissionObject.GetInventory().CreateInInventory("Glock19");
					weapon.GetInventory().CreateAttachment("PistolSuppressor");			
				MissionObject.GetInventory().CreateInInventory("Mag_Glock_15Rnd");
				MissionObject.GetInventory().CreateInInventory("Mag_Glock_15Rnd");
				MissionObject.GetInventory().CreateInInventory("FishingRod");
				MissionObject.GetInventory().CreateInInventory("Carp");
				MissionObject.GetInventory().CreateInInventory("Hook");
				MissionObject.GetInventory().CreateInInventory("Worm");
				MissionObject.GetInventory().CreateInInventory("CombatKnife");
				MissionObject.GetInventory().CreateInInventory("FieldShovel");
				MissionObject.GetInventory().CreateInInventory("Canteen");	
				MissionObject.GetInventory().CreateInInventory("MackerelFilletMeat");
				MissionObject.GetInventory().CreateInInventory("Battery9V");
				MissionObject.GetInventory().CreateInInventory("Battery9V");
				MissionObject.GetInventory().CreateInInventory("Battery9V");			
			}
		
			//barrel add firewood and start fire
			BarrelFire = BarrelHoles_ColorBase.Cast( m_MissionObjects[6] );
			BarrelFire.Open();
			BarrelFire.Synchronize();
			
			EntityAI barrel_wood = BarrelFire.GetInventory().CreateAttachment("Firewood"); 		
			BarrelFire.GetInventory().CreateAttachment("WoodenStick");
			BarrelFire.GetInventory().CreateAttachment("Rag");
			BarrelFire.StartFire( true );
			ItemBase.Cast( barrel_wood ).SetQuantity(6);
			
			//spawn 5 fish filets for smoking in barrel
			if ( BarrelFire.IsOpen() )	BarrelFire.Close();
			for ( i=0; i < 7; i++)
			BarrelFire.GetInventory().CreateAttachment("MackerelFilletMeat");

			
			Print("[SMM] Mission rewards spawned in reward container. Randomly selected loadout was "+ selectedLoadout +"." );
		}
		int card = Math.RandomIntInclusive(0,9);
		int coin = Math.RandomIntInclusive(0,1);
		if ( card <= 4 && coin ==1 ) MissionObject.GetInventory().CreateInInventory("RedemptionKeyCard_01" );
		if ( card > 4 && card < 8 && coin ==1 ) MissionObject.GetInventory().CreateInInventory("RedemptionKeyCard_02" );
		if ( card >= 8 && coin ==1 ) MissionObject.GetInventory().CreateInInventory("RedemptionKeyCard_03" );	
		EntityAI.Cast( Heli ).SetLifetime( 3888000 );
		
			//Spawn infected pilot
		vector InfectedPos = FlagBase.ModelToWorld("8 0 -24");
		Print("[SMM] There are currently"+m_MissionAIs.Count()+" Mision AIs." );
		int PilotAI = m_MissionAIs.Count();
		m_MissionAIs.Insert( GetGame().CreateObject( "ZmbM_CommercialPilotOld_Olive" , InfectedPos , false , true ) );
		DayZInfected InfectedSurvivor = DayZInfected.Cast( m_MissionAIs[PilotAI] );
			InfectedSurvivor.GetInventory().CreateAttachment("ZSh3PilotHelmet_Green");
		ItemBase Vest =	InfectedSurvivor.GetInventory().CreateAttachment("UKAssVest_Olive");
			InfectedSurvivor.GetInventory().CreateInInventory("Glock19");
			InfectedSurvivor.GetInventory().CreateInInventory("Mag_Glock_15Rnd");
			InfectedSurvivor.GetInventory().CreateInInventory("Battery9V");
		Grenade_Base smoke = Vest.GetInventory().CreateAttachment("M18SmokeGrenade_Purple");
		smoke.Unpin();	
		
		
		CarScript carScript2 = CarScript.Cast( Heli );
		m_MissionObjects.Remove(2);
		if( carScript2 )
		{
			MCK_CarKey_Base carKey = MCK_CarKey_Base.Cast(HeliKey);            
            if(carKey)
            {
                int mck_id = carKey.GenerateNewID();
				carKey.SetNewMCKId(mck_id);                

                carScript2.m_CarKeyId = mck_id;
                carScript2.m_HasCKAssigned = true;
				carScript2.SynchronizeValues(); 
		//		m_MCKLogger.LogMCKActivity("Heli Mission has assigned new owner and key (ID: " + mck_id + " ) to vehicle " + carScript.GetDisplayName() + " (" + carScript.m_CarScriptId + ")");
            }
			carScript2.m_IsCKLocked = false;
			carScript2.SynchronizeValues();
			carScript2.ResetLifetime();
		}
		//log team work players
		for ( i=0; i < m_PlayersInZone.Count(); i++)
		Print("[SMM] Player with SteamID64: "+ m_PlayersInZone.Get(i) +" has assisted capturing the flag.");
	}
	#ifdef ENFUSION_AI_PROJECT
	#ifdef EXPANSIONMODAI

	//eAIBase SpawnMissionAI(vector pos)
	//{
	//	eAIBase ai;
	//	if (!Class.CastTo(ai, GetGame().CreateObject(GetRandomAI(), pos))) return null;
	//
	//	ai.SetPosition(pos);
	//
	//	//ExpansionHumanLoadout.Apply(ai,loadout, false, eAISettings.GetLoadoutDirectories());
	//			
	//	ai.SetMovementSpeedLimit(2);
	//
	//	return ai;
	//}
	
	void SpawnPatrol(vector pos)//, string loadout = "CaptureFlagLoadout.json") 
	{
    	//DayZExpansion game = GetDayZGame().GetExpansionGame();

		eAIFactionShamans Shamans = new eAIFactionShamans();
		DayZExpansion game = GetDayZGame().GetExpansionGame();
		eAIBase ai = game.SpawnAI_Patrol(pos,loadout);
	//	eAIBase ai = SpawnMissionAI(pos);
		m_Group = ai.GetGroup();
		m_Group.SetFaction(Shamans);
		deletethis = ai.GetGroup();
		foreach (vector v : patrol_101) 
		{
			m_Group.AddWaypoint(v);
		}
		//int count = 3 - 1;
		//while (count != 0)
		//{
		//	ai = SpawnMissionAI(pos);
		//	ai.SetGroup(m_Group);
		//	m_Group.SetFaction(replicant);
		//	count--;
		//}
		string aigroupfaction = ai.GetGroup().GetFaction().GetName();
		Print (" AI faction is: "+ aigroupfaction);

	//	int count = 3; // temporary so we don't decrease the actual amount
	//	while (count > 1) 
	//	{
	//		(game.SpawnAI_Helper(ai, loadout)).RequestTransition("Rejoin");
	//		//m_MissionAIs.Insert(ai);
	//	//	ai.QuickReloadWeapon(weapon);
	//	//	ai.SetUnlimitedMags(true);
	//		count--;
	//	}
	}
	
	#endif
	#endif
	void SpawnStuff()
	{
		//Bot Patrol	
		for (int u = 0; u < patrol_101.Count(); u++) 
		{
			vector pos = m_MissionPosition + patrol_101[u];
			float x = pos[0] + Math.RandomFloat(-12, 12);
			float z = pos[2] + Math.RandomFloat(-12, 12);
			float y = GetGame().SurfaceY(x, z);
			patrol_101[u] = Vector (x, y, z);
		}
		vector gpos = { Math.RandomFloatInclusive(-50, -30.0), 2, Math.RandomFloatInclusive(-50.0, -30.0) };
		vector hpos = { Math.RandomFloatInclusive(-50, -30.0), 2, Math.RandomFloatInclusive(-50.0, -30.0) };
		vector ipos = { Math.RandomFloatInclusive(-50, -30.0), 2, Math.RandomFloatInclusive(-50.0, -30.0) };
		vector SoldierBotPos1 = Table1.ModelToWorld(gpos);
		vector SoldierBotPos2 = Table1.ModelToWorld(hpos);
		vector SoldierBotPos3 = Table1.ModelToWorld(ipos);
		SpawnPatrol(SoldierBotPos1);	
		SpawnPatrol(SoldierBotPos2);	
		SpawnPatrol(SoldierBotPos3);	
	}

	#ifdef EAI

	#endif
	void SpawnAIs()
	{	
		//Spawn infected
		for ( int z = 0 ; z < InfectedSpawns.Count() ; z++ )
		{
			string RandomInfected = InfectedTypes.GetRandomElement();
			vector InfectedPos = FlagBase.ModelToWorld( InfectedSpawns.Get(z) );
			m_MissionAIs.Insert( GetGame().CreateObject( RandomInfected, InfectedPos, false, true ) );
		}		

		//Spawn infected time delayed 1,500,000ms = 25 min delay
	//	int BotDlyTime = 900000;
		int BotDlyTime = 200000;
		GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( SpawnStuff, BotDlyTime, false );
			
		//  Spawn horde on Timer
		for ( int j = 0; j < HordeDensity; j++ )
		{
			//Spawn infected time delayed 480000ms = 8 min delay
			int InfectedDlyTime = 480000;
			GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( DelayedAISpawn, InfectedDlyTime, false );
			InfectedDlyTime += 480000;
		}	

	}


	void DelayedAISpawn()
	{
		//calc new spawn position
		float x = Math.RandomFloatInclusive( 20 , 50 );
		float y = Math.RandomFloatInclusive( 20 , 50 );	
		int Dice = Math.RandomIntInclusive( 0, 9);
		if ( Dice > 4 ) x *= -1.0;
		Dice = Math.RandomIntInclusive( 0, 9);
		if ( Dice < 5 ) y *= -1.0;
		vector NewPosVector = { x, 0, y };
		
		//spawn infected
		m_MissionAIs.Insert( GetGame().CreateObject( InfectedTypes.GetRandomElement(), m_MissionPosition + NewPosVector, false, true ));
	
	}
	void ObjDespawn() 
	{	
		//nothing to despawn
	}
	
	void ExtendMission()
	{
		//select secondary mission
		m_MissionExtended = false;
		
		//Set new mission messages
		m_MissionMessage1 = "Allright survivor, it seems you have found our base "+ m_MissionLocationDir +" of "+ m_MissionLocation +". There are probably alot of infected, try to get in there safely and hold the position.";
		m_MissionMessage2 = "Be aware, the Bandits will likely return soon, and hopefully the pilot will return and unlock the heli once the zombies and bandits are gone...";
		m_MissionMessage3 = "The flag is at "+ FlagCaptureProgress +"% captured. Hold on, you will make it.";
		
		//init Messenger for new messages
		m_MsgNum = 1;					//skip msg 0, begin with msg 1
		m_MsgChkTime = m_MissionTime;
		MsgCutoff = false;
		
		//increase mission MissionTimeout
		m_MissionTimeout = m_MissionTime + CaptureTime + 900;  
		
		//deployment settings & init for extended mission
		m_MissionZoneOuterRadius = 20.0;
		m_MissionZoneInnerRadius = 0.0001;
	}
			
	void MissionFinal()
	{	//When player enters last mission target zone	

		//Finish mission
		m_RewardsSpawned = true;
		m_MsgNum = -1;
		m_MsgChkTime = m_MissionTime + MsgDlyFinish;			
	}
	
	void PlayerChecks( PlayerBase player )
	{	//Executed continuously only on players in the mission zone. Avoid complex calculations or big data processing!
		//nothing to check related to player
	}
	
	void EntityChecks()
	{	//Executed continuously while mission is active and triggered. Avoid complex calculations or big data processing!
		int crntTime = GetGame().GetTime();
		
		//flag movement & capture progress calculation
		if ( !m_MissionExtended && !CaptureFinished && FlagBase && Flag && m_PlayersInZone.Count() > 0 )
		{	
				 m_MissionTimeout = m_MissionTime + 300;
			if ( !FlagElevationChkTime ) FlagElevationChkTime = crntTime;
			
			if ( crntTime >= FlagElevationChkTime + 100 )	//checking every 100ms
			{
				vector FlagPos = Flag.GetPosition();
				vector OldFlagPos;
				vector OldFlagOri;
				float flag_height = FlagPos[1];
			
				//check if flag is at lowest position
				if ( flag_height < PoleLowPos[1] && flag_speed < 0 ) 
				{
					flag_speed *= -1;
					
					//replace flag
					OldFlagPos = FlagPos;
					OldFlagOri = Flag.GetOrientation();
					
					GetGame().ObjectDelete( Flag );
					
					Flag = Flag_Base.Cast( GetGame().CreateObject( UpFlag, OldFlagPos ));
					Flag.SetPosition( OldFlagPos );
					Flag.SetOrientation( OldFlagOri );
					Flag.ShowSelection("unfolded");
					Flag.HideSelection("folded");
					Flag.SetTakeable(false);		
					Flag.SetLifetime(7200);
					m_MissionObjects.Insert( Flag );					
				}
				
				//check if flag is at highest position
				if ( flag_height > PoleHighPos[1] && flag_speed > 0 ) 
				{
					flag_speed *= -1;
					
					//check if flag has desired type
					if ( Flag.GetType() == UpFlag )
					{
						CaptureFinished = true;
						
						TeamPlayerCount = m_PlayersInZone.Count();
						if ( TeamPlayerCount > 3 ) TeamPlayerCount = 3;
						
						//spawn rewards & finish mission
						SpawnRewards();
						MissionFinal();
						
						//Deactivate EntityChecks
						GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).Remove( EntityChecks );
					}
				}
				
				//update flag position
				Flag.SetPosition( FlagPos + Vector(0,flag_speed,0) );
				
				FlagElevationChkTime = crntTime;
			}

			//flag capture progress 0-100% (only for the progress in the message)
			if ( FlagPos[1] - PoleLowPos[1] > 0 && FlagPos[1] - PoleHighPos[1] < 0 )
			{
				if ( flag_speed < 0 ) 	//when lowering flag
				FlagCaptureProgress = 50 + ( FlagPos[1] - PoleLowPos[1] ) / ( -delta_height * 2 / 100 ); 
				else					//when raising flag
				FlagCaptureProgress = 50 + ( FlagPos[1] - PoleLowPos[1] ) / ( delta_height * 2 / 100 );
			}
			
			//refresh message 3 (capture progress)
			if ( !m_MissionExtended )
			m_MissionMessage3 = "The flag is at "+ FlagCaptureProgress +"% captured. Hold the position, you will make it.";
		}
		
		//refuel fireplaces (!does only work when fireplaces are element 2 and 4 in m_MissionObjects array!)
		if ( !CaptureFinished )
		{
			if ( !FireplaceSpendChkTime ) FireplaceSpendChkTime = crntTime;
			
			if ( crntTime >= FireplaceSpendChkTime + 300000 )	//checking every 5min
			{
				for ( int i=0; i < m_MissionObjects.Count(); i++)
				{
					Object AnyObject = m_MissionObjects.Get(i);
					
					if ( AnyObject && AnyObject.IsFireplace() )  //Inherited( FireplaceBase ) )
					{
						FireplaceBase Campfire = FireplaceBase.Cast( AnyObject ); //m_MissionObjects.[i * 2 + 2] ); 
						if ( Campfire )
						{
							for (int j=0; j < Campfire.GetInventory().AttachmentCount(); ++j )
							{
								ItemBase attachment = ItemBase.Cast( Campfire.GetInventory().GetAttachmentFromIndex(j) );
								if ( attachment && attachment.GetType() == "Firewood" )
								{
									if ( attachment.GetQuantity() < 4 )		attachment.SetQuantity(6);
								}
								
								//to delete
								Print("[SMM] "+ AnyObject.GetType() +" Fireplace "+ attachment.GetType() +", quantity: "+ attachment.GetQuantity() );
							}												
						}
					}
				}
				
				FireplaceSpendChkTime = crntTime;
			}
		}
	}
				
	bool DeployMission()
	{	//When first player enters the mission zone (primary/secondary)
		if ( m_MissionPosition != "0 0 0" )
		{
			if ( m_MissionExtended )
			{	//only if primary is selected
				//Call spawners	
				GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).Call( this.SpawnObjects );
				GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( this.SpawnAIs, 5000, false );
			}
			return true;
		}
		else 
		{
			Print("[SMM] ERROR : Mission position was rejected or doesn't exist. MissionPosition is NULL!");
			return false;
		}
	}
}

