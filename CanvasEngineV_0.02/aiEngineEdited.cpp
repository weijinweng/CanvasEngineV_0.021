#include "AiEngineEdited.h"
#include <iostream>
#include <list>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <vector>

int Math::min(int a, int b)
{
	if (a < b)
		return a;
	else
		return b;
}

int Math::max(int a, int b)
{
	if (a > b)
		return a;
	else
		return b;
}

int Math::abs(int a , int b)
{
		if (a > b)
		return (a-b);
	else
		return (b-a);
}

//////////////aiRealation/////////////////////

aiRelation::aiRelation(aiPerson* A, aiPerson* B)
{
	personA = A;
	personB = B;
}

Relation* aiRelation::generateRelation()
{
	Relation* outputRelation = new Relation;
	outputRelation->A = personA;
	outputRelation->B = personB;

	int randNum = rand()%6;
	outputRelation->relations = (ai_relation)randNum;
	randNum = rand()%7;
	outputRelation->friendship = (ai_friendship)randNum;
	outputRelation->heart = randNum * 20;
	
	int i = 0;
	int sum = 0;
	while (i < Math().min(personA->idNum,personB->idNum))
		sum += (aiSize-i-1);
	sum += Math().abs(personA->idNum,personB->idNum) - 1 + aiSize;
	outputRelation->id = sum;

	//print out
	std::cout << personA->name << " and " << personA->name <<" have a relation: " << outputRelation->relations;
	return outputRelation;
}

///////////////aiEvents///////////////////////

aiEvents::aiEvents(aiPerson* A,std::vector<aiPerson*> aiLists, ai_time time)
{
	aiList = aiLists;
	eventTime = time;
	personA = A;
}

aiEvents* aiEvents::generateEvent()
{
	if (personA->dailyEvents[eventTime] != NULL)
	{
		eventType = ai_schedule1[eventTime];

		if (eventType == EATING)
		{
			Eating eating = Eating(personA,aiList, eventTime);
			
			for (std::vector<aiPerson*>::iterator it = aiInvolved.begin(); it != aiInvolved.end(); it++)
			{
				(*it)->dailyEvents[eventTime] = &eating;
				eating.behave();
			}
			eating.possibility = aiInvolved.size * 100;
			return &eating;
		}
		else if (eventType == WORKING)
		{
			Working working = Working(personA,aiList, eventTime);

			for (std::vector<aiPerson*>::iterator it = aiInvolved.begin(); it != aiInvolved.end(); it++)
			{
				(*it)->dailyEvents[eventTime] = &working;
				working.behave();
			}
			working.possibility = aiInvolved.size * 100;
			return &working;
		}
		else if (eventType == RESTING)
		{
			Resting resting = Resting(personA,aiList, eventTime);

			for (std::vector<aiPerson*>::iterator it = aiInvolved.begin(); it != aiInvolved.end(); it++)
			{
				(*it)->dailyEvents[eventTime] = &resting;
				resting.behave();
			}
			resting.possibility = aiInvolved.size * 100;
			return &resting;
		}
		else if (eventType == FIGHTING)
		{
			Fighting fighting = Fighting(personA,aiList, eventTime);
			
			for (std::vector<aiPerson*>::iterator it = aiInvolved.begin(); it != aiInvolved.end(); it++)
			{
				(*it)->dailyEvents[eventTime] = &fighting;
				fighting.behave();
			}
			fighting.possibility = aiInvolved.size * 100;
			return &fighting;
		}
		else if (eventType == OTHER)
		{
			Other other = Other(personA,aiList, eventTime);

			for (std::vector<aiPerson*>::iterator it = aiInvolved.begin(); it != aiInvolved.end(); it++)
			{
				(*it)->dailyEvents[eventTime] = &other;
				other.behave();
			}
			other.possibility = aiInvolved.size * 100;
			return &other;
		}
	}
}

//////////////children classes////////////////////////

