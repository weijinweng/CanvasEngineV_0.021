#include "aiEngine3.h"
#include <iostream>
#include <Windows.h>

aiScheduler::aiScheduler(aiPerson* personA, std::vector<aiPerson*> aiList)
{
	this->personA = personA;
	this->aiList = aiList;
}

void aiScheduler::generateObjectives(aiTime time)
{
	aiObjectives* objectives = new aiObjectives(personA, time, aiList);
	if (objectives != NULL)
		objectiveList.push_back(objectives);
}

void aiScheduler::executeObjectives()
{
	int  max = 0;

	for (std::vector<aiObjectives*>::iterator it = objectiveList.begin(); it != objectiveList.end(); it++)
	{
		if (max < (*it)->objPriority)
		{
			max = (*it)->objPriority;
			currentObjective = (*it);
		}
	}
	currentObjective->currentTime = getTime();
	if (currentObjective->halted == false && currentObjective->currentAction != NULL)
	{
		currentObjective->generateAction();         //generate the subsequent actions
		currentObjective->receiveEvent();
	}
	if (currentObjective->completed)
	{
		currentObjective->generateObjectiveEffect();        //generate aiEffect to influence the world
		for (std::vector<aiObjectives*>::iterator it = objectiveList.begin(); it != objectiveList.end(); it++)
		{
			if ((*it) == currentObjective)
				objectiveList.erase(it);
			break;
		}
	}
}

aiObjectives::aiObjectives(aiPerson* personA, aiTime time, std::vector<aiPerson*> aiList)
{
	this->personA = personA;
	startTime = time;
	this->aiList = aiList;
	completed = false;
	interrupted = false;
	halted = false;
	busy = false;
	step = 0;
	generateDetail();
	for (std::vector<aiPerson*>::iterator it = aiInvolved.begin(); it != aiInvolved.end(); it++)
	{
		aiObjectives objective = *this;
		objective.personA = (*it);
		(*it)->dailyObjectives.push_back(&objective);
	}
}

void aiObjectives::generateDetail()
{
	objectives = (ai_objectives)(rand() % 6);
	if (objectives == OUTING)
	{
		relationInvolved = FRIENDS;
		location = PARK;
	}
	else if (objectives == WORKING)
	{
		relationInvolved = COLLEGUE;
		location = COMPANYA;
	}
	else if (objectives = TALKING)
	{
		if (rand() % 3 == 0)
		{
			relationInvolved = COLLEGUE;
			location = COMPANYA;
		}
		else if (rand() % 3 == 1)
		{
			relationInvolved = FRIENDS;
			location = RESTAURANTA;
		}
		else
		{
			relationInvolved = FAMILY;
			location = HOME;
		}
	}
	else if (objectives = FIGHTING)
	{
		relationInvolved = ENEMY;
		location = PARK;
	}
	else if (objectives = EATING)
	{
		if (rand() % 3 == 0)
		{
			relationInvolved = COLLEGUE;
			location = COMPANYA;
		}
		else if (rand() % 3 == 1)
		{
			relationInvolved = FRIENDS;
			location = RESTAURANTA;
		}
		else
		{
			relationInvolved = FAMILY;
			location = RESTAURANTA;
		}
	}
	else
	{
		relationInvolved = FAMILY;
		location = HOME;
	}
	aiInvolved.push_back(personA);
	if (specificPeople)          //generate ccertain people, these people are part of the objectives (WHO)
	{
		for (std::vector<Relation*>::iterator it = personA->relationList.begin(); it != personA->relationList.end(); it++)
		{
			if (aiInvolved.size() > 5)
			{
				if ((*it)->relations == relationInvolved && (rand() % 2 == 0))
				{
					if ((*it)->A == personA)
						aiInvolved.push_back((*it)->B);
					else if ((*it)->B == personA)
						aiInvolved.push_back((*it)->A);
				}
			}
			else
				break;
		}
	}
}

