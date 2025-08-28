\
class HandCrankWinch : ItemBase
{
    protected CarScript m_TargetVehicle;
    protected bool m_IsConnected;
    protected bool m_IsPulling;
    protected vector m_WinchAnchor; // world pos
    protected vector m_VehicleAnchor; // world pos (estimated bumper)
    protected float m_TotalPulled;
    protected float m_LastConnectTime;

    // client-only visuals
    protected Shape m_RopeShape;

    void HandCrankWinch()
    {
        m_IsConnected = false;
        m_IsPulling = false;
        m_TotalPulled = 0;
        m_LastConnectTime = 0;
        SetEventMask(EntityEvent.FRAME);
    }

    override bool IsDeployable()
    {
        return true;
    }

    bool IsConnected()
    {
        return m_IsConnected && m_TargetVehicle;
    }

    CarScript GetTarget() { return m_TargetVehicle; }

    // -------- Connection / anchors --------
    protected vector EstimateVehicleAnchor(CarScript car)
    {
        // Try memory points commonly used by community car mods
        vector p;
        if (car.MemoryPointExists("Front_hook"))
            p = car.ModelToWorld(car.GetMemoryPointPos("Front_hook"));
        else if (car.MemoryPointExists("front_hook"))
            p = car.ModelToWorld(car.GetMemoryPointPos("front_hook"));
        else if (car.MemoryPointExists("hook_front"))
            p = car.ModelToWorld(car.GetMemoryPointPos("hook_front"));
        else
        {
            // Fallback: pick front or rear depending on where winch is
            vector toWinch = GetPosition() - car.GetPosition();
            vector fwd = car.GetDirection();
            float frontDot = vector.Dot(toWinch, fwd);
            vector anchorLocal = (frontDot > 0) ? "0 0 2" : "0 0 -2"; // ~2m to bumper
            p = car.ModelToWorld(anchorLocal);
        }
        return p;
    }

    void ServerConnectToNearestVehicle(PlayerBase player)
    {
        if (!GetGame().IsServer()) return;
        float now = GetGame().GetTime() * 0.001;
        if (now - m_LastConnectTime < WinchConstants.CONNECT_COOLDOWN) return;
        m_LastConnectTime = now;

        // Find nearest vehicle
        array<Object> results = new array<Object>();
        array<CargoBase> proxy = new array<CargoBase>();
        GetGame().GetObjectsAtPosition(GetPosition(), WinchConstants.ATTACH_RADIUS, results, proxy);

        CarScript best;
        float bestDist = 99999;
        foreach (Object o : results)
        {
            CarScript c;
            if (Class.CastTo(c, o))
            {
                float d = vector.Distance(GetPosition(), c.GetPosition());
                if (d < bestDist)
                {
                    best = c;
                    bestDist = d;
                }
            }
        }

        if (best)
        {
            m_TargetVehicle = best;
            m_WinchAnchor = GetPosition();
            m_VehicleAnchor = EstimateVehicleAnchor(best);
            m_IsConnected = true;
            m_TotalPulled = 0;

            // TODO: Play vanilla click sound here or custom soundset if configured
        }
    }

    void ServerDisconnect(PlayerBase player)
    {
        if (!GetGame().IsServer()) return;
        m_IsPulling = false;
        m_IsConnected = false;
        m_TargetVehicle = null;
        m_TotalPulled = 0;
        // TODO: Play disconnect click
    }

    void ServerStartPull()
    {
        if (!GetGame().IsServer()) return;
        if (!IsConnected()) return;
        m_IsPulling = true;
    }

    void ServerStopPull()
    {
        if (!GetGame().IsServer()) return;
        m_IsPulling = false;
    }

    // Server tick: move vehicle gently toward the winch
    override void EOnFrame(IEntity other, float timeSlice)
    {
        if (GetGame() && GetGame().IsServer())
        {
            if (m_IsPulling && m_TargetVehicle)
            {
                vector carPos = m_TargetVehicle.GetPosition();
                vector dir = m_WinchAnchor - carPos;
                float dist = dir.Length();
                if (dist > 0.05)
                {
                    dir.Normalize();
                    vector step = dir * WinchConstants.PULL_STEP;
                    vector newPos = carPos + step;

                    m_TargetVehicle.SetPosition(newPos);
                    m_TotalPulled += step.Length();

                    if (m_TotalPulled > WinchConstants.MAX_PULL_DISTANCE || dist < 0.5)
                    {
                        m_IsPulling = false;
                    }
                }
            }
        }

        // Client rope visual
        if (GetGame() && GetGame().IsClient())
        {
            UpdateRopeVisual();
        }
    }

    // Clean up visuals
    protected void DestroyRope()
    {
        if (m_RopeShape)
        {
            m_RopeShape.Destroy();
            m_RopeShape = null;
        }
    }

    protected void UpdateRopeVisual()
    {
        if (!IsConnected())
        {
            DestroyRope();
            return;
        }

        vector a = m_WinchAnchor;
        vector b;
        if (m_TargetVehicle)
        {
            b = EstimateVehicleAnchor(m_TargetVehicle);
        }
        else
        {
            b = m_VehicleAnchor;
        }

        if (!m_RopeShape)
        {
            m_RopeShape = Shape.CreateLines(ShapeFlags.VISIBLE, -1);
        }
        float pts[6];
        pts[0] = a[0]; pts[1] = a[1] + 0.3; pts[2] = a[2];
        pts[3] = b[0]; pts[4] = b[1] + 0.3; pts[5] = b[2];
        m_RopeShape.SetPosition(a);
        m_RopeShape.SetVector(0, b - a);
        m_RopeShape.SetScale(1.0);
        // Simple two-point line:
        m_RopeShape.CreateLines(pts, 2);
    }

    override void EEDelete(EntityAI parent)
    {
        super.EEDelete(parent);
        DestroyRope();
    }

    // Allow pickup only when idle (not connected/pulling)
    override bool CanPutInCargo( EntityAI parent )
    {
        return !m_IsPulling && !m_IsConnected && super.CanPutInCargo(parent);
    }

    override bool CanPutIntoHands(EntityAI parent)
    {
        return !m_IsPulling && !m_IsConnected && super.CanPutIntoHands(parent);
    }
};
