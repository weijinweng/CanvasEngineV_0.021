#pragma once
#include <stdlib.h>
#include <iostream>
#include <list>
#include <vector>
#include <string>
#include <random>
#include <time.h>
#include "Engine.h"

enum ai_event {WORKING, EATING, RESTING, FIGHTING, OTHER, NONE};
enum ai_name {Alex, Alexander, Aristotle, Blackberry, Carols, Dummy, Eagle, DrPepper};
enum ai_relation {STRANGER, COLLEGUE, ACQUAINTANCE, FAMILY, FRIENDS, LOVERS};
enum ai_friendship {OPPONENT,UNFRIENDLY,NOTGOOD, JUSTSOSO, NOTBAD, FRIENDLY, BROTHER}; 

class aiPerson;
class aiRelation;
class aiEvents;

enum ai_check {CHECKED, UNCHECKED};

enum ai_occupation {
	STUDENT = 1 << 0,
	HOUSEWIFE = 1 << 1,
	CLERK = 1 << 2,
	BANKER = 1 << 3,
	POLICE = 1 <<4,
	GANGSTER = 1 << 5,
	THIEF = 1 << 6,
	DOCTOR = 1 << 7,
	LAWYER = 1 << 8,
	PEER = 1 << 9,
	BOSS = 1 << 10
}; 

enum ai_personality {
	KIND = 1 << 0,
	TENDER = 1 << 1,
	RASH = 1 << 2,
	OPTIMISTIC = 1 << 3,
	PESSIMISTIC = 1 << 4,
	HARDWORKING = 1 << 5,
	LAZY = 1 << 6,
	HOSTILE = 1 << 7
};

enum ai_mood{
	OUTRAGEOUS,
	DESPARATE,
	ANGRY,
	ANXIOUS,
	SAD,
	UNHAPPY,
	FRUSTRATION, 
	NERVOUS,
	COMPOSED,
	HAPPY,
	HILAROUS,
	CONFIDENT,
	CRAZY
};

struct aiDebuff
{
	int ILL;
	int SLEEPY;
	int HUNGRY;
	int TIRED;
	int OVERWORKED;
	int BEHINDSCHEDULE;
};

struct aiTime
{
	int day;
	int hour;
	int min;
	int sec;
};

const int aiSize = 50;

static std::vector<ai_event> ai_schedule1;   //defined in aiEngine class as a primitive one which will be altered later

struct Relation
{
	int heart;
	aiPerson* A;
	aiPerson* B;
	ai_relation relations;
	ai_friendship friendship;
	int id;
};

struct Schedule
{
	int id;
	aiPerson* person;
	std::vector<aiEvents*> eventList;
};

class Math
{
public:
	Math();
	int min(int a, int b);
	int max(int a, int b);
	int abs(int a , int b);
};

class aiEvents
{
public:
	aiEvents(aiPerson* A,std::vector<aiPerson*>& aiLists, aiTime* time);
	ai_event eventType;
	aiTime* time;
	aiPerson* personA;
	int eventStage;             //in another word, the time of the event
	std::vector<aiEvents*> eventList;
	std::vector<aiPerson*> aiInvolved;
	std::vector<aiPerson*> aiList;
	std::vector<ai_friendship> friendshipList;
	int eventId;
	int possibility;
	bool completed;  //whether or not the event(objcetive is completeed on time
	ai_check checked;    //whether the event remains to be not inflenced by the interruptive events
	
	std::vector<aiPerson*> generateEvent();

	void generateEffect(int stages);
	double generatePersonalityEffect(int stages); 
	double generateFriendshipEffect(int stages); 
	double generateLuckEffect(int stages);
	double generateMoodEffect(int stages);
	double generateBuffEffect(int stages);
	void updateEvent(int stages, aiPerson* person, ai_event);	
	void eventCausedByBuff(int stages);
};

class Working:public aiEvents
{
public:
	enum ai_working {MEETING, MANUFACTURING, GROUPWORKING, EXPERIMENTING, CODING};
	ai_working workingType;
	std::vector<aiPerson*> collegue;
	Working(aiPerson* personA,std::vector<aiPerson*>& aiLists, aiTime* time);
	void generateWorking();
	void generateWorkingInterruptive();
};

