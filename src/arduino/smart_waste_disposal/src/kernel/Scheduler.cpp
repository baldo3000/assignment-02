
#include "Scheduler.h"
#include <Arduino.h>

void Scheduler::init(const int basePeriod)
{
    this->basePeriod = basePeriod;
    this->nTasks = 0;
    this->prevTickStartTime = 0;
}

bool Scheduler::addTask(Task *task)
{
    if (this->nTasks < MAX_TASKS - 1)
    {
        this->taskList[nTasks] = task;
        this->nTasks++;
        return true;
    }
    else
    {
        return false;
    }
}

void Scheduler::schedule()
{
    const long currTime = millis();
    if (currTime - this->prevTickStartTime >= this->basePeriod)
    {
        for (int i = 0; i < this->nTasks; i++)
        {
            Task *task = this->taskList[i];
            if (task->isActive() && task->updateAndCheckTime(this->basePeriod))
            {
                task->tick();
            }
        }
        this->prevTickStartTime = currTime;
    }
}
