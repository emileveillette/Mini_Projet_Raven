#include "Goal_Hide.h"
#include "../Raven_Bot.h"
#include "../Raven_SteeringBehaviors.h"
#include "time/CrudeTimer.h"
#include "../navigation/Raven_PathPlanner.h"
#include "misc/cgdi.h"
#include "Goal_SeekToPosition.h"
#include "Goal_FollowPath.h"

#include "debug/DebugConsole.h"



//---------------------------- ctor -------------------------------------------
//-----------------------------------------------------------------------------
Goal_Hide::Goal_Hide(Raven_Bot* pBot) :

    Goal<Raven_Bot>(pBot,
        goal_Hide)

{}

//---------------------------- Activate -------------------------------------
//-----------------------------------------------------------------------------  
void Goal_Hide::Activate()
{
    m_iStatus = active;

    m_pOwner->GetSteering()->FleeOn();
    
    debug_con << "BOT " << m_pOwner->ID() << " Fuit comme un lache!" << "";
}

//------------------------------ Process --------------------------------------
//-----------------------------------------------------------------------------
int Goal_Hide::Process()
{
    //if status is inactive, call Activate()
    ActivateIfInactive();

    //test to see if the bot has become stuck

    //test to see if the bot has reached the waypoint. If so terminate the goal


        if (!m_pOwner->GetTargetSys()->isTargetWithinFOV())
        {
            m_iStatus = completed;
        }


    return m_iStatus;
}



//---------------------------- Terminate --------------------------------------
//-----------------------------------------------------------------------------
void Goal_Hide::Terminate()
{
    debug_con << "BOT " << m_pOwner->ID() << " ne fuit plus" << "";
    m_pOwner->GetSteering()->FleeOff();
    m_pOwner->GetSteering()->ArriveOff();

    m_iStatus = completed;
}

//----------------------------- Render ----------------------------------------
//-----------------------------------------------------------------------------
void Goal_Hide::Render()
{
    if (m_iStatus == active)
    {
        gdi->GreenBrush();
        gdi->BlackPen();
        gdi->Circle(m_vPosition, 3);
    }

    else if (m_iStatus == inactive)
    {

        gdi->RedBrush();
        gdi->BlackPen();
        gdi->Circle(m_vPosition, 3);
    }
}

