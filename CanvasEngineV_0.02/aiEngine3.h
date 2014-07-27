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

enum ai_event {WORKING, EATING, RESTING, FIGHTING, OTHER, NONE};
enum ai_action {WALKING, CALLING, YELLING, RESTROOM, SLEEPING, WRITINGMAIL, EATING, WOKRING};
enum ai_objectives {OUTING, WOKRING, PLAYING, SLEEPING, FIGHTING};

enum ai_location {HOME, SHOPPINGMALL, COMPANYA, COMPANYB, RESTAURANTA, RESTAURANTB};
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

};

class aiPerson
{

};

class aiActions
{
	
};

class aiEvents
{
public:
	aiEvents(aiPerson* personA, std::vector<aiActions*> actionList, aiTime time);
	
	aiPerson* personA;
	ai_event event;
	aiTime time;
	ai_location location;
	std::vector<aiPerson*> aiInvolved;
	
	void printEvent();
	void generateAction();
	void generateEventEffect();
};

class aiEffects
{
	
};

class aiObjectives
{
public:
	aiObjectives(aiPerson* personA, aiTime time);
	
	int objPriority;
	aiPerson* personA;
	ai_objectives objectives;
	aiTime time;
	ai_location location;
	std::vector<aiPerson*> aiInvolved;

	void printObjectives();
	void generateEvent();
	void generateObjectiveEffect();
};

class aiScheduler
{
public:
	aiScheduler(aiPerson* personA);

	std::vector<aiObjectives> objectiveList;
	aiPerson* personA;
	void generateObjectives();
	void executeObjectives();       //according to priority   

};

class aiEngine
{
	
};