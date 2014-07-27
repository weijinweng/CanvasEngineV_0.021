#include "aiEngine2.h"
#include <iostream>
#include <list>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <fstream>

///////////////Math functions/////////////////
Math::Math()
{
}

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
	{
		sum += (aiSize-i-1);
		i++;
	}
	sum += Math().abs(personA->idNum,personB->idNum) - 1 + aiSize;
	outputRelation->id = sum;

	//print out
	//std::cout << outputRelation->A->idNum << " and " << outputRelation->B->idNum <<" have a relation: " << outputRelation->relations << "relation id is " << outputRelation->id << std::endl;
	return outputRelation;
}

///////////////aiEvents///////////////////////


//////////////aiPerson///////////////////////
aiPerson::aiPerson(int i, std::string name)
{
	this->idNum = i;
	this->name = name;
	debuff->ANGRY = 40;
	debuff->BEHINDSCHEDULE = 40;
	debuff->ILL = 40;
	debuff->OVERWORKED = 40;
	debuff->SLEEPY = 40;
	debuff->HUNGRY = 40;
}

std::vector<Relation*> aiPerson::generateRelation(std::vector<aiPerson*>& npc)
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

void aiPerson::generateEvent(std::vector<aiPerson*>& npc, aiTime time)
{
	if (currentEvent == NULL)
	{
		generateNewEvent(time);
	}
	else
	{
		if (needToBeAltered(time))  
		{
			if (currentEvent->expire)    //event expire
			{
				currentEvent->generateEffect();
				updateBuff();
				currentEvent->timeEnded = time;
				dailyEvents.push_back(currentEvent);
				generateNewEvent(time);
			}
			else
			{
				if (interruptiveEvent == NULL)       //no interruptive event
				{
					currentEvent->timeEnded = time;
					dailyEvents.push_back(currentEvent);
					currentEvent = NULL;   //the ai is idle
				}
				else               //have interruptive event
				{
					currentEvent->completed = false;
					debuff->ANGRY += 5;
					updateBuff();
					currentEvent->timeEnded = time;
					dailyEvents.push_back(currentEvent);
					currentEvent = interruptiveEvent;
				}
			}
		}
	}
}

void aiPerson::updateBuff()
{
	int size = dailyEvents.size();
	if (currentEvent->completed)
	{
		if (debuff->BEHINDSCHEDULE > 50 && currentEvent->eventType == WORKING)
		{
			debuff->BEHINDSCHEDULE -= 10;
			debuff->TIRED += 10;           //finish an event on time when PROCASTINATING casued TIRED
		}
		if (dailyEvents.size() > 0)
		{
			if (dailyEvents[size-1]->eventType == WORKING && currentEvent->eventType == WORKING)
			{
				debuff->OVERWORKED += 10;     // have working 2 in a row cause OVERWORKED
				debuff->ANGRY += 10;
			}
		}
		if (currentEvent->eventType == RESTING)
		{
			debuff->TIRED -= 10;             // clear TIRED if RESTING is completed
			debuff->ANGRY -= 5;
		}
		if (currentEvent->eventType == FIGHTING)
		{
			debuff->ILL += 10;          //hurt by FIGHTING
			debuff->ANGRY -= 10;
		}
		if (currentEvent->eventType == EATING)
			debuff->HUNGRY -= 10;
	}
	else
	{
		if (currentEvent->eventType == WORKING)
		{
			debuff->BEHINDSCHEDULE += 20;
			debuff->ANGRY +=10;
		}
		if (currentEvent->eventType == RESTING)
		{
			debuff->TIRED += 10;             
			debuff->ANGRY += 5;
		}
		if (currentEvent->eventType == FIGHTING)
		{
			debuff->ILL += 20;         
			debuff->ANGRY += 15;
		}
		if (currentEvent->eventType == EATING)
			debuff->HUNGRY += 10;
	}
	std::cout << "show Buff... " << std::endl;
	std::cout << "debuff behindschedule " << debuff->BEHINDSCHEDULE << std::endl;
	std::cout << "debuff Tired " << debuff->TIRED << std::endl;
	std::cout << "debuff Ill " << debuff->ILL << std::endl;
	std::cout << "debuff Hungry " << debuff->HUNGRY << std::endl;
}

bool aiPerson::needToBeAltered(aiTime time)
{
	aiTime subtraction;
	subtraction.day = time.day - currentEvent->timeStarted.day;
	subtraction.hour = time.hour - currentEvent->timeStarted.hour;
	subtraction.min = time.min - currentEvent->timeStarted.min;
	subtraction.sec = time.sec - currentEvent->timeStarted.sec;
	double subtractionSum = subtraction.day *24*60*60 + subtraction.hour*60*60 + subtraction.min*60 + subtraction.sec;
	double estimatedSum = currentEvent->estimatedLength.day *24*60*60 + currentEvent->estimatedLength.hour*60*60 + currentEvent->estimatedLength.min*60 + currentEvent->estimatedLength.sec;
	
	if (subtractionSum >= estimatedSum)
	{
		currentEvent->expire = true;
		return true;
	}
	else
	{
		currentEvent->expire = false;
		int randNum = 100*(estimatedSum - subtractionSum)/estimatedSum;
		if (rand() % 250 <= randNum)  //event doesnt need to be altered
			return false;
		else
			return true;
	}
}

void aiPerson::generateNewEvent(aiTime tiem)
{
	//check buff and determine which event is the next 
	//generate specific type of events
}

/////////////aiEngine////////////////////////
aiEngine::aiEngine()
{
	time->day = 0;
	time->hour = 6;
	time->min = 0;
	time->sec = 0;
}

bool aiEngine::initialize()
{
	std::map<int, std::string> aiName;
	aiName[0] = "ALEX";
	aiName[1] = "ALEXANDER";
	aiName[2] = "ARISTOTLE";
	aiName[3] = "BLACKBERRY";
	aiName[4] = "CAROLS";
	aiName[5] = "DUMMY";
	aiName[6] = "EAGLE";
	aiName[7] = "DRPEPPER";

	std::cout << "initialize begins..." <<std::endl;	

	//generate aiPerson
	int i = 0;
	while (i < aiSize)
	{
		aiPerson* personA = new aiPerson(i,aiName[rand() % 8]);
		aiList.push_back(personA);
		i++;
	}
	std::cout << "person generator ends" << std::endl;
	std::cout << "aiRelation generator begins..." << std::endl;
	
	//generate aiRelation
	for (std::vector<aiPerson*>::iterator it = aiList.begin(); it != aiList.end(); it++)
	{
		relationList.push_back((*it)->generateRelation(aiList));
	}
	std::cout << "aiRelation generator ended" << std::endl;

	return true;
}

bool aiEngine::simulate()
{
	std::cout << "simulate begine..." << std::endl;
	for (std::vector<aiPerson*>::iterator it = aiList.begin(); it != aiList.end(); it++)
	{
		std::cout << "simulate " << "person " << (*it)->idNum << " begins..." << std::endl;
		(*it)->generateEvent(aiList, *time);
	}
	return true;
}

bool aiEngine::end()
{
	return true;
}

void aiEngine::getTime()
{
	unsigned int overallTime = timer.getDeltaTime();
	time->sec += (overallTime / 24 / 60 / 1.5);
	while (time->sec >= 60)
	{
		time->min++;
		time->sec -= 60;
	}
	while (time->min >= 60)
	{
		time->hour++;
		time->min -= 60;
	}
	while (time->hour >= 24)
	{
		time->day++;
		time->hour -= 24;
	}
}