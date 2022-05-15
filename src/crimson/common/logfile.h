// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*-
// vim: ts=8 sw=2 smarttab

#pragma once

#include <string_view>

// Write a logfile with the current log, using the configuration in the
// provided conf structure.
// Return 0 on success,
//  -EINVAL if the logfile_close() atexit failed
//  -errno if the logfile_open() failed
[[nodiscard]] int logfile_set_ostream(std::string_view log_file);

//close and release the log file
void logfile_close();
