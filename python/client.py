import vimsupport as vs
import vim_buffer as vb

NO_COMPILE_FLAGS_MESSAGE = 'Still no compile flags.'

class Client(object):
    def __init__(self):
        self._name = "client"
        self._buffer_dict = {}
        self._req_data = None
        self._cnt = 0

    def requestData(self):
        print("req cnt:"+str(self._req_data['cnt'])+"\n")
        return self._req_data

    def setRequestData(self, req_data):
        self._req_data = req_data
        self._req_data['cnt'] = self._cnt;
        self._cnt = self._cnt + 1

    def parseCurrentBuffer(self, force=False):
        bufnr = vs.getCurrentBufferNumber()
        self._getVimBufferByNumber(bufnr).handleParseRequest(force)

    def handleResponse(self, response):
        print("rsp cnt:"+response['cnt']+"\n")
        if 'bufnr' in response:
            bufnr = int(response['bufnr'])
            self._getVimBufferByNumber(bufnr).handleParseResponse(response)
        else:
            print("no bufnr in response, cnt:"+response['cnt']+"\n")

    def refreshWindowHighlight(self):
        self._getVimBufferByNumber(vs.getCurrentBufferNumber()).refreshHighlights()

    def moveHighlight(self, begin, end):
        self._getVimBufferByNumber(vs.getCurrentBufferNumber()).moveHighlight(begin, end)

    def _getVimBufferByNumber(self, bufnr):
        try:
            buf = self._buffer_dict[bufnr]
        except KeyError:
            buf = vb.VimBuffer(bufnr, self)
            self._buffer_dict[bufnr] = buf
        return buf