Working::Working(aiPerson* A,std::vector<aiPerson*> aiLists, ai_time time):aiEvents(A,aiLists,time)
{
	workingType = (ai_working)(rand()%5);
	for (std::vector<Relation*>::iterator it = personA->relationList.begin(); it != personA->relationList.end(); it++)
	{
		if ((*it)->relations == COLLEGUE)
		{
			if ((*it)->A == personA)
				collegue.push_back((*it)->B);
			else if ((*it)->B == personA)
				collegue.push_back((*it)->A);
		}
	}
	switch(workingType)
	{
	case(MEETING):
		aiInvolved = collegue;
		aiInvolved.push_back(personA);
		break;
	case(MANUFACTURING):
		int randNum = rand()%collegue.size();
		aiInvolved.push_back(collegue[randNum]);
		aiInvolved.push_back(personA);
		break;
	case(GROUPWORKING):
		aiInvolved = collegue;
		aiInvolved.push_back(personA);
		break;
	case(EXPERIMENTING):
		int randNum = rand()%collegue.size();
		aiInvolved.push_back(collegue[randNum]);
		aiInvolved.push_back(personA);
		break;
	case(CODING):
		aiInvolved.push_back(personA);
		break;
	}
}

void Working::behave()
{
}

Eating::Eating(aiPerson* A,std::vector<aiPerson*> aiLists, ai_time time):aiEvents(A,aiLists,time)
{
	switch (eventTime)
	{
	case(EIGHTAM): eatingType = BREAKFAST;
		break;
	case(TWELVEAM): eatingType = LUNCH;
		break;
	case(SIXPM):eatingType = SUPPER;
		break;
	default:eatingType = SNACK;
		break;
	}

	for (std::vector<Relation*>::iterator it = personA->relationList.begin(); it != personA->relationList.end(); it++)
	{
		if ((*it)->relations == FAMILY && (eatingType == BREAKFAST || eatingType == SUPPER))
		{
			if ((*it)->A == personA)
				collegue.push_back((*it)->B);
			else if ((*it)->B == personA)
				collegue.push_back((*it)->A);
		}
		else if ((*it)->relations == COLLEGUE && eatingType == LUNCH)
		{
			if ((*it)->A == personA)
				collegue.push_back((*it)->B);
			else if ((*it)->B == personA)
				collegue.push_back((*it)->A);
		}
		else if (((*it)->relations == COLLEGUE || (*it)->relations == FRIENDS) && eatingType == SNACK)
		{
			if ((*it)->A == personA)
				collegue.push_back((*it)->B);
			else if ((*it)->B == personA)
				collegue.push_back((*it)->A);
		}
	}

	switch(eatingType)
	{
	case(BREAKFAST):
		aiInvolved = collegue;
		aiInvolved.push_back(personA);
		break;
	case(LUNCH):
		int randNum = rand()%collegue.size();
		aiInvolved.push_back(collegue[randNum]);
		aiInvolved.push_back(personA);
		break;
	case(SUPPER):
		aiInvolved = collegue;
		aiInvolved.push_back(personA);
		break;
	case(SNACK):
		int randNum = rand()%collegue.size();
		aiInvolved.push_back(collegue[randNum]);
		aiInvolved.push_back(personA);
		break;
	}
}

void Eating::behave()
{
}

