#include "AiEngine.h"
#include <iostream>
#include <list>
#include <string>
#include <stdlib.h>
#include <time.h>

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

////ai event////
aiEvent::aiEvent(aiPerson* now, std::vector<aiPerson*> npcList,ai_time aitime)
{
	//make sure that two npc has two different events between them
	personA = now;
	aiList = npcList;

	int randNum = rand()%aiSize;
	while (randNum == personA->idNum)
		randNum = rand()%aiSize;
	personB = aiList[randNum];
	time = aitime;
}

Events* aiEvent::generateEvent(std::vector<Events*> eventList, int k)
{
	Events* outputEvents = new Events;
	outputEvents->A = personA;	
	int randEvent = rand()%19;
	outputEvents->aievents = (ai_event)randEvent;
	outputEvents->aitime = time;
	outputEvents->possibility = 85;
	outputEvents->checked = false;

	label:
	if (personA->full[k] == NULL && personB->full[k] == NULL)
	{
		outputEvents->B = personB;	

		int i = 0;
		int sum = 0;
		while (i < Math().min(personA->idNum,personB->idNum))
			sum += (aiSize-i-1);
		sum += Math().abs(personA->idNum,personB->idNum) - 1 + aiSize;
		outputEvents->id = sum;
		outputEvents->relationId = sum;
		//get relation id number, event id number and assign it to the event struct for future use
		personB->full[k] = personA->idNum * 8 +k;
		personA->full[k] = personA->idNum * 8 +k;
		return outputEvents;
	}
	else if (personA->full[k] != NULL )
	{

		//Events eventB = *eventList[personA->full[k]];
		//aiPerson* temp = eventB.A;
		//eventB.A = eventB.B;
		//eventB.B = temp;
		return eventList[personA->full[k]];
	}
	else if (personB->full[k] != NULL &&  personA->full[k] == NULL)
	{
		while (personB->idNum == personA->idNum)
		{
		int randNum = rand()%aiSize;
		while (randNum == personA->idNum)
			randNum = rand()%aiSize;
		personB = aiList[randNum];
		}
		goto label;
	}
}

////ai relation////
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

	int randRelation = rand()%7;
	outputRelation->relations = (ai_relation)randRelation;
	outputRelation->heart = randRelation * 20;
	
	//assign an id to the relation
	//there is no repition in counting the relaiton between A and B
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

aiSchedule::aiSchedule(aiPerson* personA,std::vector<aiPerson*> npcList, std::vector<Events*> eList)
{
	aiSchedules->person = personA;
	aiList = npcList;
	eventList = eList;
}

Schedule* aiSchedule::generateSchedulePre()
{
	for (int i = 0; i < 8; i++)
	{
		aiSchedules->eventList.push_back(eventList[aiSchedules->person->idNum * 8 + i]);
	}
	return aiSchedules;
}

Schedule* aiSchedule::generateScheduleAft()
{
	int j = 0;
	int& buff = aiSchedules->person->debuff;
	while (j < 8)
	{
		for (int i = 0; i < 8; i++)
		{
			generatePersonalityEffect(i);
			generateMoodEffect(i);
			generateLuckEffect(i);
			generateRelationEffect(i);
			generateBuffEffect(i);
		}
		int possibility = rand()%100 + 1;
		if (possibility >= aiSchedules->eventList[j]->possibility && aiSchedules->eventList[j]->checked == false)
		{
			aiSchedules->eventList[j]->completed = true;
			aiSchedules->eventList[j]->checked = true;
			aiList[aiSchedules->eventList[j]->B->idNum]->personSchedule->eventList[j]->completed = true;
			aiList[aiSchedules->eventList[j]->B->idNum]->personSchedule->eventList[j]->checked = true;
			if (aiSchedules->person->aiMood != CRAZY)
				aiSchedules->person->aiMood = ai_mood(aiSchedules->person->aiMood + 1);
			return aiSchedules;
		}
		else if (possibility < aiSchedules->eventList[j]->possibility && aiSchedules->eventList[j]->checked == false)
		{
			aiSchedules->eventList[j]->completed = false;
			aiSchedules->eventList[j]->checked = true;
			aiList[aiSchedules->eventList[j]->B->idNum]->personSchedule->eventList[j]->completed = false;
				aiList[aiSchedules->eventList[j]->B->idNum]->personSchedule->eventList[j]->checked = true;
			if (aiSchedules->person->aiMood >= UNHAPPY)
				aiSchedules->person->aiMood = ai_mood(aiSchedules->person->aiMood - 1);
			if (aiSchedules->eventList[j]->aievents <= WORK)  //add debuff procastinating, showing that the sim's previous work is not done
				buff = buff|PROCASTINATING;
			if (aiSchedules->eventList[j]->aievents <= DINNER && aiSchedules->eventList[j]->aievents >= BREAKFAST)
				buff = buff|HUNGRY;
			if (aiSchedules->eventList[j]->aievents == REST)
				buff = buff|SLEEPY;
			return aiSchedules;
			///////////////////////
			//add more codes here//
			///////////////////////
		}
		j++;
	}
}

