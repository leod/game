#include "core/Tasks.hpp"

#include <iostream>
#include <algorithm>

#include "core/Error.hpp"

namespace game {

TaskInfo::TaskInfo(Time period, Task task)
    : period(period), task(task), sleepTime(Time::Zero) {
}

bool compareTaskPriority(TaskInfo const& a, TaskInfo const& b) {
    // We want tasks with a lower sleepTime first
    return a.sleepTime > b.sleepTime; 
}

void Tasks::run(Time delta) {
    if (tasks.empty())
        return;

    std::make_heap(tasks.begin(), tasks.end(), compareTaskPriority);

    do {
        ASSERT(!tasks.empty());

        // Task is the task with the lowest sleepTime
        TaskInfo task = tasks.front();

        std::pop_heap(tasks.begin(), tasks.end(), compareTaskPriority);
        tasks.pop_back();

        // If this task doesn't fit into the delta time, no task will
        if (task.sleepTime > delta) {
            tasks.push_back(task);
            break;
        }

        task.task(); 
        task.sleepTime += task.period;

        tasks.push_back(task);
        std::push_heap(tasks.begin(), tasks.end(), compareTaskPriority);
    } while(true); 

    for (auto& task : tasks)
        task.sleepTime -= delta;
}

} // namespace game
