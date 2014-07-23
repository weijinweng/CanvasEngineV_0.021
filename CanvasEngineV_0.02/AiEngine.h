#pragma once
#include <iostream>
#include <list>
#include <vector>
#include <string>
#include <random>
#include <stdlib.h>
#include <time.h>

enum ai_event {DESIGN, MEETING, CODING ,EXPERIMENT,WORK, PREPAREMEAL,TAKEBUS, TALK, BREAKFAST, LUNCH, DINNER, OUTING, ARGUE, FIGHTING,FIRED, INTERVIEWING, THEFT, ROBBERY, REST}; //pls add some more stuff into the event enum
enum ai_name {Alex, Alexander, Aristotle, Blackberry, Carols, Dummy, Eagle, DrPepper};
enum ai_relation {ENEMY,HOSTILITY,NOTGOOD, STRANGER, ACQUAINTANCE, FRIENDS, LOVERS};
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
	COLLEGUE = 1 << 10,
	BOSS = 1 << 11
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
	ANGRY,
	DESPARATE,
	ANXIOUS,
	SAD,
	UNHAPPY,
	FRUSTRATION, 
	NERVOUS,
	COMPOSED,
	HAPPY,
	HILAROUS,
	CONFIDENT,
	PROUD,
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
 
//event struct
struct Events
{
	aiPerson* A;
	aiPerson* B;
	ai_event aievents;
	ai_time aitime;
	int id;
	int relationId;  //the relation id is  the same as the relation id in the relation class. there is a one to one correspondence to the relation
	bool completed;  //whether or not the event(objcetive is completeed on time
	bool checked;    //whether the event remains to be not inflenced by the interruptive events
	int possibility;
};

//relation struct
struct Relation
{
	int heart;
	aiPerson* A;
	aiPerson* B;
	ai_relation relations;
	int id;
};

struct Schedule
{
	int id;
	aiPerson* person;
	std::vector<Events*> eventList;
};

class Math
{
public:
	int min(int a, int b);
	int max(int a, int b);
	int abs(int a , int b);
};


class aiEvent
{
public:
	ai_time time;
	aiPerson* personA;
	aiPerson* personB;
	std::vector<aiPerson*> aiList;
	aiEvent(aiPerson* now, std::vector<aiPerson*> npcList,ai_time aitime);   //initialize the event class  
	Events* generateEvent(std::vector<Events*> eventList, int k);    //generate the event between A and B    and output the event in the form of string
};

class aiInterruptiveEvent:aiEvent
{
public:
	aiPerson* personC;
	void updateEvent();
};

/////////////////////////////////////////////////////////////////
//the logic of how different factors affect the daily schedules//
/////////////////////////////////////////////////////////////////
class aiSchedule
{
public:
	aiSchedule(aiPerson* personA, std::vector<aiPerson*> npcList,std::vector<Events*> eventList);
	std::vector<aiPerson*> aiList;
	std::vector<Events*> eventList;
	Schedule* aiSchedules;

	//Lock lock;   //determine which generateEffect function has the authorization to run
	Schedule* generateSchedulePre();   //objectives
	Schedule* generateScheduleAft();   //edited objectives, be influenced by interrupted events
	void generatePersonalityEffect(int stages); //influence the schedule according to thte factor
	void generateRelationEffect(int stages);    //it returns false to show that other factors has already effected the event while true means it has effect on the event
	void generateLuckEffect(int stages);
	void generateMoodEffect(int stages);
	void generateBuffEffect(int stages);
	//bool generator(ai_factor aiFactors, int stages);   //the generator return true if the event has already been influenced by factors
};

class aiRelation
{
public:
	aiPerson* personA;
	aiPerson* personB;
	aiRelation(aiPerson* A, aiPerson* B);   //takes into two Person
	//Relation* relationship;
	Relation* generateRelation();
};


class aiPerson
{
public:
	//lack vector2D class
	Schedule* personSchedule;
	bool eventStatus;  //whether the ai has an event
	std::vector<Relation*> relations;   //relation lists that relate to personA(this);
	ai_mood aiMood;
	int debuff;   //it is a product of the interruptive events. Toghether with the ai_mood,it helps decide how the other objectives proceed
	int aiPersonality;
	ai_occupation occupation;
	std::string name;                 
	int idNum;        //there is a one to one correspondence between name and idNum
	aiPerson(double x, double y,std::string name);
	void getSchedulePre(std::vector<aiPerson*> npc,std::vector<Events*> eventList);   //use the aiEvent::generateEvent() and assign the event into personEvents
	void getScheduleAft(std::vector<aiPerson*> npc,std::vector<Events*> eventList);
	void getRelation(std::vector<aiPerson*> npc);     //use the aiRelation::gnerateRelation() and assign the event into the vector
	//bool full;            //indicate whether its schedule is full or not
	int* full;
};

class AiEngine
{
public:
	AiEngine(void);
	std::vector<aiPerson*> aiList;
	std::list<aiPerson*> aiListQueued;
	std::vector<std::vector<Relation*>> relationList;
	std::vector<std::vector<Relation*>> relationListQueued;
	std::vector<Schedule*> scheduleListPre;
	std::vector<Schedule*> scheduleListAft;
	std::list<Schedule*> scheduleListQueued;
	std::vector<Events*> eventList;

	void initialize();
	void simulate();
	void end();
	void getAllEvents();
	~AiEngine(void);
};
