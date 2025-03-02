#pragma once

// Poll macro
#ifdef _WIN32
#define Poll WSAPoll
#else
#define Poll poll
#endif

constexpr int POLL_WAIT     = 100;

constexpr int MAX_RECV      = 1000;


