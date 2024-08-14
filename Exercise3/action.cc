#include "RunAction.hh"
#include "EventAction.hh"
#include "TrackingAction.hh"
#include "SteppingAction.hh"
#include "action.hh"

MyActionInitialization::MyActionInitialization()
{}

MyActionInitialization::~MyActionInitialization()
{}

void MyActionInitialization::Build() const
{
	MyPrimaryGenerator *generator = new MyPrimaryGenerator();
	SetUserAction(generator);
	
	//Manages actions at the level of the entire simulation run, which conists of many events
	RunAction *runAction = new RunAction;
	SetUserAction(runAction);
	//Manages actions that occur for each event in the simulation
  	EventAction *eventAction = new EventAction(runAction);
	SetUserAction(eventAction);
	SetUserAction(new TrackingAction(eventAction));
	/*
	//Controls what happens at each step of a particle through the simulation geometry.
	SetUserAction(new SteppingAction(eventAction));
	*/

}