class Eating:public aiEvents
{
public:
	enum ai_eating {BREAKFAST, LUNCH, SUPPER, SNACK};
	ai_eating eatingType;
	std::vector<aiPerson*> collegue;
	Eating(aiPerson* personA,std::vector<aiPerson*>& aiLists, aiTime* time);
	void generateEating();
	void generateEatingInterruptive();
};

class Resting:public aiEvents
{
public:
	enum ai_resting {NAP, SLEEP, GAME, DRINKING, OVERSLEEP};
	ai_resting restingType;
	std::vector<aiPerson*> collegue;
	Resting(aiPerson* personA,std::vector<aiPerson*>& aiLists, aiTime* time);
	void generateResting();
	void generateRestingInterruptive();

};

class Fighting:public aiEvents
{
public:
	enum ai_fighting {ARGUE, DEBATE, TEAMFIGHT, DUEL}; //family, collegue, friend, opponenet
	ai_fighting fightingType;
	std::vector<aiPerson*> collegue;
	Fighting(aiPerson* personA,std::vector<aiPerson*>& aiLists, aiTime* time);
	void generateFighting();
	void generateFightingInterruptive();
};

class Other:public aiEvents
{
public:
	enum ai_other {TRAVELINGTOWORK, SHOPPING, WANDERING, LOST};
	ai_other otherType;
	std::vector<aiPerson*> collegue;
	Other(aiPerson* personA,std::vector<aiPerson*>& aiLists, aiTime* time);
	void generateOther();
	void generateOtherInterruptive();
};

class aiPerson
{
public:
	aiPerson(int);
	std::string name;
	int aiPersonality;               
	int idNum;        //there is a one to one correspondence between name and idNum
	aiTime* time;
	aiDebuff* debuff;
	ai_occupation occupation;
	ai_mood aiMood;
	Schedule* aiSchedule;
	std::vector<Relation*> relationList;
	std::vector<aiEvents*> dailyEvents;
	aiPerson(double x, double y,std::string name);
	std::vector<Relation*> generateRelation(std::vector<aiPerson*>& npc);
	Schedule* generateSchedule(std::vector<aiPerson*>& npc, aiTime* time);


	void updateBuff(int stages);

};

class aiRelation
{
public:
	aiPerson* personA;
	aiPerson* personB;
	aiRelation(aiPerson* A, aiPerson* B);   
	Relation* generateRelation();
};

class aiEngine:public CanvasSystem
{
public:
	aiTime* time;
	CanvasTimer timer;
	aiEngine();
	std::vector<aiPerson*> aiList;
	std::list<aiPerson*> aiListQueued;
	std::vector<std::vector<Relation*> > relationList;
	std::vector<std::vector<Relation*> > relationListQueued;
	std::vector<Schedule*> scheduleListPre;
	std::vector<Schedule*> scheduleListAft;
	std::list<Schedule*> scheduleListQueued;
	std::vector<aiEvents*> eventList;

	void getTime();
	bool initialize();
	bool simulate();
	bool end();

	void preScheduleGenerator()
	{
		ai_schedule1.push_back(RESTING);   //6am
		ai_schedule1.push_back(EATING);   //7am
		ai_schedule1.push_back(OTHER);    //8am
		ai_schedule1.push_back(WORKING);  //9am
		ai_schedule1.push_back(WORKING);   //10am
		ai_schedule1.push_back(WORKING);   //11am
		ai_schedule1.push_back(EATING);   //12am
		ai_schedule1.push_back(RESTING);  //1pm
		ai_schedule1.push_back(WORKING);  //2pm
		ai_schedule1.push_back(WORKING);  //3pm
		ai_schedule1.push_back(WORKING);  //4pm
		ai_schedule1.push_back(OTHER);    //5pm 
		ai_schedule1.push_back(EATING);   //6pm
		ai_schedule1.push_back(RESTING);   //7pm
		ai_schedule1.push_back(RESTING);   //8pm
		ai_schedule1.push_back(FIGHTING);   //9pm
		ai_schedule1.push_back(RESTING);   //10pm
	}

};

