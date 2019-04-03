import vimsupport as vs
import vim

class Highlight( object ):
  def __init__( self, bufnr):
    self._bufnr = bufnr
    self._highlights = []
    self._line_begins = {}
    self._line_ends   = {}

  def updateWithNewHighlights( self, highlights):
    #print("update buffer: "+str(self._bufnr)+", hl_num: "+str(len(highlights)))
    self._highlights = highlights

    with vs.currentWindow():
      for window in vs.getWindowsForBuffer( self._bufnr ):
        vs.switchWindow( window )

        # assign a unique_id for window, this should only be needed for the first window, do to
        # youcompleteme defered initialization
        if not window.vars.has_key('unique_id'):
          unique_id = vs.getIntValue("g:color_cpp_unique_window_id")
          vim.command("let w:unique_id ={0}".format(unique_id))
          vim.command("let g:color_cpp_unique_window_id += 1")

        # the highlight index of the window
        bufname = str(self._bufnr)+"."+str(window.vars['unique_id'])
        if not window.vars.has_key('color_code_name'):
          vim.command("let w:color_code_name=" +"\""+bufname+"\"")

        self._line_begins[bufname], self._line_ends[bufname] = vs.getLineRange(self._bufnr)

        # clear the old highlight
        vs.clearHighlightMatch(bufname);
        
        # apply new ones
        for highlight in highlights:
          line = int(highlight['line'])
          if line>=self._line_begins[bufname] and line<=self._line_ends[bufname]:
            vs.addHighlightMatch(bufname, highlight['type'], line, highlight['col'], len(highlight['text']))
  
  def refreshHighlights(self):
    # update with old highlights
    self.updateWithNewHighlights(self._highlights)
  
  def moveHighlight(self, start, end):
    window = vs.getCurrentWindow()

    # assign a unique_id for window, this should only be needed for the first window, do to
    # youcompleteme defered initialization
    if not window.vars.has_key('unique_id'):
      unique_id = vs.getIntValue("g:color_cpp_unique_window_id")
      vim.command("let w:unique_id ={0}".format(unique_id))
      vim.command("let g:color_cpp_unique_window_id += 1")

    # the highlight index of the window
    bufname = str(self._bufnr)+"."+str(window.vars['unique_id'])
    if not window.vars.has_key('color_code_name'):
      vim.command("let w:color_code_name=" +"\""+bufname+"\"")

    if bufname in self._line_begins and bufname in self._line_ends:
      if self._line_begins[bufname] <= start and self._line_ends[bufname] >= end:
        return

    # record the new line range
    [self._line_begins[bufname], self._line_ends[bufname]] = [start-25, end+25];

    #remove the old ones
    vs.clearHighlightMatch(bufname);

    # apply new ones
    for highlight in self._highlights:
      line = int(highlight['line'])
      # only apply ones between line_begin and line_end
      if line >=self._line_begins[bufname] and line<=self._line_ends[bufname]:
        vs.addHighlightMatch(bufname, highlight['type'], line, highlight['col'], len(highlight['text']))
