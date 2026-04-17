case 2312:
      CRunningScript::CollectParameters(this, 2u);
      LODWORD(CPool<CObject>::AtHandle(CPools::ms_pObjectPool, ScriptParams[0])->physical.m_fTurnMass) = ScriptParams[1];
      return 0;