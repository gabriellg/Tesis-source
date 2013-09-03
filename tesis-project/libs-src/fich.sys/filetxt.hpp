// Lectura de ficheros de texto

ArrString *filetxt_leer_fichero_texto(const char *fichero_txt);

void filetxt_escribe_fichero_texto(const char *fichero_txt, const ArrString *lineas);

class CArrayString *filetxt_readFileTxt(const char *filenameTxt);

class CArrayString *filetxt_readFileTxtNumLines(const char *filenameTxt, unsigned long numLines);

void filetxt_writeFileTxt(const char *filename, const class CArrayString *lineas);
