import contextlib
import vim
from ycmd.utils import (JoinLinesAsUnicode, ToUnicode)

def getCurrentBufferNumber():
    return vim.current.buffer.number


def getBufferFiletypeByNumber( bufnr ):
  command = 'getbufvar({0}, "&ft")'.format( bufnr )
  return ToUnicode( vim.eval( command ) ).split( '.' )[0]


def getBufferNameByNumber(bufnr):
    return vim.buffers[bufnr].name


def getBufferContentByNumber(bufnr):
    # buffer_object can be treated as list, see :help python-buffer
    # add a newline to match what gets saved to disk
    return JoinLinesAsUnicode(vim.buffers[bufnr]) + '\n'


def getBufferByNumber(bufnr):
    return vim.buffers[bufnr]


def getCurrentWindow():
    return vim.current.window


def publishRequest():
    command_string = "call ch_sendexpr(g:colorcpp_job_channel, py3eval('cli.requestData()'))"
    vim.command(command_string)


def getWindowsForBuffer( bufnr):
    return [window for window in vim.windows
            if window.buffer.number == bufnr]


def switchWindow( window ):
  """Move to the window object |window|. This function should be called under
  the CurrentWindow context if you are going to switch back to the original
  window."""
  vim.current.window = window


def clearHighlightMatch(bufnr):
  vim.command('call colorcpp#clear_highlight_match('+'\"'+bufnr+'\"'+')')


def addHighlightMatch(bufnr, tp, line, col, size):
  vim.command('call colorcpp#add_highlight_match('+ '\"' + bufnr + '\"' +','
                                                  + '\"' + tp    + '\"' +','
                                                  +        str(line)    +','
                                                  +        str(col)     +','
                                                  +        str(size)    +')')


def getIntValue( variable ):
    return int( vim.eval( variable ) )


def getLineRange(bufnr):
  return [getIntValue('line("w0")'), getIntValue('line("w$")')]


def getPreviousWindowNumber():
  return getIntValue( 'winnr("#")' ) - 1


@contextlib.contextmanager
def autocommandEventsIgnored( events = [ 'all' ] ):
  """Context manager to perform operations without triggering autocommand
  events. |events| is a list of events to ignore. By default, all events are
  ignored."""
  old_eventignore = vim.options[ 'eventignore' ]
  ignored_events = {
    event for event in ToUnicode( old_eventignore ).split( ',' ) if event }
  ignored_events.update( events )
  vim.options[ 'eventignore' ] = ','.join( ignored_events )
  try:
    yield
  finally:
    vim.options[ 'eventignore' ] = old_eventignore


@contextlib.contextmanager
def currentWindow():
  """Context manager to perform operations on other windows than the current one
  without triggering autocommands related to window movement. Use the
  SwitchWindow function to move to other windows while under the context."""
  previous_window = vim.windows[ getPreviousWindowNumber() ]
  current_window = vim.current.window
  with autocommandEventsIgnored( [ 'WinEnter', 'Winleave', 'BufEnter', 'BufLeave' ] ):
    try:
      yield
    finally:
      # Ensure <c-w>p still go to the previous window.
      vim.current.window = previous_window
      vim.current.window = current_window