Fighting::Fighting(aiPerson* A,std::vector<aiPerson*> aiLists, ai_time time):aiEvents(A,aiLists,time)
{
	switch (eventTime)
	{
	case(EIGHTAM): fightingType = ARGUE;
		break;
	case(SIXPM):fightingType = TEAMFIGHT;
		break;
	case(EIGHTPM):fightingType = ARGUE;
		break;
	case(TENPM):fightingType = ARGUE;
		break;
	default:fightingType = ARGUE;
		break;
	}

	for (std::vector<Relation*>::iterator it = personA->relationList.begin(); it != personA->relationList.end(); it++)
	{
		if ((*it)->relations == FAMILY && (fightingType == ARGUE))
		{
			if ((*it)->A == personA)
				collegue.push_back((*it)->B);
			else if ((*it)->B == personA)
				collegue.push_back((*it)->A);
		}
		else if ((*it)->relations == COLLEGUE && fightingType == DEBATE)
		{
			if ((*it)->A == personA)
				collegue.push_back((*it)->B);
			else if ((*it)->B == personA)
				collegue.push_back((*it)->A);
		}
		else if (((*it)->relations == FRIENDS || (*it)->relations == OPPONENT) && fightingType == TEAMFIGHT)
		{
			if ((*it)->A == personA)
				collegue.push_back((*it)->B);
			else if ((*it)->B == personA)
				collegue.push_back((*it)->A);
		}
		else if ((*it)->relations == OPPONENT && fightingType == DUEL)
		{
			if ((*it)->A == personA)
				collegue.push_back((*it)->B);
			else if ((*it)->B == personA)
				collegue.push_back((*it)->A);
		}
	}

	switch(fightingType)
	{
	case(TEAMFIGHT):
		aiInvolved = collegue;
		aiInvolved.push_back(personA);
		break;
	case(DEBATE):
		int randNum = rand()%collegue.size();
		aiInvolved.push_back(collegue[randNum]);
		aiInvolved.push_back(personA);
		break;
	case(DUEL):
		aiInvolved = collegue;
		aiInvolved.push_back(personA);
		break;
	case(ARGUE):
		int randNum = rand()%collegue.size();
		aiInvolved.push_back(collegue[randNum]);
		aiInvolved.push_back(personA);
		break;
	}
}

void Fighting::behave()
{
}

Resting::Resting(aiPerson* A,std::vector<aiPerson*> aiLists, ai_time time):aiEvents(A,aiLists,time)
{
	switch (eventTime)
	{
	case(EIGHTAM): restingType = OVERSLEEP;
		break;
	case(SIXPM):restingType = DRINKING;
		break;
	case(EIGHTPM):restingType = GAME;
		break;
	case(TENPM):restingType = SLEEP;
		break;
	default:restingType = NAP;
		break;
	}

	for (std::vector<Relation*>::iterator it = personA->relationList.begin(); it != personA->relationList.end(); it++)
	{
		if (((*it)->relations == FRIENDS || (*it)->relations == FAMILY) && (restingType == DRINKING || restingType == GAME))
		{
			if ((*it)->A == personA)
				collegue.push_back((*it)->B);
			else if ((*it)->B == personA)
				collegue.push_back((*it)->A);
		}
		else if (eventType == NAP || restingType == OVERSLEEP || restingType == SLEEP)
		{
			collegue.push_back(personA);
		}
	}

	switch(restingType)
	{
	case(NAP):
		aiInvolved = collegue;
		break;
	case(SLEEP):
		aiInvolved = collegue;
		break;
	case(OVERSLEEP):
		aiInvolved = collegue;
		break;
	case(GAME):
		int randNum = rand()%collegue.size();
		aiInvolved.push_back(collegue[randNum]);
		aiInvolved.push_back(personA);
		break;
	case(DRINKING):
		int randNum = rand()%collegue.size();
		aiInvolved.push_back(collegue[randNum]);
		aiInvolved.push_back(personA);
		break;
	}
}

void Resting::behave()
{
}

Other::Other(aiPerson* A,std::vector<aiPerson*> aiLists, ai_time time):aiEvents(A,aiLists,time)
{
}

void Other::behave()
{
}

////////////////aiEvents effect generator//////////////////////

double aiEvents::generateEffect(int stages)
{
	double possibilities = 0;
	possibilities += generatePersonalityEffect(stages); 
	possibilities += generateFriendshipEffect(stages);    
	possibilities += generateLuckEffect(stages);
	possibilities += generateMoodEffect(stages);
	possibilities += generateBuffEffect(stages);
	return possibilities;
}

