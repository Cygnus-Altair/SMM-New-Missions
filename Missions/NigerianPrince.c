class NigerianPrinceMission extends SurvivorMissions
{
	int MissionCutoffTime;
	//Mission related entities	
	ItemBase MissionObject;
	Object MissionBuilding;
	Object PicnicTable;
	
	//Mission parameters
	int ReqRubleAmount = 1;				//pieces, requested Ruble amount
	int ReqBookAmount = 1;
	int ExtendedTimout = 1800;				//seconds, mission duration time for extended mission
	int MsgDlyFinish = 60;					//seconds, message delay time after player has finished mission
	
	//Mission containers
	ref array<vector> ExtendedPosList = new array<vector>;
	ref array<vector> Spawnpoints = new array<vector>;
	ref array<vector> PriInfectSpawns = new array<vector>;
	ref array<vector> SecInfectSpawns = new array<vector>;
	ref array<string> InfectedTypes = new array<string>;
	vector BookPosition = m_MissionPosition;
	
	//Mission variables 
	string SurvivorName;		
	vector BribePosition = "0.84 -0.75 0.16";
	vector RewardsPosition = "0.84 16.75 0.16";	
	vector BookFloorCastle = "0 -2.6 0";
	
	bool IsExtended() return true;
	
	void NigerianPrinceMission()
	{	
		//Select primary mission
		m_MissionExtended = true;
				
		//Mission timeout
		m_MissionTimeout = 2700;			//seconds, mission max duration time
		
		//Mission zones
		m_MissionZoneOuterRadius = 120.0;	//meters (!Do not set lower than 200m), mission activation distance
		m_MissionZoneInnerRadius = 3.0;		//meters (!Do not set outside of 1-5m), mission finishing distance to target object
				
		//Mission informant
		m_MissionInformant = "Dr. Legasov";
	
		//Mission person names
		TStringArray SurvivorNames = {"Adetokunbo", "Ajibade", "Oladele", "Aladesanmi", "Obasa", "Taiwo", "Sadoluwa", "Yemisi", "Cornelius", "Orisadipe"};
		SurvivorName = SurvivorNames.GetRandomElement();
		
		//Set mission messages for primary mission
		m_MissionMessage1 = "I received a plea for help from Prince "+ SurvivorName +" of Nigeria.  He was separated from his party when they were attacked by the infected.  He has a opportunity for you near "+ m_MissionLocation +".";
		m_MissionMessage2 = "He said 'I seize this opportunity to extend my compliments to you.  My guide left and took my personal belongings, foolishly leaving me my equipment.  However, I need my passport back so I can be evacuated.  I had it hidden in a Bible. The guide has agreed to return this Bible in exchange for 1,000 Rubles in 100 Ruble Bills (10x$100).";
		m_MissionMessage3 = "The guide says to leave the money in a bag at a rest area \n** "+ m_MissionLocationDir +" of "+ m_MissionLocation +" **\n My leg is broken, therefore I am seeking for a reliable survivor to invest $1000 in gold (single gold bar) to bribe the guide and return the Bible to me.  If I don't get it, I will likely die here and my treasure will be lost.  I henceforth offer great wealth, if you will recover my passport for me.";
		
		//Spawnpoint for MissionObject for secondary,Primary mission
		Spawnpoints.Insert("0.84 16.75 0.16");
		Spawnpoints.Insert("0.84 -0.75 0.16");
		//Infected spawnpoints for primary mission
		PriInfectSpawns.Insert("-11 0 -7");
		PriInfectSpawns.Insert("-8 0 8");
		PriInfectSpawns.Insert("8 0 -9");
		PriInfectSpawns.Insert("6 0 -5");
		PriInfectSpawns.Insert("18 0 15");
		
		//Infected spawnpoints for secondary mission 
		SecInfectSpawns.Insert("1 0 1");
		SecInfectSpawns.Insert("2 10.6 1");
		SecInfectSpawns.Insert("1 10.6 2");
		SecInfectSpawns.Insert("1 19 1");
		SecInfectSpawns.Insert("1 19 1");
		
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
			if ( FoundObject.GetType() == "Land_Misc_TrailRoof_Small")
			{	
				MissionBuilding = FoundObject;	
				PicnicTable = FoundObject;				
				Print("[SMM] MissionBuilding is "+ MissionBuilding.GetType() +" at "+ m_MissionDescription[2] +" of "+ m_MissionDescription[1] +" @ "+ MissionBuilding.GetPosition() );
				break;
			}	
		}
		if ( !MissionBuilding ) Print("[SMM] Table couldn't be found. Mission rejected!");
		else
		{
			//Close all farm doors
			Building table = Building.Cast( MissionBuilding );
		//	for ( int j=0; j < 5; j++ ) if ( farm.IsDoorOpen(j) ) farm.CloseDoor(j); 
	
			GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( GetGame().UpdatePathgraphRegionByObject, 1000, false, table );		
		}				
		MissionCutoffTime = MissionSettings.RestartCycleTime - (m_MissionTimeout + MissionSettings.DelayTime + ExtendedTimout );
    
    if ( GetGame().GetTime() * 0.001 > MissionCutoffTime )
    MissionSettings.DelayTime = 3600;
	}
	
	void ~NigerianPrinceMission()
	{	
		//Despawn all remaining objects
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
	
	void SpawnTableBag()
	{
	//	MissionObject = ItemBase.Cast( GetGame().CreateObject( "HuntingBag", m_MissionPosition ));
	//	MissionObject.SetPosition( m_MissionPosition );
	//	MissionObject.SetOrientation( PicnicTable.GetOrientation() + "-90 -25 0" );			
	//	m_MissionObjects.InsertAt( MissionObject, 0 );
	//	Print("[SMM] Mission Object is" +MissionObject+"!");
	}
	
	void SpawnBible()
	{
		//new MissionObject after deleting hunting bag
		MissionObject = ItemBase.Cast( GetGame().CreateObject( "BookBible", BookPosition ));
		MissionObject.SetPosition( BookPosition );
		MissionObject.SetOrientation( MissionBuilding.GetOrientation() + "179 180 179" );		
		m_MissionObjects.InsertAt( MissionObject, 1 );
		
		Print("[SMM] Mission Object is" +MissionObject+"!");
		//MissionObject.GetInventory().CreateInInventory("BookBible");
		GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( this.SpawnContainer, 1000 );
		Print("[SMM] Mission Object is" +MissionObject+"!");
	}
	
	void SpawnContainer()
	{
				//Spawn mission target bag
			MissionObject = ItemBase.Cast( GetGame().CreateObject( "MountainBag_Orange", m_MissionPosition ));
			m_MissionObjects.InsertAt( MissionObject, 2 );	
			Print("[SMM] Mission Object is" +MissionObject+"!");
	}
	
	void SpawnRewards()
	{
		//new MissionObject after deleting protector case
		MissionObject = ItemBase.Cast( GetGame().CreateObject( "MountainBag_Green", m_MissionPosition ));
		Print("[SMM] Mission Object is" +MissionObject+"!");
		
		//Get random loadout 
		int selectedLoadout = Math.RandomIntInclusive( 0, 6);	//!change randomization limit after adding new loadouts!	

		//Spawn selected loadout items in mission object
		EntityAI weapon;
				
		if ( selectedLoadout == 0 )
		{
			weapon = MissionObject.GetInventory().CreateInInventory("M4A1");
				weapon.GetInventory().CreateAttachment("M4_RISHndgrd");
				weapon.GetInventory().CreateAttachment("M4_OEBttstck");
				weapon.GetInventory().CreateAttachment("TTC_HAMR");
				weapon.GetInventory().CreateAttachment("M4_Suppressor");
			MissionObject.GetInventory().CreateInInventory("Mag_STANAG_30Rnd");
			MissionObject.GetInventory().CreateInInventory("Mag_STANAG_30Rnd");
			MissionObject.GetInventory().CreateInInventory("M4_T3NRDSOptic");
			MissionObject.GetInventory().CreateInInventory("AmmoBox_556x45_20Rnd");
			MissionObject.GetInventory().CreateInInventory("AmmoBox_556x45_20Rnd");
			MissionObject.GetInventory().CreateInInventory("Goldbar_Base");
			MissionObject.GetInventory().CreateInInventory("PeachesCan");
			MissionObject.GetInventory().CreateInInventory("Goldbar_Base");
			MissionObject.GetInventory().CreateInInventory("Battery9V");
			MissionObject.GetInventory().CreateInInventory("Goldbar_Base");
		}
		if (selectedLoadout == 1)
		{
			weapon = MissionObject.GetInventory().CreateInInventory("SVD");
				weapon.GetInventory().CreateAttachment("PSO1Optic");
			MissionObject.GetInventory().CreateInInventory("Mag_SVD_10Rnd");
			MissionObject.GetInventory().CreateInInventory("Mag_SVD_10Rnd");
			MissionObject.GetInventory().CreateInInventory("PSO1Optic");
			MissionObject.GetInventory().CreateInInventory("KazuarOptic");
			MissionObject.GetInventory().CreateInInventory("AmmoBox_762x54_20Rnd");
			MissionObject.GetInventory().CreateInInventory("AmmoBox_762x54_20Rnd");
			MissionObject.GetInventory().CreateInInventory("Goldbar_Base");
			MissionObject.GetInventory().CreateInInventory("Goldbar_Base");
			MissionObject.GetInventory().CreateInInventory("Goldbar_Base");
			MissionObject.GetInventory().CreateInInventory("CanOpener");
			MissionObject.GetInventory().CreateInInventory("PeachesCan");
			MissionObject.GetInventory().CreateInInventory("Canteen");
			MissionObject.GetInventory().CreateInInventory("Battery9V");
		}
		if (selectedLoadout == 2)
		{
			weapon = MissionObject.GetInventory().CreateInInventory("AKM");
				weapon.GetInventory().CreateAttachment("MSFC_AK_RailHndgrd_Woodland");
				weapon.GetInventory().CreateAttachment("MSFC_AK_PlasticButtstock_Woodland");
				weapon.GetInventory().CreateAttachment("PSO1Optic");
				weapon.GetInventory().CreateAttachment("AK_Suppressor");
			MissionObject.GetInventory().CreateInInventory("Mag_AKM_30Rnd");
			MissionObject.GetInventory().CreateInInventory("Mag_AKM_30Rnd");
			MissionObject.GetInventory().CreateInInventory("AmmoBox_762x39_20Rnd");
			MissionObject.GetInventory().CreateInInventory("AmmoBox_762x39_20Rnd");
			MissionObject.GetInventory().CreateInInventory("Goldbar_Base");
			MissionObject.GetInventory().CreateInInventory("Goldbar_Base");
			MissionObject.GetInventory().CreateInInventory("PeachesCan");
			MissionObject.GetInventory().CreateInInventory("Canteen");
			MissionObject.GetInventory().CreateInInventory("Battery9V");
		}
		if (selectedLoadout == 3)
		{
			weapon = MissionObject.GetInventory().CreateInInventory("TTC_FAL");
				weapon.GetInventory().CreateAttachment("Fal_OeBttstck");
				weapon.GetInventory().CreateAttachment("TTC_FAL_RIS_Hndgrd");
			MissionObject.GetInventory().CreateInInventory("TTC_FAL_Magazine");
			MissionObject.GetInventory().CreateInInventory("TTC_Coupled_FAL_Magazine");
			MissionObject.GetInventory().CreateInInventory("TTC_Universal_Suppressor_BLACK")
			MissionObject.GetInventory().CreateInInventory("TTC_HAMR");
			MissionObject.GetInventory().CreateInInventory("FNX45");
			MissionObject.GetInventory().CreateInInventory("Mag_FNX45_15Rnd");
			MissionObject.GetInventory().CreateInInventory("Mag_FNX45_15Rnd");
			MissionObject.GetInventory().CreateInInventory("AmmoBox_45ACP_25Rnd");
			MissionObject.GetInventory().CreateInInventory("FNP45_MRDSOptic");
			MissionObject.GetInventory().CreateInInventory("PistolSuppressor");
			MissionObject.GetInventory().CreateInInventory("Goldbar_Base");
			MissionObject.GetInventory().CreateInInventory("Goldbar_Base");
			MissionObject.GetInventory().CreateInInventory("Battery9V");
		}	
		if (selectedLoadout == 4)
		{
			weapon = MissionObject.GetInventory().CreateInInventory("SKS");
				weapon.GetInventory().CreateAttachment("PUScopeOptic");
			MissionObject.GetInventory().CreateInInventory("AmmoBox_762x39_20Rnd");
			weapon = MissionObject.GetInventory().CreateInInventory("FNX45");
				weapon.GetInventory().CreateAttachment("PistolSuppressor");
				EntityAI weaponlight = weapon.GetInventory().CreateAttachment("TLRLight");
					weaponlight.GetInventory().CreateAttachment("Battery9V");
			MissionObject.GetInventory().CreateInInventory("Mag_FNX45_15Rnd");
			MissionObject.GetInventory().CreateInInventory("AmmoBox_45ACP_25Rnd");
			MissionObject.GetInventory().CreateInInventory("AmmoBox");
			MissionObject.GetInventory().CreateInInventory("Goldbar_Base");
			MissionObject.GetInventory().CreateInInventory("Goldbar_Base");
		}	
		if (selectedLoadout == 5)
		{
			weapon = MissionObject.GetInventory().CreateInInventory("Winchester70");
				weapon.GetInventory().CreateAttachment("AD_Pilad");
			MissionObject.GetInventory().CreateInInventory("Ammo_308Win");
			MissionObject.GetInventory().CreateInInventory("Ammo_308Win");
			MissionObject.GetInventory().CreateInInventory("FNX45");
			MissionObject.GetInventory().CreateInInventory("Mag_FNX45_15Rnd");
			MissionObject.GetInventory().CreateInInventory("AmmoBox_45ACP_25Rnd");
			MissionObject.GetInventory().CreateInInventory("AmmoBox");
			MissionObject.GetInventory().CreateInInventory("Goldbar_Base");
			MissionObject.GetInventory().CreateInInventory("TLRLight");
			MissionObject.GetInventory().CreateInInventory("Battery9V");
			MissionObject.GetInventory().CreateInInventory("Goldbar_Base");
		}
		if (selectedLoadout == 6)
		{
			weapon = MissionObject.GetInventory().CreateInInventory("TTC_SCARHBlack");
				weapon.GetInventory().CreateAttachment("TTC_HAMR");
			MissionObject.GetInventory().CreateInInventory("TTC_SCARHMag");
			MissionObject.GetInventory().CreateInInventory("Ammo_308Win");
			MissionObject.GetInventory().CreateInInventory("TTC_SCARHMag");
			MissionObject.GetInventory().CreateInInventory("Goldbar_Base");
			MissionObject.GetInventory().CreateInInventory("Goldbar_Base");
			MissionObject.GetInventory().CreateInInventory("AmmoBox");
			MissionObject.GetInventory().CreateInInventory("Goldbar_Base");
			MissionObject.GetInventory().CreateInInventory("Goldbar_Base");
			MissionObject.GetInventory().CreateInInventory("Goldbar_Base");
			MissionObject.GetInventory().CreateInInventory("Goldbar_Base");
		}
		int card = Math.RandomIntInclusive(0,9);
		int coin = Math.RandomIntInclusive(0,1);
		if ( card <= 4 && coin ==1 ) MissionObject.GetInventory().CreateInInventory("RedemptionKeyCard_01" );
		if ( card > 4 && card < 8 && coin ==1 ) MissionObject.GetInventory().CreateInInventory("RedemptionKeyCard_02" );
		if ( card >= 8 && coin ==1 ) MissionObject.GetInventory().CreateInInventory("RedemptionKeyCard_03" );	
		Print("[SMM] Mission rewards spawned in reward container. Randomly selected loadout was "+ selectedLoadout +"." );	
	}
	
	void SpawnObjects()
	{			
		Object FoundObject;
		
		if ( m_MissionExtended )
		{	
						//New MissionPosition is the picnic table
			m_MissionPosition = MissionBuilding.ModelToWorld( BribePosition );
			
			
			//Cleanup position before spawning bag case
			GetGame().GetObjectsAtPosition( m_MissionPosition, 1.0, m_ObjectList, m_ObjectCargoList );
			if ( m_ObjectList.Count() > 0)
			{	
				for ( int k = 0; k < m_ObjectList.Count(); k++ )
				{
					FoundObject = m_ObjectList.Get(k);
					if ( FoundObject.IsItemBase() )
					GetGame().ObjectDelete( FoundObject );
				}	
			}
			//spawn first mission target hunting bag
			//GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( this.SpawnTableBag, 1000 );
		MissionObject = ItemBase.Cast( GetGame().CreateObject( "HuntingBag", m_MissionPosition ));
		MissionObject.SetPosition( m_MissionPosition );
		MissionObject.SetOrientation( PicnicTable.GetOrientation() + "-90 -25 0" );			
		m_MissionObjects.InsertAt( MissionObject, 0 );
		Print("[SMM] Mission Object is" +MissionObject+"!");
			Print("[SMM] Survivor Mission "+ m_selectedMission +" :: "+ m_MissionName +" ...primary mission deployed!");
		}
		else 
		{				
			if ( !m_MissionExtended && !MissionBuilding )
			{
				Print("[SMM] Extended MissionBuilding couldn't be found in "+ m_MissionDescription[3] +" ... Mission rejected!");
			}
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
				//Spawn some infected at the guide
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
				//Spawn some infected at castle  - commented out to spawn using FinalMission()
				for ( int k = 0 ; k < SecInfectSpawns.Count() ; k++ )
				{
		    	   	RandomInfected = InfectedTypes.GetRandomElement();
					InfectedPos = MissionBuilding.ModelToWorld( SecInfectSpawns.Get(k) );
					Zed = DayZInfected.Cast( GetGame().CreateObject( RandomInfected, InfectedPos, false, true ));
					m_MissionAIs.Insert( Zed );
				}		
			}
		}
	}
	
	void ObjDespawn() 
	{	
		//Despawn nothing
	}
	
	void ExtendMission()
	{
		//Set new mission messages
		//Set messages for secondary mission
		m_MissionMessage1 = "It is most auspicious that you have found the bag.  Take the bag and place a single gold bar inside it individually.  Then, hide it somewhere and leave it behind. The guide will verify the amount once you have left the area.";
		m_MissionMessage2 = "Upon completion of the verification, the guide said his associate will leave the Bible on the first floor of the Castle.  Pick it up from there and place it in my luggage on the top floor of\n** "+ m_MissionDescription[3] +" Castle. **\nOnce I have the Bible you shall have your reward.";
		m_MissionMessage3 = "Be careful on your way to the castle at "+ m_MissionDescription[3] +", there might be bandits around who could intercepted our little radio talk here. Good luck!";
		
		//init Messenger for new messages
		m_MsgNum = 1;					//skip msg 0, begin with msg 1
		m_MsgChkTime = m_MissionTime;
		MsgCutoff = false;
		
		//increase mission MissionTimeout
		m_MissionTimeout = m_MissionTime + ExtendedTimout;  
		
				//deployment settings & init for secondary mission		
		m_MissionZoneOuterRadius = 120.0;
		m_MissionZoneInnerRadius = 2.0;
		
		//Get secondary building position

	if ( EventsWorldData.GetBuildingsAtLoc("Land_Castle_Bergfrit", m_MissionDescription[3], ExtendedPosList ))
		m_MissionPosition = ExtendedPosList.GetRandomElement();
		else Print("[SMM] Can't get secondary MissionPosition in "+ m_MissionDescription[3] +" from EventsWorldData!");
			
			Print("[SMM] Survivor Mission Extension "+ m_selectedMission +" :: "+ m_MissionName +" ...secondary mission deployed!");

	}
	
	void MissionFinal()
	{	
		//set off a smoke grenade
	//notworking	MissionObject = ItemBase.Cast( GetGame().CreateObject( "M18SmokeGrenade_Green", m_MissionPosition ));
	//notworking		m_MissionObjects.InsertAt( MissionObject, 3 );
	//notworking		Object grenade = MissionObject;
	//notworking		grenade.ActionUnpin;
	//notworking		Print("[SMM] Mission Object is" +MissionObject+"!");
		
	}
	
	void PlayerChecks( PlayerBase player )
	{

			//Check if container has desired amount of rubles collected at primary mission position
		if ( MissionObject && MissionObject.ClassName() == "HuntingBag" )
		{
			int CargoCount = MissionObject.GetInventory().CountInventory();
			int LastCount = 0;
			int FoundObjects = 0;
			if ( CargoCount != LastCount )
			{
				if ( CargoCount >= ReqRubleAmount && FoundObjects <= ReqRubleAmount)
				{	
					CargoBase CargoItems1 = MissionObject.GetInventory().GetCargo();		
					
					for ( int i = 0; i < CargoCount; i++ )
					{
						EntityAI CargoItem = CargoItems1.GetItem(i);
					//	if ( CargoItem.GetType() == "TraderPlus_Money_Dollar100" )    int CashCount = Math.Floor( ItemBase.Cast( CargoItem ).GetQuantity() );
						//	for ( int m = 0; m < CashCount; m++ )
						//	{
							
								if ( m_MissionObjects.Find( CargoItem ) && CargoItem.GetType() == "Goldbar_Base" ) FoundObjects++;
							//if ( m_MissionObjects.Find( CargoItem ) && CargoItem.GetType() == "TraderPlus_Money_Dollar50" ) FoundObjects++;
							//if ( m_MissionObjects.Find( CargoItem ) && CargoItem.GetType() == "TraderPlus_Money_Dollar20" ) FoundObjects++;
							//if ( m_MissionObjects.Find( CargoItem ) && CargoItem.GetType() == "TraderPlus_Money_Dollar10" ) FoundObjects++;
							//if ( m_MissionObjects.Find( CargoItem ) && CargoItem.GetType() == "TraderPlus_Money_Dollar5" ) FoundObjects++;
							//if ( m_MissionObjects.Find( CargoItem ) && CargoItem.GetType() == "TraderPlus_Money_Dollar1" ) FoundObjects++;
							//if ( m_MissionObjects.Find( CargoItem ) && CargoItem.GetType() == "TraderPlus_Money_Euro500" ) FoundObjects++;
							//if ( m_MissionObjects.Find( CargoItem ) && CargoItem.GetType() == "TraderPlus_Money_Ruble1000" ) FoundObjects++;
							//if ( m_MissionObjects.Find( CargoItem ) && CargoItem.GetType() == "Goldbar_Base" ) FoundObjects++;
							
							
							else continue;						
						//	}
						//	CashCount = 0;
						//When requested amount of rubles are present, despawn MissionObject & spawn next mission object
						if ( FoundObjects >= ReqRubleAmount )
						{
							Print("[SMM] Player with SteamID64: "+ player.GetIdentity().GetPlainId() +" has successfully stored the requested amount of "+ ReqRubleAmount +" MissionObjects in the container.");
							//delete container first
							GetGame().ObjectDelete( MissionObject );
							//call rewards spawn one second later 
							GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( this.SpawnBible, 1000 );
							break;
						} 
					}
					LastCount = CargoCount;
				}
			} 
		}	
		if ( MissionObject && MissionObject.ClassName() == "MountainBag_Orange" && !m_MissionExtended )
		{
			 CargoCount = MissionObject.GetInventory().CountInventory();
			LastCount = 0;
			int FoundBibleObject;
			
			if ( CargoCount != LastCount )
			{
				if ( CargoCount >= ReqBookAmount )
				{	
					CargoBase CargoItems2 = MissionObject.GetInventory().GetCargo();		
					
					for ( int j = 0; j < CargoCount; j++ )
					{
						EntityAI CargoItem2 = CargoItems2.GetItem(j);
						if ( CargoItem2 && CargoItem2.GetType() == "BookBible" ) FoundBibleObject++;
						else continue;					
						
						//When requested amount of Bibles are present, despawn MissionObject & spawn rewards
						if ( FoundBibleObject >= ReqBookAmount  )
						{
							Print("[SMM] Player with SteamID64: "+ player.GetIdentity().GetPlainId() +" has successfully stored the requested amount of "+ ReqBookAmount +" MissionObjects in the container.");
							//delete container first
							GetGame().ObjectDelete( MissionObject );
							//call rewards spawn one second later 
							GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( this.SpawnRewards, 1000 );
							m_RewardsSpawned = true;
							m_MsgNum = -1;
							m_MsgChkTime = m_MissionTime + MsgDlyFinish;
							Print("remaining time is: "+ m_MsgChkTime +" misiontime is: " +m_MissionTime);
							break;
						} 
					}
					LastCount = CargoCount;
				}
			} 
		}		
	}
	void UpdateBots(float dt)
	{
		//No bots involved in this mission		
	}
	
	bool DeployMission()
	{	//When first player enters the mission zone (primary/secondary)
		//Get MissionBuilding at mission position
		if ( !m_MissionExtended ) 
		{
			if ( EventsWorldData.GetBuildingsAtLoc("Land_Castle_Bergfrit", m_MissionDescription[3], ExtendedPosList ))
			{		
				//Get MissionPosition for MissionBuilding from secondary mission
				m_MissionPosition = ExtendedPosList.GetRandomElement();						
				GetGame().GetObjectsAtPosition( m_MissionPosition, 1.0 , m_ObjectList , m_ObjectCargoList );
				for ( int m=0; m < m_ObjectList.Count(); m++ )
				{ 
					Object FoundObject = m_ObjectList.Get(m);
					if ( FoundObject.GetType() == "Land_Castle_Bergfrit" )
					{	
						MissionBuilding = FoundObject;			 
						Print("[SMM] MissionBuilding extended is "+ MissionBuilding.GetType() +" at "+ m_MissionDescription[3] +" @ "+ MissionBuilding.GetPosition() );
											
						//new MissionPosition is rewards spawnpoint
						m_MissionPosition = MissionBuilding.ModelToWorld( RewardsPosition );
						BookPosition = MissionBuilding.ModelToWorld( BookFloorCastle );
						break;
					}	
				}
			}	
		}
		else 
		{	
			if ( !m_MissionExtended )
			Print("[SMM] Can't find MissionBuilding Castle in "+ m_MissionDescription[3] +"!");
		}
		if ( m_MissionExtended )
		{	
		GetGame().GetObjectsAtPosition( m_MissionPosition , 1.0 , m_ObjectList , m_ObjectCargoList );
		for ( int i = 0 ; i < m_ObjectList.Count(); i++ )
		{ 
			FoundObject = m_ObjectList.Get(i);
			if ( FoundObject.GetType() == "Land_Misc_TrailRoof_Small" )
			{	
				MissionBuilding = FoundObject;		
				Print("[SMM] MissionBuilding is "+ MissionBuilding.GetType() +" at "+ m_MissionDescription[2] +" of "+ m_MissionDescription[1] +" @ "+ MissionBuilding.GetPosition() );
				break;
			}	
		}
		if ( !MissionBuilding ) Print("[SMM] DeployMission-Picnic Table couldn't be found. Mission rejected!");	
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
			if ( MissionSettings.DebugMode ) Print("[SMM] MissionBuilding - deploy no picnic is NULL!");
			return false;
		}
		
	}
}


