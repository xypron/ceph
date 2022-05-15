// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*-
// vim: ts=8 sw=2 smarttab

#include <fstream>
#include <seastar/util/log.hh>

#include "common/debug.h"
#include "logfile.h"
#include "log.h"

#define dout_context g_ceph_context
#define dout_prefix *_dout
#define dout_subsys ceph_subsys_

namespace {
  seastar::logger& logger() {
    return crimson::get_logger(ceph_subsys_osd);
  }
}  // namespace

struct logfh {
  std::string log_file_path;
  std::ofstream log_ofstream;

  ~logfh() {
    log_ofstream.flush();
    log_ofstream.close();
  }
  
  bool is_open() const {
    return !log_file_path.empty() && log_ofstream.is_open();
  }
  int open(std::string_view log_file);
};

std::unique_ptr<logfh> lfh;
int logfh::open(std::string_view log_file)
{
  log_file_path = log_file;

  log_ofstream.open(log_file_path, std::ios::app | std::ios::out);
  if (log_ofstream.fail()) {
     return -errno;
  }

  return 0;
}

void logfile_close()
{
  lfh.reset(nullptr);
}

int logfile_set_ostream(std::string_view log_file)
{
  if (log_file.empty()) {
    return 0;
  }

  ceph_assert(!lfh);

  lfh = std::make_unique<logfh>();
  if (atexit(logfile_close)) {
    return -EINVAL;
  }

  int r = lfh->open(log_file);
  if (r != 0) {
    return r;
  }
  logger().set_ostream(lfh->log_ofstream);
  return 0;
}
