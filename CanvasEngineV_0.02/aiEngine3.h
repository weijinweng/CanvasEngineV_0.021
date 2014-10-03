#pragma once
#include <stdlib.h>
#include <iostream>
#include <list>
#include <vector>
#include <string>
#include <random>
#include <time.h>
#include <map>;
#include "Engine.h"

enum ai_event {OUTING, WOKRING, TALKING, RESTING, FIGHTING, EATING, PLAYING, PAY, NEGOTIATING,SERVING};
enum ai_action {OUTING, WORKING, TALKING, RESTING, FIGHTING, EATING, PLAYING,PAY, NEGOTIATING,SERVING, TRAVELING, CONTACTING, YELLING, RESTROOM, SLEEPING, WRITINGMAIL, FINDING, EATALONE, WORKALONE};
enum ai_objectives {OUTING, WOKRING, TALKING, RESTING, FIGHTING, EATING, PLAYING, PAY, NEGOTIATING,SERVING};

enum ai_location {HOME, SHOPPINGMALL, COMPANYA, COMPANYB, RESTAURANTA, RESTAURANTB, PARK};
enum ai_relation {ENEMY, STRANGER, COLLEGUE, ACQUAINTANCE, FAMILY, FRIENDS};
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
	int ANGRY;
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

const int aiSize = 10;

struct Relation
{
	int heart;
	aiPerson* A;
	aiPerson* B;
	ai_relation relations;
	ai_friendship friendship;
	int id;
};

class Math
{
public:
	Math();
	int min(int a, int b);
	int max(int a, int b);
	int abs(int a , int b);
};

class aiRelation
{
public:
	aiPerson* personA;
	aiPerson* personB;
	aiRelation(aiPerson* A, aiPerson* B);   
	Relation* generateRelation();
};

class aiPerson
{
public:
	aiPerson(int);
	std::string name;
	int aiPersonality;
	int idNum;

	bool occupied;

	aiTime time;
	aiDebuff* debuff;
	ai_occupation occupation;
	ai_mood aiMood;
	ai_location location;
	std::vector<Relation*> relationList;
	aiObjectives* currentObjective;
	std::vector<aiObjectives*> dailyObjectives;
};

class aiActions
{
public:
	aiActions(aiPerson* personA, aiTime time, ai_action actionType, std::vector<aiPerson*> aiList);

	aiPerson* personA;
	aiTime startTime;
	ai_action actionType;
	ai_location locationFrom;
	ai_location locationTo;
	std::vector<aiPerson*> aiInvolved;
	std::vector<aiPerson*> aiList;
	aiTime duration;

	bool completed;
	bool eventSignal;
	bool alreadyGenerateEvent;
	bool halted;

	void printAction();
	void executeAction();
	void generateActionEffect();
	void generateDetail();
	aiEvents* generateEvent(); 
	friend aiTime & operator+(aiTime timeStarted, aiTime duration);
	friend aiTime & operator-(aiTime timeStarted, aiTime duration);
	friend bool & operator>(aiTime time1, aiTime time2);
};

class aiEvents
{
public:
	aiEvents(aiPerson* personA, std::vector<aiPerson*> aiInvolved, aiTime time, ai_event eventType, std::vector<aiPerson*> aiList, aiTime duration);
	aiEvents(aiPerson* personA, aiTime time, ai_event eventType, std::vector<aiPerson*>aiList, aiTime duration);
	aiPerson* personA;
	ai_event eventType;
	aiTime startTime;
	ai_location location;
	std::vector<aiPerson*> aiInvolved;
	std::vector<aiPerson*> aiInteracted;
	std::vector<aiPerson*> aiList;
	aiTime duration;
	
	bool halted;
	bool completed;

	void printEvent();
	void generateDetail();
	void executeEvent();
	//void updateEvent(std::vector<aiPerson*> aiInvolved, bool interacted);
	void generateEventEffect();
	friend aiTime & operator+(aiTime timeStarted, aiTime duration);
	friend aiTime & operator-(aiTime timeStarted, aiTime duration);
	friend aiTime & operator>(aiTime time1, aiTime time2);
};

class aiEffects
{
	
};

class aiObjectives
{
public:
	aiObjectives(aiPerson* personA, aiTime time, std::vector<aiPerson*> aiList);
	
	int objPriority;
	aiPerson* personA;
	ai_objectives objectives;
	aiTime startTime;
	aiTime duration;
	aiTime currentTime;
	ai_location location;
	ai_relation relationInvolved;
	std::vector<aiPerson*> aiInvolved;
	aiActions* currentAction;
	aiEvents* currentEvent;
	std::vector<aiPerson*> aiList;
	std::vector<aiEvents*> eventList;
	std::vector<aiActions*> actionList;

	int step;

	bool halted;
	bool completed;
	bool interrupted;
	bool specificPeople;     //check whether aiInvolved are specified or are just randomly picked
	bool busy;

	void generateDetail();
	void printObjectives();
	void generateEvent();     //what to do list
	void generateAction();

	void executeAction();
	void receiveEvent();
	void generateObjectiveEffect();
	friend aiTime & operator+(aiTime timeStarted, aiTime duration);
	friend aiTime & operator-(aiTime timeStarted, aiTime duration);
	friend aiTime & operator>(aiTime time1, aiTime time2);
};

class aiScheduler
{
public:
	aiScheduler(aiPerson* personA, std::vector<aiPerson*> aiList);

	std::vector<aiObjectives*> objectiveList;
	std::vector<aiPerson*> aiList;
	aiObjectives* currentObjective;
	aiPerson* personA;
	void generateObjectives(aiTime);
	void executeObjectives();       //according to priority   

};

class aiEngine
{
	
};