void aiObjectives::generateAction()
{
	if (step == 0 && busy == false)
	{
		for (std::vector<aiPerson*>::iterator it = aiInvolved.begin(); it != aiInvolved.end(); it++)
		{
			aiActions* actionContact = new aiActions((*it), startTime, (ai_action)CONTACTING, aiList);
			actionContact->locationFrom = (*it)->location;
			actionContact->locationTo = (*it)->location;
			actionContact->generateActionEffect();
			aiEvents* event = actionContact->generateEvent();       //generate TALKING event according to the actions
			if (event->halted == false)
			{
				event->generateEventEffect();	
				(*it)->currentObjective->actionList.push_back(actionContact);
				(*it)->currentObjective->eventList.push_back(event);
				(*it)->currentObjective->currentAction = actionContact;
				(*it)->currentObjective->currentEvent = event;
				busy = true;
			}
			else
				halted = event->halted;
		}
	}
	else if (step == 1 && busy == false)
	{
		// go to the place
		for (std::vector<aiPerson*>::iterator it = aiInvolved.begin(); it != aiInvolved.end(); it++)
		{
			aiActions* actionTravel = new aiActions((*it), currentTime, TRAVELING, aiList);
			actionTravel->locationFrom = (*it)->location;
			actionTravel->locationTo = this->location;
			actionTravel->generateActionEffect();
			(*it)->currentObjective->actionList.push_back(actionTravel);
			(*it)->currentObjective->currentAction = actionTravel;
			(*it)->currentObjective->currentEvent = NULL;
			busy = true;
		}
	}
	else if (step == 2 && busy == false)
	{
		// find the person
		for (std::vector<aiPerson*>::iterator it = aiInvolved.begin(); it != aiInvolved.end(); it++)
		{
			aiActions* actionFind = new aiActions((*it), currentTime, FINDING, aiList);
			actionFind->locationFrom = (*it)->location;
			actionFind->locationTo = actionFind->locationFrom;
			actionFind->generateActionEffect();
			aiEvents* event = actionFind->generateEvent();   //genereawte MEET event
			if (event->halted == false)
			{
				event->generateEventEffect();
				(*it)->currentObjective->eventList.push_back(event);
				(*it)->currentObjective->actionList.push_back(actionFind);
				(*it)->currentObjective->currentAction = actionFind;
				(*it)->currentObjective->currentEvent = event;
				busy = true;
			}
			else
				halted = event->halted;
		}
	}
	else if (step == 3 && busy == false)
	{
		// execute
		for (std::vector<aiPerson*>::iterator it = aiInvolved.begin(); it != aiInvolved.end(); it++)
		{
			aiActions* actionExecute = new aiActions((*it), currentTime, (ai_action)objectives, aiList);
			actionExecute->locationFrom = (*it)->location;
			actionExecute->locationTo = actionExecute->locationFrom;
			actionExecute->generateActionEffect();        //generawte the objective event
			aiEvents* event = actionExecute->generateEvent();
			if (event->halted == false)
			{
				event->generateEventEffect();
				(*it)->currentObjective->actionList.push_back(actionExecute);
				(*it)->currentObjective->eventList.push_back(event);
				(*it)->currentObjective->currentAction = actionExecute;
				(*it)->currentObjective->currentEvent = event;
				busy = true;
			}
			else
				halted = event->halted;
		}
	}
	if (startTime + currentAction->duration > currentTime)
	{
		step++;
		busy = false;
	}
}

void aiObjectives::receiveEvent()
{

}
///////////////////////////////////aiActions//////////////////////////////////

aiActions::aiActions(aiPerson* personA, aiTime time, ai_action actionType, std::vector<aiPerson*> aiInvolved)
{
	this->personA = personA;
	this->startTime = time;
	this->actionType = actionType;
	this->aiInvolved = aiInvolved;
	//this->personA->location = locationTo;
	//this->personA->time = this->startTime + duration
	eventSignal = false;
	alreadyGenerateEvent = false;
	generateDetail();
}

void aiActions::generateDetail()
{
	if (actionType == FINDING)
	{
		duration.day = 0;
		duration.hour = 0;
		duration.min = 10;
		duration.sec = 0;
	}
	else if (actionType == TRAVELING)
	{
		duration.day = 0;
		duration.hour = 0;
		duration.min = 45;
		duration.sec = 0;
	}
	else if (actionType == OUTING)
	{
		duration.day = 0;
		duration.hour = 1;
		duration.min = 0;
		duration.sec = 0;
	}
	else if (actionType == WOKRING)
	{
		duration.day = 0;
		duration.hour = 1;
		duration.min = 30;
		duration.sec = 0;
	}
	else if (actionType == CONTACTING)
	{
		duration.day = 0;
		duration.hour = 0;
		duration.min = 15;
		duration.sec = 0;
	}
	else if (actionType == RESTING)
	{
		duration.day = 0;
		duration.hour = 1;
		duration.min = 0;
		duration.sec = 0;
	}
	else if (actionType == FIGHTING)
	{
		duration.day = 0;
		duration.hour = 0;
		duration.min = 20;
		duration.sec = 0;
	}
	else if (actionType == CONTACTING)
	{
		duration.day = 0;
		duration.hour = 0;
		duration.min = 5;
		duration.sec = 0;
		eventSignal = true;
	}
	else if (actionType == EATING)
	{
		duration.day = 0;
		duration.hour = 0;
		duration.min = 45;
		duration.sec = 0;
		if (locationFrom == locationTo && locationTo == RESTAURANTA)
			eventSignal = true;
	}
	else if (actionType == PLAYING)
	{
		duration.day = 0;
		duration.hour = 1;
		duration.min = 0;
		duration.sec = 0;
		if (locationFrom == locationTo && locationTo == PARK)
			eventSignal = true;
	}
	else if (actionType == PAY)
	{
		duration.day = 0;
		duration.hour = 0;
		duration.min = 5;
		duration.sec = 0;
		eventSignal = true;
	}
	else if (actionType == TALKING)
	{
		duration.day = 0;
		duration.hour = 0;
		duration.min = 20;
		duration.sec = 0;
		eventSignal = true;
	}
	else if (actionType == SERVING)
	{
		duration.day = 0;
		duration.hour = 0;
		duration.min = 20;
		duration.sec = 0;
	}
}

