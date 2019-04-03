from ycmd.completers.cpp.flags import Flags
from ycmd.utils import ToCppStringCompatible
import vimsupport as vs
import highlight as hl

SUPPORTED_FILE_TYPES = ['cpp', 'c', 'h', 'hpp', 'cxx', 'cc']

# global flags store the compile flag of all files
flag_store = Flags()

class VimBuffer(object):
    def __init__(self, bufnr):
        self._bufnr = bufnr 
        self._parse_doing = False
        self._highlight = hl.Highlight(self._bufnr)
        self._req_data = {}


    def requestData(self):
        return self._req_data


    def handleParseRequest(self):
        if self._parse_doing:
            return

        filetype = vs.getBufferFiletypeByNumber(self._bufnr)
        if not filetype in SUPPORTED_FILE_TYPES:
            return

        global flag_store
        filename = vs.getBufferNameByNumber(self._bufnr)
        flags, translation_unit_name = flag_store.FlagsForFile(filename)
        if not flags:
            return

        content = vs.getBufferContentByNumber(self._bufnr)

        parse_request = {}
        parse_request['bufnr'] = self._bufnr;
        parse_request['filename'] = translation_unit_name;
        parse_request['flags'] = flags;
        parse_request['unsaved'] = {'filename':filename, 'content':content}

        self._req_data = parse_request
        vs.publishRequest()
        self._parse_doing = True


    def handleParseResponse(self, response):
        if 'highlights' in response:
            self._highlight.updateWithNewHighlights(response['highlights'])
        self._parse_doing = False;


    def refreshHighlights(self):
        self._highlight.refreshHighlights()


    def moveHighlight(self, begin, end):
        self._highlight.moveHighlight(begin, end)

