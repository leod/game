#include "core/Tasks.hpp"

#include <iostream>

#include <algorithm>

#include "core/Error.hpp"

namespace game {

TaskInfo::TaskInfo(Time period, Task task)
    : period(period), task(task), sleepTime(Time::Zero) {
}

bool isTaskBigger(const TaskInfo& a, const TaskInfo& b) {
    // We want tasks with a lower sleepTime first
    return a.sleepTime > b.sleepTime; 
}

void Tasks::add(float frequency, Task task) {
    tasks.push_back(TaskInfo(seconds(1.0f / frequency), task));
}

void Tasks::run(Time delta) {
    if (tasks.empty())
        return;

    std::make_heap(tasks.begin(), tasks.end(), isTaskBigger);

    do {
        ASSERT(!tasks.empty());

        // Task is the task with the lowest timeUntilNextRun
        TaskInfo task = tasks.front();

        std::pop_heap(tasks.begin(), tasks.end(), isTaskBigger);
        tasks.pop_back();

        // If this task doesn't fit into the delta time, no task will
        if (task.sleepTime > delta) {
            tasks.push_back(task);
            break;
        }

        task.task(); 
        task.sleepTime += task.period;

        tasks.push_back(task);
        std::push_heap(tasks.begin(), tasks.end(), isTaskBigger);
    } while(true); 

    for (auto& task : tasks)
        task.sleepTime -= delta;
}

} // namespace game
