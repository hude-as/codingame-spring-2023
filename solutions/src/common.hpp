// #pragma GCC optimize "O3,omit-frame-pointer,inline"

#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <array>
#include <map>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <numeric>
#include <random>
#include <queue>

using namespace std;
// Rules
#define TIME_LIMIT_MS 100000
#define MAX_CELLS 100
#define MAX_BASE_PER_PLAYER 2
#define MAX_PLAYER 2
#define ME 0
#define OP 1
#define EGGS 1
#define CRYS 2
#define EMPTY 0

typedef chrono::time_point<chrono::high_resolution_clock> tphr_t;
typedef unsigned short ushort;
typedef unsigned char uchar;

#include "Action.hpp"
#include "State.hpp"
#include "Engine.hpp"
#include "AI.hpp"
#include "GameUtils.hpp"
