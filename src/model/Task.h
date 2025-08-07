#pragma once

#include <string>
#include <vector>

using namespace std;

enum class TaskCompletionState
{
    NotStarted,
    InProgress,
    Completed,
};

struct Task
{
    string Title;
    string Description;
    int Priority;
    TaskCompletionState State;
};

typedef std::vector<Task *> Tasks;