double aiEvents::generatePersonalityEffect(int stages)
{
	double possibilities = 0;

	for (std::vector<aiPerson*>::iterator it = aiInvolved.begin(); it != aiInvolved.end(); it++)
	{
		int personality = (*it)->aiPersonality;
		//ai_event eventType = personA->dailyEvents[stages]->eventType;
		double possibility = 100;

		////////////////////////////////////add mood intot he personality effect generator/////////

		if (personality & HARDWORKING  ||  personality & OPTIMISTIC)
		{
			if (eventType == WORKING)
				possibility *= 1.25;
		}
		if (personality & PESSIMISTIC)
		{
			if (eventType == WORKING)
				possibility *= 0.75;
		}
		if (personality & LAZY)
		{
			if (eventType == WORKING)
				possibility  *= 0.75;
			else if (eventType == RESTING)
				possibility *= 1.28;
		}
		if (personality & KIND || personality & TENDER)
		{
			if (eventType == FIGHTING)
				possibility  *= 0.75;
			else
				possibility *= 1.125;
		}
		if (personality & RASH || personality & HOSTILE)
		{
			if (eventType == FIGHTING)
				possibility *= 1.25;
			else if (eventType == WORKING || eventType == RESTING)
				possibility *= 0.875;
		}
		else
			possibility *= 0.90;
		possibilities += possibility;
	}
	return possibilities/aiInvolved.size;
}

double aiEvents::generateFriendshipEffect(int stages)
{
	int count = 0;
	double possibilities = 0;
	for (std::vector<aiPerson*>::iterator it = aiInvolved.begin(); it != aiInvolved.end(); it++)
	{
		for (std::vector<aiPerson*>::iterator iter = (it+1); iter != aiInvolved.end(); iter++)
		{
			ai_friendship friendship;
			
			for (std::vector<Relation*>::iterator itRelation = (*it)->relationList.begin(); itRelation != (*it)->relationList.end(); itRelation++)
			{
				if (((*itRelation)->A == (*it) && (*itRelation)->B == (*iter)) || ((*itRelation)->B == (*it) && (*itRelation)->A == (*iter)))
					friendship = (*itRelation)->friendship;
			}

			double possibility = 100;

			if(friendship > JUSTSOSO)  //relation is better than JUSTSOSO
			{
				if (eventType != FIGHTING)
					possibility *= (1+0.05*(friendship-3));
				else
					possibility *= (1-0.05*(friendship-3));
			}
			else if (friendship == JUSTSOSO)  //relation is stranger
			{
				if (rand()%2 == 0)
				{
					friendship = NOTBAD;  //the relation is enhanced
					//npcRelation->heart = 20 * friendship;   
				}
				else
				{
					(*it)->aiMood = UNHAPPY;   //mood changed
					possibility *= (1+0.05*(friendship-3));
					if (rand()%2 == 0)
					{
						friendship = (ai_friendship)(friendship-1); 
						//npcRelation->heart = 20 * friendship; 
					}
				}
			}
			else   //relation is worse than JUSTSOSO
			{
				if (eventType != FIGHTING)
				{
					if (rand()%5 == 0)     //there is a 20% possibility that the coefficent wont decrease rapidly
					{
						possibility *= (1+0.05*(friendship-3));
						friendship = (ai_friendship)(friendship+1);
						//npcRelation->heart = 20 * npcRelation->relations; 
					}
					else
					{
						(*it)->aiMood = ANGRY; 
						possibility *= (1+0.075*(friendship-3));

						if (rand()%3 == 0 && (friendship != OPPONENT))
						{
							friendship = (ai_friendship)(friendship-1);
							//npcRelation->heart = 20 * npcRelation->relations; 
						}
					}
				}
				else  //if the event is fighting
				{
					(*it)->aiMood = OUTRAGEOUS;   //mood changed
					possibility *= (1 - 0.05*(friendship-3));

					if (rand()%3 == 0 && (friendship != OPPONENT))
					{
						friendship = (ai_friendship)(friendship-1);
						//npcRelation->heart = 20 * npcRelation->relations; 
					}
				}
			}
			count++;
			possibilities += possibility;
		}
	}
	return possibilities/count;
}

