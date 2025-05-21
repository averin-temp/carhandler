#include "CarController.h"
#include "CarDriver.h"

CarController::CarController()
{
    driver = new CarDriver();
}
CarController::~CarController()
{
    delete driver;
}
void CarController::setState(Command* command)
{
    if(
        state.upButton == command->upButton && 
        state.downButton == command->downButton && 
        state.leftButton == command->leftButton && 
        state.rightButton == command->rightButton
    ) return;

    state = *command;

    if(state.upButton && state.downButton) {
        driver->commandStop();
    } else if(state.upButton == false && state.downButton == false) {
        driver->commandStop();
    } else if(state.upButton) {
        driver->commandForward();
    } else if(state.downButton) {
        driver->commandBackward();
    }

    if(state.leftButton && state.rightButton) {
        driver->commandTurnCenter();
    } else if(state.leftButton == false && state.rightButton == false) {
        driver->commandTurnCenter();
    } else if(state.leftButton) {
        driver->commandTurnLeft();
    } else if(state.rightButton) {
        driver->commandTurnRight();
    }
}

void CarController::reset()
{
    Command cmd;

    setState(&cmd);
}

