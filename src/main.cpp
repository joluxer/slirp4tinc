/*
 * main.cpp
 *
 *  Created on: 29.08.2020
 *      Author: lode
 */

#include "SlirpHandler.h"
#include "umlplug/UmlNetworkSocketPlug.h"
#include "FdSelectHandler.h"

#include "arg/arg.h"
#include "arg/val.h"

#include <string>
#include <cassert>

int main(int argc, char ** argv)
{
  ::std::string socketPath("/var/run/tinc.umlsocket");
  ::arg::Parser p;

  p.add_opt('u', "socket").stow(socketPath).help("set the path to the UML socket").show_default();

  p.add_opt_version("devel");
  p.add_opt_help();

  p.parse(argc, argv);

  UmlNetworkSocketPlug umlPlug(socketPath.c_str());
  assert(not umlPlug.hasPosixError());

  SlirpConfig slirpcfg;
  vdeslirp_init(&slirpcfg, VDE_INIT_DEFAULT);

  // TODO: initialize the slirp configuration from command line

  auto myslirp = vdeslirp_open(&slirpcfg);

  SlirpHandler slirpNetwork(myslirp);

  PacketRingBuffer uml2slirpBuffers;
  PacketRingBuffer slirp2umlBuffers;

  slirpNetwork.connectTransferBuffers(&uml2slirpBuffers, &slirp2umlBuffers);
  umlPlug.connectTransferBuffers(&slirp2umlBuffers, &uml2slirpBuffers);

  FdSelectHandler* connections(nullptr);

  slirpNetwork.addTo(connections);
  umlPlug.addTo(connections);

  while (true)
  {
    timespec tv;

    tv.tv_sec = 0;
    tv.tv_nsec = 100 * 1000 * 1000;

    FdSelectHandler::waitOn(connections, &tv);
  }

  vdeslirp_close(myslirp);
}