double aiEvents::generateMoodEffect(int stages)
{
	ai_mood &npcMood = personA->aiMood;
	int possibility = 100;

	if (npcMood < COMPOSED && npcMood > DESPARATE)   //0.05
		possibility *= (1 + 0.05*(npcMood-8));
	if (npcMood >= COMPOSED && npcMood < CONFIDENT)   //0.025
		possibility *= (1 + 0.025*(npcMood-8));
	if (npcMood == OUTRAGEOUS|| npcMood == ANGRY || npcMood == DESPARATE)   //0.05
	{
		if (eventType == FIGHTING)
			possibility *= (1 - 0.05*(npcMood-8));
		else
			possibility *= (1 + 0.05*(npcMood-8));
	}
	if (npcMood == CRAZY || npcMood == CONFIDENT)
	{
		if (eventType == WORKING)
			possibility *= (1 - 0.075*(npcMood-8));
		else
			possibility *= (1 + 0.075*(npcMood-8));
	}
	return possibility;
}

double aiEvents::generateLuckEffect(int stages)
{
}

double aiEvents::generateBuffEffect(int stages)
{
}

/////////////////////aiPerson////////////////////////////////

aiPerson::aiPerson()
{
	this->name = ai_name(rand()%8);
	occupation = (ai_occupation)(rand()%12+1);
	debuff = NONE;

	int i = 0;
	while (i < 3)
	{
		if (rand()%3 != 0)
			aiPersonality = aiPersonality|(ai_personality)(rand()%4 + 2*i);
		i++;
	}

	aiMood = COMPOSED;
	full =  new int[8];
	
	dailyEvents = std::vector<aiEvents*>(8);
}

std::vector<Relation*> aiPerson::generateRelation(std::vector<aiPerson*> npc)
{
	int i = 0;
	while (i < aiSize)
	{
		if(this->idNum < i)
		{
			Relation* relation = (aiRelation(this,npc[i]).generateRelation());
			this->relationList.push_back(relation);
			npc[i]->relationList.push_back(relation);
		}
		i++;
	}
	return relationList;
}

Schedule* aiPerson::generateSchedule(std::vector<aiPerson*> npc)
{
	for(int i = 0; i <= TENPM; i++)
		aiEvents(this,npc,(ai_time)i);

	aiSchedule->eventList = dailyEvents;
	aiSchedule->id = this->idNum;
	aiSchedule->person = this;

	return aiSchedule;
}

////////////////////aiEngine//////////////////////////////////
aiEngine::aiEngine()
{
}

void aiEngine::initialize()
{
	ai_schedule1.push_back(EATING);
	ai_schedule1.push_back(WORKING);
	ai_schedule1.push_back(EATING);
	ai_schedule1.push_back(WORKING);
	ai_schedule1.push_back(WORKING);
	ai_schedule1.push_back(EATING);
	ai_schedule1.push_back(RESTING);
	ai_schedule1.push_back(RESTING);


	int i = 0;
	//aiList = std::vector<aiPerson*>(aiSize);
	while (i < aiSize)
	{
		aiPerson npcA = aiPerson();
		npcA.idNum = i;
		aiList.push_back(&npcA);
	}

	for (std::vector<aiPerson*>::iterator it = aiList.begin(); it != aiList.end(); it++)
		relationList.push_back((*it)->generateRelation(aiList));

	for (std::vector<aiPerson*>::iterator it = aiList.begin(); it != aiList.end(); it++)
		scheduleListPre.push_back((*it)->generateSchedule);

}