void aiSchedule::generatePersonalityEffect(int stages)
{
	int personality = aiSchedules->person->aiPersonality;
	int& possibility = aiSchedules->eventList[stages]->possibility;
	ai_event event = aiSchedules->eventList[stages]->aievents;

	if (personality & HARDWORKING  ||  personality & OPTIMISTIC)
	{
		if (event == WORK || event == MEETING || event == CODING  || event == PREPAREMEAL || event == EXPERIMENT)
			possibility *= 0.95;
	}
	if (personality & PESSIMISTIC)
	{
		if (event == DESIGN || event == EXPERIMENT)
			possibility *= 0.8;
	}
	if (personality & LAZY)
	{
		if (event == WORK || event == MEETING || event == CODING  || event == PREPAREMEAL || event == OUTING)
			possibility  *= 0.8;
	}
	if (personality & KIND || personality & TENDER)
	{
		if (event == FIGHTING || event == THEFT || event == ROBBERY || event == ARGUE)
				possibility  *= 0.8;
		else
		{
			possibility  *= 1.2;
		if (possibility  >= 100)
			possibility  = 100;
		}
	}
	if (personality & RASH || personality & HOSTILE)
	{
		if (event == FIGHTING || event == THEFT || event == ROBBERY || event == ARGUE)
		{
			possibility *= 1.2;
			if (possibility >= 100)
				possibility = 100;
		}
		if (event == WORK || event == MEETING || event == CODING  || event == PREPAREMEAL || event == OUTING)
			possibility *= 0.75;
	}
	else
		possibility *= 0.90;
}

void aiSchedule::generateRelationEffect(int stages)
{
	Relation* npcRelation = aiSchedules->person->relations[aiSchedules->eventList[stages]->relationId]; 
	int& possibility = aiSchedules->eventList[stages]->possibility;
	ai_event event = aiSchedules->eventList[stages]->aievents;

	if(npcRelation->relations > STRANGER)  //relation is better than acquaintance
	{
		possibility *= (1+0.025*(npcRelation->relations-3));
		if (possibility >= 100)
			possibility = 100;
	}
	else if (npcRelation->relations == STRANGER)  //relation is stranger
	{
		if (rand()%2 == 0)
		{
			possibility *= (1+0.025*(npcRelation->relations-3));
			npcRelation->relations = ACQUAINTANCE;  //the relation is enhanced
			npcRelation->heart = 20 * npcRelation->relations;   //the numerical value of relation (heart) increases
		}
		else
		{
			aiSchedules->person->aiMood = UNHAPPY;   //personality changed
			possibility *= (1+0.05*(npcRelation->relations-3));
			if (rand()%2 == 0)
			{
				npcRelation->relations = (ai_relation)(npcRelation->relations - 1);  //the relationship degrades to HOSITILITY
				npcRelation->heart = 20 * npcRelation->relations; 
			}
		}
	}
	else   //relation is worse than stranger
	{
		if (event != FIGHTING || event != ARGUE)
		{
			if (rand()%5 == 0)     //there is a 20% possibility that the coefficent wont decrease rapidly
			{
				possibility *= (1+0.05*(npcRelation->relations-3));
				npcRelation->relations = (ai_relation)(npcRelation->relations + 1);  //the relation is enhanced
				npcRelation->heart = 20 * npcRelation->relations;   //the numerical value of relation (heart) increases
			}
			else
			{
				aiSchedules->person->aiMood = ANGRY;   //mood changed
				possibility *= (1+0.75*(npcRelation->relations-3));
				if (rand()%3 == 0 && (npcRelation->relations != ENEMY))
				{
					npcRelation->relations = (ai_relation)(npcRelation->relations - 1);  //the relationship degrades to HOSITILITY
					npcRelation->heart = 20 * npcRelation->relations; 
				}
			}
		}
		else  //if the event is fighting or arguing
		{
			aiSchedules->person->aiMood = OUTRAGEOUS;   //mood changed
			possibility *= (1 - 0.1*(npcRelation->relations-3));
			if (possibility >= 100)
				possibility = 100;
			if (rand()%3 == 0 && (npcRelation->relations != ENEMY))
			{
				npcRelation->relations = (ai_relation)(npcRelation->relations - 1);  //the relationship degrades to HOSITILITY
				npcRelation->heart = 20 * npcRelation->relations; 
			}
		}

	}
}

