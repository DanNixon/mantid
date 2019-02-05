# Mantid Repository : https://github.com/mantidproject/mantid
#
# Copyright &copy; 2017 ISIS Rutherford Appleton Laboratory UKRI,
#     NScD Oak Ridge National Laboratory, European Spallation Source
#     & Institut Laue - Langevin
# SPDX - License - Identifier: GPL - 3.0 +
#  This file is part of the mantid workbench.
#
#
from __future__ import (absolute_import, unicode_literals)

# system imports
import sys
import threading
import time
from traceback import extract_tb


def blocking_async_task(target, args=(), kwargs=None, blocking_cb=None,
                        period_secs=0.05):
    """Run the target in a separate thread and block the calling thread
    until execution is complete.

    :param target: A Python callable object
    :param args: Arguments to pass to the callable
    :param kwargs: Keyword arguments to pass to the callable
    :param blocking_cb: An optional callback to process while waiting for the task
    to finish
    :param period_secs: Sleep for this many seconds at the start of each loop that checks
    the task is still alive. This will be the minimum time between calls to blocking_cb.
    :returns: An AsyncTaskResult object
    """
    blocking_cb = blocking_cb if blocking_cb is not None else lambda: None

    class Receiver(object):
        output, exc_value = None, None

        def on_success(self, result):
            self.output = result.output

        def on_error(self, result):
            self.exc_value = result.exc_value

    recv = Receiver()
    task = AsyncTask(target, args, kwargs, success_cb=recv.on_success,
                     error_cb=recv.on_error)
    task.start()
    while task.is_alive():
        time.sleep(period_secs)
        blocking_cb()

    if recv.exc_value is not None:
        raise recv.exc_value
    else:
        return recv.output


class AsyncTask(threading.Thread):

    def __init__(self, target, args=(), kwargs=None,
                 success_cb=None, error_cb=None,
                 finished_cb=None):
        """

        :param target: A Python callable object
        :param args: Arguments to pass to the callable
        :param stack_chop: If an error is raised then chop this many entries
        from the top of traceback stack.
        :param kwargs: Keyword arguments to pass to the callable
        :param success_cb: Optional callback called when operation was successful
        :param error_cb: Optional callback called when operation was not successful
        :param finished_cb: Optional callback called when operation has finished
        """
        super(AsyncTask, self).__init__()
        if not callable(target):
            raise TypeError("Target object is required to be callable")

        self.target = target
        self.args = args
        self.kwargs = kwargs if kwargs is not None else {}

        self.success_cb = success_cb if success_cb is not None else lambda x: None
        self.error_cb = error_cb if error_cb is not None else lambda x: None
        self.finished_cb = finished_cb if finished_cb is not None else lambda: None

    def run(self):
        def elapsed(start):
            return time.time() - start
        try:
            time_start = time.time()
            out = self.target(*self.args, **self.kwargs)
        except SyntaxError as exc:
            # treat SyntaxErrors as special as the traceback makes no sense
            # and the lineno is part of the exception instance
            self.error_cb(AsyncTaskFailure(elapsed(time_start), SyntaxError, exc, None))
        except:  # noqa
            self.error_cb(AsyncTaskFailure.from_excinfo(elapsed(time_start)))
        else:
            self.success_cb(AsyncTaskSuccess(elapsed(time_start), out))

        self.finished_cb()


class AsyncTaskResult(object):
    """Object describing the execution of an asynchronous task
    """

    def __init__(self, elapsed_time):
        self.elapsed_time = elapsed_time
        self.timestamp = time.ctime()


class AsyncTaskSuccess(AsyncTaskResult):
    """Object describing the successful execution of an asynchronous task
    """

    def __init__(self, elapsed_time, output):
        super(AsyncTaskSuccess, self).__init__(elapsed_time)
        self.output = output

    @property
    def success(self):
        return True


class AsyncTaskFailure(AsyncTaskResult):
    """Object describing the failed execution of an asynchronous task
    """

    @staticmethod
    def from_excinfo(elapsed_time):
        """
        Create an AsyncTaskFailure from the current exception info

        :param elapsed_time Time take for task
        :param chop: Trim this number of entries from
        the top of the stack listing
        :return: A new AsyncTaskFailure object
        """
        exc_type, exc_value, exc_tb = sys.exc_info()
        return AsyncTaskFailure(elapsed_time, exc_type, exc_value,
                                exc_tb)

    def __init__(self, elapsed_time, exc_type, exc_value, stack):
        super(AsyncTaskFailure, self).__init__(elapsed_time)
        self.exc_type = exc_type
        self.exc_value = exc_value
        self.stack = stack

    def __str__(self):
        error_name = type(self.exc_value).__name__
        filename, lineno, _, _ = extract_tb(self.stack)[-1]
        msg = self.exc_value.args
        if isinstance(msg, tuple):
            msg = msg[0]
        return '{} on line {} of \'{}\': {}'.format(error_name, lineno, filename, msg)

    @property
    def success(self):
        return False
