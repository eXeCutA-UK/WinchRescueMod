\
class CfgPatches
{
    class WinchRescue
    {
        units[] = {"HandCrankWinch"};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] = {"DZ_Data","DZ_Scripts"};
    };
};

class CfgMods
{
    class WinchRescue
    {
        dir = "WinchRescue";
        picture = "WinchRescue\data\winch_icon_512.paa";
        action = "";
        hideName = 0;
        hidePicture = 0;
        name = "Winch Rescue";
        credits = "";
        author = "You + ChatGPT";
        authorID = "0";
        version = "1.0";
        type = "mod";
        dependencies[] = {"Game","World","Mission"};
        class defs
        {
            class gameScriptModule
            {
                value = "";
                files[] = {"WinchRescue\scripts\3_Game"};
            };
            class worldScriptModule
            {
                value = "";
                files[] = {"WinchRescue\scripts\4_World"};
            };
            class missionScriptModule
            {
                value = "";
                files[] = {"WinchRescue\scripts\5_Mission"};
            };
        };
    };
};

class CfgVehicles
{
    class Inventory_Base;
    class HandCrankWinch: Inventory_Base
    {
        scope = 2;
        displayName = "Hand-Crank Winch";
        descriptionShort = "Place near a vehicle, connect, pull, then disconnect to rescue it.";
        model = "WinchRescue\data\models\hand_crank_winch.obj"; // TODO: convert to P3D and change this path
        weight = 5000;
        itemSize[] = {3,2}; // 6 slots
        physLayer = "item_large";
        lootCategory = "Crafted";
        attachments[] = {};
        rotationFlags = 17;
        varTemperatureMax = 40;
        repairableWithKits[] = {5};
        repairCosts[] = {25.0};
        soundImpactType = "metal";
        hiddenSelections[] = {};
        class DamageSystem
        {
            class GlobalHealth
            {
                class Health
                {
                    hitpoints = 200;
                };
            };
        };
        canBeSplit = 0;
    };
};

// --- OPTIONAL: Add custom sounds here later ---
// class CfgSoundShaders
// {
//     class WR_WinchCrank_SoundShader
//     {
//         samples[] = {{"WinchRescue\data\sound\winch_crank",1}}; // provide .wss/.ogg
//         volume = 0.9;
//         range = 25;
//     };
// };
// class CfgSoundSets
// {
//     class WR_WinchCrank_SoundSet
//     {
//         soundShaders[] = {"WR_WinchCrank_SoundShader"};
//         volumeFactor = 1.0;
//         spatial = 1;
//         doppler = 0;
//     };
// };