void aiSchedule::generateMoodEffect(int stages)
{
	ai_mood &npcMood = aiSchedules->person->aiMood;
	int& possibility = aiSchedules->eventList[stages]->possibility;
	ai_event event = aiSchedules->eventList[stages]->aievents;

	if (npcMood < COMPOSED && npcMood > DESPARATE)   //0.05
		possibility *= (1 + 0.05*(npcMood-8));
	if (npcMood >= COMPOSED && npcMood < PROUD)   //0.025
	{
		possibility *= (1 + 0.025*(npcMood-8));
		if (possibility >= 100)
			possibility = 100;
	}
	if (npcMood == OUTRAGEOUS|| npcMood == ANGRY || npcMood == DESPARATE)   //0.05
	{
		if (event == FIGHTING || event == FIRED || event == THEFT || event == ROBBERY)
		{
			possibility *= (1 - 0.05*(npcMood-8));
			if (possibility >= 100)
				possibility = 100;
		}
		else
			possibility *= (1 + 0.05*(npcMood-8));
	}
	if (npcMood == PROUD || npcMood == CRAZY)
	{
		if (event == DESIGN || event == CODING || event == EXPERIMENT) //0.075
		{
			possibility *= (1 - 0.075*(npcMood-8));
			if (possibility >= 100)
				possibility = 100;
		}
		else
			possibility *= (1 + 0.075*(npcMood-8));
	}

}

void aiSchedule::generateLuckEffect(int stages)
{
}

void aiSchedule::generateBuffEffect(int stages)
{
	int& buff = aiSchedules->person->debuff;
	int& possibility = aiSchedules->eventList[stages]->possibility;
	ai_event event = aiSchedules->eventList[stages]->aievents;

	if (aiSchedules->eventList[stages]->aitime > SIXPM && event <= WORK)  //if it is already 8pm and sim is still working, he will have SLEEPY buff
		buff = buff|SLEEPY;

	if (buff & PROCASTINATING)
	{
		if (rand()%3 != 0)    //33% possibility for the sim to fail his next event,cuz he has to make up for the previous
			possibility *= 0.5;
		else   //the PROCASTINATING buff is cleared and the sim has a higher possibility to fullfill its objective,yet the sim will be tired
		{
			buff -= PROCASTINATING;
			buff = buff|OVERWORKED;
			possibility *= 0.75;
		}
	}
	if (buff & HUNGRY)
	{
		if (event <= WORK)
			possibility *= (1+(event-9)*0.025);
		else if (event == PREPAREMEAL)
		{
			possibility *= 1.5;
			if (possibility > 100)
				possibility = 100;
		}
		else if(event >= BREAKFAST && event <= DINNER)
		{
			possibility *= 1.5;
			if (possibility > 100)
				possibility = 100;
			buff -= HUNGRY;
		}
	}
	if (buff & OVERWORKED)
	{
		if (event == REST)
		{
			possibility *= 1.5;
			if (possibility > 100)
				possibility = 100;
			buff -= OVERWORKED;
		}
		else if (event <= WORK)
		{
			possibility *= (1+(event-9)*0.025);
			buff = buff|TIRED;
		}
	}
	if (buff & TIRED)
	{
		if (event == REST)
		{
			possibility *= 1.5;
			if (possibility > 100)
				possibility = 100;
			buff -= TIRED;
		}
		else if(event >= BREAKFAST && event <= DINNER)
		{
			possibility *= 1.25;
			if (possibility > 100)
				possibility = 100;
			buff -= TIRED;
		}
		else
			possibility *= 0.75;
	}
	if (buff & SLEEPY)
	{
		if (event == REST)
		{
			possibility *= 2;
			if (possibility > 100)
				possibility = 100;
			buff -= SLEEPY;
		}
		else
			possibility *= 0.875;
	}

	//////////////////////
	//ILL remains uneditted
	if (buff & ILL)
	{
		possibility *= 0.5;
	}
}




