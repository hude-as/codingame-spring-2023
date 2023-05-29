// #pragma GCC optimize "O3,omit-frame-pointer,inline"

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <map>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <numeric>
#include <random>

using namespace std;
#define TIME_LIMIT_MS 100000

typedef chrono::time_point<chrono::high_resolution_clock> tphr_t;

#include "Action.hpp"
#include "State.hpp"
#include "Engine.hpp"
#include "AI.hpp"
#include "GameUtils.hpp"
