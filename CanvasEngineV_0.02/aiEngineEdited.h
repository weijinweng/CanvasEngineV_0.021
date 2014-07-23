#pragma once
#include <iostream>
#include <list>
#include <vector>
#include <string>
#include <random>
#include <stdlib.h>
#include <time.h>

enum ai_event {WORKING, EATING, RESTING, FIGHTING, OTHER};
enum ai_name {Alex, Alexander, Aristotle, Blackberry, Carols, Dummy, Eagle, DrPepper};
enum ai_relation {STRANGER, COLLEGUE, ACQUAINTANCE, FAMILY, FRIENDS, LOVERS};
enum ai_friendship {OPPONENT,UNFRIENDLY,NOTGOOD, JUSTSOSO, NOTBAD, FRIENDLY, BROTHER}; 
enum ai_time {EIGHTAM, TENAM, TWELVEAM, TWOPM, FOURPM, SIXPM, EIGHTPM, TENPM}; 

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

enum negativeBuff
{
	ILL = 1 << 0,
	SLEEPY = 1 << 1,
	HUNGRY = 1 << 2,
	TIRED = 1 << 3,
	OVERWORKED = 1 << 4,
	PROCASTINATING = 1 << 5,
	NONE = 1 << 6
};

const int aiSize = 50;

std::vector<ai_event> ai_schedule1;   //defined in aiEngine class as a primitive one which will be altered later

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
	int min(int a, int b);
	int max(int a, int b);
	int abs(int a , int b);
};

class aiEvents
{
public:
	aiEvents(aiPerson* A,std::vector<aiPerson*> aiLists, ai_time time);
	ai_event eventType;
	ai_time eventTime;
	aiPerson* personA;
	std::vector<aiEvents*> eventList;
	std::vector<aiPerson*> aiInvolved;
	std::vector<aiPerson*> aiList;
	std::vector<ai_friendship> friendshipList;
	int eventId;
	int possibility;
	bool completed;  //whether or not the event(objcetive is completeed on time
	bool checked;    //whether the event remains to be not inflenced by the interruptive events
	aiEvents* generateEvent();
	double generateEffect(int stages);
	double generatePersonalityEffect(int stages); 
	double generateFriendshipEffect(int stages); 
	double generateLuckEffect(int stages);
	double generateMoodEffect(int stages);
	double generateBuffEffect(int stages);
};

class Working:public aiEvents
{
public:
	enum ai_working {MEETING, MANUFACTURING, GROUPWORKING, EXPERIMENTING, CODING};
	ai_working workingType;
	std::vector<aiPerson*> collegue;
	Working(aiPerson* personA,std::vector<aiPerson*> aiLists, ai_time time);
	void behave();
};

class Eating:public aiEvents
{
public:
	enum ai_eating {BREAKFAST, LUNCH, SUPPER, SNACK};
	ai_eating eatingType;
	std::vector<aiPerson*> collegue;
	Eating(aiPerson* personA,std::vector<aiPerson*> aiLists, ai_time time);
	void behave();
};

class Resting:public aiEvents
{
public:
	enum ai_resting {NAP, SLEEP, GAME, DRINKING, OVERSLEEP, };
	ai_resting restingType;
	std::vector<aiPerson*> collegue;
	Resting(aiPerson* personA,std::vector<aiPerson*> aiLists, ai_time time);
	void behave();
};

class Fighting:public aiEvents
{
public:
	enum ai_fighting {ARGUE, DEBATE, TEAMFIGHT, DUEL}; //family, collegue, friend, opponenet
	ai_fighting fightingType;
	std::vector<aiPerson*> collegue;
	Fighting(aiPerson* personA,std::vector<aiPerson*> aiLists, ai_time time);
	void behave();
};

class Other:public aiEvents
{
public:
	Other(aiPerson* personA,std::vector<aiPerson*> aiLists, ai_time time);
	void behave();
};

class aiPerson
{
public:
	aiPerson();
	std::string name;
	int idNum;
	Schedule* aiSchedule;
	std::vector<Relation*> relationList;
	std::vector<aiEvents*> dailyEvents;
	ai_mood aiMood;
	int debuff;   //it is a product of the interruptive events. Toghether with the ai_mood,it helps decide how the other objectives proceed
	int aiPersonality;
	ai_occupation occupation;               
	int idNum;        //there is a one to one correspondence between name and idNum
	aiPerson(double x, double y,std::string name);
	int* full;

	std::vector<Relation*> generateRelation(std::vector<aiPerson*> npc);
	Schedule* generateSchedule(std::vector<aiPerson*> npc);
};

class aiRelation
{
public:
	aiPerson* personA;
	aiPerson* personB;
	aiRelation(aiPerson* A, aiPerson* B);   
	Relation* generateRelation();
};

class aiEngine
{
public:
	aiEngine(void);
	std::vector<aiPerson*> aiList;
	std::list<aiPerson*> aiListQueued;
	std::vector<std::vector<Relation*>> relationList;
	std::vector<std::vector<Relation*>> relationListQueued;
	std::vector<Schedule*> scheduleListPre;
	std::vector<Schedule*> scheduleListAft;
	std::list<Schedule*> scheduleListQueued;
	std::vector<aiEvents*> eventList;

	void initialize();
	void simulate();
	void end();
	void getAllEvents();
	~aiEngine(void);
	
}


