#pragma once

#include <functional>
#include <vector>

#include "core/Time.hpp"

namespace game {

typedef std::function<void(void)> Task;

struct TaskInfo {
    Task task;

    float period;
};

struct Tasks {
    void add(float frequency, Task);
    void run(Time delta); 

private:
    std::vector<TaskInfo> tasks;
};

} // namespace game
