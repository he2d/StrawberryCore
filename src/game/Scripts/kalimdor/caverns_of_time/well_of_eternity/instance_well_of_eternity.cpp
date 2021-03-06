/* Copyright (C) 2010 - 2012 Strawberry Scripts <http://www.strawberry-pr0jcts.com/>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* ScriptData
SDName: instance_well_of_eternity
SD%Complete: 80
SDComment: Does not support the Quests.
SDCategory: Caverns of Time, Well of Eternity
EndScriptData */

#include "pchdef.h"
#include "instance_well_of_eternity.h"

instance_well_of_eternity::instance_well_of_eternity(Map *pMap) : ScriptedInstance(pMap)
{
    Initialize();
}

void instance_well_of_eternity::Initialize()
{
    memset(&m_uiEncounter, 0, sizeof(m_uiEncounter));
}

void instance_well_of_eternity::Load(const char* chrIn)
{
    if (!chrIn)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(chrIn);

    std::istringstream loadStream(chrIn);
    loadStream >> m_uiEncounter[0] >> m_uiEncounter[1] >> m_uiEncounter[2] >> m_uiEncounter[3] >> m_uiEncounter[4];
    
    for(uint8 i = 0;i < MAX_ENCOUNTER;++i)
        if(m_uiEncounter[i] == IN_PROGRESS)
            m_uiEncounter[i] = NOT_STARTED;

    OUT_LOAD_INST_DATA_COMPLETE;
}

void instance_well_of_eternity::SetData(uint32 uiType, uint32 uiValue)
{
    m_uiEncounter[uiType] = uiValue;

    if(uiValue == DONE)
    {
        OUT_SAVE_INST_DATA; 

        std::ostringstream saveStream;
        saveStream << m_uiEncounter[0] << " " << m_uiEncounter[1] << " " << m_uiEncounter[2] << " " << m_uiEncounter[3] << " " << m_uiEncounter[4];
        m_chrInStr = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

uint32 instance_well_of_eternity::GetData(uint32 uiType)
{
    return m_uiEncounter[uiType];
}

void instance_well_of_eternity::OnCreatureCreate(Creature * creature)
{
    switch(creature->GetEntry())
    {
        case BOSS_PEROTH_ARN:		m_mNpcEntryGuidStore[BOSS_PEROTH_ARN] = creature->GetObjectGuid();       break;
        case BOSS_QUEEN_AZSHARA:        m_mNpcEntryGuidStore[BOSS_QUEEN_AZSHARA] = creature->GetObjectGuid();       break;
        case BOSS_CAPTAIN_VAROTHEN:     m_mNpcEntryGuidStore[BOSS_CAPTAIN_VAROTHEN] = creature->GetObjectGuid();    break;
        case BOSS_MANNOROTH:     m_mNpcEntryGuidStore[BOSS_MANNOROTH] = creature->GetObjectGuid();    break;
    }
}

void instance_well_of_eternity::OnCreatureEnterCombat(Creature * creature)
{
    switch(creature->GetEntry())
    {
        case BOSS_PEROTH_ARN:        SetData(DATA_PEROTH_ARN_EVENT,IN_PROGRESS);     break;
        case BOSS_QUEEN_AZSHARA:        SetData(DATA_QUEEN_AZSHARA_EVENT,IN_PROGRESS);     break;
        case BOSS_CAPTAIN_VAROTHEN:     SetData(DATA_CAPTAIN_VAROTHEN_EVENT,IN_PROGRESS);  break;
        case BOSS_MANNOROTH:     SetData(DATA_MANNOROTH_EVENT,IN_PROGRESS);       break;
    }
}

void instance_well_of_eternity::OnCreatureDeath(Creature * creature)
{
    switch(creature->GetEntry())
    {
        case BOSS_PEROTH_ARN:        SetData(DATA_PEROTH_ARN_EVENT,DONE);            break;
        case BOSS_QUEEN_AZSHARA:        SetData(DATA_QUEEN_AZSHARA_EVENT,DONE);            break;
        case BOSS_CAPTAIN_VAROTHEN:     SetData(DATA_CAPTAIN_VAROTHEN_EVENT,DONE);         break;
        case BOSS_MANNOROTH:     SetData(DATA_MANNOROTH_EVENT,DONE);              break;
    }
}

void instance_well_of_eternity::OnCreatureEvade(Creature * creature)
{
    switch(creature->GetEntry())
	{
        case BOSS_PEROTH_ARN:        SetData(DATA_PEROTH_ARN_EVENT,FAIL);            break;
        case BOSS_QUEEN_AZSHARA:        SetData(DATA_QUEEN_AZSHARA_EVENT,FAIL);            break;
        case BOSS_CAPTAIN_VAROTHEN:     SetData(DATA_CAPTAIN_VAROTHEN_EVENT,FAIL);            break;
        case BOSS_MANNOROTH:     SetData(DATA_MANNOROTH_EVENT,FAIL);            break;
    }

}

InstanceData * GetInstanceData_instance_well_of_eternity(Map* pMap)
{
    return new instance_well_of_eternity(pMap);
}

void AddSC_instance_well_of_eternity()
{
    Script * pScript;

    pScript = new Script;
    pScript->Name = "instance_well_of_eternity";
    pScript->GetInstanceData = &GetInstanceData_instance_well_of_eternity;
    pScript->RegisterSelf();
}