aiPerson::aiPerson(double x, double y, std::string name)
{
	//lack vector2D class
	this->name = name;
	debuff = NONE;
	occupation = (ai_occupation)(rand()%12+1);
	
	//a simple method to assign 0-3 personality to one person
	//there wont be any repetition
	int i = 0;
	while (i < 3)
	{
		if (rand()%3 != 0)
			aiPersonality = aiPersonality|(ai_personality)(rand()%4 + 2*i);
		i++;
	}
	//initialize aiMood
	aiMood = COMPOSED;
	full =  new int[8];

}

void aiPerson::getSchedulePre(std::vector<aiPerson*> npc,std::vector<Events*> eventList)
{
	aiSchedule schedule = aiSchedule(this,npc,eventList);
	personSchedule = schedule.generateSchedulePre();
}

void aiPerson::getScheduleAft(std::vector<aiPerson*> npc,std::vector<Events*> eventList)
{
	aiSchedule schedule = aiSchedule(this,npc,eventList);
	personSchedule = schedule.generateScheduleAft();
}

void aiPerson::getRelation(std::vector<aiPerson*> npc)
{
	int i = 0;
	while (i < npc.size())
	{
		//the 'if' statement make sure that for each pair of person, there is only one relation
		if(this->idNum < i)
			relations.push_back(aiRelation(this,npc[i]).generateRelation());
		i++;
	}
}


AiEngine::AiEngine(void)
{


}

void AiEngine::getAllEvents()
{
	for (int i = 0; i < aiSize; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			Events* eventss =aiEvent(aiList[i],aiList,(ai_time)j).generateEvent(eventList,j);
			eventList.push_back(eventss);
			aiList[i]->full[i] = true;  
		}
	}
}

void AiEngine::initialize()
{
	int i = 0;
	aiList = std::vector<aiPerson*>(aiSize);
	while (i < aiSize)
	{
		aiList[i]->name = (ai_name)(rand()%8);
		aiList[i]->idNum = i;
	}
	
	getAllEvents();    //generate all events
					   //for every event there are two copies (repetitions)

	for (std::vector<aiPerson*>::iterator it = aiList.begin(); it != aiList.end(); it++)
	{
		(*it)->getRelation(aiList);
		(*it)->getSchedulePre(aiList,eventList);

		scheduleListPre.push_back((*it)->personSchedule);
		relationList.push_back((*it)->relations);
	}
}

void AiEngine::simulate()
{
	while (true)
	{
		for (std::vector<aiPerson*>::iterator it = aiList.begin(); it != aiList.end(); it++)
		{
			(*it)->getScheduleAft(aiList,eventList);

			scheduleListAft.push_back((*it)->personSchedule);
			relationList.push_back((*it)->relations);
		}
	}
}

void AiEngine::end()
{
}

AiEngine::~AiEngine(void)
{

}