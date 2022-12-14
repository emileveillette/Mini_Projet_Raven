#include "Weapon_RocketLauncher.h"
#include "../Raven_Bot.h"
#include "misc/Cgdi.h"
#include "../Raven_Game.h"
#include "../Raven_Map.h"
#include "../lua/Raven_Scriptor.h"
#include "fuzzy/FuzzyOperators.h"


//--------------------------- ctor --------------------------------------------
//-----------------------------------------------------------------------------
RocketLauncher::RocketLauncher(Raven_Bot*   owner):

                      Raven_Weapon(type_rocket_launcher,
                                   script->GetInt("RocketLauncher_DefaultRounds"),
                                   script->GetInt("RocketLauncher_MaxRoundsCarried"),
                                   script->GetDouble("RocketLauncher_FiringFreq"),
                                   script->GetDouble("RocketLauncher_IdealRange"),
                                   script->GetDouble("Rocket_MaxSpeed"),
                                   owner)
{
    //setup the vertex buffer
  const int NumWeaponVerts = 8;
  const Vector2D weapon[NumWeaponVerts] = {Vector2D(0, -3),
                                           Vector2D(6, -3),
                                           Vector2D(6, -1),
                                           Vector2D(15, -1),
                                           Vector2D(15, 1),
                                           Vector2D(6, 1),
                                           Vector2D(6, 3),
                                           Vector2D(0, 3)
                                           };
  for (int vtx=0; vtx<NumWeaponVerts; ++vtx)
  {
    m_vecWeaponVB.push_back(weapon[vtx]);
  }

  //setup the fuzzy module
  InitializeFuzzyModule();

}


//------------------------------ ShootAt --------------------------------------
//-----------------------------------------------------------------------------
inline void RocketLauncher::ShootAt(Vector2D pos)
{ 
  if (NumRoundsRemaining() > 0 && isReadyForNextShot())
  {
    //fire off a rocket!
    m_pOwner->GetWorld()->AddRocket(m_pOwner, pos);

    m_iNumRoundsLeft--;

    UpdateTimeWeaponIsNextAvailable();

    //add a trigger to the game so that the other bots can hear this shot
    //(provided they are within range)
    m_pOwner->GetWorld()->GetMap()->AddSoundTrigger(m_pOwner, script->GetDouble("RocketLauncher_SoundRange"));
  }
}



//---------------------------- Desirability -----------------------------------
//
//-----------------------------------------------------------------------------
double RocketLauncher::GetDesirability(double DistToTarget)
{
  if (m_iNumRoundsLeft == 0)
  {
    m_dLastDesirabilityScore = 0;
  }
  else
  {
    //fuzzify distance and amount of ammo
    m_FuzzyModule.Fuzzify("DistToTarget", DistToTarget);
    m_FuzzyModule.Fuzzify("AmmoStatus", (double)m_iNumRoundsLeft);

    m_dLastDesirabilityScore = m_FuzzyModule.DeFuzzify("Desirability", FuzzyModule::max_av);
  }

  return m_dLastDesirabilityScore;
}

