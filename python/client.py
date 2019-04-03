import vimsupport as vs
import vim_buffer as vb

NO_COMPILE_FLAGS_MESSAGE = 'Still no compile flags.'

class Client(object):
    def __init__(self):
        self._name = "client"
        self._buffer_dict = {}

    def requestData(self):
        bufnr = vs.getCurrentBufferNumber()
        return self._getVimBufferByNumber(bufnr).requestData()

    def parseCurrentBuffer(self):
        bufnr = vs.getCurrentBufferNumber()
        self._getVimBufferByNumber(bufnr).handleParseRequest()

    def handleResponse(self, response):
        if 'bufnr' in response:
            bufnr = int(response['bufnr'])
            self._getVimBufferByNumber(bufnr).handleParseResponse(response)

    def refreshWindowHighlight(self):
        self._getVimBufferByNumber(vs.getCurrentBufferNumber()).refreshHighlights()

    def moveHighlight(self, begin, end):
        self._getVimBufferByNumber(vs.getCurrentBufferNumber()).moveHighlight(begin, end)

    def _getVimBufferByNumber(self, bufnr):
        try:
            buf = self._buffer_dict[bufnr]
        except KeyError:
            buf = vb.VimBuffer(bufnr)
            self._buffer_dict[bufnr] = buf
        return buf

