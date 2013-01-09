#pragma once

#include <functional>
#include <vector>

#include "core/Time.hpp"

namespace game {

typedef std::function<void(void)> Task;

struct TaskInfo {
    Time period;
    Task task;

    Time sleepTime;

    TaskInfo(Time period, Task);
};

struct Tasks {
    template<typename F>
    void add(float frequency, F fn) {
        tasks.push_back(TaskInfo(seconds(1.0f / frequency), fn));
    }

    void run(Time delta); 

private:
    std::vector<TaskInfo> tasks;
};

} // namespace game