//-------------------------  InitializeFuzzyModule ----------------------------
//
//  set up some fuzzy variables and rules
//-----------------------------------------------------------------------------
void RocketLauncher::InitializeFuzzyModule()
{
  //Question a)
  FuzzyVariable& DistToTarget = m_FuzzyModule.CreateFLV("DistToTarget");
  FzSet& Target_Close = DistToTarget.AddLeftShoulderSet("Target_Close",0,0,50);
  FzSet& Target_Almost_close = DistToTarget.AddTriangularSet("Target_Kinda_close", 0, 100, 150);
  FzSet& Target_Medium = DistToTarget.AddTriangularSet("Target_Medium",100,150,250);
  FzSet& Target_Almost_Far = DistToTarget.AddTriangularSet("Target_Kinda_Far", 200, 250, 350);
  FzSet& Target_Far = DistToTarget.AddRightShoulderSet("Target_Far",250,350,1000);

  FuzzyVariable& Desirability = m_FuzzyModule.CreateFLV("Desirability"); 
  FzSet& VeryDesirable = Desirability.AddRightShoulderSet("VeryDesirable", 75, 87.5, 100);
  FzSet& Almost_VeryDesirable = DistToTarget.AddTriangularSet("Almost_VeryDesirable", 62.5, 75, 87.5);
  FzSet& Desirable = Desirability.AddTriangularSet("Desirable", 37.5, 50, 62.5);
  FzSet& Almost_Undesirable = DistToTarget.AddTriangularSet("Almost_Undesirable", 12.5, 25, 37.5);
  FzSet& Undesirable = Desirability.AddLeftShoulderSet("Undesirable", 0, 12.5, 25);

  FuzzyVariable& AmmoStatus = m_FuzzyModule.CreateFLV("AmmoStatus");
  FzSet& Ammo_Loads = AmmoStatus.AddRightShoulderSet("Ammo_Loads", 20, 35, 100);
  FzSet& Ammo_Almost_Loaded = AmmoStatus.AddTriangularSet("Ammo_Almost_Loaded", 15, 20, 35);
  FzSet& Ammo_Okay = AmmoStatus.AddTriangularSet("Ammo_Okay",10, 15, 20);
  FzSet& Ammo_Almost_Low = AmmoStatus.AddTriangularSet("Ammo_Almost_Low", 0, 10, 15);
  FzSet& Ammo_Low = AmmoStatus.AddTriangularSet("Ammo_Low", 0, 0, 10);


  m_FuzzyModule.AddRule(FzAND(Target_Close, Ammo_Loads), Undesirable);
  m_FuzzyModule.AddRule(FzAND(Target_Close, Ammo_Almost_Loaded), Undesirable);
  m_FuzzyModule.AddRule(FzAND(Target_Close, Ammo_Okay), Undesirable);
  m_FuzzyModule.AddRule(FzAND(Target_Close, Ammo_Almost_Low), Undesirable);
  m_FuzzyModule.AddRule(FzAND(Target_Close, Ammo_Low), Undesirable);

  m_FuzzyModule.AddRule(FzAND(Target_Almost_close, Ammo_Loads), Desirable);
  m_FuzzyModule.AddRule(FzAND(Target_Almost_close, Ammo_Almost_Loaded), Desirable);
  m_FuzzyModule.AddRule(FzAND(Target_Almost_close, Ammo_Okay), Almost_Undesirable);
  m_FuzzyModule.AddRule(FzAND(Target_Almost_close, Ammo_Almost_Low), Almost_Undesirable);
  m_FuzzyModule.AddRule(FzAND(Target_Almost_close, Ammo_Low), Almost_VeryDesirable);

  m_FuzzyModule.AddRule(FzAND(Target_Medium, Ammo_Loads), VeryDesirable);
  m_FuzzyModule.AddRule(FzAND(Target_Medium, Ammo_Almost_Loaded), VeryDesirable);
  m_FuzzyModule.AddRule(FzAND(Target_Medium, Ammo_Okay), Almost_VeryDesirable);
  m_FuzzyModule.AddRule(FzAND(Target_Medium, Ammo_Almost_Low), Almost_VeryDesirable);
  m_FuzzyModule.AddRule(FzAND(Target_Medium, Ammo_Low), Desirable);

  m_FuzzyModule.AddRule(FzAND(Target_Almost_Far, Ammo_Loads), Almost_VeryDesirable);
  m_FuzzyModule.AddRule(FzAND(Target_Almost_Far, Ammo_Almost_Loaded), Almost_VeryDesirable);
  m_FuzzyModule.AddRule(FzAND(Target_Almost_Far, Ammo_Okay), Desirable);
  m_FuzzyModule.AddRule(FzAND(Target_Almost_Far, Ammo_Almost_Low), Almost_VeryDesirable);
  m_FuzzyModule.AddRule(FzAND(Target_Almost_Far, Ammo_Low), Almost_VeryDesirable);

  m_FuzzyModule.AddRule(FzAND(Target_Far, Ammo_Loads), Desirable);
  m_FuzzyModule.AddRule(FzAND(Target_Far, Ammo_Almost_Loaded), Almost_Undesirable);
  m_FuzzyModule.AddRule(FzAND(Target_Far, Ammo_Okay), Almost_Undesirable);
  m_FuzzyModule.AddRule(FzAND(Target_Far, Ammo_Almost_Low), Undesirable);
  m_FuzzyModule.AddRule(FzAND(Target_Far, Ammo_Low), Undesirable);
}


//-------------------------------- Render -------------------------------------
//-----------------------------------------------------------------------------
void RocketLauncher::Render()
{
    m_vecWeaponVBTrans = WorldTransform(m_vecWeaponVB,
                                   m_pOwner->Pos(),
                                   m_pOwner->Facing(),
                                   m_pOwner->Facing().Perp(),
                                   m_pOwner->Scale());

  gdi->RedPen();

  gdi->ClosedShape(m_vecWeaponVBTrans);
}