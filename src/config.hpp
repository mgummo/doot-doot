#pragma once

#include <vector>
#include <memory>
#include "model/Task.h"

struct AppConfig {
  std::string title;
  std::vector<Task*> tasks;
};

AppConfig load_config();