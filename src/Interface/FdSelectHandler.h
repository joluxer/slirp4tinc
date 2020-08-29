/*
 * FdSelectHandler.h
 *
 *  Created on: 04.10.2012
 *      Author: lode
 */

#ifndef FDSELECTHANDLER_H_
#define FDSELECTHANDLER_H_

#include <sys/time.h>

/**
 * This class encapsulates the details of select() calls to file descriptors.
 * Derived classes implement the reaction to events at the file descriptor.
 */
class FdSelectHandler
{
public:
  FdSelectHandler();
  virtual ~FdSelectHandler();

  void addTo(FdSelectHandler* &chain); ///< append the object to given the chain
  void removeFromChain(); ///< remove object from the chain, it is in

  /**
   * This method is used for the destruction of the connection and all associated clean-up work.
   *
   * Dynamically allocated objects are no longer available after this call.
   * Statically created objects may no longer be used, their function is destroyed.
   */
  virtual
  void destroy() =0;

  /***
   * This static method waits for the availability of all FdSelectHandler objects
   * of the chain, if you have configured a wait condition.
   *
   * If no timeout (nullptr) is specified, the wait is endless.
   * If the specified timeout expires without an FdSelectHandler object becoming available
   * (writeable or readable), the method returns false, otherwise true.
   *
   * The method uses the system's pselect() routine to perform the wait operation.
   *
   * @param chain is the chain of observed objects
   * @param relativeTimeout is the time specification how long the chain should be observed, or 0 (nullptr) for endless.
   * @return true, if at least one object became available before the timeout expired, otherwise (on timeout) false
   */
  static
  bool waitOn(FdSelectHandler* &chain, struct timespec* relativeTimeout = 0);

  /***
   * This method calls waitOn() with a timeout of 0 (no waiting time).
   *
   * It returns true if an object was available for I/O, otherwise false.
   * @param chain is the chain of observed objects
   * @return true, if at least one object was available, otherwise (at timeout) false
   */
  static
  bool checkAll(FdSelectHandler* &chain); ///< executes a select on all descriptors of the given chain with zero timeout

protected:
  bool waitForRead:1;   ///< This flag must be set by the inheriting class as required.
  bool waitForWrite:1;  ///< This flag must be set by the inheriting class as required.

  FdSelectHandler *prev, *next, **chain;

  virtual
  void execute(bool readable, bool writable) =0;  ///< this implements the workhorse for reacting to readable and writable I/O

  virtual
  int fd() =0;  ///< this must return the fd of object
};

#endif /* FDSELECTHANDLER_H_ */
