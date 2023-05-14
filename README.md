# SMM-New-Missions
New missions for Survivor Mission Mod

Main is for Enfusion AI  (eAI)
Expansion is for Expansion AI

you will need to create your own loadouts from the default loadouts, or rename the loadouts mentioned in the mission files, such as ConvoyLoadout.json.

Note Helisteal hasnt been updated since I replaced it with the Capture the Flag mission.

you will also need to edit your MissionSettings.c //mission module settings to include:

	static int RestartCycleTime = 14100;  //seconds (!Do not set lower than 3600 seconds), Scheduled server restart      edit: 14,400 = 4 hours