void aiActions::executeAction()
{

}

aiEvents* aiActions::generateEvent()
{
	if (eventSignal && (!alreadyGenerateEvent))
	{
		aiEvents* newEvent = new aiEvents(personA, aiInvolved, startTime, (ai_event)actionType, aiList, duration);

		for (std::vector<aiPerson*>::iterator it = aiList.begin(); it != aiList.end(); it++)
		{
			if ((*it)->location == locationTo && (*it)->currentObjective->currentAction->actionType == SERVING)
			{
				aiTime waitingTime = (*it)->currentObjective->currentEvent->startTime + (*it)->currentObjective->currentEvent->duration - startTime;
				if (!(*it)->occupied)   //the servant is kind of idle
				{
					alreadyGenerateEvent = true;
					(*it)->currentObjective->currentAction->alreadyGenerateEvent = true;
					aiEvents* newEvent2 = new aiEvents((*it), startTime, (ai_event)SERVING, aiList, duration);
					newEvent2->aiInteracted = aiInvolved;
					(*it)->currentObjective->currentEvent = newEvent2;
					(*it)->occupied = true;
				}
				else if (waitingTime > duration)   //if the servant will not be idle in a certain period
				{
					continue;
				}
				else if (duration > waitingTime)
				{
					continue;
				}
			}
		}
		if (alreadyGenerateEvent == false)
			halted = true;
	}
}

////////////////////////////////////////////////////////////////

aiEvents::aiEvents(aiPerson* personA, std::vector<aiPerson*> aiInvolved, aiTime time, ai_event eventType, std::vector<aiPerson*> aiList, aiTime duration)
{
	this->personA = personA;
	this->aiInvolved = aiInvolved;
	this->startTime = time;
	this->eventType = eventType;
	this->completed = false;
	this->aiList = aiList;
	this->duration = duration;
	generateDetail();
}

void generateDetail()
{
}

aiTime & operator+(aiTime timeStarted, aiTime duration)
{
	aiTime addition;
	addition.day = timeStarted.day + duration.day;
	addition.hour = timeStarted.hour + duration.hour;
	addition.min = timeStarted.min + duration.min;
	addition.sec = timeStarted.sec + duration.sec;
	while (addition.sec >= 60)
	{
		addition.min++;
		addition.sec -= 60;
	}
	while (addition.min >= 60)
	{
		addition.hour++;
		addition.min -= 60;
	}
	while (addition.hour >= 24)
	{
		addition.day++;
		addition.hour -= 24;
	}
	return addition;
}

aiTime & operator-(aiTime timeStarted, aiTime duration)
{
	aiTime subraction;
	subraction.day = timeStarted.day - duration.day;
	subraction.hour = timeStarted.hour - duration.hour;
	subraction.min = timeStarted.min - duration.min;
	subraction.sec = timeStarted.sec - duration.sec;
	while (subraction.sec < 0)
	{
		subraction.min--;
		subraction.sec += 60;
	}
	while (subraction.min < 0)
	{
		subraction.hour--;
		subraction.min += 60;
	}
	while (subraction.hour < 0)
	{
		subraction.day--;
		subraction.hour += 24;
	}
	return subraction;
}

bool & operator>(aiTime time1, aiTime time2)
{
	aiTime subtraction = time1 - time2;
	int sum = subtraction.day*24*3600 + subtraction.hour*3600 + subtraction.min*60 + subtraction.sec;
	if (sum <= 0)
		return false;
	else 
		return true;
}


aiEngine3::aiEngine3(void)
{
}


aiEngine3::~aiEngine3(void)
{
}
