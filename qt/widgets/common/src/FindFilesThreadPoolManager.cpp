#include "MantidQtWidgets/Common/FindFilesThreadPoolManager.h"
#include "MantidAPI/AlgorithmManager.h"
#include "MantidAPI/FileFinder.h"
#include "MantidAPI/FileProperty.h"
#include "MantidAPI/MultipleFileProperty.h"
#include "MantidKernel/ConfigService.h"
#include "MantidKernel/FacilityInfo.h"
#include "MantidKernel/VectorHelper.h"

#include <QCoreApplication>
#include <QSharedPointer>
#include <Poco/File.h>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>

using namespace Mantid::Kernel;
using namespace Mantid::API;
using namespace MantidQt::API;

QThreadPool FindFilesThreadPoolManager::m_pool;

FindFilesThreadPoolManager::FindFilesThreadPoolManager() {
  // Default allocator function. This just creates a new worker
  // on the heap.
  m_workerAllocator = [](const FindFilesSearchParameters &parameters) {
    return new FindFilesWorker(parameters);
  };
}

/** Set the allocator function for the thread pool.
 *
 * This is currently used for unit testing so that a fake QRunnable object
 * can be used in place of a real worker.
 *
 * @param allocator :: The thread allocator function to user to create new
 * worker objects
 */
void FindFilesThreadPoolManager::setAllocator(ThreadAllocator allocator) {
  m_workerAllocator = allocator;
}

void FindFilesThreadPoolManager::createWorker(
    const QObject *parent, const FindFilesSearchParameters &parameters) {
  cancelWorker(parent);

  // if parent is null then don't do anything as there will be no
  // object listening for the search result
  if (!parent)
    return;

  auto worker = m_workerAllocator(parameters);
  connectWorker(parent, worker);

  // pass ownership to the thread pool
  // we do not need to worry about deleting worker
  m_pool.start(worker);
  m_searchIsRunning = true;
}

/** Connect a new worker to the listening parent
 *
 * This will hook up signals/slots for:
 *  - Returning the result of the search
 *  - Indicating when the search is finished
 *  - When the search is cancelled
 *
 *  @param parent :: the listening parent object waiting for search results
 *  @param worker :: the worker to connect signals/slots for.
 */
void FindFilesThreadPoolManager::connectWorker(const QObject* parent, const FindFilesWorker* worker) {
  parent->connect(worker, SIGNAL(finished(const FindFilesSearchResults &)),
                  parent,
                  SLOT(inspectThreadResult(const FindFilesSearchResults &)),
                  Qt::QueuedConnection);

  parent->connect(worker, SIGNAL(finished(const FindFilesSearchResults &)),
                  parent, SIGNAL(fileFindingFinished()), Qt::QueuedConnection);

  this->connect(worker, SIGNAL(finished(const FindFilesSearchResults &)), this,
                SLOT(searchFinished()), Qt::QueuedConnection);

  this->connect(this, SIGNAL(disconnectWorkers()), worker,
                SLOT(disconnectWorker()), Qt::QueuedConnection);
}

/** Cancel the currently running worker
 *
 * This will disconnect any signals from the worker and then let the
 * internal QThreadPool clean up the worker at a later time.
 *
 * @param parent :: the parent widget to disconnect signals for.
 */
void FindFilesThreadPoolManager::cancelWorker(const QObject *parent) {
	// Just disconnect any signals from the worker. We leave the worker to
	// continue running in the background because 1) terminating it directly
	// is dangerous (we have no idea what it's currently doing from here) and 2)
	// waiting for it to finish before starting a new thread locks up the GUI
	// event loop.
  emit disconnectWorkers();
  m_searchIsRunning = false;
  QCoreApplication::sendPostedEvents();
}

/** Check if a search is currently executing.
 *
 * @returns true if the current worker object is null
 */
bool FindFilesThreadPoolManager::isSearchRunning() const {
  return m_searchIsRunning;
}

/** Wait for all threads in the pool to finish running.
 *
 * Warning: This call will block execution until ALL threads in the pool
 * have finished executing. Using this in a GUI thread will cause the GUI
 * to freeze up.
 */
void FindFilesThreadPoolManager::waitForDone() {
  m_pool.waitForDone();
}

/** Mark the search as being finished.
 */
void FindFilesThreadPoolManager::searchFinished() {
	m_searchIsRunning = false;
}
